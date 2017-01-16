#include "Parser.hpp"
#include <iterator>
#include <fstream>

namespace RecDescent{

Parser::Parser(std::string const &file_name) 
  : file_(std::ifstream (file_name.c_str(), std::ios::binary) )
  , file_data_(std::vector<char> ((std::istreambuf_iterator<char>(file_)),
                                   std::istreambuf_iterator<char>()) )
{
  //std::ifstream file(file_name.c_str(), std::ios::binary);
  
  
  
  
}


} //end namespace RecDescent
