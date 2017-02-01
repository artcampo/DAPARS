#include "Output.hpp"
#include "Symbol.hpp"
#include "Tokenizer.hpp"
#include "GrammarLR1.hpp"
#include "LR1_Item.hpp"
#include <string>
#include <iostream>


namespace GrammarAnalyzer{

std::ostream&
operator<<(std::ostream &os, const LR1_Item& c) { 
  return os << c.str();
}

std::ostream&
operator<<(std::ostream &os, const SetLR1_Item& c) { 
  for(const auto& it : c)
    os << it << "\n";
  return os;
}

} //end namespace GrammarAnalyzer