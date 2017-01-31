#pragma once
#include "GrammarLR1.hpp"
#include "LR1_Item.hpp"
#include "Action.hpp"
#include "Identifiers.hpp"
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <set>


namespace GrammarAnalyzer{
  
  using SetLR1_Item           = std::set<LR1_Item>;
  using SetOfSetsLR1_Item     = std::set<SetLR1_Item>;
  using VectorOfSetsLR1_Item  = std::vector<SetLR1_Item>;  
  
class GrammarLALR : public GrammarLR1{

public:  
  GrammarLALR();
  
  void BuildTables() noexcept;  
  

  
  std::set<LR1_Item> Goto(const std::set<LR1_Item>& set, const Symbol& symbol);  
  

  
private:  

  void CanonicalCollection();
  void BuildActionTable() noexcept;
  
  
  std::map<SetLR1_Item, StateId> merged_set;
  
  void MergeLR1SetsIntoLALRSets() noexcept;
  
  void AssignId (const std::set<LR1_Item>& set);
  StateId    free_state_id_;
/*
  std::map<SetLR1_Item, StateId> set_id_;
//   std::map<StateId, SetLR1_Item> set_by_id_;
  
  
  //indexed with: StateId, SymbolId (term)
  std::vector< std::vector<Action>> action_table_;
  
  //indexed with: StateId, SymbolId (nonterm)
  std::vector< std::vector<StateId>> goto_table_;
  
  //indexed with: StateId, SymbolId (term)
  std::vector< std::vector<StateId>> transition_table_; 
  
  void InitTables();
  */
};

} //end namespace GrammarAnalyzer
