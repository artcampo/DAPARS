#include "BaseParser.hpp"
#include "AST/Node.hpp"
#include "AST/ASTNodeCreation.hpp"
#include "Types.hpp"
#include "Locus.hpp"
#include <iterator>
#include <fstream>
#include <iostream>
#include <exception>

namespace Common{

  /*
class ExceptionNotEndFile: public exception{
  virtual const char* what() const throw()
  {
    return "Parsed last Expr, but more ";
  }
};
*/

BaseParser::BaseParser(const std::vector<char>& parse_data
                      , CompilationUnit& unit)
  : file_data_(parse_data)
  , current_position_(file_data_.cbegin())
  , skip_symbols_ {' ','\n'}
  , unit_(unit)
  , num_errors_(0)
  , num_fatal_errors_(0)
  , continue_parsing_(true)
{
  std::cout << "Parsing: \"";
  for(const auto& it : parse_data ) std::cout << it;
  std::cout << "\"\n";
  unit_.SetFileData(&file_data_);
}

BaseParser::BaseParser(std::string const &file_name, CompilationUnit& unit)
  : file_(std::ifstream (file_name.c_str(), std::ios::binary) )
  , file_data_(std::vector<char> ((std::istreambuf_iterator<char>(file_)),
                                   std::istreambuf_iterator<char>()) )
  , current_position_(file_data_.cbegin())
  , skip_symbols_ {' ','\n'}
  , unit_(unit)
  , num_errors_(0)
  , num_fatal_errors_(0)
  , continue_parsing_(true)
{
  unit_.SetFileData(&file_data_);
}



void BaseParser::NextToken() noexcept{
  Skip();
  if(current_position_ == file_data_.cend()){
    if(token_ == Tokenizer::kToken::eof){
      Error("[err:1] NextToken performed after reaching eof");
      continue_parsing_ = false;
    }
    token_ = Tokenizer::kToken::eof;
  }else{
    //(?) store init/end position of current token for conversion
    prev_token_int_value_     = token_int_value_;
    prev_token_string_value_  = token_string_value_;
    previous_position_        = current_position_;
    token_ = Tokenizer::ParseToken(current_position_, file_data_.cend());
    if(token_ == Tokenizer::kToken::numerical){
      token_int_value_ = atoi( std::string( previous_position_
                                    , current_position_).c_str());
    }else if(token_ == Tokenizer::kToken::name){
      token_string_value_ = std::string( previous_position_, current_position_);
      if(IsNameType(token_string_value_)) token_ = Tokenizer::kToken::name_type;

    }

    if(token_ == kToken::error){
      Error("[err:2] Token not recognized");
    }
  }

/*
  //Debug info for when things go south
  std::cout << "NextToken: " <<  str(token_);
  if(token_ == Tokenizer::kToken::numerical) std::cout << ": " << token_int_value_;
  if(token_ == Tokenizer::kToken::name) std::cout << ": " << token_string_value_;
  std::cout << "\n";
*/
}

bool BaseParser::Accept(const kToken& token, const std::string& error) noexcept{
  if(token_ != token){
    Error(error);
    return false;
  }
  NextToken();
  return true;
}

bool BaseParser::Accept(const std::vector<kToken>& tokens
      , const std::string& error) noexcept{
  if(Check(tokens)) { NextToken();  return true; }
  else              { Error(error); return false;}
}

bool BaseParser::TryAndAccept(const kToken& token) noexcept{
  if(token_ == token){
    NextToken();
    return true;
  }
  return false;
}


bool BaseParser::AcceptEmpty(const std::vector<kToken>& tokens
                           , const std::string& error) noexcept{
  for(const auto it : tokens)
    if(token_ == it) return true;
  Error(error);
  return false;
}

bool BaseParser::Check(const std::vector<kToken>& tokens) const noexcept{
  for(const auto it : tokens)
    if(token_ == it) return true;
  return false;
}

/*
bool BaseParser::Check(const kToken token) const noexcept{
  return Check({token});
}
*/

void BaseParser::Skip() noexcept{
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

//Locus
//file_data_.cbegin()
void BaseParser::Error(const std::string& message){
  ++num_errors_;
  if(num_errors_ >= num_errors_to_halt_) continue_parsing_ = false;

  //Go back N chars
  unit_.Error(message, CurrentLocus());
}

void BaseParser::ErrorCritical(const std::string& message){
  Error(message);
//   exit(1);
}

void BaseParser::FatalError(const std::string& message){
  Error(message);
  ++num_fatal_errors_;
//   exit(1);
}

//Used for error recovery, whenever a symbol has not been matched, and we
//want to discard all tokens until a recover point.
//Recover points are tipically: ';', ')'
void BaseParser::ConsumeTokensUntil(const kToken& token) noexcept{
  while(token_ != token and continue_parsing_){
    NextToken();
  }
}


void BaseParser::RegNameType(const std::string& name){
  name_types_.push_back(std::string(name));
}

bool BaseParser::IsNameType(const std::string& name) const noexcept{
  for(const auto& it : name_types_)
    if(it == name) return true;
  return false;
}


} //end namespace Common

