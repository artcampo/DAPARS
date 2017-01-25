#pragma once
#include <vector>
#include <string>
#include "Symbol.hpp"


namespace GrammarAnalyzer{

class Rule{
  
public:  
  Rule(const Symbol& head, const std::vector<Symbol>& derived)
  : head_(head), derived_(derived)
  {}
  
  const Symbol        head_;
  std::vector<Symbol> derived_;
  
  std::string str() const noexcept{
    std::string s(head_.str());
    s += " -> ";
    for(const auto &symbol : derived_) { s += symbol.str(); s+= " ";} 
    return s;
  }
    
  const bool operator< ( const Rule &s ) const{ 
    return ( head_ < s.head_ and derived_ < s.derived_);
  }
  
private:

};

} //end namespace GrammarAnalyzer
