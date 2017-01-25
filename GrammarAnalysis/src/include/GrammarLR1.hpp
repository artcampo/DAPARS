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
  
public:  
  GrammarLR1();
  
  void BuildTables() noexcept;
  
  size_t NumSymbols() const noexcept;
  
  void DumpTables() const noexcept;
  
  LR1_Item InitLR1_Item(const Rule& rule, const Symbol& symbol) const noexcept;
  LR1_Item InitLR1_Item(const Rule& rule) const noexcept;
  
  std::set<LR1_Item> Closure(const std::set<LR1_Item>& set) const noexcept;
  
private:

  
};

} //end namespace GrammarAnalyzer
