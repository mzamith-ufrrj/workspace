/*
 *  Rules.h
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
//It is a file with only rules includes
#ifndef _RULES_H_
#define _RULES_H_
#include <set>
#include <vector>
#include <ModelTypes.hpp>
#include <Grid.hpp>
#include <Vehicle.hpp>
#include <MovementSensor.hpp>
#include <ctime>
#include <iostream>

using namespace std;
double gamaFunction (double n);
class Rules
{

public:
    Rules(const std::string m);
    ~Rules();

    static int const RIGHT2LEFT = 0;
    static int const LEFT2RIGHT = 1;

    virtual void initialCondition(float);


    void debug(int);

    virtual void applyRules(int, bool = false);
    virtual void executeRules(Entity *, bool = false ) = 0;
    virtual void finalizer(void);
 

    virtual void update(int, bool=false, bool=false); 
    

    std::string getModelName(void)  {return mModelName; };
    Grid * getGrid(void)            {return &mGrid; };
    tpParam * getParam(void)        {return &mParam; };
    MovementSensor *getSensor(void) {return &mSensor; };

    int trunc (float);
    int truncFloor(float);


protected:

     
    
    float getRandom(void);
    float alphaFunction(float);
    
     




   Grid                             mGrid;
   tpParam                          mParam;
   MovementSensor                   mSensor;
   std::string                      mModelName;

   bool mStatistic;

   //int mMAX;
};



#endif
