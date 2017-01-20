#include "Grammar.hpp"
#include <iterator>
#include <fstream>
#include <iostream>
#include <exception>

namespace GrammarAnalyzer{



  
Grammar::Grammar()
{
 
}


void Grammar::AddRule(const Rule& rule) noexcept{
  rules_.push_back(rule);
}

} //end namespace GrammarAnalyzer
