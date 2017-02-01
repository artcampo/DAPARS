#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>

namespace Common{
namespace Tokenizer{
  
  
enum class kToken{ eof, lpar, rpar, plus, numerical,
                   token_c, token_d   //grammar: dragon
};

kToken ParseToken(std::vector<char>::const_iterator& current_position) noexcept;

std::string str(const kToken& t);


} //end namespace Tokenizer
} //end namespace Common

