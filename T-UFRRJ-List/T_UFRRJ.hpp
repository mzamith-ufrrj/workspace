#ifndef _T_UFRRJ_HPP_
#define _T_UFRRJ_HPP_
#include <Rules.hpp>

class T_UFRRJ : public Rules
{
public:
    T_UFRRJ(const std::string);
    ~T_UFRRJ();
    
    virtual void executeRules(Entity *, bool = false);
    void movementRules(Vehicle*);
    void  getDistanceAndVelocity(Vehicle *, int *, int *);
protected:
   int truncCurve3(double);
   double betaFunction (double, double, double, double, double, double );
   double betaFunction (double *);
};


/*
 * Multiroad model symmetric rules
 **/
class T_UFRRJ_ML_SYM : public T_UFRRJ
{
public:
    
    T_UFRRJ_ML_SYM(const std::string);
    ~T_UFRRJ_ML_SYM();
   
    
    //virtual void initialCondition(float); if I consider blocked lane
    
    virtual void applyRules(int, bool = false);
    virtual void executeRules(Entity *, bool = false);
    virtual void changeLaneRules(Vehicle *);
    virtual void finalizer(void);
    virtual int  inFlow(void);
    virtual void insertVehicle(int); 

protected:
   bool rule_BestFlow(Vehicle *, int);
   bool rule_MovingFaster(Vehicle*);
   bool rule_Safe(Vehicle *, int);
    
   bool mStatistic;                     /*<! It indicates what time should be recorded */
   float inFlow;
   // int mThereIsBlocked;
    
    
};


/*
 * Multiroad model symmetric rules
 **/
class T_UFRRJ_ML_ASYM : public T_UFRRJ_ML_SYM
{
public:
    
    T_UFRRJ_ML_ASYM(const std::string);
    ~T_UFRRJ_ML_ASYM();
    
     virtual void changeLaneRules(Vehicle *);
protected:
    
    bool rule_FasterAhead(Vehicle *);
    bool rule_FasterBehind(Vehicle *);
    
    Entity * getBack(int *,  
                     int,
                     int, 
                     int);
                      

};

#endif