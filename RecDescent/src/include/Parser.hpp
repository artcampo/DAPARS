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
  std::vector<char> skip_symbols_;
  
  std::ifstream     file_;
  std::vector<char> file_data_;
  std::vector<char>::const_iterator current_position_;
  
  Tokenizer::kToken token_;
  
  void Skip() noexcept;
  void NextToken() noexcept;
  
  bool Expr();
  bool ExprPrime();
  bool Factor();
  
  void Error(const std::string& message);
};

} //end namespace RecDescent
