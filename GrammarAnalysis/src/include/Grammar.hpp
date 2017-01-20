#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "Rule.hpp"
#include "Symbol.hpp"

namespace GrammarAnalyzer{

class Grammar{
  
public:  
  Grammar();
  
  void AddRule(const Rule& rule)  noexcept;

  
private:
  std::vector<Rule> rules_;
  
  
};

} //end namespace GrammarAnalyzer
