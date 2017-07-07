#ifndef _MOVEMENTSENSOR_HPP_
#define _MOVEMENTSENSOR_HPP_
#include <vector>
#include <Grid.hpp>
#include <ModelTypes.hpp>
#include <string>
#include <cstring> 

using namespace std;
class MovementSensor
{
   
public:
   
   MovementSensor();
   ~MovementSensor();
   
   
   void setGrid(Grid* p){mGrid = p;};
   Grid *getGrid(void){ return mGrid; };
   void setFileName(string f){mFileName = f; };
   
   void saveStatistic(int);
   void update(void);
   void reset(void);

protected:
    Grid *mGrid;
  //Grid *mGrid;
   vector<float>   mS,
                   mD,
                   mV;
   float mSamples;
   
   string mFileName;
    
   
};

#endif
