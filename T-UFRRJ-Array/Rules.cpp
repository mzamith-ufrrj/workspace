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
    mGrid.allocVehicle(totalVehicles, dGlobal);
    
    int iID = 0;
   
    for (int road = 0; road < mParam.cellY; road++){
        int cell = 0;
        for (int i = 0; i < mGrid.getVehiclesTypeSize(); i++){
            tpVehiclesType *type  = mGrid.getVehicleType(i);
            int length    = type->size;
            int iLocalID  = 1;
            int nVehicles = vehiclesTypes[i];
            for(int iveicles = 0; iveicles < nVehicles; iveicles++){
                Vehicle vehicle;
                
                vehicle.ID = iID;
                vehicle.LID = iLocalID;
                vehicle.x = cell + length - 1;
                vehicle.vx = 0;
                vehicle.vxNew = 0;
                vehicle.y = road;
                vehicle.vy = 0;
                vehicle.yChange = false;
                vehicle.lg = type->size;
                vehicle.myDensity = dGlobal;
                vehicle.type = mGrid.getVehicleType(i);
                vehicle.defineLattice(mParam.cellX, mParam.cellY);
                
                 
                
                mGrid.addVehicles(vehicle, iID);
                iID++;
                iLocalID++;
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

void Rules::getBack(int *out, Vehicle *vehicle, int idx){

   int x = (mGrid.getVehicles(idx).x) - static_cast<int>(mGrid.getVehicles(idx).lg),
       y = mGrid.getVehicles(idx).y,
       iDist = 0,
       iCell = -2;

   //x++;
   do{

      if (x < 0)
         x = mParam.cellX + x;


      iCell = mGrid.getGrid(x, y);
      if (iCell == -1)
      {
          iDist++;
          x--;
      }
   } while  (iCell == -1);



   *vehicle = mGrid.getVehicles(iCell);

   *out = iDist;


};



int Rules::getDistanceWithVehicle(int idx, Vehicle *v)
{

   //tpVehiclesType *type = mGrid.getVehicles(idx).type;
   int x = mGrid.getVehicles(idx).x,
       y = mGrid.getVehicles(idx).y,
       iDist = 0,
       cell = -2;
       //iMax = static_cast<int> (type->vMax);


   do
   {
      x++;
      iDist++;
      if (x == mParam.cellX)
         x = 0;
      cell = mGrid.getGrid(x, y);

   } while ((cell == -1) && (iDist <= mParam.cellX));

   if (iDist > mParam.cellX){
	   v->ID = 0xFFFFFFFF;
	   v->x = 0;
	   v->vx = static_cast <unsigned char> (mParam.vMax); //Significa que nao tem veiculo
   }else
	   *v = mGrid.getVehicles(cell);

   iDist--;
   if (iDist < 0)
      iDist = 0;

   return iDist;

};

void Rules::getDistanceAndVelocity(int idx, int *dist, int *vel)
{

   //tpVehiclesType *type = mGrid.getVehicles(idx).type;
   int x = mGrid.getVehicles(idx).x,
       y = mGrid.getVehicles(idx).y,
       iDist = 0,
       cell = -2;
       //iMax = static_cast<int> (type->vMax);


   do
   {
      x++;
      iDist++;
      if (x == mParam.cellX)
         x = 0;
      cell = mGrid.getGrid(x, y);

   } while ((cell == -1) && (iDist <= mParam.vMax));

   if (iDist >= mParam.vMax   ){
	   *vel = mParam.vMax; //Significa que nao tem veiculo
   }else
	   *vel = mGrid.getVehicles(cell).vx;

   iDist--;
   if (iDist < 0)
      iDist = 0;

   *dist = iDist;

};

int Rules::getDistance(int idx, Vehicle *v)
{

   tpVehiclesType *type = mGrid.getVehicles(idx).type;
   int x = mGrid.getVehicles(idx).x,
       y = mGrid.getVehicles(idx).y,
       iDist = 0,
       cell = -2,
       iMax = static_cast<int> (type->vMax);


   do
   {
      x++;
      iDist++;
      if (x == mParam.cellX)
         x = 0;
      cell = mGrid.getGrid(x, y);
   } while ( (cell == -1)  && (iDist < iMax + 1) ) ;

   if (v != NULL)
   {
      if (iDist == iMax + 1)
      {
         v->ID = 0xFFFFFFFF;
         v->x = 0;
         v->vx = 0xFF; //Significa que nao tem veiculo
         iDist = iMax+2;
      }else if (cell != -1)
      {
         *v = mGrid.getVehicles(cell);
      }
      else {
         std::cerr << "[ERROR] File: " << __FILE__ << std::endl;
         exit(0);
      }

   }

   iDist--;
   if (iDist < 0)
      iDist = 0;

   return iDist;

};


int Rules::getDistance2(int idx, Vehicle *v)
{
    
   //    tpVehiclesType *type = mGrid.getVehicles(idx).type;
   int x = mGrid.getVehicles(idx).x,
   y = mGrid.getVehicles(idx).y,
   iDist = 0,
   cell = -2;


   do
   {
      x++;
      iDist++;
      if (x == mParam.cellX)
         x = 0;
      cell = mGrid.getGrid(x, y);
   } while  (cell == -1);

   assert (v != NULL);
   *v = mGrid.getVehicles(cell);

   iDist--;
   if (iDist < 0)
      iDist = 0;

   return iDist;
    
};


bool Rules::normalPDF(float* xOut, float av, float sd){

   const float variable2 =  2.0f * (sd * sd);
   float x, y, y1;

   x  = static_cast <float> (rand() % 65535 + 1) / 65535.0f;

   y1 = static_cast <float> (rand() % 65535 + 1) / 65535.0f;


   y = exp( (((x - av) * (x - av)) / variable2) * -1.0f ) / (sd * SQRT2PI);

   if (y1 <= y){

      if (x > 1.0f)
         x  = 1.0f;

      if (x < 0.0f)
         x = 0.0f;
      *xOut = x;
      return true;
   }
   return false;

};


float Rules::normal (float av, float sd){
   const float variable2 =  2.0f * (sd * sd);
   float x, y, y1;
   do{
      x  = static_cast <float> (rand() % 65535 + 1) / 65535.0f;
      y1 = static_cast <float> (rand() % 65535 + 1) / 65535.0f;

      y = exp( (((x - av) * (x - av)) / variable2) * -1.0f ) / (sd * SQRT2PI);
   }while (y1 > y);

   if (x < 0.0f)
      x = 0.0f;

   if (x > 1.0f)
      x = 1.0f;

   return x;
};


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
void Rules::solve(void){};

/*
 * It is a method that depends on what king of information I would show.
 * It means: I can see on the screen or saved into a file.
 */
void Rules::debug(int time){
      cerr << "Time = " << time << endl;
   for (int j = 0; j < getParam()->cellY; j++){
      for (int i = 0; i < getParam()->cellX;  i++){
               int idx = getGrid()->getGrid(i, j);
         if (idx != -1){
               Vehicle *pVehicle    = mGrid.getVehiclePointer(idx);
               cerr << pVehicle->ID << "," << pVehicle->x << " , " << (int) pVehicle->y << endl;

         }
      }
   }
   cerr << "--------------------------------------------------------------------------------------------";
   cerr << endl;

};




/*
 * Used for defining correctly position of each vehicle
 */
void Rules::apply(int step, float density, bool changing){
//#pragma omp parallel num_threads(2)
//{
//   #pragma omp for
	for (int i = 0; i < mGrid.getVehiclesSize(); i++)
		apply(i);
//}

   update(step, density, changing);


};

void Rules::update(int step, float density, bool changing, bool initial){

   std::fstream output;

   //Updating all vehicles
   bool  statistic = (step >= getParam()->dTime) && (step > 0);
   getGrid()->clearGrid();

   for (int idx = 0;  idx < getGrid()->getVehiclesSize(); idx++){

      Vehicle *pVehicle = getGrid()->getVehiclePointer(idx);
      Vehicle **ppVehicle = &pVehicle;

      assert(ppVehicle != NULL);
      if (!initial){
         if (changing)
         (*ppVehicle)->updateY(statistic, step);
         else
         (*ppVehicle)->update(statistic, step);

      }
      


      int y = pVehicle->y;
      for (int k = 0 ; k <  static_cast <int> ( pVehicle->lg); k++){
         int x  =  pVehicle->x - k;
         if (x < 0)
            x = getParam()->cellX + x;

         assert(!changing);

         if ((getGrid()->getGrid(x, y) != -1) && (!changing)){
            std::cerr << "\t [ERROR] " << __FILE__ << " at " << __LINE__ << "\t Time:" << step <<  std::endl ;

            int mycar = getGrid()->getGrid(x, y);
            Vehicle v = getGrid()->getVehicles(mycar);

            // v.mBreakForeseenForeseenLight = true;
            std::cerr << "IDX: "<< mycar << " ";
            std::cerr << "V1(ID= " << v.ID << ",X= "
            << v.x  << ",Y= "
            << v.y  << ", VX="
            << (int)v.vx << std::endl;


            v = getGrid()->getVehicles(idx);
            std::cerr << "IDX: "<< mycar << " ";
            std::cerr << "V2(ID= " << v.ID << ",X= "
            << v.x  << ",Y= "
            << v.y  << ", VX="
            << (int)v.vx << std::endl;
            exit(-1);

         }

         getGrid()->setGrid(x, y, idx);

      }

   }


      if ((!changing) && (step > 0)){
         //Verifying if there is vehicle stopped on moviment detector cell.
         if (statistic){

            getGrid()->defineGaps();
            
            getSensor()->update();
            if ((step % getParam()->stTime) == 0){
               std::string txt = getGrid()->calculateGaps();
               getSensor()->saveStatistic(step, txt);
               getGrid()->clearGaps();
               
            }
         }//end- if (statistic){

      }

};




