#include <T_UFRRJ.hpp>
#include <cassert>
#include <cmath>
T_UFRRJ::T_UFRRJ(const std::string m):
Rules(m)
{};

T_UFRRJ::~T_UFRRJ(){
    
#ifdef _DEBUG
	std::cerr << __FILE__ << ":" << __FUNCTION__ << std::endl;
#endif
    
};



void T_UFRRJ::apply(int vIndex, bool changing){
   double   dVel = 0.0f,
            dTH   = 0.0f,
            deltaV    = 0.0f,
            dheadway  = 0.0f,
            dsafedist = 0.0f,
//            deltaVMax = 50.0f,
            ds = 0.0f,
            alpha = 0.0f,
            beta  = 0.0f;
            
   int   vDef  = 0,
         GAP   = 0;            
         
   int iVel = 0,
       iDist = 0,
       iAheadVel = 0,
       iMax = 0;
   
   Vehicle *pVehicle    = mGrid.getVehiclePointer(vIndex);
   Vehicle **ppVehicle  = &pVehicle;
   tpVehiclesType *type = (*ppVehicle)->type;
   
   alpha = betaFunction(type->param),
   beta  = 1.0f - betaFunction(type->param + 5);
          
   deltaV    =  (*ppVehicle)->type->inc;
   dheadway  =  (*ppVehicle)->type->aheadInt;
   dsafedist =   (*ppVehicle)->type->safeDist;
          
   iMax = min(mParam.vMax, (*ppVehicle)->type->vMax);
   getDistanceAndVelocity(vIndex, &iDist, &iAheadVel);
   GAP = iDist;
   
   dVel = static_cast<double> ((*ppVehicle)->vx) + ( (deltaV * alpha) - 0.5f);
   //plot '2_4.txt' u (floor((4*$1)-0.5)):($2) w p
   dVel = floor(dVel);
   
   iVel = static_cast<int> (dVel);
   
   iVel = max(iVel, 0);
   
   iVel = min (iVel, iMax);
   vDef  = iVel;
   
   
   dTH = static_cast<double>(iDist) / (dVel - static_cast<double> (iAheadVel)); //+ (deltaV * alpha));

   if (( dTH >  0.0f) && (dTH <= (dheadway * beta))){
    ds = (dsafedist * beta) ;//* dTH	;
   }
   

   assert(ds >= 0.0f);
   iDist -= truncCurve3(ds);

   if (iDist < 0)
      iDist = 0;

   iVel  = min(iVel, iDist);

   (*ppVehicle)->vxNew = iVel;

    
}


int T_UFRRJ::truncCurve3(double v){
    int result = static_cast<int> (floor(v+0.5f));
    
    
    return result;
    
}




double  T_UFRRJ::betaFunction (double gamaAlpha,
                                    double gamaBeta,
                                    double gamaAlphaBeta,
                                    double alpha,
                                    double beta,
                                    double x){
    
    
	double  xalpha = pow(x, (alpha-1.0f)),
                xbeta  = pow((1.0f - x), (beta-1.0f));
    
	double ret = gamaAlphaBeta / (gamaAlpha * gamaBeta);
    
    ret *= xalpha * xbeta;
    
    return ret;
}

double  T_UFRRJ::betaFunction (double *vet){
    
	double x, y, y1;
    do{
        x  = static_cast <double> (rand() % 65535 + 1) / 65535.0f;
        y1 = static_cast <double> (rand() % 65535 + 1) / 65535.0f;
        
        y = betaFunction( vet[2],
                         vet[3],
                         vet[4],
                         vet[0],
                         vet[1],
                         x);
        
        
    }while (y1 > y);
    
    if (x < 0.0f)
        x = 0.0f;
    
    if (x > 1.0f)
        x = 1.0f;
    
    return x;
    
}


//------------------------------------------------------------------------------------


/*
 * Multilane model
 */

/*
 *   Multiroad rules!!!!
 **/

T_UFRRJ_MR::T_UFRRJ_MR(const std::string m):
T_UFRRJ(m),
mStatistic(false){
    //mIndexDensity(0){
};

T_UFRRJ_MR::~T_UFRRJ_MR(){
#ifdef _DEBUG
	std::cerr << __FILE__ << ":" << __FUNCTION__ << std::endl;
#endif
    
};

