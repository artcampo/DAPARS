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
  std::vector<Statement*> stmts_inht;
  Block* stmts_synt = Stmts(stmts_inht);
  
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
  std::cout << "<-Exp\n";
  return eprime_synt;
}


Node* ParserLL1RecDesc::Term(){
//   std::cout << "T\n";
  return Factor();
}



// E'    := + T E'     ** E'1.inht = new Node(+, E'.inht, T.node)
//                        E'.synt  = E'1.synt
//       |  empty      ** E'.synt  = E'1.synt
Node* ParserLL1RecDesc::ExprPrime(Node* eprime_inht){
  std::cout << "Exp'\n";
  Node* eprime_synt = nullptr;
  
  if(token_ == Tokenizer::kToken::plus){
    Accept(kToken::plus, "Expecting +.");
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
        or token_ == Tokenizer::kToken::rcbr
        or token_ == Tokenizer::kToken::kwd_if
        or token_ == Tokenizer::kToken::semicolon
          ))
      Error("Expecting eof or rpar.");
    eprime_synt = eprime_inht;
  }
  
  std::cout << "<-Exp'\n";
  return eprime_synt;
}


// F := ( E ) | numerical
Node* ParserLL1RecDesc::Factor(){
  std::cout << "Fact\n";
  Node* f_synt;
  
  if(token_ == Tokenizer::kToken::numerical){
    f_synt = NewLiteral(token_int_value_);
    Accept(kToken::numerical, "Expecting numerical.");
  }else //TODO: Accept returning bool
  if(token_ == Tokenizer::kToken::lpar){
    Accept(kToken::lpar, "Expecting lpar.");
    f_synt = Expr();
    Accept(kToken::rpar, "Expecting rpar.");
  }else{
    Error("Expecting lpar.");
    NextToken();
    return Factor();
  }
  
  std::cout << "<-Fact\n";
  return f_synt;
}



Statement* ParserLL1RecDesc::Stmt(){
  std::cout << "stmt\n";
  Statement* stmt_synt = nullptr;
  
  if(token_ == Tokenizer::kToken::kwd_if){
    //if(E){STMTS}
    std::cout << "stmt::if\n";
    
    Accept(kToken::kwd_if, "Expecting if.");
    Accept(kToken::lpar, "if missing lpar.");
    Node* expr_synt = Expr();
    if(expr_synt == nullptr) Error("if condition wrong.");
    
    Accept(kToken::rpar, "if missing rpar.");
    
    Accept(kToken::lcbr, "if missing lcbr.");
    std::vector<Statement*> stmts_inht;
    Block* stmts_synt = Stmts(stmts_inht);
    Accept(kToken::rcbr, "if missing rcbr.");
    
    Block* ifelse_synt = IfElse();
    
    if(ifelse_synt == nullptr)
      stmt_synt = NewStmtIf(dynamic_cast<Expression*>(expr_synt), stmts_synt);
    else
      stmt_synt = NewStmtIf(dynamic_cast<Expression*>(expr_synt), stmts_synt, ifelse_synt);
    
  }else{
    std::cout << "stmt::exp stmt\n";
    Node* expr_synt = Expr();
    stmt_synt       = NewExpressionStatement(expr_synt);
    
    Accept(kToken::semicolon, "Expecting semicolon.");
  }
  std::cout << "<-stmt\n";
  return stmt_synt;
}

Block* ParserLL1RecDesc::IfElse(){
  std::cout << "IfElse\n";
  Block* ifelse_synt = nullptr;
  if(token_ == Tokenizer::kToken::kwd_else){
    Accept(kToken::kwd_else, "else missing kwd_else.");
    Accept(kToken::lcbr, "else missing lcbr.");
    std::vector<Statement*> stmts_inht;
    Block* stmts_synt = Stmts(stmts_inht);
    if(stmts_synt == nullptr) Error("Statements within else wrong.");
    ifelse_synt = stmts_synt;
    Accept(kToken::rcbr, "else missing rcbr.");
  }
  std::cout << "<-IfElse\n";
  return ifelse_synt;
}

Block* ParserLL1RecDesc::Stmts(std::vector<Statement*>& stmts_inht){
  std::cout << "stmts\n";
  Block* stmts_synt = nullptr;
  
  if(  token_ == Tokenizer::kToken::numerical
    or token_ == Tokenizer::kToken::lpar
    or token_ == Tokenizer::kToken::plus
    or token_ == Tokenizer::kToken::kwd_if){
      Statement* stmt_synth = Stmt();
      
      stmts_inht.push_back(stmt_synth);
      stmts_synt = Stmts(stmts_inht);

    }
  else{
    //check follow(Stmts)
    if(not (token_ == Tokenizer::kToken::eof
         or token_ == Tokenizer::kToken::rcbr))
      Error("Block not finishing in eof or rcbr");
    stmts_synt = NewBlock(stmts_inht);
  }  
  std::cout << "<-stmts\n";
  return stmts_synt;
}
  
} //end namespace RecDescent
 
