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
                 , equality, lessthan, equalto
                 , dot, colon
                 , kwd_if, kwd_else, kwd_while, kwd_return
                 , kwd_int, kwd_bool, kwd_void, kwd_class
                 , kwd_true, kwd_false
                 , kwd_or, kwd_and, kwd_not
                 , semicolon, comma, ampersand
                 , name_type  //only if pre-registered
                 , error  //should not be issued

                 //not used by dalang
                 , token_c, token_d   //grammar: dragon
};

//pass end_position as reference
kToken ParseToken(std::vector<char>::const_iterator& current_position
                , std::vector<char>::const_iterator end_position) noexcept;

std::string str(const kToken& t);


} //end namespace Tokenizer
} //end namespace Common