void T_UFRRJ_MR::update(int step, float density, bool changing){
    

    
    //Updating all vehicles
    mStatistic = (step >= getParam()->dTime) && (step > 0);
    
    
    getGrid()->clearGrid();
    /*
     for (int j = 0; j < getParam()->cellY; j++)
     for(int i = 0; i < getParam()->cellX; i++)
     getGrid()->setGrid(i, j, -1);
     */
    
    for (int idx = 0;  idx < getGrid()->getVehiclesSize(); idx++)
    {
        
        Vehicle *pVehicle = getGrid()->getVehiclePointer(idx);
        Vehicle **ppVehicle = &pVehicle;
        
        assert(ppVehicle != NULL);
        if (changing)
            (*ppVehicle)->updateY(mStatistic, step);
        else
            (*ppVehicle)->update(mStatistic, step);
        //mRules->getGrid()->getVehicles(idx).update(statistic, step);
        
        
        
        int y = pVehicle->y;
        
        
        for (int k = 0 ; k <   pVehicle->lg; k++)
        {
            int x  =  pVehicle->x - k;
            if (x < 0)
                x = getParam()->cellX + x;
            
            //	            if (mStatistic)
            //	            	mLocalLaneRate[y]++;
            
            if ((getGrid()->getGrid(x, y) != -1) && (!changing))
            {
                std::cerr << "\t [ERROR] " << __FILE__ << " at " << __LINE__ << "\t Time:" << step <<  std::endl ;
                
                int mycar = getGrid()->getGrid(x, y);
                Vehicle v = getGrid()->getVehicles(mycar);
                
                // v.mBreakForeseenLight = true;
                std::cerr << "IDX: "<< mycar << " ";
                std::cerr << "V1(ID= " << v.ID << ",X= "
                << v.x  << ",Y= "
                << v.y  << ", VX="
                << (int)v.vx <<  ")" << std::endl;
                
                
                v = getGrid()->getVehicles(idx);
                std::cerr << "IDX: "<< mycar << " ";
                std::cerr << "V2(ID= " << v.ID << ",X= "
                << v.x  << ",Y= "
                << v.y  << ", VX="
                << (int)v.vx <<  ")" << std::endl;
                
                
                exit(0);
                
            }
            
            getGrid()->setGrid(x, y, idx);
            
        }
        
        /*
         if (mStatistic)
         for (size_t i = 0; i < mLocalLaneRate.size(); i++){
         mLaneRate.at(mIndexDensity).at(i) += (mLocalLaneRate[i] / static_cast<float>(getParam()->cellX));
         mLocalLaneRate[i] = 0.0f;
         }
         */
        
    }
    
    
    
    if ((!changing) && (step > 0)){
        
        //   if (mStatistic)
        //     solveRealDistance();
        
        //Verifying if there is vehicle stopped on moviment detector cell.
        if (mStatistic){
            
            for (int j = 0; j < getParam()->cellY; j++){
                
                int idx = getGrid()->getGrid(getParam()->cellX-1, j);
                
                if (idx != -1){ //if there is a vehicle
                    if (getGrid()->getVehicles(idx).vx < 1){
                        getSensor()->actived(j, step);
                        
                        Vehicle *pVehicle    = getGrid()->getVehiclePointer(idx);
                        Vehicle **ppVehicle  = &pVehicle;

                        
                        //  getSensor()->actived(j, step);
                    }
                    
                }
                
            }//end- for (int j = 0; j < mRules->getParam()->cellY; j++){
        }//end- if (statistic){
        if ((step % getParam()->stTime) == 0)
            getSensor()->statistics(step, density);
        
    }
    
};




