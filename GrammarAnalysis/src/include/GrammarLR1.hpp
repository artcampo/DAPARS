#pragma once
#include "Grammar.hpp"
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
  
class GrammarLR1 : public Grammar{

public:  
  GrammarLR1();
  
  void BuildTables() noexcept;  
  size_t NumSymbols() const noexcept;

  void DumpTables() const noexcept;
  void DumpCC() const noexcept;
  
  LR1_Item InitLR1_Item(const Rule& rule, const Symbol& symbol) const noexcept;
  LR1_Item InitLR1_Item(const Rule& rule) const noexcept;
  
  std::set<LR1_Item> Closure(const std::set<LR1_Item>& set);
  std::set<LR1_Item> Goto(const std::set<LR1_Item>& set, const Symbol& symbol);  
  
  using SetLR1_Item           = std::set<LR1_Item>;
  using SetOfSetsLR1_Item     = std::set<SetLR1_Item>;
  using VectorOfSetsLR1_Item  = std::vector<SetLR1_Item>;
  
  Action GetAction(const StateId& state, const SymbolId& terminal_symbol)
  {return action_table_[state][terminal_symbol];}
  
private:  

  void CanonicalCollection();
  void BuildActionTable() noexcept;
  
  
  std::map<SetLR1_Item, bool> marked_;
  SetOfSetsLR1_Item cc_;
  std::map<SetLR1_Item, SetId> set_id_;
//   std::map<SetId, SetLR1_Item> set_by_id_;
  SetId    free_symbol_id_;
  
  //indexed with: SetId, SymbolId (term)
  std::vector< std::vector<Action>> action_table_;
  
  //indexed with: SetId, SymbolId (nonterm)
  std::vector< std::vector<SetId>> goto_table_;
  
  //indexed with: SetId, SymbolId (term)
  std::vector< std::vector<SetId>> transition_table_;
  
  void NewCC    (const std::set<LR1_Item>& set);
  void AssignId (const std::set<LR1_Item>& set);
  
  void InitSymbolsIds() noexcept;
  
  void InitTables();
};

} //end namespace GrammarAnalyzer
