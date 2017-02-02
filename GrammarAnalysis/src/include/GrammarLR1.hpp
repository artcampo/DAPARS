#pragma once
#include "Grammar.hpp"
#include "LR1_Item.hpp"
#include "LR0_Item.hpp"
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

const SetLR0_Item GetKernel(const SetLR1_Item& set);
  
class GrammarLR1 : public Grammar{

public:  
  GrammarLR1();
  
  void BuildTables() noexcept;  
  
  size_t NumStates() const noexcept{ return free_state_id_;};
  size_t NumNonTerminals() const noexcept{return symbols_.size() - 1;};

  void DumpTables() const noexcept;
  void DumpCC() const noexcept;
  
  LR1_Item InitLR1_Item(const Rule& rule, const Symbol& symbol) const noexcept;
  LR1_Item InitLR1_Item(const Rule& rule) const noexcept;
  
  std::set<LR1_Item> Closure(const std::set<LR1_Item>& set);
  std::set<LR1_Item> Goto(const SetLR1_Item& set, const Symbol& symbol);  
  
  
  Action GetAction(const StateId& state, const SymbolId& terminal_symbol)
  {return tables_.action_table_[state][terminal_symbol];}
  
  StateId GetGoto(const StateId& state, const SymbolId& non_terminal_symbol)
  {return tables_.goto_table_[state][non_terminal_symbol];}  
  
protected:
  const SetOfSetsLR1_Item& CC() const{ return cc_;}
  
  void BuildActionTable(SetOfSetsLR1_Item& cc,
                 std::map<SetLR1_Item, StateId>& set_id,
                 std::vector< std::vector<Action>>& action_table_);

  void InitTables(LR_Tables& tables, const size_t num_states
                 ,const size_t num_nonterm, const size_t num_term);
  
private:  

  void CanonicalCollection();
  void BuildActionTable() noexcept;
  
  
  std::map<SetLR1_Item, bool> marked_; //todo: inefficient
  SetOfSetsLR1_Item cc_;
  
  std::map<SetLR1_Item, StateId> set_id_;
  StateId    free_state_id_;
  
  LR_Tables tables_;
  
  void NewCC    (const std::set<LR1_Item>& set);
  void AssignId (const std::set<LR1_Item>& set);
  
  void InitTables();
  

};


} //end namespace GrammarAnalyzer
