#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "Tokenizer.hpp"

class Block;
class Node;

namespace Common{

using namespace Tokenizer;

class BaseParser{
  
public:  
  BaseParser(std::string const &file_name, Block* &programBlock);
  
protected:
  const static int num_characters_to_display_before_error_ = 5;
  std::vector<char> skip_symbols_;
  
  Block*& programBlock_;
  std::ifstream     file_;
  std::vector<char> file_data_;
  std::vector<char>::const_iterator current_position_;
  std::vector<char>::const_iterator previous_position_;
  
  Tokenizer::kToken token_;
  int               token_int_value_;
  
  int               num_errors_;
  
  void Skip() noexcept;
  void NextToken() noexcept;
  
  void Error(const std::string& message);
  
  
};

} //end namespace Common
