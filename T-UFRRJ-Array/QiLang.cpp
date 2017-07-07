#include <QiLang.hpp>
#include <cassert>
#include <cmath>
QiLang::QiLang(const std::string m):
Rules(m)
{};

QiLang::~QiLang(){
    
#ifdef _DEBUG
	std::cerr << __FILE__ << ":" << __FUNCTION__ << std::endl;
#endif
    
};

void QiLang::apply(int vIndex, bool changing)
{
    int   iVel  = 0,
          iDist = 0,
          iMax  = 0;
          
    int   vDef  = 0,
          GAP   = 0;
          
    float pv1 = 0.0f,
          pv2 = 0.0f,
          a  =  getRandom();
          
    Vehicle *pVehicle    = mGrid.getVehiclePointer(vIndex);
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = (*ppVehicle)->type;
    

    
    iMax  = min(mParam.vMax, type->vMax);

    
    iVel  = (*ppVehicle)->vx;
    
    a = (getRandom() * static_cast <float> (iMax)); // / (static_cast <float> (iMax + 1));
    
    //a = floor( a + 0.5f );
    
    //if (a > 0.25f) iVel++;

    iVel  = min( iVel + static_cast<int>(a), iMax);
    vDef  = iVel;
//    iVel  = min( iVel + 1, iMax);
    
    iDist = getDistance(vIndex);
    GAP = iDist;
    
    iVel  = min(iVel, iDist);
    
    float fv = static_cast <float> ((*ppVehicle)->vx);
    fv--;
    if (fv < 0.0f)
        fv = 0.0f;
    
    pv1 = (fv       ) / (2.0f * static_cast <float> (iMax) );
    pv2 = (fv + 1.0f) / (2.0f * static_cast <float> (iMax) );
    
    iVel  = min( iVel, iMax);

    float p = getRandom();
    if ((p >= pv1) && (p < pv2)){
        if (iVel == iDist)
            iVel = max(iVel - 1, 0);
    }
    
    /*

    if (iVel == iDist){
        if ((p >= pv1) && (p < pv2))
            iVel = max(iVel - 1, 0);
    }
*/

    assert(iVel >= 0);

    (*ppVehicle)->vxNew = iVel;


    (*ppVehicle)->vDef = vDef;
    (*ppVehicle)->GAP = GAP;

    
};
