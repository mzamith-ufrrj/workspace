/* 
 * File:   Vehicle.h
 * Author: marcelozamith
 *
 * Created on October 20, 2010, 10:28 AM
 */

#ifndef _VEHICLE_H_
#define _VEHICLE_H_
#include <ModelTypes.hpp>

class Vehicle {
public:
   Vehicle();
   ~Vehicle();
   
   void apply(void);
   void update(bool, int);
   void updateY(bool, int = -1); //Updating in (y)
   
   void defineLattice(int, int);
   
   
   void clearParameters(void);
     
   
    
public:
     int mCellX;            /*<! Number of cells in latticle (x) */
    int mCellY;            /*<! Number of cells in latticle (y) */
    int mStates;           /*<! Informs whether the vehicle passed or not */
    int mType;             /*<! Indicates the type of vehicle (blocked or not), for instance */
    unsigned int ID;       /*<! Vehicle's ID */
    int LID;               /*<! Vehilce's ID in according to its tipe */
    int x;                 /*<! Vehicle's x position on the road */
    int y;                 /*<! Vehicle's y position. It is the lane */
    int vx;                /*<! Vehicle's velocity in [0:25] */
    int vxNew;             /*<! Vehicle's velocity in the current time (t)*/
    int vy;                /*<! Vehicle's lane change [-1 is left] [1 is right] [0 is current] */
    //int vyNew;             /*<! Vehicle's lane in the current time (t)*/
    int lg;                /*<! Vehicle's length */
    int GAP;               /*<! Gap between itself and the next vehicle */
    int vDef;              /*<! The desired velocity  */
    bool yChange;          /*<! Indicates if vehicle change the lane or not (false = not change / true = change); */
    tpVehiclesType *type;  /*<! Vehicle's behavior   */
    float myDensity;       /*<! Used to identify which density this vehicle is moving */
    
};

#endif	/* VEHICLE_H */

