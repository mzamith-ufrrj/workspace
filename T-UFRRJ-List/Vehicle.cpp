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
#include <cstring>
#include <fstream>

using namespace std;

   
Entity::Entity():
 ID(0),
 
 x(0),
 y(0),
 lg(0)
{}

Entity::~Entity(){}

string Entity::whoAmI(void){ return AmI; }
void Entity::setwhoAmI(string who){ AmI = who; }

//--------------------------------------------------------------------------------------------
 
Blocked::Blocked():
Entity()
{
   
}
Blocked::~Blocked(){
   
}
   
string Blocked::whoAmI(void){ return "Blocked" ; }
 

//--------------------------------------------------------------------------------------------

Vehicle::Vehicle():
Entity(),
 mCellX(0),
 mCellY(0),
 vx(0),
 vxNew(0),
 vy(0),
 myDensity(0.0f),
 mStates(0),
 type(NULL),
 yChange(false){};



Vehicle::~Vehicle(){};

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
    
    /*
    bool log = false;
    
    string substring = to_string(LID) + ",";
    string myStr = string(type->vehicleLog);
    
    log = (myStr.find(substring) !=  string::npos);
    
    if (log){
      std::fstream output;
      string fileName = "V." + string(type->description) + "." + to_string(LID) + "." + to_string(myDensity) + ".txt";
      output.open(fileName.c_str(), fstream::out | fstream::app);
      assert(output.is_open());   
            
      output << time << " " << \
                x    << " " << \
                y    << " " << \
                vx   << " " << \
                vy   << endl;
            
      output.close();
    }
    */
    
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

string Vehicle::whoAmI(void) { return "Vehicle"; }