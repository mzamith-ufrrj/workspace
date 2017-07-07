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
 * Multiroad model
 **/
class T_UFRRJ_MR : public T_UFRRJ
{
public:
    
   T_UFRRJ_MR(const std::string);
   ~T_UFRRJ_MR();
   virtual void update(int, float, bool=false);
   // void initialCondition(float);
   virtual void apply(int, float, bool=false);
   virtual void apply(int, bool = false);
   virtual void finalizer(void);
   
private:
   bool r05_Change(int , int );
   //  bool r04_Motivated(int);
   void symmetric(int idx);
   void asymmetric(int idx);
   bool r03_Motivated(int);
   bool r02_Motivated(int);
   bool r01_Motivated(int);//, int);
   float r01C_Motivated(int idx, int iDir);
   bool r01B_Motivated(int);
   bool r01A_Motivated(int, int);

   bool mStatistic;                     /*<! It indicates what time should be recorded */
   int mThereIsBlocked;
    
    
};
#endif
