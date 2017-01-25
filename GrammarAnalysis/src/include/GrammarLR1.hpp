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

  using SetLR1_Item       = std::set<LR1_Item>;
  using SetOfSetsLR1_Item = std::set<SetLR1_Item>;
  using SetId             = size_t;
  
public:  
  GrammarLR1();
  
  void BuildTables() noexcept;
  
  size_t NumSymbols() const noexcept;
  
  void DumpTables() const noexcept;
  
  LR1_Item InitLR1_Item(const Rule& rule, const Symbol& symbol) const noexcept;
  LR1_Item InitLR1_Item(const Rule& rule) const noexcept;
  
  std::set<LR1_Item> Closure(const std::set<LR1_Item>& set);
  std::set<LR1_Item> Goto(const std::set<LR1_Item>& set, const Symbol& symbol);
  
private:

  void CanonicalCollection();
  
  
  std::map<SetLR1_Item, bool> marked_;
  SetOfSetsLR1_Item cc_;
  std::map<SetLR1_Item, SetId> set_id_;
  SetId free_id_;
  
  std::map<SetId, std::map<Symbol,SetId>> action_table_;
  std::map<SetId, std::map<Symbol,SetId>> goto_table_;
  
  
  void NewCC    (const std::set<LR1_Item>& set);
  void AssignId (const std::set<LR1_Item>& set);
};

} //end namespace GrammarAnalyzer
