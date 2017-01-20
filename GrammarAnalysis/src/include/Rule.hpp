#pragma once
#include <vector>
#include "Symbol.hpp"


namespace GrammarAnalyzer{

class Rule{
  
public:  
  Rule(const Symbol& head, const std::vector<Symbol>& derived)
  : head_(head), derived_(derived)
  {}
  
  const Symbol              head_;
  const std::vector<Symbol> derived_;
  
private:

};

} //end namespace GrammarAnalyzer
