#include "Parser.hpp"
#include <iterator>
#include <fstream>
#include <iostream>

namespace RecDescent{

Parser::Parser(std::string const &file_name) 
  : file_(std::ifstream (file_name.c_str(), std::ios::binary) )
  , file_data_(std::vector<char> ((std::istreambuf_iterator<char>(file_)),
                                   std::istreambuf_iterator<char>()) )
  , current_position_(file_data_.cbegin())
  , skip_symbols_ {' ','\n'}
{
  //std::ifstream file(file_name.c_str(), std::ios::binary);
  
  
  
  
}

void Parser::parse(){
  while( current_position_ != file_data_.cend()){
    std::cout << *current_position_;
    ++current_position_;
  }
}


} //end namespace RecDescent
