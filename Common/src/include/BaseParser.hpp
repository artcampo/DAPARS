#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "Tokenizer.hpp"
#include "AST.hpp"
#include "CompilationUnit.hpp"
#include "Locus.hpp"


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


  void Skip() noexcept;

  bool Accept(const kToken& token, const std::string& error) noexcept;
  bool AcceptEmpty(const std::vector<kToken>& tokens
                 , const std::string& error) noexcept;
  bool Check(const std::vector<kToken>& tokens) const noexcept;
  bool TryAndAccept(const kToken& token) noexcept;

  void NextToken() noexcept;
  bool ContinueParsing(){ return continue_parsing_;}


  const Locus CurrentLocus() const noexcept { return Locus(current_position_);}


  //Error handling
  void Error(const std::string& message);
  void ErrorCritical(const std::string& message);

  //Creation of AST nodes with error checking
  PtrBinaryOp
  NewBinaryOp(PtrExpr& lhs, const int op, PtrExpr& rhs
                        , const ScopeId id, const Locus& locus);

  PtrVar
  NewVar(const std::string& name, const Type& type
                        , const ScopeId id, const Locus& locus);

  PtrLiteral
  NewLiteral(const uint32_t &value, const Type& type
                        , const ScopeId id, const Locus& locus);

  PtrBlock
  NewBlock(std::vector<PtrStatement>& stmts_inht
                        , const ScopeId id, const Locus& locus);

  PtrIfStmt
  NewIfStmt(PtrExpr& condition, PtrBlock& block1
                        , const ScopeId id, const Locus& locus);

  PtrIfStmt
  NewIfStmt(PtrExpr& condition, PtrBlock& block1, PtrBlock& block2
                        , const ScopeId id, const Locus& locus);

  PtrDeclStmt
  NewDeclStmt(PtrVarDeclList& list, const ScopeId id
                        , const Locus& locus);

  PtrVarDeclList
  NewVarDeclList(std::vector<PtrVarDecl>& list
                        , const ScopeId id, const Locus& locus);

  PtrVarDecl
  NewVarDecl(const std::string& name, const Type& type
                        , const ScopeId id, const Locus& locus);

  PtrAssignStmt
  NewAssignStmt(PtrExpr& lhs, PtrExpr& rhs
                        , const ScopeId id, const Locus& locus);

  PtrWhileStmt
  NewWhileStmt(PtrExpr& condition, PtrBlock& body
                        , const ScopeId id, const Locus& locus);
private:
  int               num_errors_;
  bool              continue_parsing_;

  std::ifstream     file_;
  std::vector<char> file_data_;
  std::vector<char>::const_iterator current_position_;
  std::vector<char>::const_iterator previous_position_;

  std::vector<char> skip_symbols_;
};

} //end namespace Common
