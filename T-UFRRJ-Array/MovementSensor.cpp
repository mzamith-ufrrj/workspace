/*
 * 
 *   plot 'statistic.SYNC-FASTER-4-2.txt' u 4:3 w p, 'statistic.SYNC-FASTER-4-2.txt' u 7:6 w p
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

using namespace std;
MovementSensor::MovementSensor()
{ };

MovementSensor::~MovementSensor(){
#ifdef _DEBUG
	std::cerr << __FILE__ << ":" << __FUNCTION__ << std::endl;
#endif

}
//This update method works quite well

void MovementSensor::saveStatistic(int timeInstant, string gaps){
     std::fstream output;
     string fileName = "statistic." + mFileName;
      
     output.open(fileName, std::fstream::out | std::fstream::app);
     assert(output.is_open());
     output << timeInstant << " " << mSamples << " ";
     for (int i = 0; i < getGrid()->getCellYi(); i++){
       
        float v = mV[i] / mSamples;
        float d = mD[i] / mSamples;
        
        output << (v * d) <<  " " ;
        output <<  d      <<  " " ;
        output <<  v      <<  " " ;
        
        
        //output << mC[i] / mSamples << " ";
         
         
     }
    
     output << gaps << endl;
     output.close();
    
     for (int i = 0; i < getGrid()->getCellYi(); i++){
       mS[i] = mD[i] = mV[i] = mC[i] = 0.0f;
     }

     mSamples = 0.0f;
}


void MovementSensor::reset(void){
 

  // mTime=0.0f;
   int y =  getGrid()->getCellYi();
   mS.resize(y);
   mD.resize(y);
   mV.resize(y);
   mC.resize(y);
   mSamples = 0.0f;
   

}

void MovementSensor::update(void){

    
    mSamples++;
    
    for (int y = 0; y < getGrid()->getCellYi(); y++){
        
        float v = 0.0f;
        float s = 0.0f;
        float c = 0.0f;
        for (int x = 0; x < getGrid()->getCellXi(); x++){
            int idx = getGrid()->getGrid(x, y);
            if (idx != -1){
                 v+= static_cast<float>( getGrid()->getVehiclePointer(idx)->vx);
                 if (getGrid()->getVehiclePointer(idx)->yChange)
                     c++;
                 s++;
            }//end-if (idx != -1){
        }//end-for (int x = 0; x < getGrid()->getCellXi(); x++){
        float av = v / s;
        float ad = (s / getGrid()->getCellXf());
        mV[y] += av;
        mD[y] += ad;
        mS[y] += s;
        mC[y] += (c / s);
    }//end-for (int y = 0; y < getGrid()->getCellYi(); y++){
    
    
    for (int index = 0; index < getGrid()->getVehiclesSize(); index++){
            Vehicle *pVehicle    = getGrid()->getVehiclePointer(index);
            Vehicle **ppVehicle  = &pVehicle;
            (*ppVehicle)->yChange = false;
    }//end-for (int index = 0; index < mGrid.getVehiclesSize(); index++){
  
}

