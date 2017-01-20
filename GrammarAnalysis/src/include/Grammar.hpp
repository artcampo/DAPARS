#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include "Rule.hpp"
#include "Symbol.hpp"

namespace GrammarAnalyzer{

class Grammar{
  
public:  
  Grammar();
  
  void AddRule(const Rule& rule)  noexcept;

  void Analyze() noexcept;
  
private:
  std::vector<Rule> rules_;
  bool analized_;
  
  void ComputeFirstSets() noexcept;
  void ComputeFollowSets() noexcept;
  void ComputeFirstPlusSets() noexcept;
  
};

} //end namespace GrammarAnalyzer
