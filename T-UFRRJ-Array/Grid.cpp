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
vehicle(NULL),
grid(NULL),
grid_boolean(NULL),
vehiclesType(NULL),
mMaxGap(0),
cellX(-1),
cellY(-1),
mGAPS(NULL){
   
   
    
};

Grid::~Grid()
{
#ifdef _DEBUG
	std::cerr << __FILE__ << ":" << __FUNCTION__ << std::endl;
#endif
    


};


void Grid::finalize(void){
    if (grid_boolean != NULL)
        free(grid_boolean);
    
    if (vehicle != NULL)
        free(vehicle);
    
    if (grid != NULL)
       free(grid);
    
    if (vehiclesType != NULL)
        clearVehiclesTypes();
    
    if (mGAPS->eq_gap != NULL)
        free(mGAPS->eq_gap);

    if (mGAPS->lt_gap != NULL)
        free(mGAPS->lt_gap);

    if (mGAPS->gt_gap != NULL)
        free(mGAPS->gt_gap);
    
    if (mGAPS != NULL)
        delete mGAPS;
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
    
   assert(posix_memalign((void**) &grid, ALIGN, cellX * cellY *  sizeof(int)) == 0);
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
void Grid::setGrid(int x, int y, int v){
    int p = y * cellX + x;
    grid[p] = v;
    grid_boolean[p] = true;
   // setGrid_boolean(x, y);
    
};

/**
 *  Retruns the vehicle information
 *  @param x: x position
 *  @param y: y position
 *  @return Vehicle postion
 */
int  Grid::getGrid(int x, int y){
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
 *  Allocs vehicles data structure size
 *  @param s: s is size
 *  @param d: is global density
 */
void Grid::allocVehicle(int s, float d){
    sVehicle = s;
    if (vehicle != NULL){
        free(vehicle);
    }
    assert(posix_memalign((void**) &vehicle, ALIGN, sVehicle *  sizeof(Vehicle)) == 0);
   
    assert(vehicle != NULL);
    
   
    clearGaps();
    //Allocing GAP statistic
    
    
};



/**
 *  Adds a new vehicle in data structure
 *  @param v: v is vehicle
 *  @param idx: idx is index of vector
 */

void Grid::addVehicles (Vehicle v, int idx){
    assert(idx < sVehicle);
    vehicle[idx] = v;
    
};

/**
 *  Returns a vehicle
 *  @param idx: idx is vehicle index
 *  @retunr vehicle
 */

Vehicle Grid::getVehicles(int idx){
     assert(idx < sVehicle);
    return vehicle[idx];
};


Vehicle *Grid::getVehiclePointer(int idx){
    assert(idx < sVehicle);
    return &vehicle[idx];
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
      grid[i] = -1;
   }
};


/**
 *  Clear the vehicles type vector
 *  @param
 */
void Grid::clearVehiclesTypes(void){
   for (int i = 0; i < sVehiclesType; i++){
      free(vehiclesType[i].param);
   }
   
   free(vehiclesType);
}


/**
 * Set the maximum GAP among vehicles.
 * Attention: This parameter is based on global parameter, not
 * behavior parameter;
 * @param g is the maximum gap possible used to save statistic
 */

void Grid::allocMaximumGaps(int g){ 
    mMaxGap = (g + 1); 
    assert(g > 0);
    
    if (mGAPS == NULL)
        mGAPS = new tpSpace();
    
     if (mGAPS->eq_gap != NULL)
        free(mGAPS->eq_gap);
   
    if (mGAPS->lt_gap != NULL)
        free(mGAPS->lt_gap);

    if (mGAPS->gt_gap != NULL)
        free(mGAPS->gt_gap);
    
    assert(mMaxGap > 0);
    

    
    assert(posix_memalign((void**) &mGAPS->eq_gap, ALIGN, mMaxGap * sizeof(int)) == 0);  
    
    assert(posix_memalign((void**) &mGAPS->lt_gap, ALIGN, mMaxGap * sizeof(int)) == 0);  

    assert(posix_memalign((void**) &mGAPS->gt_gap, ALIGN, mMaxGap * sizeof(int)) == 0);  

    mGAPS->samples = 0;
    
}

/**
 * Define the gaps (v < gap) or (v == gap)
 
 */ 
void Grid::defineGaps(void){
    for (int i = 0; i < sVehicle ; i++){
        Vehicle *pVehicle    = getVehiclePointer(i);
        Vehicle **ppVehicle  = &pVehicle;
        int gap = (*ppVehicle)->GAP;
        int vel = (*ppVehicle)->vDef;
        
        assert(gap < mMaxGap);
        assert(gap >= 0);
            
        
        if (vel == gap)
            mGAPS->eq_gap[gap]++;
        else if (vel < gap)
            mGAPS->lt_gap[gap]++;
        else{

            mGAPS->gt_gap[gap]++;
            
        }
        
        (*ppVehicle)->clearParameters();
        mGAPS->samples++;
    }
        
}

/**
 * calulate gap - return a string with statistic
 * It does not matter the number of lane. We use vehicles list
 */ 
string Grid::calculateGaps(void){
    string txt;
    
    txt = to_string(sVehicle) + " ";
    for (int i = 0; i < mMaxGap; i++){
        txt += to_string(mGAPS->eq_gap[i]) + " " + to_string(mGAPS->lt_gap[i]) + " " + to_string(mGAPS->gt_gap[i]) + " " ;
    }
     
    txt += to_string(mGAPS->samples);
    
    return txt;
        
}

void Grid::clearGaps(void){ 
    memset(mGAPS->eq_gap, 0x00, mMaxGap * sizeof(int)); 
    memset(mGAPS->lt_gap, 0x00, mMaxGap * sizeof(int)); 
    memset(mGAPS->gt_gap, 0x00, mMaxGap * sizeof(int)); 
    mGAPS->samples = 0;
    
}