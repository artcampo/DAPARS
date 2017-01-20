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
  
  const Symbol              head_;
  const std::vector<Symbol> derived_;
  
  std::string str() const noexcept{
    std::string s(head_.str());
    s += " -> ";
    for(const auto &symbol : derived_) { s += symbol.str(); s+= " ";} 
    return s;
  }
    
  
private:

};

} //end namespace GrammarAnalyzer
