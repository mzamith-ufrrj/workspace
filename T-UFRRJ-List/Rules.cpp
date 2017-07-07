/*
 *  Rules.cpp
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 * plot '2012_06_22.TACUFF2-L1-A4acc-B8acc-A4ant-B8ant.txt' u 41:40 w p, '2012_06_22.TACUFF2-L1-A4acc-B8acc-A4ant-B8ant.txt' u 6:5 w p
 *
 */

#include <Rules.hpp>
#include <cassert>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <omp.h>
#define PI      (float) 3.1415926535897932384626433832795
#define PI2     (float) 2.0f*PI
#define SQRT2PI (float) sqrt(PI2)
using namespace std;
double gamaFunction (double n){
    
    unsigned long in1 = static_cast <unsigned long> (n-1),
    iacc = in1;
    
    if (n <= 2.0f) return 1.0f;
    
    for (unsigned long i = in1-1 ; i > 0 ; i--)
        iacc *= i;
    
    return static_cast <double> (iacc);
}


Rules::Rules(const std::string m):
mStatistic(false){
    mModelName = m;
    srand ( time(NULL) );
    
};

Rules::~Rules(){
   
};

/**
 *  --  Basic rules  --
 */
void Rules::initialCondition(float dGlobal){
    //Define how many vehicles will be on the road
    Vehicle vehicle;
/*
 * Observation 2017, March 18th 
 * For each vehicle Verifying if it is in a list of log vehicles vehicleLog
 * To do this: Verifying if there is a substring with the number.
 *             Set true vehicle parameter
 *             Put in Vehicle.cpp code to log the vehicle performance
 * 
 */ 
    int totalVehicles = 0,
    *vehiclesTypes = new int[mGrid.getVehiclesTypeSize()];
    
    
    for (int i = 0; i < mGrid.getVehiclesTypeSize(); i++){
        tpVehiclesType *type  = mGrid.getVehicleType(i);
        
        float dVehicle  = type->percent * dGlobal / static_cast<float> (type->size);
        
        int   nVehicles = static_cast <int> (dVehicle * static_cast <float> (mParam.cellX));
        
        if (nVehicles == 0) nVehicles = 1;
        
        totalVehicles += nVehicles;
        
        vehiclesTypes[i] = nVehicles;
    }
    totalVehicles *= mParam.cellY;
    
    getGrid()->clearMemories();
    
   
    for (int road = 0; road < mParam.cellY; road++){
        int cell = 0;
        for (int i = 0; i < mGrid.getVehiclesTypeSize(); i++){
            tpVehiclesType *type  = mGrid.getVehicleType(i);
            int length    = type->size;
            int nVehicles = vehiclesTypes[i];
            for(int iveicles = 0; iveicles < nVehicles; iveicles++){
                Vehicle *vehicle = new Vehicle();
                
                vehicle->ID = mGrid.getID();
                 
                vehicle->x = cell + length - 1;
                vehicle->vx = 0;
                vehicle->vxNew = 0;
                vehicle->y = road;
                vehicle->vy = 0;
                vehicle->yChange = false;
                vehicle->lg = type->size;
                vehicle->myDensity = dGlobal;
                vehicle->setwhoAmI("Vehicle");
                vehicle->next = NULL;
                vehicle->prev = NULL;
                vehicle->type = mGrid.getVehicleType(i);
                vehicle->defineLattice(mParam.cellX, mParam.cellY);
                mGrid.addEntity(vehicle);
                cell += length;
                
            }
            
        }
    }
    
     
    delete[] vehiclesTypes;
    vehiclesTypes = NULL;
    
    
    //Setting up sensor
    mSensor.setFileName(mParam.STFile);
    mSensor.setGrid(&mGrid);
    mSensor.reset();
    
    
    
};


float Rules::getRandom(void)
{ return static_cast <float> (rand() % 10000 + 1) / 10000.0f;};


float Rules::alphaFunction(float a)
{ return 1.0f-a; }; ///*powf((1-a), 2);*/ };





