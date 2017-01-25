#pragma once
#include <vector>
#include <string>
#include "Rule.hpp"
#include "Symbol.hpp"


namespace GrammarAnalyzer{

class LR1_Item{
  
public:  
  LR1_Item(const Rule& rule, const Symbol& symbol)
  : rule_(rule), symbol_(symbol)
  {}
  
  const Rule                rule_;
  const Symbol              symbol_;
  
  
  std::string str() const noexcept{
    std::string s("[");
    s += rule_.str();
    s += ", ";
    s += symbol_.str();
    s += std::string ("]");
    return s;
  }
  
  const bool operator< ( const LR1_Item &s ) const{ 
    return ( rule_ < s.rule_ and symbol_ < s.symbol_);
  }
  
private:

};

} //end namespace GrammarAnalyzer
