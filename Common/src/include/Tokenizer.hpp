#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>

namespace Common{
namespace Tokenizer{
  
  
enum class kToken{ eof, numerical, name
                 , lpar, rpar, lcbr, rcbr
                 , plus, minus, mult, div
                 , token_c, token_d   //grammar: dragon
                 , kwd_if, kwd_else
                 , kwd_int, kwd_bool
                 , semicolon
                  
};

kToken ParseToken(std::vector<char>::const_iterator& current_position) noexcept;

std::string str(const kToken& t);


} //end namespace Tokenizer
} //end namespace Common

