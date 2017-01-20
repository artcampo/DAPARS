#include "Grammar.hpp"
#include <iterator>
#include <fstream>
#include <iostream>
#include <exception>

namespace GrammarAnalyzer{



  
Grammar::Grammar(){}


void Grammar::AddRule(const Rule& rule) noexcept{
  rules_.push_back(rule);
}


void Grammar::Analyze() noexcept{
  ComputeFirstSets();
  ComputeFollowSets();
  ComputeFirstPlusSets();
}

void Grammar::ComputeFirstSets() noexcept{
}

void Grammar::ComputeFollowSets() noexcept{
}

void Grammar::ComputeFirstPlusSets() noexcept{
}

} //end namespace GrammarAnalyzer
