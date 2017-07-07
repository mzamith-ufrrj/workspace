/*
 *  Grid.cpp
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/16/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <Grid.hpp>
#include <cassert>
#include <iostream>
Grid::Grid():
entities(NULL),
grid(NULL),
grid_boolean(NULL),
vehiclesType(NULL),
iID(0),
cellX(-1),
cellY(-1){};

Grid::~Grid()
{
#ifdef _DEBUG
	std::cerr << __FILE__ << ":" << __FUNCTION__ << std::endl;
#endif
    


};


void Grid::finalize(void){
   if (grid_boolean != NULL)
      free(grid_boolean);
/*
   if (entity != NULL){
      for (int i = 0; i < sEntity; i++){
         if (entity[i] != NULL)
              delete entity[i];
      }
      free(entity);
   }
*/

   if (grid != NULL)
      free(grid);

   if (vehiclesType != NULL)
      clearVehiclesTypes();

}




/**
 *  Allocs the lattice
 *  @param x: x size
 *  @param y: y size
 */
void Grid::allocGrid(int x, int y){
   cellX = x;
   cellY = y;
   if (grid != NULL)
        free(grid);

   if (grid_boolean != NULL)
        free(grid);
    
   assert(posix_memalign((void**) &grid, ALIGN, cellX * cellY *  sizeof(Entity*)) == 0);
   assert(posix_memalign((void**) &grid_boolean, ALIGN, cellX * cellY *  sizeof(bool)) == 0);
   
   
   assert(grid != NULL);
   
   assert(grid_boolean != NULL);
   
   
   
   

};


/**
 *  Sets the lattice with boolean value
 *  @param x: x position
 *  @param y: y position
 *  @param v: Vehicle's postition
 */
void Grid::setGrid_boolean(int x, int y){
    int p = y * cellX + x;
    grid_boolean[p] = true;
    
};

/**
 *  Sets the lattice
 *  @param x: x position
 *  @param y: y position
 *  @param v: Vehicle's postition
 */
void Grid::setGrid(int x, int y, Entity *e){
    int p = y * cellX + x;
    grid[p] = e;
    grid_boolean[p] = true;
   // setGrid_boolean(x, y);
    
};

/**
 *  Retruns the vehicle information
 *  @param x: x position
 *  @param y: y position
 *  @return Vehicle postion
 */
Entity* Grid::getGrid(int x, int y){
    int p = y * cellX + x;
    return grid[p] ;
    
};

/**
 *  Retruns the whether the cell is busy or not
 *  @param x: x position
 *  @param y: y position
 *  @return Vehicle postion
 */
bool  Grid::getGrid_boolean(int x, int y){
    int p = y * cellX + x;
    return grid_boolean[p] ;
    
};

/**
 *  Atomic operation. Test if there is a vehicle or not
 *  if false set true. At the same time return the state
 *  Retruns the whether the cell is busy or not
 *  @param x: x position
 *  @param y: y position
 *  @param s: s size of vehicles
 *  @return Vehicle postion
 */
bool Grid::atomic(int x, int y, int s){
    assert( (y >= 0) && (y < cellY) && (x >= 0) && (x < cellX) );
    bool ret = false;
    int  k = 0;
//The first verifies if the cells is occupied or not
/*
    while ((!ret) && (k < s)){
        int x1  =  x + k;
        if (x1 == cellX)
            x1 = 0;
        int p = y * cellX + x;
        
        ret = grid_boolean[p];
        k++;
    }
*/
 
    while ((!ret) && (k < s)){
        int x1  =  x - k;
        if (x1 < 0)
            x1 = cellX + x1;
        int p = y * cellX + x1;
        
        ret = grid_boolean[p];
        k++;
    }
    
    
    if (!ret){
        for (k = 0 ; k < s; k++)
        {
            int x1  =  x - k;
            if (x1 < 0)
                x1 = cellX + x1;
            int p = y * cellX + x1;
            
            grid_boolean[p] = true;
        }
        
    }
    
    return ret;
    
};



/**
 *  Adds a new vehicle in data structure
 *  @param v: v is vehicle
 *  @param idx: idx is index of vector
 */

void Grid::addEntity (Entity *e){
    if (entities == NULL){
        entities = e;
        return; 
    }
    
    Entity *ptr = entities;
    while (ptr->next != NULL)
        ptr = ptr->next;
    
    ptr->next = e;
    e->prev = ptr;
    
};



void Grid::allocVehicleType(int s){
    sVehiclesType = s;
    
    if (vehiclesType != NULL){
        clearVehiclesTypes();
    }
   
   assert(posix_memalign((void**) &vehiclesType, ALIGN, sVehiclesType *  sizeof(tpVehiclesType)) == 0);
   assert(vehiclesType != NULL);
};

/**
 *  Adds a new vehicle type in data structure
 *  @param v: v is vehicle type
 *  @param idx: idx is index of vector
 */

void Grid::addVehicleType(tpVehiclesType v, int idx){
    assert(idx < sVehiclesType);
    vehiclesType[idx] = v;

};

/**
 *  Adds a new vehicle type in data structure
 *  @param v: v is vehicle
 *  @param idx: idx is index of vector
 */

tpVehiclesType *Grid::getVehicleType(int idx){
    assert(idx < sVehiclesType);
    return &vehiclesType[idx];
};


/**
 *  Clear the lattice, setting the default value as -1
 *  @param
 */
void Grid::clearGrid(void){
    int size = cellX * cellY;
    //memset(grid_boolean, false, size * sizeof(bool));
    //memset(grid, 0xFF, size * sizeof(int));

   for (int i = 0; i < size; i++){
      grid_boolean[i] = false;
      grid[i] = EMPTY;
   }
}

void Grid::clearMemories(void){
    clearGrid();
    
    Entity *entity = entities;
    while (entity != NULL){
        Entity *ptr = entity;
        entity = entity->next;
        free(ptr);
        ptr = NULL;
    }
    entities = NULL;
}
/**
 *  Clear the vehicles type vector
 *  @param
 */
void Grid::clearVehiclesTypes(void){
  
   
   free(vehiclesType);
}



