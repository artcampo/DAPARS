#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include "Rule.hpp"
#include "Symbol.hpp"
#include "Grammar.hpp"

namespace GrammarAnalyzer{

class GrammarLR1 : public Grammar{
  
public:  
  GrammarLR1(){};
  
  void BuildTables() noexcept;
  
  void DumpTables() const noexcept;
  
private:

  
};

} //end namespace GrammarAnalyzer
