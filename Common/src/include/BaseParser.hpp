#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "Tokenizer.hpp"
#include "AST.hpp"
#include "CompilationUnit.hpp"
#include "Locus.hpp"
#include "Symbol.hpp"
#include "ASTNodeCreation.hpp"


namespace Common{

using namespace Tokenizer;
using namespace Compiler::AST;
using Compiler::CompilationUnit;
using Compiler::Locus;
using namespace Compiler::AST::Ptrs;


class BaseParser : public ASTNodeCreation{

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



  //Error handling
  void Error(const std::string& message);
  void ErrorCritical(const std::string& message);

private:
  int               num_errors_;
  bool              continue_parsing_;

  std::ifstream     file_;
  std::vector<char> file_data_;
  std::vector<char>::const_iterator current_position_;
  std::vector<char>::const_iterator previous_position_;

  std::vector<char> skip_symbols_;

  void Skip() noexcept;

};

} //end namespace Common
