#include "Tokenizer.hpp"
#include <iterator>
#include <fstream>
#include <iostream>

namespace Common{
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
  if(*current_position == 'c'){ ++current_position; return kToken::token_c; }
  if(*current_position == 'd'){ ++current_position; return kToken::token_d; }
  if(ParseNumerical(current_position))              return kToken::numerical;
}

std::string str(const kToken& t){
  switch(t){
    case kToken::eof:       return std::string("eof");  break;
    case kToken::lpar:      return std::string("lpar"); break;
    case kToken::rpar:      return std::string("rpar"); break;
    case kToken::plus:      return std::string("plus"); break;
    case kToken::numerical: return std::string("num");  break;
    case kToken::token_c:   return std::string("c");  break;
    case kToken::token_d:   return std::string("d");  break;
    default:                break;
  }
  return std::string("error");
}


} //end namespace Tokenizer
} //end namespace Common
