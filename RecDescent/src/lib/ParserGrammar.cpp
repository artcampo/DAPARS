#include "Parser.hpp"
#include "Node.hpp"
#include <iterator>
#include <fstream>
#include <iostream>
#include <exception>

namespace RecDescent{


bool Parser::Prog(){
  NextToken();
  if(Expr()){
    std::cout << "Prog" << std::endl;
    if(token_ != Tokenizer::kToken::eof){
      std::cout << "More data after program.";
      return false;
    }
  }
 std::cout << "Program is syntactically correct." << std::endl;
 std::cout << "-> " << str(token_);
 return true;
}

// E->F E'
Node* Parser::Expr(){
  std::cout << std::endl << "Exp";
  Node* return_node;
  
  if(Factor() != nullptr){
    return_node = ExprPrime();
  }else {
    Error("Factor missing.");
  }
  return return_node;
}

// E'-> + F E' | empty
// returns:
// - nullptr, if E' is empty
// - Node of plus(E_caller,F)
Node* Parser::ExprPrime(){
  std::cout << std::endl << "Exp'";
  Node* return_node = nullptr;
  
  if(token_ == Tokenizer::kToken::plus){
   
    NextToken();
    Factor();
//     if(success) std::cout << "Exp'" << std::endl;
    ExprPrime();
    
    //return_node = new BinaryOp($1, IR_ADD, $3);
    
  } else{
    //check Follow(E')
    if(not(token_ == Tokenizer::kToken::eof or token_ == Tokenizer::kToken::rpar))
      Error("Expecting eof or rpar.");
    }
  
  return return_node;
}


Node* Parser::Factor(){
  std::cout << std::endl << "Fact";
  Node* return_node;
  // F := ( E ) | numerical
  if(token_ == Tokenizer::kToken::numerical){
    Node* return_node = new Literal(2);
    NextToken();
  }
  
  if(token_ == Tokenizer::kToken::lpar){
    NextToken();
    return_node = Expr();
    //std::cout << "[[test )]]";
    if(token_ == Tokenizer::kToken::rpar){
      NextToken();
//       std::cout << "Fact" << std::endl;
    }else {
      Error("Expecting rpar.");
    }
  } else 
    Error("Expectinglrpar.");
  
  return return_node;
}




} //end namespace RecDescent
