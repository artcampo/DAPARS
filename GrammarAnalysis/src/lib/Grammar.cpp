#include "Grammar.hpp"
#include <exception>
#include <vector>
#include <iostream>

namespace GrammarAnalyzer{



  
Grammar::Grammar()
  : analized_(false)
  , symbols_({Symbol::Empty(), Symbol::Eof()})
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
  for(const auto &s : symbols_)
    if(s.IsTerminal())
      first_[s] = {s};
  
  bool hasChanged = true;
  while(hasChanged){
    hasChanged = false;
    for(const auto &r : rules_){  
      bool all_have_empty = true;
      std::set<Symbol> new_set;
      
      for(const auto &symbol : r.derived_){
        bool symbol_has_empty_symbol = 
            (first_[symbol].find(Symbol::Empty())!= first_[symbol].end());
        all_have_empty = all_have_empty and symbol_has_empty_symbol;
        std::set<Symbol> set = first_[symbol];
        set.erase(Symbol::Empty());
        new_set.insert(set.cbegin(), set.cend());
        if(not symbol_has_empty_symbol){
          break;
        }
      }
      
      if(all_have_empty)
        new_set.insert(Symbol::Empty());
      
      
      //check if set stays the same, and if not, update it

      if(first_[r.head_] != new_set){ 
        std::cout << "Rule: " << r.str() << std::endl;
        std::cout << "change: " << std::endl;
        
        for(const auto &symbol : first_[r.head_])
          std::cout << symbol.str() <<" ";
        std::cout << "" << std::endl;
        
        for(const auto &symbol : new_set)
          std::cout << symbol.str() <<" ";
        std::cout << "" << std::endl;        
        
        hasChanged = true;
        first_[r.head_] = new_set;
      }
      
    }
  }
}

void Grammar::ComputeFollowSets() noexcept{
}

void Grammar::ComputeFirstPlusSets() noexcept{
}

bool Grammar::IsBackTrackFree() noexcept{
  if(not analized_) Analyze();
}

} //end namespace GrammarAnalyzer
