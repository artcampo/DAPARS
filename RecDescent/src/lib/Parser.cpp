#include "Parser.hpp"
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
  

Parser::Parser(std::string const &file_name, Block* &programBlock) 
  : BaseParser(file_name, programBlock)
{ 
}


void Parser::Parse(){

  if(num_errors_ != 0){
    std::cout << "Program syntactically incorrect\n";
  }
}

bool Parser::Prog(){
  programBlock_ = new Block();
  Node* node_expr;
  
  NextToken();
  node_expr = Expr();
  if(node_expr != nullptr){
//     std::cout << "Prog" << std::endl;
    if(token_ != Tokenizer::kToken::eof){
      std::cout << "More data after program.";
      return false;
    }
  }
  
  ExpressionStatement* exp_stmt = new ExpressionStatement(dynamic_cast<Expression*>(node_expr));
  programBlock_->statements.push_back(exp_stmt);
  
  return true;
}


// E->F E'
Node* Parser::Expr(){
//   std::cout << std::endl << "Exp";
  Node* return_node;
  Node* return_factor;
  Node* return_prime;
  
  return_factor = Factor();
  return_node   = return_factor;
  
  if(return_node != nullptr){
    return_prime = ExprPrime(return_factor);
    if(return_prime != nullptr)
      return_node = return_prime; //return plus(F,E')
  }else {
    Error("Factor missing.");
  }
  return return_node;
}


// E'-> + F E' | empty
// returns:
// - nullptr, if E' is empty
// - Node of plus(E_caller,F)
//Node* Parser::ExprPrime(const Node* lhs){
Node* Parser::ExprPrime(Node* lhs){
//   std::cout << std::endl << "Exp'";
  Node* return_node = nullptr;
  
  if(token_ == Tokenizer::kToken::plus){
    NextToken();
    Node* return_factor = Factor();
    
    /*
    return_node = new BinaryOp( dynamic_cast<const Expression*>(lhs)
                              , IR_ADD
                              , dynamic_cast<Expression*>(return_factor) );
                              */
    
    return_node = new BinaryOp( dynamic_cast<Expression*>(lhs)
                              , IR_ADD
                              , dynamic_cast<Expression*>(return_factor) );    
    
    //A new E' will op against current op+
    Node* return_expprr = ExprPrime(return_node);
    if(return_expprr != nullptr)
      return_node = return_expprr;
    
  } else{
    //check Follow(E')
    if(not(token_ == Tokenizer::kToken::eof or token_ == Tokenizer::kToken::rpar))
      Error("Expecting eof or rpar.");
  }
  
  return return_node;
}


// F := ( E ) | numerical
Node* Parser::Factor(){
//   std::cout << std::endl << "Fact";
  Node* return_node;
  
  if(token_ == Tokenizer::kToken::numerical){
    return_node = new Literal(token_int_value_);
    NextToken();
    return return_node;
  }
  
  if(token_ == Tokenizer::kToken::lpar){
    NextToken();
    return_node = Expr();
    if(token_ == Tokenizer::kToken::rpar)
      NextToken();
    else 
      Error("Expecting rpar.");
  }else 
    Error("Expectinglrpar.");
  
  return return_node;
}

  
} //end namespace RecDescent
 
