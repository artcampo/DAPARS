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
  Node* e_synt = Expr();
  
  if(e_synt != nullptr){
//     std::cout << "Prog" << std::endl;
    if(token_ != Tokenizer::kToken::eof){
      Error("More data after program.");
    }
  }
  
  programBlock_ = new Block();
  ExpressionStatement* exp_stmt = new ExpressionStatement(dynamic_cast<Expression*>(e_synt));
  programBlock_->statements.push_back(exp_stmt);
  
}


Node* ParserLL1RecDesc::Expr(){
//   std::cout << std::endl << "Exp";
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
//   std::cout << std::endl << "Exp'";
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
    if(not(token_ == Tokenizer::kToken::eof or token_ == Tokenizer::kToken::rpar))
      Error("Expecting eof or rpar.");
    eprime_synt = eprime_inht;
  }
  
  return eprime_synt;
}


// F := ( E ) | numerical
Node* ParserLL1RecDesc::Factor(){
//   std::cout << std::endl << "Fact";
  Node* f_synt;
  
  if(token_ == Tokenizer::kToken::numerical){
    f_synt = new Literal(token_int_value_);
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
  }else 
    Error("Expecting lpar.");
  
  return f_synt;
}

  
} //end namespace RecDescent
 
