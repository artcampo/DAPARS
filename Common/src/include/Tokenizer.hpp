#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>

namespace Common{
namespace Tokenizer{


enum class kToken{ eof, numerical, name
                 , lpar, rpar, lcbr, rcbr
                 , plus, minus, astk, div
                 , equality
                 , kwd_if, kwd_else, kwd_while
                 , kwd_int, kwd_bool
                 , kwd_true, kwd_false
                 , semicolon, comma
                 , error

                 //not used by dalang
                 , token_c, token_d   //grammar: dragon
};

//pass end_position as reference
kToken ParseToken(std::vector<char>::const_iterator& current_position
                , std::vector<char>::const_iterator end_position) noexcept;

std::string str(const kToken& t);


} //end namespace Tokenizer
} //end namespace Common