bool T_UFRRJ_MR::r05_Change(int idx, int iDir){
   Vehicle *pVehicle    = mGrid.getVehiclePointer(idx),
            //vAheadVehicle,
            vBackVehicle;

   Vehicle **ppVehicle  = &pVehicle;
   tpVehiclesType *type = (*ppVehicle)->type;


   int   iMyY = (*ppVehicle)->y,
         iMyX = (*ppVehicle)->x,
         iBackDistance = 0,
         iAheadDistance = 0,
         iPtr = 0,
         iLen =  type->size ,
         iVel = (*ppVehicle)->vx,
         iVelVehicle = 0,
         iDistEffective = 0;

  
   
   double alpha = betaFunction(type->param),
          gama  = 1.0f - betaFunction(type->param),
          beta  = 1.0f - betaFunction(type->param + 5),
          dheadway  =  static_cast<double> (type->aheadInt);

   //////////////////////////////////
   // Highway direction:
   // Initial --> Final
   // 0 - left
   // 1 - right
   //////////////////////////////////


   if ((iDir == RIGHT2LEFT) && (iMyY == 0))
      return false;

   if ((iDir == LEFT2RIGHT) && (iMyY == (mParam.cellY-1)))
      return false;

   switch (iDir){
      case RIGHT2LEFT: iMyY--; break;
      case LEFT2RIGHT: iMyY++; break;
   }

   


   //Immediately beside
   for (int i = 0; i < iLen; i++){
      iMyX = (*ppVehicle)->x - i;
      
      if (iMyX < 0)
               iMyX = mParam.cellX + iMyX;
      
      iPtr = mGrid.getGrid(iMyX, iMyY);
      if (iPtr != -1)
               return false;
   }



   //Find ahead  vehicle
   iMyX = (*ppVehicle)->x;
   iAheadDistance = 0;
   do{
      iMyX++;
      iAheadDistance++;
      if (iMyX == mParam.cellX)
               iMyX = 0;
      
      iPtr =  mGrid.getGrid(iMyX, iMyY);
      
      
   }while ((iPtr == -1) && (iAheadDistance <= mParam.cellX));



   if (iAheadDistance > mParam.cellX)
      return true;

   if (iAheadDistance > 0)
      iAheadDistance--;
   
   //Find back  vehicle
   iMyX = (*ppVehicle)->x - iLen;
   iBackDistance = 0;
   //Finding the previous vehicle
   do{
      iMyX--;
      iBackDistance++;
      if (iMyX < 0)
               iMyX = mParam.cellX - 1;
      
      iPtr = mGrid.getGrid(iMyX, iMyY);
      //assert(iAheadDistance < 101);
   }while ((iPtr == -1) && (iBackDistance <= mParam.cellX));


   if (iBackDistance > mParam.cellX)
      return true;


   vBackVehicle = mGrid.getVehicles(iPtr);
   if (iBackDistance > 0)
      iBackDistance--;
   vBackVehicle =  mGrid.getVehicles(iPtr);
   
   double dVel = 0.0f,
          dMax = 0.0f,
          dThAhead = 0.0f,
          dThBack  = 0.0f,
          deltaV   = 0.0f,
          dsafedist = 0.0f,
          deltaVMax = 1.0f,
          ds        = 0.0f;
   
   //Aqui, nas duas regras abaixo que vou inserir o comportamento
   //03/09/2016
   if ( iBackDistance < vBackVehicle.vx )
        return false;
   
   if ( iVel  >  iAheadDistance)
        return false;
  
//Aqui começa a história
   /*
   double aux = static_cast <double> (vAheadVehicle.vx - iVel),
          dThAhead = 0.0f,
          dThBack = 0.0f;
   if (aux >= 1.0f)
      dThAhead = static_cast <double> (iAheadDistance) / aux;
   
   aux = static_cast <double> (vBackVehicle.vx - iVel);
   
   if (aux >= 1.0f)
      dThBack = static_cast <double> (iBackDistance) / aux;
   
   
   aux = (dheadway * beta);
   
   if ((dThAhead >= aux) && (dThBack <= aux))
      return true;
   */
   return true;
   

}
/* Configuração padrão: th1 = 3 e th2 = 6;
 * Melhor configuração que funcionou: th1 = 9 / th2 = 6
 * th1 = altera a densidade onde ocorre a mudança
 * th2 = os pontos de minimos e maximo das faixas é definido por este
 * parametros
 * th1 = regra r02_Motivated
 * th2 = regra r03_Motivated
 */


bool T_UFRRJ_MR::r03_Motivated(int idx){
    
    
    int   iMyVel = 0,
    iDist  = 0;
    
    Vehicle aheadVehicle;
    Vehicle *pVehicle    = mGrid.getVehiclePointer(idx);
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = (*ppVehicle)->type;
    
    iMyVel = static_cast<int> ((*ppVehicle)->vx);
    iDist = getDistance(idx, &aheadVehicle);
    
    
    if ((iDist == iMyVel) && (iDist == 0))
        return true;
    
    
    float fh =  0.0f; //alphaFunction(pVehicle->alphaOld) * 12;
    int ih = max(trunc(fh), 1); //minimo de 3 segundos para nao bloquear o motorista para os casos de alpha 0
    
    
    if (aheadVehicle.ID != 0xFFFFFFFF){
        if ((iDist) > ih * iMyVel)//  if (iDist > 9 * iMyVel) //9 It is must be 9
    	   	return true;
        else
            return false;
    }
    
    return false;
    
}



