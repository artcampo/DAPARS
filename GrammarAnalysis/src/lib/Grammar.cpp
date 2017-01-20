#include "Grammar.hpp"
#include <exception>
#include <vector>

namespace GrammarAnalyzer{



  
Grammar::Grammar()
  : analized_(false)
  , symbols_({Symbol::Empty()})
{}


void Grammar::AddRule(const Rule& rule) noexcept{
  rules_.push_back(rule);
  symbols_.insert(rule.head_);
  for(const auto &s : rule.derived_)
    symbols_.insert(s);
}


void Grammar::Analyze() noexcept{
  if(not analized_){
    ComputeFirstSets();
    ComputeFollowSets();
    ComputeFirstPlusSets();
    analized_ = true;
  }
}

void Grammar::ComputeFirstSets() noexcept{
}

void Grammar::ComputeFollowSets() noexcept{
}

void Grammar::ComputeFirstPlusSets() noexcept{
}

bool Grammar::IsBackTrackFree() noexcept{
  if(not analized_) Analyze();
}

} //end namespace GrammarAnalyzer
