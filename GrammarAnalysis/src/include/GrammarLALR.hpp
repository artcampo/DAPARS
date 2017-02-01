#pragma once
#include "GrammarLR1.hpp"
#include "LR1_Item.hpp"
#include "Action.hpp"
#include "Identifiers.hpp"
#include "LR_Tables.hpp"
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <set>


namespace GrammarAnalyzer{

  
class GrammarLALR : public GrammarLR1{

public:  
  GrammarLALR();
  
  void BuildTables() noexcept;  
  size_t NumStates() const noexcept{ return free_state_id_;};
  
  void DumpTables() const noexcept;

private:  
  
  std::set<LR1_Item> Goto(const SetLR1_Item& set, const Symbol& symbol);  

  void CanonicalCollection();
  void BuildActionTable() noexcept;
  void InitTables();
  
  SetOfSetsLR1_Item cc_;
  std::map<SetLR1_Item, StateId> set_id_;

  LR_Tables tables_;
  
  
  void MergeLR1SetsIntoLALRSets() noexcept;
  SetLR1_Item MergeSets(std::vector<const SetLR1_Item*> sets) noexcept;
  
  void AssignId (const SetLR1_Item& set);
  void NewCC(const SetLR1_Item& set);
  StateId    free_state_id_;
  


};

} //end namespace GrammarAnalyzer