bool T_UFRRJ_MR::r02_Motivated(int idx){
    
    
    //Veículo que vem detrás
    Vehicle vBackVehicle;
    Vehicle *pVehicle    = mGrid.getVehiclePointer(idx);
    Vehicle **ppVehicle  = &pVehicle;
    
    int iMyVel     = static_cast<int> ((*ppVehicle)->vx),
    iBackVel   = 0,
    iBackDist  = 0;
    
    getBack(&iBackDist, &vBackVehicle, idx);
    
    iBackVel = static_cast<int> (vBackVehicle.vx);
    
    //    if (vBackVehicle.ID == pVehicle->ID)
    //    	return true;
    
    float fh =  0.0f ; //alphaFunction(pVehicle->alphaOld) * 12;
    int ih = max(trunc(fh), 1); //minimo de 3 segundos para nao bloquear o motorista para os casos de alpha 0
    
    if ((iBackDist <  ih * (iBackVel)) && (iMyVel <= iBackVel)) //[6] - In the second is 18 - This value changes the point where there is the lane inversion. Increasing the value, increasing the point flow
        return true;
    return false;
    
};

bool T_UFRRJ_MR::r01_Motivated(int vIndex){
    
   double   dVel  = 0.0f,
         dMax  = 0.0f,
         dTH   = 0.0f,
         deltaV    = 0.0f,
         dheadway  = 0.0f,
         dsafedist = 0.0f,
         deltaVMax = 1.0f,
         ds = 0.0f;
         
   int iVel = 0,
       iDist = 0;
       
   Vehicle *pVehicle    = mGrid.getVehiclePointer(vIndex);
   Vehicle **ppVehicle  = &pVehicle;
   Vehicle aheadVehicle;
   tpVehiclesType *type = (*ppVehicle)->type;
   
   double alpha = betaFunction(type->param),
          gama  = 1.0f - betaFunction(type->param),
          beta  = 1.0f - betaFunction(type->param + 5);
   
   dMax      = static_cast<double> (min(mParam.vMax, type->vMax));
   iDist     = getDistanceWithVehicle(vIndex, &aheadVehicle);
   dVel      = static_cast<double> ((*ppVehicle)->vx);
   deltaV    =  static_cast<double> (type->inc);
   dheadway  =  static_cast<double> (type->aheadInt);
   dsafedist =  static_cast<double> (type->safeDist);
   
   //dVel = min(dVel + (deltaV * alpha), dMax - (deltaVMax * gama));
   
   double aux = (dVel - static_cast<double> (aheadVehicle.vx));
   if (aux >= 1.0f)
      dTH =  static_cast<double>(iDist) / aux;
   

   if (( dTH >  0.0f) && (dTH <= (dheadway * beta))){
    ds = dsafedist * beta * dTH	;
   }
   
   iVel = truncCurve3(dVel);

   assert(ds >= 0.0f);
   iDist -= truncCurve3(ds);

   if (iDist < 0)
      iDist = 0;

  
   if (iVel > iDist)
      return true; //ret = r01A_Motivated(vIndex, iDir);
   
      
   return false;
    
    
}

