#include "Tokenizer.hpp"
#include <iterator>
#include <fstream>
#include <iostream>

namespace Common{
namespace Tokenizer{

bool
ParseNumerical(std::vector<char>::const_iterator& current_position
             , std::vector<char>::const_iterator end_position) noexcept{
  bool matched = false;
  if(*current_position >= '0' and *current_position <= '9'){
    matched = true;
    ++current_position;
    while(*current_position >= '0' and *current_position <= '9'
      and current_position != end_position)
      ++current_position;
  }
  return matched;
}

bool
ParseKeyword(std::vector<char>::const_iterator& current_position
             , std::vector<char>::const_iterator end_position
           , kToken& t) noexcept{
  int chars_left = std::distance(current_position, end_position);

  //if
  if(chars_left >= 2
    and *(current_position + 0) == 'i' and *(current_position + 1) == 'f'){
    current_position += 2; t = kToken::kwd_if; return true;
  }

  //else
  if( chars_left >= 4
      and *(current_position + 0) == 'e'  and *(current_position + 1) == 'l'
      and *(current_position + 2) == 's' and *(current_position + 3) == 'e'){
    current_position += 4; t = kToken::kwd_else; return true;
  }

  //int
  if( chars_left >= 3
     and *(current_position) == 'i'  and *(current_position + 1) == 'n'
     and *(current_position + 2) == 't'){
    current_position += 3; t = kToken::kwd_int; return true;
  }

  //bool
  if( chars_left >= 4
     and *(current_position + 0) == 'b' and *(current_position + 1) == 'o'
     and *(current_position + 2) == 'o' and *(current_position + 3) == 'l'){
    current_position += 4; t = kToken::kwd_bool; return true;
  }

  //while
  if( chars_left >= 5
     and *(current_position + 0) == 'w' and *(current_position + 1) == 'h'
     and *(current_position + 2) == 'i' and *(current_position + 3) == 'l'
     and *(current_position + 4) == 'e'){
    current_position += 5; t = kToken::kwd_while; return true;
  }

  //true
  if( chars_left >= 4
     and *(current_position + 0) == 't' and *(current_position + 1) == 'r'
     and *(current_position + 2) == 'u' and *(current_position + 3) == 'e'){
    current_position += 4; t = kToken::kwd_true; return true;
  }

  //false
  if( chars_left >= 5
     and *(current_position + 0) == 'f' and *(current_position + 1) == 'a'
     and *(current_position + 2) == 'l' and *(current_position + 3) == 's'
     and *(current_position + 4) == 'e'){
    current_position += 5; t = kToken::kwd_false; return true;
  }



  //For dragon test grammar
  if(chars_left >= 2){
    if(*current_position == '_' and *(current_position + 1) == 'c')
      { current_position += 2; t = kToken::token_c; return true;}
    if(*current_position == '_' and *(current_position + 1) == 'd')
      { current_position += 2; t = kToken::token_d; return true;}
  }

  return false;
}

bool
ParseName(std::vector<char>::const_iterator& current_position
             , std::vector<char>::const_iterator end_position) noexcept{
  //TODO check +1 still valid
  if(*current_position >= 'a' and *current_position <= 'z')
  {
    ++current_position;
    while( ((*current_position >= '0' and *current_position <= '9')
        or (*current_position >= 'a' and *current_position <= 'z'))
         and current_position != end_position)
      ++current_position;
    return true;
  }
  return false;
}

// returns current token starting at current_position, and
// updates current_position after it
kToken
ParseToken(std::vector<char>::const_iterator& current_position
             , std::vector<char>::const_iterator end_position) noexcept{
  if(*current_position == '('){ ++current_position; return kToken::lpar; }
  if(*current_position == ')'){ ++current_position; return kToken::rpar; }
  if(*current_position == '{'){ ++current_position; return kToken::lcbr; }
  if(*current_position == '}'){ ++current_position; return kToken::rcbr; }
  if(*current_position == '+'){ ++current_position; return kToken::plus; }
  if(*current_position == ';'){ ++current_position; return kToken::semicolon; }
  if(*current_position == ','){ ++current_position; return kToken::comma; }
  if(*current_position == '='){ ++current_position; return kToken::equality; }

  if(ParseNumerical(current_position, end_position)) return kToken::numerical;

  kToken t;
  if(ParseKeyword(current_position, end_position, t)) return t;
  if(ParseName(current_position, end_position)) return kToken::name;

  return kToken::error;
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
    case kToken::semicolon: return std::string(";");  break;
    case kToken::equality:  return std::string("=");  break;
    case kToken::kwd_if:    return std::string("if");  break;
    case kToken::kwd_else:  return std::string("else");  break;
    case kToken::kwd_while: return std::string("while");  break;
    case kToken::kwd_int:   return std::string("int");  break;
    case kToken::kwd_bool:  return std::string("bool");  break;
    case kToken::kwd_true:  return std::string("true");  break;
    case kToken::kwd_false: return std::string("false");  break;
    case kToken::name:      return std::string("name");  break;

    case kToken::token_c:   return std::string("_c");  break;
    case kToken::token_d:   return std::string("_d");  break;

    default:                break;
  }
  return std::string("error");
}


} //end namespace Tokenizer
} //end namespace Common
