#ifndef _NASCH_HPP_
#define _NASCH_HPP_
#include <Rules.hpp>

class NaSch : public Rules
{
public:
    NaSch(const std::string);
    ~NaSch();
    
    virtual void apply(int, bool = false);
    
};
#endif