float T_UFRRJ_MR::r01C_Motivated(int idx, int iDir){
    
    if (!mThereIsBlocked){
        return 1.0f;
    }
    
    Vehicle *pVehicle    = mGrid.getVehiclePointer(idx),
    vAheadVehicle;
    
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = (*ppVehicle)->type;
    
    int iMyY = static_cast<int> ((*ppVehicle)->y),
    iMyX = (*ppVehicle)->x,
    iDist = 0,
    iPtr = 0,
    iLen =  type->size ,
    iVel = static_cast<int> ((*ppVehicle)->vx);
    
    if ((iDir == RIGHT2LEFT) && (iMyY == 0))
        return 0.0f;
    
    if ((iDir == LEFT2RIGHT) && (iMyY == (mParam.cellY-1)))
        return 0.0f;
    
    switch (iDir){
        case RIGHT2LEFT: iMyY--; break;
        case LEFT2RIGHT: iMyY++; break;
    }
    
    //Immediately beside
    for (int i = 0; i < iLen; i++){
        iMyX = (*ppVehicle)->x - i;
        
        if (iMyX < 0)
            iMyX = mParam.cellX + iMyX;
        
        iPtr = mGrid.getGrid(iMyX, iMyY);
        if (iPtr != -1)
            return 0.0f;
    }
    
    iMyX = (*ppVehicle)->x;
    
    bool found = false;
    do
    {
        iMyX++;
        iDist++;
        if (iMyX == mParam.cellX)
            iMyX = 0;
        iPtr = mGrid.getGrid(iMyX, iMyY);
        
        if (iPtr != -1){
            vAheadVehicle = mGrid.getVehicles(iPtr);
            if ((vAheadVehicle.mType & 0x00FFFFFF) == BLOCKED)
                found = true;
        }
    } while  ((!found) && (iDist < mParam.cellX));
    
    if ((iDist >= mParam.cellX) || (!found)){
        return 1.0f;
    }
    
    type = vAheadVehicle.type;
    if ((iDist <= type->inc) && ((vAheadVehicle.mType & 0x00FFFFFF) == BLOCKED)){
        return 0.0f;
    }
    
    return 1.0f;
}

bool T_UFRRJ_MR::r01B_Motivated(int idx){
    Vehicle aheadVehicle;
    
    int iDist = getDistance2(idx, &aheadVehicle);
    
    if (aheadVehicle.ID != 0xFFFFFFFF){
        tpVehiclesType *type = aheadVehicle.type;
        
        if ((iDist <= type->inc) && ((aheadVehicle.mType & 0x00FFFFFF) == BLOCKED))
            return true;
    }
    
    return false;
    
}

/* 
 * This rule motives any driver to change lane iff the next lane is
 * better and current one.
 */ 

bool T_UFRRJ_MR::r01A_Motivated(int idx, int iDir){
    
    
   Vehicle *pVehicle    = mGrid.getVehiclePointer(idx),
   vAheadVehicle,
   vBesideVehicle;

   Vehicle **ppVehicle  = &pVehicle;
   tpVehiclesType *type = (*ppVehicle)->type;

   //----------
   double alpha = betaFunction(type->param),
          gama  = 1.0f - betaFunction(type->param),
          beta  = 1.0f - betaFunction(type->param + 5),
          dMax  = static_cast<double> (min(mParam.vMax, type->vMax)),
          dVel      = static_cast<double> ((*ppVehicle)->vx),
          deltaV    =  static_cast<double> (type->inc),
          dheadway  =  static_cast<double> (type->aheadInt),
          dsafedist =  static_cast<double> (type->safeDist),
          deltaVMax = 1.0f,
          dThAhead = 0.0f,
          ds = 0.0f;
          
   dVel = min(dVel + (deltaV * alpha), dMax - (deltaVMax * gama));
   //----------
   
   int   iMyY = (*ppVehicle)->y,
         iMyX = (*ppVehicle)->x,
         iBesideDistance = 0,
         iPtr = 0,
         iLen =  type->size;

   if ((iDir == RIGHT2LEFT) && (iMyY == 0))
      return false;

   if ((iDir == LEFT2RIGHT) && (iMyY == (mParam.cellY-1)))
      return false;

   switch (iDir){
      case RIGHT2LEFT: iMyY--; break;
      case LEFT2RIGHT: iMyY++; break;
   }
   //Immediately beside
   for (int i = 0; i < iLen; i++){
      iMyX = (*ppVehicle)->x - i;
      
      if (iMyX < 0)
         iMyX = mParam.cellX + iMyX;
      
      iPtr = mGrid.getGrid(iMyX, iMyY);
      if (iPtr != -1)
         return false;
   }

   
   iMyX = (*ppVehicle)->x;
   //iAheadDistance = 0;
   do{
      iMyX++;
      iBesideDistance++;
      if (iMyX == mParam.cellX)
         iMyX = 0;
      
      iPtr =  mGrid.getGrid(iMyX, iMyY);
      
      
   }while ((iPtr == -1) && (iBesideDistance <= mParam.cellX));

   if (iBesideDistance > mParam.cellX)
      return true;

   if (iBesideDistance > 0)
      iBesideDistance--;

   vBesideVehicle =  mGrid.getVehicles(iPtr);

   
   dThAhead = static_cast<double>(iBesideDistance) / ((static_cast<double> (vBesideVehicle.vx) + (deltaV * alpha)) - dVel);

   if (( dThAhead >  0.0f) && (dThAhead <= (dheadway * beta))){
      return true;
   }
   
   //Flow in behind lane is better tham mine due to 
   //Behind vehicle moves faster than me
   //if (iVelBeside > iVel)
     // return true;
/*

   //iBesideDistance += iVelBeside;

   type = vBesideVehicle.type;
   int iMax   = min(mParam.vMax, type->vMax);
   int iDeltaV = type->inc;
   iVelBeside = min(vBesideVehicle.vx + iDeltaV, iMax);


   if (iVelBeside <= 0)
      return false;

   iBesideDistance += iVelBeside;

   if (iVel <=  iBesideDistance)
      return true;
*/

   return false;
    
    
}



