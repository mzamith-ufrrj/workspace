#ifndef _MODELTYPES_HPP_
#define _MODELTYPES_HPP_
#include <cstring>
#include <string>
using namespace std;
enum ePDF{
   UNIFORM,
   NORMAL,
   BETA
};

static const int   STRINGSIZE       = 256;
static const int   VEHICLEPERKM     = 133;
static const float PERCENT2VEHICLES = 1000.0f / 7.5f;
static const int   NUMBEROFTYPES    = 3;                                /*!< Defines the number of PDFs */
//static const char* PDFNAME[]        = {"UNIFORM", "NORMAL", "BETA"};     /*!< PDFs implemented */
static const int   PDFPARAM[]       = {1, 2, 5};                        /*!< Parameters for each PDF */
static const int   ALIGN            = 64;

struct tpSpace{
   //int density;
   int *eq_gap;    /* dist = vel */
   int *lt_gap;    /* vel  < dist */
   int *gt_gap;    /* vel  > dist */
   int samples;
};
 
/*
 * Vehicles parameters
 */

struct tpVehiclesType
{
    char       description[STRINGSIZE];
    float      percent;
    int        size;
    int        vMax;
    int            inc;
   // int            dec;
    int       aheadInt;
    int       safeDist;
    int            PDF; 
    double      *param;
    float      left_p;
    float     right_p;
    int       lo_ID;
    int       hi_ID;
    bool vehicleLog;
    char sufix[STRINGSIZE];

};

/*
 * Config parameters
 */
struct tpParam
{

    char STFile[STRINGSIZE];   
    char LGFile[STRINGSIZE];
    char modelName[STRINGSIZE];
   

    int cellX;   //Space in X
    int cellY;   //Space in Y --> number of the roads
    float deltaH;
    float defaultSize; //default size of one vehicle - it means 7.5mts
   
    int sTime;   //Simulation time
    int dTime;   //Discarde time
    int stTime;   //Statistic time

    int vMax;      //Max velocity
    float iDensity; //Initial density
    float fDensity; //Final density
    float dDensity; //Delta density

   
};




struct tpTableDensityMem
{
   int   vehicles;
   float density;
   unsigned long memory;
   unsigned int time;
};


/*
 *
 */
struct stDiskMemory
{char byte_a, byte_b, byte_c, byte_d;};

/*
 *
 */
union uDiskMemory
{
   int iDisk;
   unsigned int uDisk;
   float fDisk;
   stDiskMemory bytes;
};

/*
 *
 */
typedef uDiskMemory tpDiskMemory;

#endif
