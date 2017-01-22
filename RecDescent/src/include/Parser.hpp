#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>

#include "Tokenizer.hpp"

namespace RecDescent{

class Parser{
  
public:  
  Parser(std::string const &file_name);
  void Parse();
  
private:
  const static int num_characters_to_display_before_error_ = 5;
  std::vector<char> skip_symbols_;
  
  std::ifstream     file_;
  std::vector<char> file_data_;
  std::vector<char>::const_iterator current_position_;
  
  Tokenizer::kToken token_;
  
  void Skip() noexcept;
  void NextToken() noexcept;
  
  void Error(const std::string& message);
  
  bool Prog();
  bool Expr();
  bool ExprPrime();
  bool Factor();
  
  
};

} //end namespace RecDescent
