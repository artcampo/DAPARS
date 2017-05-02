#include "Tokenizer.hpp"
#include <iterator>
#include <fstream>
#include <iostream>

namespace Common{
namespace Tokenizer{

bool
ParseKeyword(std::vector<char>::const_iterator& current_position
             , std::vector<char>::const_iterator end_position
           , kToken& t) noexcept{
  int chars_left = std::distance(current_position, end_position);

  //==
  if(chars_left >= 2
    and *(current_position + 0) == '=' and *(current_position + 1) == '='){
    current_position += 2; t = kToken::equalto; return true;
  }

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

  //bool
  if( chars_left >= 4
     and *(current_position + 0) == 'v' and *(current_position + 1) == 'o'
     and *(current_position + 2) == 'i' and *(current_position + 3) == 'd'){
    current_position += 4; t = kToken::kwd_void; return true;
  }

  //kwd_class
  if( chars_left >= 5
     and *(current_position + 0) == 'c' and *(current_position + 1) == 'l'
     and *(current_position + 2) == 'a' and *(current_position + 3) == 's'
     and *(current_position + 4) == 's'){
    current_position += 5; t = kToken::kwd_class; return true;
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

  //return
  if( chars_left >= 6
     and *(current_position + 0) == 'r' and *(current_position + 1) == 'e'
     and *(current_position + 2) == 't' and *(current_position + 3) == 'u'
     and *(current_position + 4) == 'r' and *(current_position + 5) == 'n'){
    current_position += 6; t = kToken::kwd_return; return true;
  }

  if(chars_left >= 2
    and *(current_position + 0) == 'o' and *(current_position + 1) == 'r'){
    current_position += 2; t = kToken::kwd_or; return true;
  }

  if( chars_left >= 3
     and *(current_position) == 'a'  and *(current_position + 1) == 'n'
     and *(current_position + 2) == 'd'){
    current_position += 3; t = kToken::kwd_and; return true;
  }

  //TODO: delete these
  //For dragon test grammar
  if(chars_left >= 2){
    if(*current_position == '_' and *(current_position + 1) == 'c')
      { current_position += 2; t = kToken::token_c; return true;}
    if(*current_position == '_' and *(current_position + 1) == 'd')
      { current_position += 2; t = kToken::token_d; return true;}
  }

  return false;
}

bool IsLetter(const char& c){
 return((c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z') or (c == '_'));
}

bool IsNumber(const char& c){
 return(c >= '0' and c <= '9');
}

bool
ParseName(std::vector<char>::const_iterator& current_position
             , std::vector<char>::const_iterator end_position) noexcept{
  //TODO check +1 still valid
  if(IsLetter(*current_position))
  {
    ++current_position;
    while( ((IsLetter(*current_position) or IsNumber(*current_position))
         and current_position != end_position))
      ++current_position;
    return true;
  }
  return false;
}


bool
ParseNumerical(std::vector<char>::const_iterator& current_position
             , std::vector<char>::const_iterator end_position) noexcept{
  bool matched = false;
  if(IsNumber(*current_position)){
    matched = true;
    ++current_position;
    while(IsNumber(*current_position) and current_position != end_position)
      ++current_position;
  }
  return matched;
}

// returns current token starting at current_position, and
// updates current_position after it
kToken
ParseToken(std::vector<char>::const_iterator& current_position
             , std::vector<char>::const_iterator end_position) noexcept{
  //TODO:OPT: use switch
  if(*current_position == '('){ ++current_position; return kToken::lpar; }
  if(*current_position == ')'){ ++current_position; return kToken::rpar; }
  if(*current_position == '{'){ ++current_position; return kToken::lcbr; }
  if(*current_position == '}'){ ++current_position; return kToken::rcbr; }
  if(*current_position == '+'){ ++current_position; return kToken::plus; }
  if(*current_position == '*'){ ++current_position; return kToken::astk; }
  if(*current_position == ';'){ ++current_position; return kToken::semicolon; }
  if(*current_position == ','){ ++current_position; return kToken::comma; }

  if(*current_position == '<'){ ++current_position; return kToken::lessthan; }
  if(*current_position == '&'){ ++current_position; return kToken::ampersand; }
  if(*current_position == '.'){ ++current_position; return kToken::dot; }
  if(*current_position == ':'){ ++current_position; return kToken::colon; }


  if(ParseNumerical(current_position, end_position)) return kToken::numerical;

  kToken t;
  if(ParseKeyword(current_position, end_position, t)) return t;
  if(ParseName(current_position, end_position)) return kToken::name;

  if(*current_position == '='){ ++current_position; return kToken::equality; }

  return kToken::error;
}


std::string str(const kToken& t){
  switch(t){
    case kToken::eof:       return "eof";  break;
    case kToken::lpar:      return "lpar"; break;
    case kToken::rpar:      return "rpar"; break;
    case kToken::lcbr:      return "lcbr"; break;
    case kToken::rcbr:      return "rcbr"; break;

    case kToken::plus:      return "+";  break;
    case kToken::astk:      return "*";  break;
    case kToken::semicolon: return ";";  break;
    case kToken::equality:  return "=";  break;
    case kToken::lessthan:  return "<";  break;
    case kToken::equalto:   return "==";  break;
    case kToken::ampersand: return "&";  break;
    case kToken::dot:       return ".";  break;
    case kToken::colon:     return ":";  break;

    case kToken::kwd_if:    return "if";  break;
    case kToken::kwd_else:  return "else";  break;
    case kToken::kwd_while: return "while";  break;
    case kToken::kwd_return:return "return";  break;

    case kToken::kwd_int:   return "int";  break;
    case kToken::kwd_bool:  return "bool";  break;
    case kToken::kwd_void:  return "void";  break;
    case kToken::kwd_class: return "class";  break;

    case kToken::kwd_true:  return "true";  break;
    case kToken::kwd_false: return "false";  break;

    case kToken::kwd_or:    return "or";  break;
    case kToken::kwd_and:   return "and";  break;

    case kToken::numerical: return "num";  break;
    case kToken::name:      return "name";  break;

    case kToken::token_c:   return "_c";  break;
    case kToken::token_d:   return "_d";  break;

    default:                break;
  }
  return "error";
}


} //end namespace Tokenizer
} //end namespace Common
