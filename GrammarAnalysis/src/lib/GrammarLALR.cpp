#include "GrammarLALR.hpp"
#include <exception>
#include <vector>
#include <iostream>
#include <stack>

namespace GrammarAnalyzer{

GrammarLALR::GrammarLALR(): free_state_id_(0){
  
}






std::set<LR1_Item> GrammarLALR::Goto(const std::set<LR1_Item>& set, const Symbol& symbol){
  std::set<LR1_Item> moved;

  return moved;
}



void GrammarLALR::BuildTables() noexcept{
  GrammarLR1::BuildTables();
  MergeLR1SetsIntoLALRSets();
}


void GrammarLALR::MergeLR1SetsIntoLALRSets() noexcept{
  const SetOfSetsLR1_Item& setsLR1 = GrammarLR1::CC();
  for(const auto& set : setsLR1){
    
  }
}

void GrammarLALR::BuildActionTable() noexcept{

  
}





} //end namespace GrammarAnalyzer
