/* 
 * File:   Vehicle.h
 * Author: marcelozamith
 *
 * Created on October 20, 2010, 10:28 AM
 */

#ifndef _VEHICLE_H_
#define _VEHICLE_H_
#include <ModelTypes.hpp>
#include <string>
using namespace std;
class Entity{
public:
   
   Entity();
  ~Entity();
  
    unsigned int ID;       /*<! Vehicle's ID */
    
     
    int x;                 /*<! Vehicle's x position on the road */
    int y;                 /*<! Vehicle's y position. It is the lane */
    int lg;                /*<! Vehicle's length */
    virtual string whoAmI(void);
    void setwhoAmI(string who);
    Entity *next;
    Entity *prev;
protected:
    string AmI;
    
};

class Blocked: public Entity{
   Blocked();
   ~Blocked();
   string whoAmI(void);
};

class Vehicle: public Entity {
public:
   Vehicle();
   ~Vehicle();
   
   void apply(void);
   void update(bool, int);
   void updateY(bool, int = -1); //Updating in (y)
   
   void defineLattice(int, int);
   
   
   void clearParameters(void);
   
   string whoAmI(void);
   
    
public:
    int mCellX;            /*<! Number of cells in latticle (x) */
    int mCellY;            /*<! Number of cells in latticle (y) */
    int mStates;           /*<! Informs whether the vehicle passed or not */
   

    int vx;                /*<! Vehicle's velocity in [0:25] */
    int vxNew;             /*<! Vehicle's velocity in the current time (t)*/
    int vy;                /*<! Vehicle's lane change [-1 is left] [1 is right] [0 is current] */
    //int vyNew;             /*<! Vehicle's lane in the current time (t)*/
    bool yChange;          /*<! Indicates if vehicle change the lane or not (false = not change / true = change); */
    tpVehiclesType *type;  /*<! Vehicle's behavior   */
    float myDensity;       /*<! Used to identify which density this vehicle is moving */
    
};

#endif	/* VEHICLE_H */

