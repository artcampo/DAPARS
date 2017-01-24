#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>

namespace LR1{
namespace Tokenizer{
  
enum class kToken{ eof, lpar, rpar, plus, numerical };

kToken ParseToken(std::vector<char>::const_iterator& current_position) noexcept;

std::string str(const kToken& t);

} //end namespace Tokenizer
} //end namespace LR1
