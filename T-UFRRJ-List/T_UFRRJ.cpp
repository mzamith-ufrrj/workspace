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


void T_UFRRJ::movementRules(Vehicle *pVehicle){
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
   
   Vehicle **ppVehicle  = &pVehicle;
   tpVehiclesType *type = (*ppVehicle)->type;
   
   alpha = betaFunction(type->param),
   beta  = 1.0f - betaFunction(type->param + 5);
          
   deltaV    =  (*ppVehicle)->type->inc;
   dheadway  =  (*ppVehicle)->type->aheadInt;
   dsafedist =   (*ppVehicle)->type->safeDist;
          
   iMax = min(mParam.vMax, (*ppVehicle)->type->vMax);
   getDistanceAndVelocity(pVehicle, &iDist, &iAheadVel);
   
  
   
   dVel = static_cast<double> ((*ppVehicle)->vx) + ( (deltaV * alpha) - 0.5f);
   //plot '2_4.txt' u (floor((4*$1)-0.5)):($2) w p
   dVel = floor(dVel);
   
   iVel = static_cast<int> (dVel);
   
   iVel = max(iVel, 0);
   
   iVel = min (iVel, iMax);
   
   
   
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

void T_UFRRJ::executeRules(Entity *entity, bool changing){

   if (entity->whoAmI().compare("Vehicle") == 0){
       movementRules(static_cast<Vehicle *> (entity));
   }

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



void T_UFRRJ::getDistanceAndVelocity(Vehicle *v, int *dist, int *vel){

   //tpVehiclesType *type = mGrid.getVehicles(idx).type;
   int x = v->x,
       y = v->y,
       iDist = 0;
   Entity *cell = NULL;

   do
   {
      x++;
      iDist++;
      if (x == mParam.cellX)
         x = 0;
      cell = mGrid.getGrid(x, y);

   } while ((cell == EMPTY) && (iDist <= mParam.vMax));

   if (iDist >= mParam.vMax   ){
         *vel = mParam.vMax; //Significa que nao tem veiculo
   }else{
         
         if (cell->whoAmI().compare("Vehicle") == 0){
            Vehicle *v = static_cast<Vehicle *> (cell);
            *vel = v->vx;
         }else
            *vel = 0;

      
   }

   
   iDist--;
   if (iDist < 0)
      iDist = 0;

   *dist = iDist;

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
mStatistic(false),
inFlow(0.0f){
    //mIndexDensity(0){
};

T_UFRRJ_ML_SYM::~T_UFRRJ_ML_SYM(){
#ifdef _DEBUG
        std::cerr << __FILE__ << ":" << __FUNCTION__ << std::endl;
#endif
    
};


void T_UFRRJ_ML_SYM::finalizer(void){
    cerr << __FUNCTION__ << endl;
    T_UFRRJ::finalizer();
    
}

inline int T_UFRRJ_ML_SYM::inFlow(void){
    float inPart = 0.0f,
          rest = 0.0f;
    if (mParam.inFlow < 0.0f)
       return 0;
    
    
    inFlow += mParam.inFlow;
    
    rest = modf(inFlow, inPart);
    
    if (inPart > 0){
       //create vehicles
       inFlow = rest;
       return static_cast<int> (inPart);
   }
   
   return 0
   
    
}

void T_UFRRJ_ML_SYM::invertVehicles(int nVehicles){
   
   
}

/**
 * Apply change lane as well as postion definition
 * At last, updates
 */
void T_UFRRJ_ML_SYM::applyRules(int step, bool changing){
    
    //Change lanes
    Entity *ptr = mGrid.getEntities();
    while (ptr != NULL){
        executeRules(ptr, !changing);
        ptr = ptr->next;
    }
    update(step,  !changing);
   
    //Move vehicles
    ptr = mGrid.getEntities();
    while (ptr != NULL){
        executeRules(ptr, changing);
        ptr = ptr->next;
    }
    update(step,  changing);
   
    //After reposition all vehicles on the latticle.
    int iInflow = inFlow();
    if (iInflow > 0){
        invertVehicles(iInFlow);
    }

    
   
    
}

/**
 * Apply symmetric rules or simple ahead movement based on T_UFRRJ
 */ 
void T_UFRRJ_ML_SYM::executeRules(Entity *entity, bool changing){
    
    if (changing){
        if (entity->whoAmI().compare("Vehicle") == 0){
            changeLaneRules(static_cast<Vehicle *> (entity));
        }
    }else
        T_UFRRJ::movementRules(static_cast<Vehicle *> (entity));
    
    
}



void T_UFRRJ_ML_SYM::changeLaneRules(Vehicle *pVehicle){
   
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = (*ppVehicle)->type;
    int x = (*ppVehicle)->x,
        y = (*ppVehicle)->y,
        l = (*ppVehicle)->lg;
    
    bool changed = false;
    bool bestFlow = rule_BestFlow(pVehicle, RIGHT2LEFT);
    
    
    if(rule_MovingFaster(pVehicle)  && !changed && bestFlow){
        if (rule_Safe(pVehicle, RIGHT2LEFT)){
            if (getRandom() <= type->left_p){
                  if (!getGrid()->atomic(x, y-1, l)){
                     (*ppVehicle)->vy = -1;
                     changed = true;
                  }//end-if (!getGrid()->atomic(x, y-1, l)){
            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
    
    
    bestFlow = rule_BestFlow(pVehicle, LEFT2RIGHT);
    
    if(rule_MovingFaster(pVehicle) && !changed && bestFlow){
        if (rule_Safe(pVehicle, LEFT2RIGHT)){
            if (getRandom() <= type->right_p){
                if (!getGrid()->atomic(x, y+1, l)){
                  (*ppVehicle)->vy = +1;
                  changed = true;
                   
               }//end-if (!getGrid()->atomic(x, y+1, l)){
                
                
            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
}



/*
 * Verify if the destination lane presents best flow
 */ 
bool T_UFRRJ_ML_SYM::rule_BestFlow(Vehicle *pVehicle, int iDir){
    
    Entity *entity;
    Vehicle *vBesideVehicle;
    
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = (*ppVehicle)->type;
    
    int iMyY = static_cast<int> ((*ppVehicle)->y),
        iMyX = (*ppVehicle)->x,
        iBesideDistance = 0,
        iLen =  type->size ,
        iVel = (*ppVehicle)->vx,
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
        
        
        if (mGrid.getGrid(iMyX, iMyY) != EMPTY)
            return false;
    }
    
    iMyX = (*ppVehicle)->x;
    //iAheadDistance = 0;
    do{
        iMyX++;
        iBesideDistance++;
        if (iMyX == mParam.cellX)
            iMyX = 0;
        
        entity =  mGrid.getGrid(iMyX, iMyY);
        
        
    }while ((entity == EMPTY) && (iBesideDistance <= mParam.cellX));
    
    if (iBesideDistance > mParam.vMax)
        return true;
    
    if (iBesideDistance > 0)
        iBesideDistance--;
    
    if (entity->whoAmI().compare("Vehicle") != 0)
       return false;
    
    
    vBesideVehicle =  static_cast<Vehicle *> (entity);
    iVelBeside = vBesideVehicle->vx;
    
    if (iVelBeside <= 0)
        return false;
    
    
    if (iVel <=  iVelBeside)
        return true;
    
    
    return false;
    
    
}



/*
 * Being motivated whenever the driver cannot move in desired fVelocity
 * In case of symmetric hule, this motivation can be applied for both sides
 * Left to Right - Right to Left
 *
 */
bool T_UFRRJ_ML_SYM::rule_MovingFaster(Vehicle *pVehicle/*, int vIndex*/){
    
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
   
   Vehicle **ppVehicle  = &pVehicle;
   tpVehiclesType *type = (*ppVehicle)->type;
   
   alpha = betaFunction(type->param);
   beta  = 1.0f - betaFunction(type->param + 5);
          
   deltaV    =  (*ppVehicle)->type->inc;
   dheadway  =  (*ppVehicle)->type->aheadInt;
   dsafedist =   (*ppVehicle)->type->safeDist;
          
   iMax = min(mParam.vMax, (*ppVehicle)->type->vMax);
   getDistanceAndVelocity(pVehicle, &iDist, &iAheadVel);
   
   
   
   dVel = static_cast<double> ((*ppVehicle)->vx) + ( (deltaV * alpha) - 0.5f);
   //plot '2_4.txt' u (floor((4*$1)-0.5)):($2) w p
   dVel = floor(dVel);
   
   iVel = static_cast<int> (dVel);
   
   iVel = max(iVel, 0);
   
   iVel = min (iVel, iMax);
   dVel = static_cast<double> (iVel);
   if (fabs(dVel - static_cast<double> (iAheadVel)) < ERROR)
       return false;
   
   dTH = static_cast<double>(iDist) / (dVel - static_cast<double> (iAheadVel)); 
   
   if (( dTH >  0.0f) && (dTH <= (dheadway * beta))){
    ds = (dsafedist * beta) ;//* dTH    ;
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
 * Safe rules, verify if the car can chance, it he fits on the destination lane
 */ 
bool T_UFRRJ_ML_SYM::rule_Safe(Vehicle *pVehicle, int iDir){
 
    Entity *entity = NULL;
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = (*ppVehicle)->type;
    
    int iMyY = static_cast<int> ((*ppVehicle)->y),
        iMyX = (*ppVehicle)->x,
        iBackDistance = 0,
        iAheadDistance = 0,
        iLen =  type->size ,
        iVel =  ((*ppVehicle)->vx),
        iAheadVel = 0,
        iBackVel = 0;
    
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
        
        entity = mGrid.getGrid(iMyX, iMyY);
        if (entity != EMPTY)
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
        
        entity =  mGrid.getGrid(iMyX, iMyY);
        
        
    }while ((entity == EMPTY) && (iAheadDistance <= mParam.cellX));
    
    
    
    if (iAheadDistance > mParam.cellX)
        return true;
    
    if (iAheadDistance > 0)
        iAheadDistance--;
    
     
    if (entity->whoAmI().compare("Vehicle") == 0){
       Vehicle *v = static_cast<Vehicle *> (entity);
       iAheadVel = v->vx;
    }else
       iAheadVel = 0;
    
    //Find back  vehicle
    iMyX = (*ppVehicle)->x - iLen;
    iBackDistance = 0;
    //Finding the previous vehicle
    do{
        iMyX--;
        iBackDistance++;
        if (iMyX < 0)
                iMyX = mParam.cellX - 1;
        
        entity = mGrid.getGrid(iMyX, iMyY);
        //assert(iAheadDistance < 101);
    }while ((entity == EMPTY) && (iBackDistance <= mParam.cellX));
    
    
    if (iBackDistance > mParam.cellX)
        return true;
    
    if (entity->whoAmI().compare("Vehicle") == 0){
       Vehicle *v = static_cast<Vehicle *> (entity);
       iBackVel = v->vx;
    }else
       iBackVel = 0;

    
    if (iBackDistance > 0)
        iBackDistance--;
    
    //#1
    
    {
        //double alpha     = betaFunction(type->param);
        double beta      = 1.0f - betaFunction(type->param + 5);
        double dheadway  =  type->aheadInt;
        
        int diffV = iBackVel - iVel;
        if (diffV > 0){
            double dTH = static_cast<double>(iBackDistance) / static_cast<double>(diffV);
            
            if (dTH < (dheadway * beta))
                return false;
            
        }//END-if (diffV > 0){
            
        
    }
    
    
    

    //#2
    
    {
        //double alpha     = betaFunction(type->param);
        double beta      = 1.0f - betaFunction(type->param + 5);
        double dheadway  =  type->aheadInt;
        
        int diffV = iVel - iAheadVel;
        if (diffV != 0){
            double dTH = static_cast<double>(iAheadDistance) / static_cast<double>(diffV);
            if (dTH < (dheadway * beta))
                return false;
            
            
        }//end-if (diffV != 0){
            
    }
    
    
//    if (iVel > iAheadDistance)
  //    return false;
    
    return true;
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

void T_UFRRJ_ML_ASYM::changeLaneRules(Vehicle *pVehicle){
   
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = (*ppVehicle)->type;
    int x = (*ppVehicle)->x,
        y = (*ppVehicle)->y,
        l = (*ppVehicle)->lg;
    
    bool changed = false;
    bool bestFlow = rule_BestFlow(pVehicle, RIGHT2LEFT);
    
    if(rule_MovingFaster(pVehicle)  && !changed && bestFlow){
        if (rule_Safe(pVehicle, RIGHT2LEFT)){
            if (getRandom() <= type->left_p){
                  if (!getGrid()->atomic(x, y-1, l)){
                     (*ppVehicle)->vy = -1;
                     changed = true;
                  }//end-if (!getGrid()->atomic(x, y-1, l)){
            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
    
    
    
    //bestFlow = rule_BestFlow(idx, LEFT2RIGHT);
    if(rule_FasterBehind(pVehicle) && !changed ){
        if (rule_Safe(pVehicle, LEFT2RIGHT)){
            if (getRandom() <= type->right_p){
                if (!getGrid()->atomic(x, y+1, l)){
                  (*ppVehicle)->vy = +1;
                  changed = true;
                   
               }//end-if (!getGrid()->atomic(x, y+1, l)){
                
                 
            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
    
    
     bestFlow = rule_BestFlow(pVehicle, LEFT2RIGHT);
    if(rule_FasterAhead(pVehicle) && !changed && bestFlow){
        if (rule_Safe(pVehicle, LEFT2RIGHT)){
            if (getRandom() <= type->right_p){
                if (!getGrid()->atomic(x, y+1, l)){
                  (*ppVehicle)->vy = +1;
                  changed = true;
                   
               }//end-if (!getGrid()->atomic(x, y+1, l)){
                
                
            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
}


/*
 * r03_Motivated rule makes drivers change to slower lane.
 * r03_Motivated rule takes into account ahead vehicle.
 * In case of Brazil, it is right lane.
 */
bool T_UFRRJ_ML_ASYM::rule_FasterAhead(Vehicle *pVehicle){
    
   double   dVel = 0.0f,
            dTH   = 0.0f,
            deltaV    = 0.0f,
            dheadway  = 0.0f,
          
            alpha = 0.0f;
         
   int iVel = 0,
       iDist = 0,
       iAheadVel = 0,
       iMax = 0;
   
   Vehicle **ppVehicle  = &pVehicle;
   tpVehiclesType *type = (*ppVehicle)->type;
   
   alpha = betaFunction(type->param),
   
          
   deltaV    =  (*ppVehicle)->type->inc;
   dheadway  =  (*ppVehicle)->type->aheadInt;
          
   iMax = min(mParam.vMax, (*ppVehicle)->type->vMax);
   getDistanceAndVelocity(pVehicle, &iDist, &iAheadVel);
   
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
   
    double alpha2 = 1.0f + alpha;
    if (( dTH >  0.0f) && (dTH <= (dheadway * alpha2))){
        return true;
       
    }
   
    return false;

}

/*
 * Brief: back vehicle is getting closer, the analyzed one becomes
 * motived to change the lane
 */ 

bool T_UFRRJ_ML_ASYM::rule_FasterBehind(Vehicle *pVehicle){
    double   dVel = 0.0f,
             dTH   = 0.0f,
             deltaV    = 0.0f,
             dheadway  = 0.0f,
             alpha = 0.0f;
    
    //Veículo que vem detrás
    Entity *entity = NULL;
    Vehicle *vBackVehicle = NULL;
   
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = NULL;
    
    int iVel       = 0,
        //iBackVel   = 0,
        iBackDist  = 0,
        iMax = 0;
    
    entity = getBack(&iBackDist, (*ppVehicle)->x, (*ppVehicle)->y, (*ppVehicle)->lg);
    
    if (entity->whoAmI().compare("Vehicle") != 0)
       return true;
    
    vBackVehicle = static_cast<Vehicle*> (entity);
   
    type = vBackVehicle->type;
    
     iMax = min(mParam.vMax, type->vMax);
       
    alpha = betaFunction(type->param),
     
          
    deltaV    =  type->inc;
    dheadway  =  type->aheadInt;
   
    dVel = static_cast<double> (vBackVehicle->vx) + ( (deltaV * alpha) -   0.5f);
    
    dVel = floor(dVel);
   
    iVel = static_cast<int> (dVel);
   
    iVel = max(iVel, 0);
   
    iVel = min (iVel, iMax);
   
    dVel = static_cast<double> (iVel);
    int auxv = pVehicle->vx;
    
    if (fabs((dVel - static_cast<double>(auxv))) <= ERROR)
        return false;
    
    dTH = static_cast<double>(iBackDist) / (dVel - static_cast<double>(auxv)); //+ (deltaV * alpha));
    
    double alpha2 = 1.0f + alpha;
    if (( dTH >  0.0f) && (dTH <= (dheadway * alpha2))){
        return true;
       
    }
   
    return false;

    
}

Entity * T_UFRRJ_ML_ASYM::getBack(int *out, 
                                  int myX,
                                  int myY,
                                  int myLg){

   int x = myX - myLg,
       y = myY,
       iDist = 0;
   Entity *entity = NULL;

   //x++;
   do{

      if (x < 0)
         x = mParam.cellX + x;


      entity = getGrid()->getGrid(x, y);
      if (entity == EMPTY)
      {
          iDist++;
          x--;
      }
   } while  (entity == EMPTY);


   *out = iDist;

   return entity;



}