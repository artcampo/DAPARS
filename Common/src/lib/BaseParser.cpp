#include "BaseParser.hpp"
#include "Node.hpp"
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
  , continue_parsing_(true)
{
  std::cout << "Parsing: \"";
  for(const auto& it : parse_data ) std::cout << it;
  std::cout << "\"\n";
  unit_.SetFileData(&file_data_);
//   unit_.scope_ = new LexicalScope(nullptr);
}

BaseParser::BaseParser(std::string const &file_name, CompilationUnit& unit)
  : file_(std::ifstream (file_name.c_str(), std::ios::binary) )
  , file_data_(std::vector<char> ((std::istreambuf_iterator<char>(file_)),
                                   std::istreambuf_iterator<char>()) )
  , current_position_(file_data_.cbegin())
  , skip_symbols_ {' ','\n'}
  , unit_(unit)
  , num_errors_(0)
  , continue_parsing_(true)
{
//   unit_.scope_ = new LexicalScope(nullptr);
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
    }

    if(token_ == kToken::error){
      Error("[err:2] Token not recognized");
    }
  }
  /*
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

std::unique_ptr<BinaryOp>&&
BaseParser::NewBinaryOp(std::unique_ptr<Expr>& lhs, const int op
  , std::unique_ptr<Expr>& rhs, const ScopeId id, const Locus& locus){
//   if(lhs == nullptr) ErrorCritical("lhs invalid");
//   if(rhs == nullptr) ErrorCritical("rhs invalid");
  if(lhs.get() == nullptr or rhs.get() == nullptr)
    return std::move(std::unique_ptr<BinaryOp>(nullptr));

  return std::move(std::make_unique<BinaryOp>(lhs, op, rhs, id, locus));
}


std::unique_ptr<Var>&&
BaseParser::NewVar(const std::string& name, const Type& type
  , const ScopeId id, const Locus& locus){
  return std::move(std::make_unique<Var>(name, type, id, locus));
}

std::unique_ptr<Literal>&&
BaseParser::NewLiteral(const uint32_t &value, const Type& type
  , const ScopeId id, const Locus& locus){
  return std::move(std::make_unique<Literal>(value, type, id, locus));
}

std::unique_ptr<Block>&&
BaseParser::NewBlock(std::vector<std::unique_ptr<Statement>>& stmts_inht
  , const ScopeId id, const Locus& locus){
  if(stmts_inht.empty()) return std::move(std::unique_ptr<Block>(nullptr));
  for(auto& stmt : stmts_inht)
    if(stmt.get() == nullptr) return std::move(std::unique_ptr<Block>(nullptr));

  std::unique_ptr<Block> new_block = std::make_unique<Block>(id, locus);
  for(auto& stmt : stmts_inht){
    new_block->AddStatement(stmt);
  }
  return std::move(new_block);
}

std::unique_ptr<IfStmt>&&
BaseParser::NewIfStmt(std::unique_ptr<Expr>& condition, std::unique_ptr<Block>& block1
  , const ScopeId id, const Locus& locus){
  if(condition.get() == nullptr or block1.get() == nullptr)
    return std::move(std::unique_ptr<IfStmt>(nullptr));

  return std::move(std::make_unique<IfStmt>(condition, block1, id, locus));
}

std::unique_ptr<IfStmt>&&
BaseParser::NewIfStmt(std::unique_ptr<Expr>& condition, std::unique_ptr<Block>& block1
  , std::unique_ptr<Block>& block2, const ScopeId id, const Locus& locus){

  return std::move(std::make_unique<IfStmt>(condition, block1, block2, id, locus));
}

std::unique_ptr<DeclStmt>&&
BaseParser::NewDeclStmt(std::unique_ptr<VarDeclList>& list, const ScopeId id
  , const Locus& locus){
  if(list.get() == nullptr)
    return std::move(std::unique_ptr<DeclStmt>(nullptr));

  return std::move(std::make_unique<DeclStmt>(list, id, locus));
}

std::unique_ptr<VarDeclList>&&
BaseParser::NewVarDeclList(std::vector<std::unique_ptr<VarDecl>>& list
  , const ScopeId id, const Locus& locus){
  if(list.empty()) return std::move(std::unique_ptr<VarDeclList>(nullptr));
  for(auto& dec : list) if(dec.get() == nullptr) return std::move(std::unique_ptr<VarDeclList>(nullptr));

  return std::move(std::make_unique<VarDeclList>(list, id, locus));
}

std::unique_ptr<VarDecl>
BaseParser::NewVarDecl(const std::string& name
                              , const Type& type
                              , const ScopeId id
                               , const Locus& locus){
  return std::make_unique<VarDecl>(name, type, id, locus);
}

std::unique_ptr<AssignStmt>&&
BaseParser::NewAssignStmt(std::unique_ptr<Expr>& lhs
                                    , std::unique_ptr<Expr>& rhs
                                     , const ScopeId id
                                     , const Locus& locus){
  if(lhs.get() == nullptr or rhs.get() == nullptr)
    return std::move(std::unique_ptr<AssignStmt>(nullptr));
  return std::move(std::make_unique<AssignStmt>(lhs,rhs, id, locus));
}

std::unique_ptr<WhileStmt>&&
BaseParser::NewWhileStmt(std::unique_ptr<Expr>& condition, std::unique_ptr<Block>& body
    , const ScopeId id, const Locus& locus){
  if(condition.get() == nullptr or body.get() == nullptr){
    std::cout << "Null NewWhileStmt\n";
    return std::move(std::unique_ptr<WhileStmt>(nullptr));
  }

  return std::move(std::make_unique<WhileStmt>( condition, body, id, locus));
}

} //end namespace Common

