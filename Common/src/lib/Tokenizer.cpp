#include "Tokenizer.hpp"
#include <iterator>
#include <fstream>
#include <iostream>

namespace Common{
namespace Tokenizer{
    
bool 
ParseNumerical(std::vector<char>::const_iterator& current_position) noexcept{
  bool matched = false;
  if(*current_position >= '0' and *current_position <= '9'){
    matched = true;
    ++current_position;
    while(*current_position >= '0' and *current_position <= '9')
      ++current_position;
  }
  return matched;
}

bool 
ParseKeyword(std::vector<char>::const_iterator& current_position
           , kToken& t) noexcept{
  //TODO check +1 still valid
  if(*current_position == 'i' and *(current_position + 1) == 'f'){
    current_position += 2; t = kToken::kwd_if; return true;
  }
  //TODO check +3 still valid
  if(    *current_position == 'e'       and *(current_position + 1) == 'l'
     and *(current_position + 2) == 's' and *(current_position + 3) == 'e'){
    current_position += 4; t = kToken::kwd_else; return true;
  }  
  //TODO check +2 still valid
  if(    *current_position == 'i'       and *(current_position + 1) == 'n'
     and *(current_position + 2) == 't'){
    current_position += 3; t = kToken::kwd_int; return true;
  }    
  //TODO check +3 still valid
  if(    *current_position == 'b'       and *(current_position + 1) == 'o'
     and *(current_position + 2) == 'o' and *(current_position + 3) == 'l'){
    current_position += 4; t = kToken::kwd_bool; return true;
  }    
  
  return false;
}

// returns current token starting at current_position, and
// updates current_position after it
kToken 
ParseToken(std::vector<char>::const_iterator& current_position) noexcept{
  if(*current_position == '('){ ++current_position; return kToken::lpar; }
  if(*current_position == ')'){ ++current_position; return kToken::rpar; }
  if(*current_position == '{'){ ++current_position; return kToken::lcbr; }
  if(*current_position == '}'){ ++current_position; return kToken::rcbr; }  
  if(*current_position == '+'){ ++current_position; return kToken::plus; }
  if(*current_position == ';'){ ++current_position; return kToken::semicolon; }
  if(*current_position == 'c'){ ++current_position; return kToken::token_c; }
  if(*current_position == 'd'){ ++current_position; return kToken::token_d; }
  if(ParseNumerical(current_position))              return kToken::numerical;
  
  kToken t;
  if(ParseKeyword(current_position, t)) return t;
  std::cout << "Tokenizer could not recognize input"; exit(1);
}


std::string str(const kToken& t){
  switch(t){
    case kToken::eof:       return std::string("eof");  break;
    case kToken::lpar:      return std::string("lpar"); break;
    case kToken::rpar:      return std::string("rpar"); break;
    case kToken::lcbr:      return std::string("lcbr"); break;
    case kToken::rcbr:      return std::string("rcbr"); break;    
    case kToken::plus:      return std::string("plus"); break;
    case kToken::numerical: return std::string("num");  break;
    case kToken::token_c:   return std::string("c");  break;
    case kToken::token_d:   return std::string("d");  break;
    case kToken::semicolon: return std::string(";");  break;
    case kToken::kwd_if:    return std::string("if");  break;
    case kToken::kwd_else:  return std::string("else");  break;
    case kToken::kwd_int:   return std::string("int");  break;
    case kToken::kwd_bool:  return std::string("bool");  break;
    
    default:                break;
  }
  return std::string("error");
}


} //end namespace Tokenizer
} //end namespace Common
