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
 
}

void Parser::parse(){
  skip();
  while( current_position_ != file_data_.cend()){    
    std::cout << *current_position_;
    switch(*current_position_){
      case 'a': ++current_position_; break;
      case 'b': {
        ++current_position_;
        while(*current_position_ == 'c') ++current_position_;
        }; break;
      default: std::cout << "not matched"; break;
    } //SHAME ON YOU
    
    skip();
  }
}

void Parser::skip() noexcept{
  bool symbol_is_no_skip = false;
  while(not symbol_is_no_skip 
        and current_position_ != file_data_.cend() ){
    
    bool current_symbol_skipped = false;
    for(auto const &s : skip_symbols_){
      if(*current_position_ == s){
        ++current_position_;
        current_symbol_skipped = true;
        break;
      }
    }
    if(not current_symbol_skipped) symbol_is_no_skip = true;
  }
  
}

} //end namespace RecDescent