int Rules::trunc(float fValue){

   int iValue = 0;

   float fInt = 0.0f,
         frac = modf(fValue, &fInt);

   if (frac < 0.5f)
      iValue = static_cast <int> (fInt);
   else
      iValue = static_cast <int> (fInt+1.0f);


   return iValue;
};

int  Rules::truncFloor(float value){ 
   return static_cast <int> (floor(value));
}

void Rules::finalizer(void){
     getGrid()->finalize();
    
}
 

/*
 * It is a method that depends on what king of information I would show.
 * It means: I can see on the screen or saved into a file.
 */
void Rules::debug(int time){
   /*   
   cerr << "Time = " << time << endl;
   for (int j = 0; j < getParam()->cellY; j++){
      for (int i = 0; i < getParam()->cellX;  i++){
               int idx = getGrid()->getGrid(i, j);
         if (idx > EMPTY){
               Vehicle *pVehicle    = mGrid.getVehiclePointer(idx);
               cerr << pVehicle->ID << "," << pVehicle->x << " , " << (int) pVehicle->y << endl;

         }
      }
   }
   cerr << "--------------------------------------------------------------------------------------------";
   cerr << endl;
   */
};




/*
 * Used for defining correctly position of each vehicle
 */
void Rules::applyRules(int step,  bool changing){
 
    Entity *ptr = mGrid.getEntities();
    while (ptr != NULL){
        executeRules(ptr);
        ptr = ptr->next;
    }
 

   update(step,  changing);


};

void Rules::update(int step,  bool changing, bool initial){

   std::fstream output;

   //Updating all vehicles
   bool  statistic = (step >= getParam()->dTime) && (step > 0);
   getGrid()->clearGrid();
 
    Entity *pEntity = mGrid.getEntities();
    while (pEntity != NULL){
        

      if (pEntity->whoAmI().compare("Vehicle") == 0){
         Vehicle **ppVehicle = (Vehicle**) (&pEntity);
//-----------------------------------------------------------------------------
         assert(ppVehicle != NULL);
         if (!initial){ 
            if (changing)
            (*ppVehicle)->updateY(statistic, step);
            else
            (*ppVehicle)->update(statistic, step);

         }



         int y = pEntity->y;
         for (int k = 0 ; k <   pEntity->lg; k++){
            int x  =  pEntity->x - k;
            if (x < 0)
               x = getParam()->cellX + x;

           // assert(!changing);

            if ((getGrid()->getGrid(x, y) != EMPTY) && (!changing)){
               std::cerr << "\t [ERROR] " << __FILE__ << " at " << __LINE__ << "\t Time:" << step <<  std::endl ;

               Vehicle *v = static_cast<Vehicle*>(getGrid()->getGrid(x, y));

               // v.mBreakForeseenForeseenLight = true;
               std::cerr << "V1(ID= " << v->ID << ",X= "
               <<  v->x  << ",Y= "
               <<  v->y  << ", VX="
               <<  v->vx << std::endl;


               v = static_cast<Vehicle*>(getGrid()->getGrid(x, y)); 
               std::cerr << "V2(ID= " <<  v->ID << ",X= "
               <<  v->x  << ",Y= "
               <<  v->y  << ", VX="
               <<  v->vx << std::endl;
               exit(-1);

            }

            getGrid()->setGrid(x, y, pEntity);

         }//end-for (int k = 0 ; k <  static_cast <int> ( pVehicle->lg); k++){
//-----------------------------------------------------------------------------
      }//end-      if (pEntity->whoAmI().compare("Vehicle") == 0){
      pEntity = pEntity->next;
    }//end-for (int idx = 0;  idx < getGrid()->getEntitySize(); idx++){
 

   if ((!changing) && (step > 0)){
      //Verifying if there is vehicle stopped on moviment detector cell.
      if (statistic){

         
         
         getSensor()->update();
         if ((step % getParam()->stTime) == 0){
               
            getSensor()->saveStatistic(step );
            
            
         }
      }//end- if (statistic){

   }//end- if ((!changing) && (step > 0)){

};




