/*
 * 
 *   plot 'statistic.SYNC-FASTER-4-2.txt' u 4:3 w p, 'statistic.SYNC-FASTER-4-2.txt' u 7:6 w p
 *   plot 'statistic.ML-ASYM-DEFAULT.txt' u ((($4+$7)/2)*133):($3/($3+$6)) w p, 'statistic.ML-ASYM-DEFAULT.txt' u ((($4+$7)/2)*133):($6/($3+$6)) w p
 * 
 */


#include <MovementSensor.hpp>
#include <cassert>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <ModelTypes.hpp>
using namespace std;
MovementSensor::MovementSensor()
{ };

MovementSensor::~MovementSensor(){
#ifdef _DEBUG
	std::cerr << __FILE__ << ":" << __FUNCTION__ << std::endl;
#endif

}
//This update method works quite well

void MovementSensor::saveStatistic(int timeInstant){
     std::fstream output;
     const char  *path = getenv ("PBS_O_WORKDIR");
     string fileName = "";
     if (path != NULL){
         fileName = path;
         fileName += "/";
     }
     
     fileName += "statistic." + mFileName + ".txt";
      
     output.open(fileName, std::fstream::out | std::fstream::app);
     assert(output.is_open());
     output << setw(10) << timeInstant << " " << setw(4) << mSamples << " ";
     output << fixed << setw(10) << setprecision(5);
     for (int i = 0; i < getGrid()->getCellYi(); i++){
       
        float v = mV[i] / mSamples;
        float d = mD[i] / mSamples;
        
        if (isnan(v))
            v = 0.0f;
        if (isnan(d))
            d = 0.0f;
        
        output << fixed << setw(10) << setprecision(5) <<    (v * d) <<  " " ;
        output << fixed << setw(10) << setprecision(5)  <<         d  <<  " " ;
        output << fixed << setw(10) << setprecision(5)  <<         v  <<  " " ;
        
        
         
         
         
     }
    
    output << endl;
    // output << gaps << endl;
     output.close();
    
     for (int i = 0; i < getGrid()->getCellYi(); i++){
       mS[i] = mD[i] = mV[i] =   0.0f;
     }

     mSamples = 0.0f;
}


void MovementSensor::reset(void){
 

  // mTime=0.0f;
   int y =  getGrid()->getCellYi();
   assert(y > 0);
   mS.resize(y);
   mD.resize(y);
   mV.resize(y);
    
   mSamples = 0.0f;
   

}

void MovementSensor::update(void){

    
    mSamples++;
    
    for (int y = 0; y < getGrid()->getCellYi(); y++){
        
        float v = 0.0f;
        float s = 0.0f;
        float c = 0.0f;
        for (int x = 0; x < getGrid()->getCellXi(); x++){
            Entity *e = getGrid()->getGrid(x, y);
            
            if (e != EMPTY){
               
               if (e->whoAmI().compare("Vehicle") == 0){
                  Vehicle *vehicle = static_cast<Vehicle*>(e);
                  v += static_cast<float> (vehicle->vx);
                  if (vehicle->yChange)
                     c++;
                  s++;

               }//end-if (e->whoAmI().compare("Vehicle") == 0){
            }//end-  if (idx != EMPTY){
            
        }//end-for (int x = 0; x < getGrid()->getCellXi(); x++){
        float av = v / s;
        float ad = (s / getGrid()->getCellXf());
        mV[y] += av;
        mD[y] += ad;
        mS[y] += s;
        
    }//end-for (int y = 0; y < getGrid()->getCellYi(); y++){
    
    
    Entity *ptr = getGrid()->getEntities();
        while (ptr != NULL){
        if (ptr->whoAmI().compare("Vehicle") == 0){
            Vehicle *vehicle = static_cast<Vehicle *> (ptr);
            Vehicle **ppVehicle  = &vehicle;
            (*ppVehicle)->yChange = false;
        }
        ptr = ptr->next;
    }
  
}

