#include "Tokenizer.hpp"
#include <iterator>
#include <fstream>
#include <iostream>

namespace LR1{

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

std::string str(const kToken& t){
  switch(t){
    case kToken::eof:       return std::string("eof");  break;
    case kToken::lpar:      return std::string("lpar"); break;
    case kToken::rpar:      return std::string("rpar"); break;
    case kToken::plus:      return std::string("plus"); break;
    case kToken::numerical: return std::string("num");  break;
    default:                break;
  }
  return std::string("error");
}


} //end namespace Tokenizer


} //end namespace LR1
