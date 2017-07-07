/*
 *  Grid.h
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/16/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _GRID_H_
#define _GRID_H_
#include <ModelTypes.hpp>
#include <Vehicle.hpp>
#include <string>
#include <vector>
using namespace std;
/**
 * @file
 * @author  Marcelo Zamith <mzamith.prof@gmail.com>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Grid class has information about each cell of cellular automata applied for TCA
 */
class Grid
{
public:
    Grid();
    ~Grid();
    void finalize(void);
    //void reset(void);
   // void clearGAPSvet(void);
    void allocGrid(int, int);
    void setGrid(int , int , Entity *);
    void setGrid_boolean(int x, int y);
    Entity* getGrid(int, int);
    bool getGrid_boolean(int x, int y);
    bool atomic(int, int, int);

    void addEntity (Entity*);
    Entity *getEntities(void) { return entities; };
    
    
    void allocVehicleType(int);
    void addVehicleType(tpVehiclesType, int);
    tpVehiclesType *getVehicleType(int);
   
    
    int getVehiclesTypeSize(void) { return sVehiclesType; };
    
    void clearGrid(void);
    void clearMemories(void);
 
    int getCellXi(void){ return cellX; }  
    int getCellYi(void){ return cellY; }
    float getCellXf(void){ return static_cast <float> (cellX); }
    float getCellYf(void){ return static_cast <float> (cellY); }
    
    int getID(void) { return iID++; }
    
private:
    void clearVehiclesTypes(void);
   //São os vetores de dados que devem ser atualizados para cada passo de simulação.
    //Vehicle *vehicle;   /* <! vehicles represention > */
    Entity *entities;
    //Entity  **entity;    /* <! Any other entity in latticle such as blocked cells or traffic light > */
    
    Entity  **grid;      /* <! grid represention > */
    bool    *grid_boolean; /* <! it is a representation of the latticle used for change lane rules, in case of more than 2 lanes > */
    
    int     cellX,      /* <! X represention > */
            cellY;      /* <! Y represention > */

    int  sVehiclesType;
    
    int  iID;  /* Vehicle ID */
    

    tpVehiclesType *vehiclesType;  /* <! Different kind of vehicles - data strucutre > */
    
     
    


};
#endif