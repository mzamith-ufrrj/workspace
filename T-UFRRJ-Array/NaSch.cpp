#include <NaSch.hpp>
#include <cassert>
#include <cmath>
NaSch::NaSch(const std::string m):
Rules(m)
{};

NaSch::~NaSch(){
    
#ifdef _DEBUG
	std::cerr << __FILE__ << ":" << __FUNCTION__ << std::endl;
#endif
    
};

void NaSch::apply(int vIndex, bool changing)
{
    int   iVel  = 0,
          iDist = 0,
          iMax  = 0;
    int   vDef  = 0,
          GAP   = 0;
    Vehicle *pVehicle    = mGrid.getVehiclePointer(vIndex);
    Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = (*ppVehicle)->type;
    

    
    iMax  = min(static_cast<int>(mParam.vMax), static_cast<int>(type->vMax));
    
    iVel  = static_cast <int> ((*ppVehicle)->vx) + type->inc;

    iVel  = min( iVel, iMax);
    vDef  = iVel;

    iDist = getDistance(vIndex);
    GAP = iDist;
    
    iVel  = min(iVel, iDist);

    if ( getRandom() <= 0.25f)
        iVel = max(iVel - static_cast <int> (type->inc), 0);


    assert(iVel >= 0);
    
    (*ppVehicle)->vxNew = iVel;

    (*ppVehicle)->vDef = vDef;
    (*ppVehicle)->GAP = GAP;

    
};
