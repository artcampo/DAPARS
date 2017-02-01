#include "GrammarLALR.hpp"
#include "GrammarLR1.hpp" //GetKernel
#include "LR0_Item.hpp"
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
  InitTables();
  BuildActionTable();
}

void GrammarLALR::InitTables(){
  GrammarLR1::InitTables(GrammarLALR::tables_, 
                         GrammarLALR::NumStates(), 
                         GrammarLR1::NumNonTerminals(), 
                         GrammarLR1::NumTerminals() );  
}


void GrammarLALR::BuildActionTable() noexcept{
  GrammarLR1::BuildActionTable(cc_, set_id_, tables_.action_table_);
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
  std::map<const SetLR0_Item, 
           std::vector<const SetLR1_Item*> 
          > merged_states;
          
  for(const auto& set : setsLR1){
    const SetLR0_Item kernel = GetKernel(set);
    merged_states[kernel].push_back(&set);
    std::cout << "kernel set:\n" << kernel << "\n";
    std::cout << "to set:\n" << set << "\n";
  }
  
  for(const auto& it : merged_states){
    SetLR1_Item merged = MergeSets(it.second);
    NewCC(merged);
    //if(it.second.size() > 1)
    std::cout << it.second.size()<<"  merged set:\n" << merged << "\n";
  }
}


void GrammarLALR::AssignId(const SetLR1_Item& set){
  set_id_[set] = free_state_id_;
  ++free_state_id_;  
}

void GrammarLALR::NewCC(const SetLR1_Item& set){
  AssignId(set);
  cc_.insert(set);
}

void GrammarLALR::DumpTables() const noexcept{
  std::cout << "LR1 Tables:\n";
  GrammarLR1::DumpTables();
  std::cout << "LALR Tables:\n";
  std::cout << tables_;
}


} //end namespace GrammarAnalyzer
