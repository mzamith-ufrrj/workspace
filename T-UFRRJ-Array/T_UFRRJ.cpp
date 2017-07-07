#include <T_UFRRJ.hpp>
#include <cassert>
#include <cmath>
#define ERROR (double) 1E-10
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

   (*ppVehicle)->vDef = vDef;
    (*ppVehicle)->GAP = GAP;
    
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

/*
 * Multilane rules based on symmetric rules.
 */ 

/*
    //////////////////////////////////
    // Highway direction
    // Initial --> Final = 0 is left and 1 is right
    // 0 - left
    // 1 - right
    //////////////////////////////////
*/
T_UFRRJ_ML_SYM::T_UFRRJ_ML_SYM(const std::string m):
T_UFRRJ(m),
mStatistic(false){
    //mIndexDensity(0){
};

T_UFRRJ_ML_SYM::~T_UFRRJ_ML_SYM(){
#ifdef _DEBUG
	std::cerr << __FILE__ << ":" << __FUNCTION__ << std::endl;
#endif
    
};

void T_UFRRJ_ML_SYM::update(int step, float density, bool changing){
    //Updating all vehicles
    mStatistic = (step >= getParam()->dTime) && (step > 0);

    getGrid()->clearGrid();
    
    for (int idx = 0;  idx < getGrid()->getVehiclesSize(); idx++)
    {
        
        Vehicle *pVehicle = getGrid()->getVehiclePointer(idx);
        Vehicle **ppVehicle = &pVehicle;
        
        assert(ppVehicle != NULL);
        if (changing)
            (*ppVehicle)->updateY(mStatistic, step);
        else
            (*ppVehicle)->update(mStatistic, step);
 
        int y = pVehicle->y;
        
        for (int k = 0 ; k <   pVehicle->lg; k++)
        {
            int x  =  pVehicle->x - k;
            if (x < 0)
                x = getParam()->cellX + x;
            
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
    }
    
    
    
    if ((!changing) && (step > 0)){
        
        //Verifying if there is vehicle stopped on moviment detector cell.
        if (mStatistic){
             getGrid()->defineGaps();
             getSensor()->update();
            if ((step % getParam()->stTime) == 0){
               std::string txt = getGrid()->calculateGaps();
               getSensor()->saveStatistic(step, txt);
               getGrid()->clearGaps();
                //getSensor()->statistics(step, density, getGrid()->calculateGaps());
            }
        }//end- if (statistic){

    }
    
}

/**
 * Apply change lane as well as postion definition
 * At last, updates
 */
void T_UFRRJ_ML_SYM::apply(int step, float density, bool changing){
    
    
    
	for (int i = 0; i < mGrid.getVehiclesSize(); i++)
		apply(i, true);
	update(step, density, true);
    
    
	for (int i = 0; i < mGrid.getVehiclesSize(); i++)
		apply(i);
	update(step, density);
    
    
    
}

/**
 * Apply symmetric rules or simple ahead movement based on T_UFRRJ
 */ 
void T_UFRRJ_ML_SYM::apply(int idx, bool changing){
    
    if (changing)
        changeLaneRules(idx);
    else
        T_UFRRJ::apply(idx, changing);
    
    
}

void T_UFRRJ_ML_SYM::changeLaneRules(int idx){
    Vehicle *pVehicle    = mGrid.getVehiclePointer(idx);
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = (*ppVehicle)->type;
    
    bool changed = false;
    bool bestFlow = rule_BestFlow(idx, RIGHT2LEFT);
    
    if(rule_MovingFaster(idx)  && !changed && bestFlow){
        if (rule_Safe(idx, RIGHT2LEFT)){
            if (getRandom() <= type->left_p){
                
                (*ppVehicle)->vy = -1;
                changed = true;
            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
    
    
    bestFlow = rule_BestFlow(idx, LEFT2RIGHT);
    if(rule_MovingFaster(idx) && !changed && bestFlow){
        if (rule_Safe(idx, LEFT2RIGHT)){
            if (getRandom() <= type->right_p){
                (*ppVehicle)->vy = +1;
                changed = true;
                
            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
}

/*
 * Safe rules, verify if the car can chance, it he fits on the destination lane
 */ 
bool T_UFRRJ_ML_SYM::rule_Safe(int idx, int iDir){
    Vehicle *pVehicle    = mGrid.getVehiclePointer(idx),
             vAheadVehicle,
              vBackVehicle;
    
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = (*ppVehicle)->type;
    
    
    int iMyY = static_cast<int> ((*ppVehicle)->y),
        iMyX = (*ppVehicle)->x,
        iBackDistance = 0,
        iAheadDistance = 0,
        iPtr = 0,
        iLen =  type->size ,
        iVel =  ((*ppVehicle)->vx);
    
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
    vAheadVehicle =  mGrid.getVehicles(iPtr);
    
    
    
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
    
    //#1
    
    {
        type = vBackVehicle.type;
        //double alpha     = betaFunction(type->param);
        double beta      = 1.0f - betaFunction(type->param + 5);
        double dheadway  =  type->aheadInt;
        
        int diffV = vBackVehicle.vx - iVel;
        if (diffV > 0){
            double dTH = static_cast<double>(iBackDistance) / static_cast<double>(diffV);
            
            if (dTH < (dheadway * beta))
                return false;
            
            
            
        }//end-if (diffV != 0){
        /*    
        assert(ds >= 0.0f);

        if (iBackDistance < iVel); 
            return false;
        */
        //The rule is based on temporal distance which considers as distance as velocity of both vehicles
        //Both vehicles move at the same velocity: diffV is zero and in this case it is safe to change
        //My vehicle is faster than previous, diffV is minor than zero. It is safe to change
        //It is important to not hit one to another when analyzed vehicle is slower than the previous one
    }
    
    
    

    //#2
    
    {
        type = (*ppVehicle)->type;
        //double alpha     = betaFunction(type->param);
        double beta      = 1.0f - betaFunction(type->param + 5);
        double dheadway  =  type->aheadInt;
        
        int diffV = iVel - vAheadVehicle.vx;
        if (diffV != 0){
            double dTH = static_cast<double>(iAheadDistance) / static_cast<double>(diffV);
            if (dTH < (dheadway * beta))
                return false;
            
            
        }//end-if (diffV != 0){
            
    }
    
    
//    if (iVel > iAheadDistance)
  //  	return false;
    
    return true;
}

/*
 * Being motivated whenever the driver cannot move in desired fVelocity
 * In case of symmetric hule, this motivation can be applied for both sides
 * Left to Right - Right to Left
 *
 */
bool T_UFRRJ_ML_SYM::rule_MovingFaster(int vIndex){
    
   double   dVel = 0.0f,
            dTH   = 0.0f,
            deltaV    = 0.0f,
            dheadway  = 0.0f,
            dsafedist = 0.0f,
//            deltaVMax = 50.0f,
            ds = 0.0f,
            alpha = 0.0f,
            beta  = 0.0f;
         
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
   
   
   dVel = static_cast<double> ((*ppVehicle)->vx) + ( (deltaV * alpha) - 0.5f);
   //plot '2_4.txt' u (floor((4*$1)-0.5)):($2) w p
   dVel = floor(dVel);
   
   iVel = static_cast<int> (dVel);
   
   iVel = max(iVel, 0);
   
   iVel = min (iVel, iMax);
   dVel = static_cast<double> (iVel);
   if (fabs(dVel - static_cast<double> (iAheadVel)) < ERROR)
       return false;
   
   dTH = static_cast<double>(iDist) / (dVel - static_cast<double> (iAheadVel)); //+ (deltaV * alpha));

   if (( dTH >  0.0f) && (dTH <= (dheadway * beta))){
    ds = (dsafedist * beta) ;//* dTH	;
   }
   

   assert(ds >= 0.0f);
   iDist -= truncCurve3(ds);

   if (iDist < 0)
      iDist = 0;

   if (iVel > iDist)
       return true;

   return false;
   
    
    
}

/*
 * Verify if the destination lane presents best flow
 */ 
bool T_UFRRJ_ML_SYM::rule_BestFlow(int idx, int iDir){
    
    
    Vehicle *pVehicle    = mGrid.getVehiclePointer(idx),
    vAheadVehicle,
    vBesideVehicle;
    
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = (*ppVehicle)->type;
    
    int iMyY = static_cast<int> ((*ppVehicle)->y),
    iMyX = (*ppVehicle)->x,
    iBesideDistance = 0,
    iPtr = 0,
    iLen =  type->size ,
    iVel = static_cast<int> ((*ppVehicle)->vx),
    iVelBeside = 0;
    
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
    
    if (iBesideDistance > mParam.vMax)
        return true;
    
    if (iBesideDistance > 0)
        iBesideDistance--;
    
    vBesideVehicle =  mGrid.getVehicles(iPtr);
    iVelBeside = static_cast<int> (vBesideVehicle.vx);
    
    
    
    //iBesideDistance += iVelBeside;
    
   
    
    
    if (iVelBeside <= 0)
        return false;
    
    
    
    
    //iBesideDistance += iVelBeside;
    
    if (iVel <=  iVelBeside)
        return true;
    
    
    return false;
    
    
}

void T_UFRRJ_ML_SYM::finalizer(void){
    cerr << __FUNCTION__ << endl;
    T_UFRRJ::finalizer();
    
}



//-------------------------------------------------------------------------------

/**
 * Apply symmetric rules or simple ahead movement based on T_UFRRJ
 */ 
T_UFRRJ_ML_ASYM::T_UFRRJ_ML_ASYM(const std::string m):
T_UFRRJ_ML_SYM(m){
    //mIndexDensity(0){
};

T_UFRRJ_ML_ASYM::~T_UFRRJ_ML_ASYM(){
#ifdef _DEBUG
	std::cerr << __FILE__ << ":" << __FUNCTION__ << std::endl;
#endif
    
};

void T_UFRRJ_ML_ASYM::changeLaneRules(int idx){
    Vehicle *pVehicle    = mGrid.getVehiclePointer(idx);
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = (*ppVehicle)->type;
    
    bool changed = false;
    bool bestFlow = rule_BestFlow(idx, RIGHT2LEFT);
    
    if(rule_MovingFaster(idx)  && !changed && bestFlow){
        if (rule_Safe(idx, RIGHT2LEFT)){
            if (getRandom() <= type->left_p){
                
                (*ppVehicle)->vy = -1;
                changed = true;
                 
                
            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
    
    
    //bestFlow = rule_BestFlow(idx, LEFT2RIGHT);
    if(rule_FasterBehind(idx) && !changed ){
        if (rule_Safe(idx, LEFT2RIGHT)){
            if (getRandom() <= type->right_p){
                (*ppVehicle)->vy = +1;
                changed = true;
                
                 
            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
    
    
     bestFlow = rule_BestFlow(idx, LEFT2RIGHT);
    if(rule_FasterAhead(idx) && !changed && bestFlow){
        if (rule_Safe(idx, LEFT2RIGHT)){
            if (getRandom() <= type->right_p){
                (*ppVehicle)->vy = +1;
                changed = true;
                
                
            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
}


/*
 * r03_Motivated rule makes drivers change to slower lane.
 * r03_Motivated rule takes into account ahead vehicle.
 * In case of Brazil, it is right lane.
 */
bool T_UFRRJ_ML_ASYM::rule_FasterAhead(int idx){
    
   double   dVel = 0.0f,
            dTH   = 0.0f,
            deltaV    = 0.0f,
            dheadway  = 0.0f,
            dsafedist = 0.0f,
//            deltaVMax = 50.0f,
            ds = 0.0f,
            alpha = 0.0f,
            beta  = 0.0f;
         
   int iVel = 0,
       iDist = 0,
       iAheadVel = 0,
       iMax = 0;
   
   Vehicle *pVehicle    = mGrid.getVehiclePointer(idx);
   Vehicle **ppVehicle  = &pVehicle;
   tpVehiclesType *type = (*ppVehicle)->type;
   
   alpha = betaFunction(type->param),
   beta  = 1.0f - betaFunction(type->param + 5);
          
   deltaV    =  (*ppVehicle)->type->inc;
   dheadway  =  (*ppVehicle)->type->aheadInt;
   dsafedist =   (*ppVehicle)->type->safeDist;
          
   iMax = min(mParam.vMax, (*ppVehicle)->type->vMax);
   getDistanceAndVelocity(idx, &iDist, &iAheadVel);
   if (iDist >= iMax)
       return true;
   
   
   dVel = static_cast<double> ((*ppVehicle)->vx) + ( (deltaV * alpha) - 0.5f);
   //plot '2_4.txt' u (floor((4*$1)-0.5)):($2) w p
   dVel = floor(dVel);
   
   iVel = static_cast<int> (dVel);
   
   iVel = max(iVel, 0);
   
   iVel = min (iVel, iMax);
   
   dVel = static_cast<double> (iVel);
   
   
    if (fabs((dVel - static_cast<double>(iAheadVel))) <= ERROR)
        return false;
   
   dTH = static_cast<double>(iDist) / (dVel - static_cast<double> (iAheadVel)); //+ (deltaV * alpha));

   if (( dTH >  0.0f) && (dTH <= (dheadway * beta))){
    ds = (dsafedist * beta) ;//* dTH	;
   }
   

   assert(ds >= 0.0f);
   iDist -= truncCurve3(ds);

   if (iDist < 0)
      iDist = 0;

   if (iVel <= iDist)
       return true;

   return false;
   
    
    
}

/*
 * Brief: back vehicle is getting closer, the analyzed one becomes
 * motived to change the lane
 */ 

bool T_UFRRJ_ML_ASYM::rule_FasterBehind(int idx){
    double   dVel = 0.0f,
        dTH   = 0.0f,
        deltaV    = 0.0f,
        dheadway  = 0.0f,
        dsafedist = 0.0f,
//            deltaVMax = 50.0f,
        ds = 0.0f,
        alpha = 0.0f,
        beta  = 0.0f;
    
    //Veículo que vem detrás
    Vehicle vBackVehicle;
    Vehicle *pVehicle    = mGrid.getVehiclePointer(idx);
    //Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = NULL;
    
    int iVel       = 0,
        //iBackVel   = 0,
        iBackDist  = 0,
        iMax = 0;
    
    getBack(&iBackDist, &vBackVehicle, idx);
    type = vBackVehicle.type;
    
     iMax = min(mParam.vMax, type->vMax);
       
    alpha = betaFunction(type->param),
    beta  = 1.0f - betaFunction(type->param + 5);
          
    deltaV    =  type->inc;
    dheadway  =  type->aheadInt;
    dsafedist =   type->safeDist;
   
    dVel = static_cast<double> (vBackVehicle.vx) + ( (deltaV * alpha) -   0.5f);
    
    dVel = floor(dVel);
   
    iVel = static_cast<int> (dVel);
   
    iVel = max(iVel, 0);
   
    iVel = min (iVel, iMax);
   
    dVel = static_cast<double> (iVel);
    int auxv = pVehicle->vx;
    
    if (fabs((dVel - static_cast<double>(auxv))) <= ERROR)
        return false;
    
    dTH = static_cast<double>(iBackDist) / (dVel - static_cast<double>(auxv)); //+ (deltaV * alpha));

    if (( dTH >  0.0f) && (dTH <= (dheadway * beta))){
     ds = (dsafedist * beta) ;//* dTH	;
    }
   

   assert(ds >= 0.0f);
   iBackDist -= truncCurve3(ds);

   if (iBackDist < 0)
      iBackDist = 0;

   if (iVel > iBackDist)
       return true;

   return false;
}



