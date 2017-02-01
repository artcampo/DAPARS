#include "GrammarLALR.hpp"
#include "GrammarLR1.hpp" //GetKernel
#include <exception>
#include <vector>
#include <iostream>
#include <stack>

namespace GrammarAnalyzer{

GrammarLALR::GrammarLALR(): free_state_id_(0){
  
}






std::set<LR1_Item> GrammarLALR::Goto(const SetLR1_Item& set, const Symbol& symbol){
  std::set<LR1_Item> moved;

  return moved;
}



void GrammarLALR::BuildTables() noexcept{
  GrammarLR1::BuildTables();
  std::cout << "LALR build\n";
  MergeLR1SetsIntoLALRSets();
}

SetLR1_Item GrammarLALR::MergeSets(std::vector<const SetLR1_Item*> sets) noexcept{
  SetLR1_Item s;
  for(const auto& set : sets)
    s.insert( set->cbegin(), set->cend() );
  return s;
}

/*
 * Following algorithm in the dragon book, p268
 */
void GrammarLALR::MergeLR1SetsIntoLALRSets() noexcept{
  const SetOfSetsLR1_Item& setsLR1 = GrammarLR1::CC();
  std::map<const SetLR1_Item, 
           std::vector<const SetLR1_Item*> 
          > merged_states;
          
  for(const auto& set : setsLR1){
    const SetLR1_Item kernel = GetKernel(set);
    merged_states[kernel].push_back(&set);
  }
  
  for(const auto& it : merged_states){
    SetLR1_Item merged = MergeSets(it.second);
    NewCC(merged);
    std::cout << "merge set: " << merged << "\n";
  }
}

void GrammarLALR::BuildActionTable() noexcept{

  
}

void GrammarLALR::AssignId(const SetLR1_Item& set){
  set_id_[set] = free_state_id_;
  ++free_state_id_;  
}

void GrammarLALR::NewCC(const SetLR1_Item& set){

  AssignId(set);
  cc_.insert(set);
}



} //end namespace GrammarAnalyzer
