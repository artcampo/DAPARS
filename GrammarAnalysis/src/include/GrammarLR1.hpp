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
  
using SetLR1_Item           = std::set<LR1_Item>;
using SetOfSetsLR1_Item     = std::set<SetLR1_Item>;
using VectorOfSetsLR1_Item  = std::vector<SetLR1_Item>;  

const SetLR1_Item GetKernel(const SetLR1_Item& set);
  
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
  std::set<LR1_Item> Goto(const SetLR1_Item& set, const Symbol& symbol);  
  
  
  Action GetAction(const StateId& state, const SymbolId& terminal_symbol)
  {return action_table_[state][terminal_symbol];}
  
  StateId GetGoto(const StateId& state, const SymbolId& non_terminal_symbol)
  {return goto_table_[state][non_terminal_symbol];}  
  
protected:
  const SetOfSetsLR1_Item& CC() const{ return cc_;}
  
private:  

  void CanonicalCollection();
  void BuildActionTable() noexcept;
  
  
  std::map<SetLR1_Item, bool> marked_; //todo: inefficient
  SetOfSetsLR1_Item cc_;
  
  std::map<SetLR1_Item, StateId> set_id_;
//   std::map<StateId, SetLR1_Item> set_by_id_;
  StateId    free_state_id_;
  
  //indexed with: StateId, SymbolId (term)
  std::vector< std::vector<Action>> action_table_;
  
  //indexed with: StateId, SymbolId (nonterm)
  std::vector< std::vector<StateId>> goto_table_;
  
  //indexed with: StateId, SymbolId (term)
  std::vector< std::vector<StateId>> transition_table_;
  
  void NewCC    (const std::set<LR1_Item>& set);
  void AssignId (const std::set<LR1_Item>& set);
  
  void InitSymbolsIds() noexcept;
  
  void InitTables();
  
  void BuildActionTable(SetOfSetsLR1_Item& cc,
                 std::map<SetLR1_Item, StateId>& set_id,
                 std::vector< std::vector<Action>>& action_table_);
};

} //end namespace GrammarAnalyzer
