#pragma once
#include "Action.hpp"
#include "Identifiers.hpp"
#include <vector>


namespace GrammarAnalyzer{

class LR_Tables{
public:  
  //indexed with: StateId, SymbolId (term)
  std::vector< std::vector<Action>> action_table_;
  
  //indexed with: StateId, SymbolId (nonterm)
  std::vector< std::vector<StateId>> goto_table_;
  
  //indexed with: StateId, SymbolId (term)
  std::vector< std::vector<StateId>> transition_table_;  
};

} //end namespace GrammarAnalyzer
