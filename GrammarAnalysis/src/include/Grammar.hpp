#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include "Rule.hpp"
#include "Symbol.hpp"

namespace GrammarAnalyzer{

class Grammar{
  
public:  
  Grammar();
  
  void AddRule(const Rule& rule)  noexcept;

  void Analyze() noexcept;
  
  bool IsBackTrackFree() noexcept;
  
private:
  std::vector<Rule> rules_;
  bool analized_;
  std::set<Symbol> symbols_;
  std::map<Symbol, std::set<Symbol>> first_;
  
  void ComputeFirstSets() noexcept;
  void ComputeFollowSets() noexcept;
  void ComputeFirstPlusSets() noexcept;
  
};

} //end namespace GrammarAnalyzer
