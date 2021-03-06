#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "Tokenizer.hpp"
#include "CompilationUnit.hpp"
#include "Locus.hpp"
#include "Symbol.hpp"
#include "AST/AST.hpp"
#include "AST/ASTNodeCreation.hpp"


namespace Common{

using namespace Tokenizer;
using namespace Compiler::AST;
using Compiler::CompilationUnit;
using Compiler::Locus;
using namespace Compiler::AST::Ptrs;


class BaseParser{

public:
  BaseParser(std::string const &file_name, CompilationUnit& unit);
  BaseParser(const std::vector<char>& parse_data, CompilationUnit& unit);


protected:
  const static int num_errors_to_halt_                     = 10;

  CompilationUnit&  unit_;

  Tokenizer::kToken token_;
  int               token_int_value_;
  std::string       token_string_value_;
  int               prev_token_int_value_;    //for when TryAndAccept has been called
  std::string       prev_token_string_value_; //for when TryAndAccept has been called


  bool ContinueParsing() const noexcept{ return continue_parsing_;}
  const Locus CurrentLocus() const noexcept { return Locus(current_position_);}

  //Functions to consume tokens
  bool Accept(const kToken& token, const std::string& error) noexcept;
  bool Accept(const std::vector<kToken>& tokens, const std::string& error) noexcept;
  bool AcceptEmpty(const std::vector<kToken>& tokens
                 , const std::string& error) noexcept;
  bool TryAndAccept(const kToken& token) noexcept;
  void ConsumeTokensUntil(const kToken& token) noexcept;
  void NextToken() noexcept;  //avoid if possible

  //Functions to check token
  bool Check(const std::vector<kToken>& tokens) const noexcept;


  //Functions to modify the tokenizer
  void RegNameType(const std::string& name);

  //Error handling
  int  NumFatalErrors()const noexcept{ return num_fatal_errors_;}
  void Error(const std::string& message);

  //TODO: get rid of this one?
  void ErrorCritical(const std::string& message);

  //Analysis of AST cannot be done after those
  void FatalError(const std::string& message);


private:
  int               num_errors_;
  int               num_fatal_errors_;
  bool              continue_parsing_;

  std::ifstream     file_;
  std::vector<char> file_data_;
  std::vector<char>::const_iterator current_position_;
  std::vector<char>::const_iterator previous_position_;

  std::vector<char> skip_symbols_;

  std::vector<std::string> name_types_;

  void Skip() noexcept;
  bool IsNameType(const std::string& name) const noexcept;

};

} //end namespace Common
