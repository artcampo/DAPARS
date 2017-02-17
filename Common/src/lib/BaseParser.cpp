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
  std::cout << message << " at: \"";

  //Go back N chars
  unit_.Error(message, CurrentLocus());
}

void BaseParser::ErrorCritical(const std::string& message){
  Error(message);
//   exit(1);
}

BinaryOp* BaseParser::NewBinaryOp(Expr* const lhs, const int op
  , Expr* const rhs, const ScopeId id, const Locus& locus){
//   if(lhs == nullptr) ErrorCritical("lhs invalid");
//   if(rhs == nullptr) ErrorCritical("rhs invalid");
  if(lhs == nullptr or rhs == nullptr) return nullptr;

  Expr* const lhs_exp = dynamic_cast<Expr*>(lhs);
  Expr* const rhs_exp = dynamic_cast<Expr*>(rhs);
  BinaryOp* new_node = new BinaryOp(lhs_exp, op, rhs_exp, id, locus);
  return new_node;
}


Var* BaseParser::NewVar(const std::string& name, const TypeId& typeId
  , const ScopeId id, const Locus& locus){
  Var* v = new Var(name, typeId, id, locus);
  return v;
}

Literal* BaseParser::NewLiteral(const uint32_t &value, const TypeId& typeId
  , const ScopeId id, const Locus& locus){
  Literal* new_node = new Literal(value, typeId, id, locus);
  return new_node;
}

Block* BaseParser::NewBlock(const std::vector<Statement*>& stmts_inht
  , const ScopeId id, const Locus& locus){
  if(stmts_inht.empty()) return nullptr;
  for(const auto stmt : stmts_inht) if(stmt == nullptr) return nullptr;

  Block* new_block;
  new_block = new Block(id, locus);
  for(const auto stmt : stmts_inht){
    new_block->AddStatement(stmt);
  }
  return new_block;
}

IfStmt* BaseParser::NewIfStmt(Expr* const condition, Block* block1
  , const ScopeId id, const Locus& locus){
  if(condition == nullptr or block1 == nullptr) return nullptr;

  IfStmt* new_stmt_if = new IfStmt(condition, block1, id, locus);
  return new_stmt_if;
}

IfStmt* BaseParser::NewIfStmt(Expr* const condition, Block* block1
  , Block* block2, const ScopeId id, const Locus& locus){
  IfStmt* new_stmt_if = new IfStmt(condition, block1, block2, id, locus);

  return new_stmt_if;
}

DeclStmt* BaseParser::NewDeclStmt(VarDeclList* const list, const ScopeId id
  , const Locus& locus){
  if(list == nullptr) return nullptr;

  DeclStmt* s = new DeclStmt(list, id, locus);
  return s;
}

VarDeclList* BaseParser::NewVarDeclList(const std::vector<VarDecl*>& list
  , const ScopeId id, const Locus& locus){
  if(list.empty()) return nullptr;
  for(const auto dec : list) if(dec == nullptr) return nullptr;

  VarDeclList* l = new VarDeclList(list, id, locus);
  return l;
}

VarDecl* BaseParser::NewVarDecl(const std::string& name
                              , const TypeId& typeId
                              , const ScopeId id
                               , const Locus& locus){
  VarDecl* d = new VarDecl(name, typeId, id, locus);
  return d;
}

AssignStmt* BaseParser::NewAssignStmt(Expr* const lhs
                                    , Expr* const rhs
                                     , const ScopeId id
                                     , const Locus& locus){
  if(lhs == nullptr or rhs == nullptr) return nullptr;
  AssignStmt* a = new AssignStmt(lhs,rhs, id, locus);
  return a;
}

} //end namespace Common

