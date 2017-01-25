#pragma once
#include "Grammar.hpp"

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
  
private:

  
};

} //end namespace GrammarAnalyzer
