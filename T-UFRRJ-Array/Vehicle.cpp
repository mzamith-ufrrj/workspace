/* 
 * File:   Vehicle.cpp
 * Author: marcelozamith
 * 
 * Created on October 20, 2010, 10:28 AM
 */

#include <Vehicle.hpp>
#include <cassert>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

Vehicle::Vehicle():
 mCellX(0),
 mCellY(0),
 mType(0),
 ID(0),
 LID(0),
 mStates(0),
 x(0),
 y(0),
 vx(0),
 vxNew(0),
 vy(0),
 lg(0),
 GAP(-1),
 vDef(-1),
 myDensity(0.0f),
 yChange(false){};



Vehicle::~Vehicle(){};

void Vehicle::clearParameters(void){ GAP = vDef = -1; }

//Updating in (Y)
void Vehicle::updateY(bool statistic, int time){
   
   if (vy != 0)
       yChange = true;
   y += vy;
   vy = 0;


 if(!statistic)      
      return;
  
   
    


};



void Vehicle::update(bool statistic, int time){
   
   vx = vxNew;
   int   ivl = vx,
         x1 = 0,
         x2 = 0,
         ilg =  lg;
   

    
    x1 = x + ivl;
    
    x += ivl;
    
     
    
    //Updating vehicle position
    if (x >= mCellX)
        x %= mCellX;
    
    vxNew = 0;
    
    if(!statistic)
        return;
    
    if (type->vehicleLog){
        if ((LID >= type->lo_ID) && (LID <= type->hi_ID)){
            string descrip = type->description;
            fstream output;
            string fileName = "V." + descrip + "." + to_string(LID) + "." + to_string(myDensity) + "." + type->sufix;
            output.open(fileName.c_str(), fstream::out | fstream::app);
            assert(output.is_open());   
            output << time << " " << \
                      x    << " " << \
                      y    << " " << \
                      vx   << " " << \
                      vy   << " " << \
                      GAP  << endl;
            
            output.close();


        }
    }
    

    
    if ((x1 >= mCellX) && (mStates == 0)){
        mStates = 1;
    }else  if (mStates == 1)
        x1 = mCellX + x1;
    
    
    
    x2 = x1 - ilg + 1;
    
    if (mStates == 0) return;
    
    
    if (x2 < mCellX)
        x2 = mCellX;

    

};


//----------------------------------------
//
//----------------------------------------
void Vehicle::defineLattice(int lx, int ly) { mCellX = lx; mCellY = ly; };
