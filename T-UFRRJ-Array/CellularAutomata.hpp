#ifndef _CELLULARAUTOMATA_H_
#define _CELLULARAUTOMATA_H_
#include <iostream>
#include <fstream>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <Vehicle.hpp>
#include <Rules.hpp>
#include <MovementSensor.hpp>
#include <ModelTypes.hpp>
#include <Grid.hpp>


//Site do NDC - UFF - Possivelmente site do portal, incricao e treinamento uff.
//lado esquerdo da tela.

/*
 *  CellularAutomata.h
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *  This class has the basic structure as well as transition rules
 *
 */

using namespace std;

class CellularAutomata
{
    
public:
    CellularAutomata();
    ~CellularAutomata();
    void init(const string&);
    void update(void);
    
    void finalize(void);   
    void debugAllVehicles(void);
    void debug(int);
    //Save log methods
    void  openLog(void);
    void  openLog(int, float);
    void  saveLog(void);
    void  closeLog(void);
    bool  isRunning (void) {return mRunning; };

   
protected:
    //void defineVar(char *, char *);
    void loadConfigFile(const string&);

   //These objects compose a cellular automata
   Rules                            *mRules;
   
   

   //For saving log
   vector <tpTableDensityMem>       mLogTable;       //Vetor com a tabela de ponteiros para indicar onde cada tempo de simulação começa termina e seus tempos tb
   //map<std::string, std::string>    mEnvironmentVariables;
   fstream                          mOutputState;
   unsigned long                    mMemoryUsed;
   bool                             mSaveMode;
   bool                             mRunning;
   int                              mState;
   int                              mpStep;
   int                              mStep;
   float                            mdGlobal;
   
};
#endif