/**
 * Apply change lane as well as postion definition
 * At last, updates
 */
void T_UFRRJ_MR::apply(int step, float density, bool changing){
    
    
    
	for (int i = 0; i < mGrid.getVehiclesSize(); i++)
		apply(i, true);
	update(step, density, true);
    
    
	for (int i = 0; i < mGrid.getVehiclesSize(); i++)
		apply(i);
	update(step, density);
    
    
    
};
void T_UFRRJ_MR::symmetric(int idx){
    Vehicle *pVehicle    = mGrid.getVehiclePointer(idx);
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = (*ppVehicle)->type;
    
   if (r01_Motivated(idx)){
    if( r01A_Motivated(idx, RIGHT2LEFT) ){
       
        if (r05_Change(idx, RIGHT2LEFT)){
            if (getRandom() <= type->left_p){
                (*ppVehicle)->vyNew--;
                return;
            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
    
    
    
    if(r01A_Motivated(idx, LEFT2RIGHT) ){
        if (r05_Change(idx, LEFT2RIGHT)){
            if (getRandom() <= type->right_p){
                (*ppVehicle)->vyNew++;
            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
   }
   
    
    
}
void T_UFRRJ_MR::asymmetric(int idx){
   /*
    Vehicle *pVehicle    = mGrid.getVehiclePointer(idx);
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = (*ppVehicle)->type;
    int x = (*ppVehicle)->x;
    int y = (*ppVehicle)->y;
    int l = static_cast <int> ((*ppVehicle)->lg);
    
    
    bool changed = false;
    
    float factor = r01C_Motivated(idx, RIGHT2LEFT);
    
    bool blocked = r01B_Motivated(idx);
    
    if( ( (getRandom() <= (type->left_p * factor)) && r01_Motivated(idx, RIGHT2LEFT)  && !changed) || blocked ) {
        if (r05_Change(idx, RIGHT2LEFT)){
            if (!getGrid()->atomic(x, y-1, l)){
                (*ppVehicle)->vy--;
                changed = true;
            }
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
    
    
    factor = r01C_Motivated(idx, LEFT2RIGHT);
    
    int iMyY = static_cast<int> (pVehicle->y),
    iMyX = pVehicle->x;
    
    
    if ( ( (getRandom() <= (type->right_p * factor)) && r03_Motivated(idx) && !changed ) || blocked ) {
        if (r05_Change(idx, LEFT2RIGHT)){
            if (!getGrid()->atomic(x, y+1, l)){
                (*ppVehicle)->vy++;
                changed = true;
            }
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
    
    
    
    if ((getRandom() <= (type->right_p * factor)) && (r02_Motivated(idx) && !changed)) {
        if (r05_Change(idx, LEFT2RIGHT)){
            if (!getGrid()->atomic(x, y+1, l)){
                (*ppVehicle)->vy++;
                changed = true;
            }
        }//end- if (r05_Change(idx, LEFT2RIGHT)){
        
    }//end-if( r02_Motivated(idx) && !changed) {
    
    */
    
    
    
}

void T_UFRRJ_MR::apply(int idx, bool changing){
    
    if (changing){
/*        
#ifdef ASYMMETRIC
#warning "ASYMMETRIC defined"
        asymmetric(idx);
#else
#warning "SYMMETRIC defined"
        
#endif
*/ 
      symmetric(idx);
    }else
        T_UFRRJ::apply(idx, changing);
    
    
};

void T_UFRRJ_MR::finalizer(void){
    cerr << __FUNCTION__ << endl;
    T_UFRRJ::finalizer();
    
};
