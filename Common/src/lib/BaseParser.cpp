#include "BaseParser.hpp"
#include "Node.hpp"
#include <iterator>
#include <fstream>
#include <iostream>
#include <exception>

namespace Common{

  /*
class ExceptionNotEndFile: public exception{
  virtual const char* what() const throw()
  {
    return "Parsed last expression, but more ";
  }
};
*/

BaseParser::BaseParser(const std::vector<char>& parse_data, 
                       Block* &programBlock)
  : file_data_(parse_data)
  , current_position_(file_data_.cbegin())
  , skip_symbols_ {' ','\n'}
  , programBlock_(programBlock)
  , num_errors_(0)
{
  std::cout << "Parsing: \"";
  for(const auto& it : parse_data ) std::cout << it;
  std::cout << "\"\n";
}

BaseParser::BaseParser(std::string const &file_name, Block* &programBlock) 
  : file_(std::ifstream (file_name.c_str(), std::ios::binary) )
  , file_data_(std::vector<char> ((std::istreambuf_iterator<char>(file_)),
                                   std::istreambuf_iterator<char>()) )
  , current_position_(file_data_.cbegin())
  , skip_symbols_ {' ','\n'}
  , programBlock_(programBlock)
  , num_errors_(0)
  {}

void BaseParser::NextToken() noexcept{
  Skip();
  if(current_position_ == file_data_.cend()){
    if(token_ == Tokenizer::kToken::eof){
      std::cout << "NextToken performed after reaching eof"; 
      exit(1);
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
    }
    
    if(token_ == kToken::error){
      ErrorCritical("Token not recognized");
    }
  }
  /*
  std::cout << "NextToken: " <<  str(token_);
  if(token_ == Tokenizer::kToken::numerical) std::cout << ": " << token_int_value_;
  if(token_ == Tokenizer::kToken::name) std::cout << ": " << token_string_value_;
  std::cout << "\n";
  */
}

void BaseParser::Accept(const kToken& token, const std::string& error) noexcept{
  if(token_ != token) Error(error);
  NextToken();  
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

void BaseParser::Error(const std::string& message){
  ++num_errors_;
  std::cout << "\n" << message << " at: \"";
  
  //Go back N chars
  
  {
    std::vector<char>::const_iterator start_of_error = current_position_;
    for(int i = 0; i < num_characters_to_display_before_error_
                  and start_of_error != file_data_.cbegin(); ++i)
      --start_of_error;
  
    while(start_of_error != current_position_){
      std::cout << *start_of_error;
      ++start_of_error;
    }
  }  
  std::cout << "\" -> \""; 
  //Go forward K chars
  {
    std::vector<char>::const_iterator start_of_error = current_position_;
    for(int i = 0; i < num_characters_to_display_after_error_ and
        start_of_error != file_data_.cend(); ++start_of_error)
      std::cout << *start_of_error;
  }
  
  std::cout << "\"" << std::endl;
}

void BaseParser::ErrorCritical(const std::string& message){
  Error(message);
  exit(1);
}
  
Node* BaseParser::NewBinaryOp(Node* const lhs, const int op, Node* const rhs){
  if(lhs == nullptr) ErrorCritical("lhs invalid");
  if(rhs == nullptr) ErrorCritical("rhs invalid");
  
  Expression* const lhs_exp = dynamic_cast<Expression*>(lhs);
  Expression* const rhs_exp = dynamic_cast<Expression*>(rhs);
  Node* new_node = new BinaryOp(lhs_exp, op, rhs_exp );        
  return new_node; 
}

ExpressionStatement*  BaseParser::NewExpressionStatement(Node* const node_expr){
  if(node_expr == nullptr) ErrorCritical("node_expr invalid");
  
  Expression* const exp = dynamic_cast<Expression*>(node_expr);
  ExpressionStatement* new_node = new ExpressionStatement(exp);
  return new_node; 
}

Node* BaseParser::NewLiteral(const uint32_t &value){
  Literal* new_node = new Literal(value);
  return new_node; 
}
  
Block* BaseParser::NewBlock(const std::vector<Statement*>& stmts_inht){
  Block* new_block;
  new_block = new Block();
  for(const auto stmt : stmts_inht)
    new_block->AddStatement(stmt);  
  return new_block;
}

StmtIf* BaseParser::NewStmtIf(Expression* const condition, Block* block1){
  StmtIf* new_stmt_if = new StmtIf(condition, block1);
  return new_stmt_if;
}

StmtIf* BaseParser::NewStmtIf(Expression* const condition, Block* block1, Block* block2){
  StmtIf* new_stmt_if = new StmtIf(condition, block1, block2);
  return new_stmt_if;
}

DeclStmt* BaseParser::NewDeclStmt(VarDeclList* const list){
  DeclStmt* s = new DeclStmt(list);
  return s;
}

VarDeclList* BaseParser::NewVarDeclList(const std::vector<VarDecl*>& list){
  VarDeclList* l = new VarDeclList(list);
  return l;
}

VarDecl* BaseParser::NewVarDecl(const std::string& name, const TypeId& typeId){
  VarDecl* d = new VarDecl(name, typeId);
  return d;
}

} //end namespace Common
 
