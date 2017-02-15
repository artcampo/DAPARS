#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "Tokenizer.hpp"
#include "AST.hpp"
#include "CompilationUnit.hpp"


namespace Common{

using namespace Tokenizer;
using namespace Compiler;
using namespace AST;

class BaseParser{

public:
  BaseParser(std::string const &file_name, CompilationUnit& unit);
  BaseParser(const std::vector<char>& parse_data, CompilationUnit& unit);



protected:
  const static int num_characters_to_display_before_error_ = 10;
  const static int num_characters_to_display_after_error_  = 10;
  const static int num_errors_to_halt_                     = 10;


  CompilationUnit&  compilation_unit_;

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




  //Error handling
  void Error(const std::string& message);
  void ErrorCritical(const std::string& message);

  //Creation of AST nodes with error checking
//   ExprStmt*     NewExprStmt(Expr* const node_expr);
  BinaryOp*     NewBinaryOp(Expr* const lhs, const int op, Expr* const rhs);
  Var*          NewVar(const std::string& name);

  Literal*      NewLiteral(const uint32_t &value);
  Block*        NewBlock(const std::vector<Statement*>& stmts_inht);
  IfStmt*       NewIfStmt(Expr* const condition, Block* block1);
  IfStmt*       NewIfStmt(Expr* const condition, Block* block1, Block* block2);
  DeclStmt*     NewDeclStmt(VarDeclList* const list);
  VarDeclList*  NewVarDeclList(const std::vector<VarDecl*>& list);
  VarDecl*      NewVarDecl(const std::string& name, const TypeId& typeId);
  AssignStmt*   NewAssignStmt(Expr* const lhs, Expr* const rhs);

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
