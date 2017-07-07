/*
 * Paper: A cellular automata traffic flow model considering the
 *        heterogeneity of acceleration and delay probability (2016) Physical A
 * 
 */ 
#ifndef _QILANG_HPP_
#define _QILANG_HPP_
#include <Rules.hpp>

class QiLang : public Rules
{
public:
    QiLang(const std::string);
    ~QiLang();
    
    virtual void apply(int, bool = false);
    
};
#endif
