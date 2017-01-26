#pragma once
#include "Grammar.hpp"
#include "LR1_Item.hpp"
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <set>


namespace GrammarAnalyzer{
  
class GrammarLR1 : public Grammar{

  using SetLR1_Item           = std::set<LR1_Item>;
  using SetOfSetsLR1_Item     = std::set<SetLR1_Item>;
  using VectorOfSetsLR1_Item  = std::vector<SetLR1_Item>;
  using SetId                 = size_t;
  using SymbolId              = size_t;
  
  
  class Action{
  public:    
    enum class kAction{ shift, reduce, accept };
    
    kAction action_;
    SetId   next_state_;    
    
    Action(){};
    
    Action( const kAction& action, const SetId& next_state)
      : action_(action), next_state_(next_state){};

    std::string str() const noexcept{
      if(action_ == kAction::shift) return std::string("shift") + std::to_string(next_state_);
      if(action_ == kAction::reduce) return std::string("reduce") + std::to_string(next_state_);
      return std::string("accept");
    }
      
  };
  
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
  
private:

  void CanonicalCollection();
  void BuildActionTable() noexcept;
  
  
  std::map<SetLR1_Item, bool> marked_;
  SetOfSetsLR1_Item cc_;
  std::map<SetLR1_Item, SetId> set_id_;
//   std::map<SetId, SetLR1_Item> set_by_id_;
  SetId    free_symbol_id_;
  SymbolId free_term_id_;
  SymbolId free_non_term_id_;
  
  //indexed with: SetId, SymbolId (term)
  std::vector< std::vector<Action>> action_table_;
  
  //indexed with: SetId, SymbolId (nonterm)
  std::vector< std::vector<SetId>> goto_table_;
  
  //indexed with: SetId, SymbolId (term)
  std::vector< std::vector<SetId>> transition_table_;
  
  std::set<Symbol,SymbolId> symbol_id_;
  
  void NewCC    (const std::set<LR1_Item>& set);
  void AssignId (const std::set<LR1_Item>& set);
  
  void InitSymbolsIds() noexcept;
  SymbolId GetSymbolId(const Symbol& symbol);
};

} //end namespace GrammarAnalyzer
