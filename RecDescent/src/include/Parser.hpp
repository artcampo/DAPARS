#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>

namespace RecDescent{

class Parser{
public:  
  Parser(std::string const &file_name);
private:
  std::ifstream     file_;
  std::vector<char> file_data_;
};

} //end namespace RecDescent
