#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>

namespace RecDescent{

class Parser{
public:  
  Parser(std::string const &file_name);
  void parse();
private:
  std::vector<char> skip_symbols_;
  
  std::ifstream     file_;
  std::vector<char> file_data_;
  std::vector<char>::const_iterator current_position_;
  
};

} //end namespace RecDescent
