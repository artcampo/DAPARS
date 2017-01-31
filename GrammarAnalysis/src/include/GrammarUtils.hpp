#pragma once
#include "Grammar.hpp"

/*
 * Symbols depending on if they are terminals or not terminals, have unique 
 * integer IDs. Eof has ID = 0 and Empty has no ID.
 * 
 * The terminals are mapped in their order of appearance, to make it
 * convenient to map between tokens and symbols IDs.
 */

namespace GrammarAnalyzer{

std::vector<std::string> 
GenerateGrammarStrings(const Grammar& g, const size_t k);

LR1_Item GetKernel(const SetLR1_Item& set){
  
}

} //end namespace GrammarAnalyzer
