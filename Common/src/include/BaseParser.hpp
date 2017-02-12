#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "Tokenizer.hpp"

class Block;
class Node;
class Expression;
class ExpressionStatement;
class Literal;
class Statement;
class StmtIf;

namespace Common{

using namespace Tokenizer;

class BaseParser{
  
public:  
  BaseParser(std::string const &file_name, Block* &programBlock);
  BaseParser(const std::vector<char>& parse_data, Block* &programBlock);
  
  
  
protected:
  const static int num_characters_to_display_before_error_ = 5;
  std::vector<char> skip_symbols_;
  
  Block*& programBlock_;
  std::ifstream     file_;
  std::vector<char> file_data_;
  std::vector<char>::const_iterator current_position_;
  std::vector<char>::const_iterator previous_position_;
  
  Tokenizer::kToken token_;
  int               token_int_value_;
  
  int               num_errors_;
  
  void Skip() noexcept;
  
  void Accept(const kToken& token, const std::string& error) noexcept;
  void NextToken() noexcept;
  
  //Error handling
  void Error(const std::string& message);
  void ErrorCritical(const std::string& message);
  
  //Creation of AST nodes with error checking
  Node* NewBinaryOp(Node* const lhs, const int op, Node* const rhs);
  ExpressionStatement* NewExpressionStatement(Node* const node_expr);
  Node* NewLiteral(const uint32_t &value);
  Block* NewBlock(const std::vector<Statement*>& stmts_inht);
  StmtIf* NewStmtIf(Expression* const condition, Block* block1);
  StmtIf* NewStmtIf(Expression* const condition, Block* block1, Block* block2);
  
};

} //end namespace Common
