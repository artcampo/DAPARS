#include "Parser.hpp"
#include <iterator>
#include <fstream>
#include <iostream>
#include <exception>

namespace RecDescent{

  /*
class ExceptionNotEndFile: public exception{
  virtual const char* what() const throw()
  {
    return "Parsed last expression, but more ";
  }
};
*/

  
Parser::Parser(std::string const &file_name) 
  : file_(std::ifstream (file_name.c_str(), std::ios::binary) )
  , file_data_(std::vector<char> ((std::istreambuf_iterator<char>(file_)),
                                   std::istreambuf_iterator<char>()) )
  , current_position_(file_data_.cbegin())
  , skip_symbols_ {' ','\n'}
{
 
}

void Parser::Parse(){
  NextToken();
  if(Expr()){
    if(token_ != Tokenizer::kToken::eof){
      std::cout << "More data after program.";
    }
  }
}

void Parser::NextToken() noexcept{
  Skip();
  if(current_position_ == file_data_.cend())
    token_ = Tokenizer::kToken::eof;
  else{
    //(?) store init/end position of current token for conversion
    token_ = Tokenizer::ParseToken(current_position_);
  }
}

void Parser::Skip() noexcept{
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