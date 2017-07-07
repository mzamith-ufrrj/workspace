#ifndef _T_UFRRJ_HPP_
#define _T_UFRRJ_HPP_
#include <Rules.hpp>

class T_UFRRJ : public Rules
{
public:
    T_UFRRJ(const std::string);
    ~T_UFRRJ();
    
    virtual void apply(int, bool = false);
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
    virtual void update(int, float, bool=false);
    
    //virtual void initialCondition(float); if I consider blocked lane
    virtual void apply(int, float, bool=false);
    virtual void apply(int, bool = false);
    virtual void finalizer(void);
    virtual void changeLaneRules(int idx);
protected:
   
    bool rule_Safe(int, int);
    bool rule_MovingFaster(int);
    bool rule_BestFlow(int, int);
    
    bool mStatistic;                     /*<! It indicates what time should be recorded */
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
    
    virtual void  changeLaneRules(int idx);
protected:
    
    bool rule_FasterAhead(int);
    bool rule_FasterBehind(int);

};


#endif
