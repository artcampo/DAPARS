#include "Tokenizer.hpp"
#include <iterator>
#include <fstream>
#include <iostream>

namespace RecDescent{

namespace Tokenizer{
    
bool 
ParseNumerical(std::vector<char>::const_iterator& current_position) noexcept{
  bool matched = false;
  if(*current_position >= '1' and *current_position <= '9'){
    matched = true;
    ++current_position;
    while(*current_position >= '0' and *current_position <= '9')
      ++current_position;
  }
  return matched;
}

// returns current token starting at current_position, and
// updates current_position after it
kToken 
ParseToken(std::vector<char>::const_iterator& current_position) noexcept{
  if(*current_position == '('){ ++current_position; return kToken::lpar; }
  if(*current_position == ')'){ ++current_position; return kToken::rpar; }
  if(*current_position == '+'){ ++current_position; return kToken::plus; }
  if(ParseNumerical(current_position))              return kToken::numerical;
}

} //end namespace Tokenizer


} //end namespace RecDescent
