#include "Parser.hpp"
#include "Node.hpp"
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

  
Parser::Parser(std::string const &file_name, Block* &programBlock) 
  : file_(std::ifstream (file_name.c_str(), std::ios::binary) )
  , file_data_(std::vector<char> ((std::istreambuf_iterator<char>(file_)),
                                   std::istreambuf_iterator<char>()) )
  , current_position_(file_data_.cbegin())
  , skip_symbols_ {' ','\n'}
  , programBlock_(programBlock)
  , num_errors_(0)
{
 
}


void Parser::Parse(){
  if(not Prog()){
    Error("Program malformed. ");
  }  
  if(num_errors_ != 0){
    std::cout << "Program syntactically incorrect\n";
  }
}

void Parser::NextToken() noexcept{
  Skip();
  if(current_position_ == file_data_.cend())
    token_ = Tokenizer::kToken::eof;
  else{
    //(?) store init/end position of current token for conversion
    previous_position_ = current_position_;
    token_ = Tokenizer::ParseToken(current_position_);
    if(token_ == Tokenizer::kToken::numerical){
      token_int_value_ = atoi( std::string( previous_position_
                                    , current_position_).c_str());
    }
  }
  
  std::cout << "-> " <<  str(token_);
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

void Parser::Error(const std::string& message){
  ++num_errors_;
  std::cout << "\n" << message << " at: [[";
  
  //Go back N chars
  std::vector<char>::const_iterator start_of_error = current_position_;
  for(int i = 0; i < num_characters_to_display_before_error_
                and start_of_error != file_data_.cbegin(); ++i)
    --start_of_error;
  
  while(start_of_error != current_position_){
    std::cout << *start_of_error;
    ++start_of_error;
  }
  std::cout << "]]" << std::endl;
}
  
  
} //end namespace RecDescent
 
