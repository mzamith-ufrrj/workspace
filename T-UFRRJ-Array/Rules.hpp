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

    static int const UNIFORM = 1;
    static int const NORMAL  = 2;

    static int const CAR = 0;
    static int const BLOCKED = 1;
    static int const BUS = 2;


    virtual void initialCondition(float);


    void debug(int);

    virtual void apply(int, float, bool=false);
    virtual void apply(int, bool = 0 ) = 0;
    virtual void finalizer(void);
    virtual void solve(void);

    virtual void update(int, float, bool=false, bool=false);
    

    std::string getModelName(void)  {return mModelName; };
    Grid * getGrid(void)            {return &mGrid; };
    tpParam * getParam(void)        {return &mParam; };
    MovementSensor *getSensor(void) {return &mSensor; };

    int trunc (float);
    int truncFloor(float);


protected:

    bool normalPDF(float*, float, float);
    float normal (float , float);
    float getRandom(void);
    float alphaFunction(float);
    void  getBack(int *, Vehicle *, int);
    int   getDistanceWithVehicle(int, Vehicle *);
    void  getDistanceAndVelocity(int, int *, int *);
    int   getDistance(int, Vehicle* = NULL);
    int   getDistance2(int, Vehicle*);



   Grid                             mGrid;
   tpParam                          mParam;
   MovementSensor                   mSensor;
   std::string                      mModelName;

   bool mStatistic;

   //int mMAX;
};



#endif
