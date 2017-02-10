#include "ParserLL1RecDesc.hpp"
#include "Node.hpp"
#include "IRDefinition.hpp"

#include <iterator>
#include <fstream>
#include <iostream>
#include <exception>


namespace RecDescent{

using namespace Common;
using namespace IRDefinition;
using namespace SubtypesArithmetic;  
  

ParserLL1RecDesc::ParserLL1RecDesc(std::string const &file_name, Block* &programBlock)
//   , Grammar& grammar) 
  : BaseParser(file_name, programBlock){}
//   , grammar_(grammar){}

ParserLL1RecDesc::ParserLL1RecDesc(const std::vector<char>& parse_data, Block* &programBlock)
//   , Grammar& grammar) 
  : BaseParser(parse_data, programBlock){}
//   , grammar_(grammar){}    

void ParserLL1RecDesc::Parse(){

  Prog();
  if(num_errors_ != 0){
    std::cout << "Program syntactically incorrect\n";
  }
}


void ParserLL1RecDesc::Prog(){
  NextToken();
  Block* stmts_synt = Stmts();
  
  if(stmts_synt != nullptr){
//     std::cout << "Prog" << std::endl;
    if(token_ != Tokenizer::kToken::eof){
      Error("More data after program.");
    }
  }
  
  programBlock_ = stmts_synt;
  
}

//TODO: should return Expr*
Node* ParserLL1RecDesc::Expr(){
  std::cout << "Exp\n";
  Node* eprime_synt = nullptr;
  Node* term_synth  = Term();
  
  if(term_synth != nullptr){
    eprime_synt = ExprPrime(term_synth);
    if(eprime_synt == nullptr)
      Error("E' missing");
  }else {
    Error("Term missing.");
  }
  return eprime_synt;
}


Node* ParserLL1RecDesc::Term(){
  return Factor();
}



// E'    := + T E'     ** E'1.inht = new Node(+, E'.inht, T.node)
//                        E'.synt  = E'1.synt
//       |  empty      ** E'.synt  = E'1.synt
Node* ParserLL1RecDesc::ExprPrime(Node* eprime_inht){
  std::cout << "Exp'\n";
  Node* eprime_synt = nullptr;
  
  if(token_ == Tokenizer::kToken::plus){
    NextToken();
    Node* t_synt = Term();
    
    Node* eprime1_inht = NewBinaryOp(eprime_inht, IR_ADD, t_synt);  
    
    //A new E' will op against current op+
    eprime_synt = ExprPrime(eprime1_inht);
    if(eprime_synt == nullptr)
      Error("E' error");
    
  } else{
    //check Follow(E')
    if(not(token_ == Tokenizer::kToken::eof 
        or token_ == Tokenizer::kToken::rpar
        or token_ == Tokenizer::kToken::kwd_if
        or token_ == Tokenizer::kToken::semicolon
          ))
      Error("Expecting eof or rpar.");
    eprime_synt = eprime_inht;
  }
  
  return eprime_synt;
}


// F := ( E ) | numerical
Node* ParserLL1RecDesc::Factor(){
  std::cout << "Fact\n";
  Node* f_synt;
  
  if(token_ == Tokenizer::kToken::numerical){
    f_synt = NewLiteral(token_int_value_);
    NextToken();
    return f_synt;
  }
  
  if(token_ == Tokenizer::kToken::lpar){
    NextToken();
    f_synt = Expr();
    if(token_ == Tokenizer::kToken::rpar)
      NextToken();
    else 
      Error("Expecting rpar.");
  }else{
    Error("Expecting lpar.");
    NextToken();
    return Factor();
  }
  
  return f_synt;
}

Statement* ParserLL1RecDesc::Stmt(){
  std::cout << "stmt\n";
  Statement* stmt_synt = nullptr;
  
  if(token_ == Tokenizer::kToken::kwd_if){
    //if(E){STMTS}
    NextToken();
    if(token_ != Tokenizer::kToken::lpar) Error("if missing lpar.");
    NextToken();
    Node* expr_synt = Expr();
    if(expr_synt == nullptr) Error("if condition wrong.");
    
    if(token_ != Tokenizer::kToken::rpar) Error("if missing rpar.");
    NextToken();
    if(token_ != Tokenizer::kToken::lcbr) Error("if missing lcbr.");
    NextToken();
    Block* stmts_synt = Stmts();
    if(token_ != Tokenizer::kToken::rcbr) Error("if missing rcbr.");
    NextToken();
    
    
    //StmtIf* stmt_if = NewStmtIf(expr_synt, stmts_synt);
    stmt_synt = NewStmtIf(dynamic_cast<Expression*>(expr_synt), stmts_synt);
    
  }else{
    Node* expr_synt = Expr();
    stmt_synt       = NewExpressionStatement(expr_synt);
    
    if(token_ != Tokenizer::kToken::semicolon) Error("Expecting semicolon.");
    NextToken();
  }
  return stmt_synt;
}

Block* ParserLL1RecDesc::Stmts(){
  std::cout << "stmts\n";
  Block* stmts_synt = nullptr;
  
  if(  token_ == Tokenizer::kToken::numerical
    or token_ == Tokenizer::kToken::lpar
    or token_ == Tokenizer::kToken::plus
    or token_ == Tokenizer::kToken::kwd_if){
      Statement* stmt_synth = Stmt();
//       NextToken();
      
      
      stmts_synt = NewBlock(stmt_synth);
      
      //TODO: Block::AddStatement
      Block* stmts1_synt = Stmts();
      if(stmts1_synt != nullptr) stmts_synt->AddStatement(stmts1_synt->FirstStatement());
      
    }
  else{
    //check follow(Stmts)
  }  
  return stmts_synt;
}
  
} //end namespace RecDescent
 
