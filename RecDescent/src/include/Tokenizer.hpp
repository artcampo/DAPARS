#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>

namespace RecDescent{
namespace Tokenizer{
  
enum class kToken{ eof, lpar, rpar, plus, numerical };

kToken ParseToken(std::vector<char>::const_iterator& current_position) noexcept;


} //end namespace Tokenizer
} //end namespace RecDescent
