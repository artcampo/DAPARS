#include "Parser.hpp"
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

bool Parser::Expr(){
  std::cout << std::endl << "Exp";
  bool success;
  // E->F E'
  if(Factor()){
//    Accept
    success = ExprPrime();
//     if(success) std::cout << "Exp" << std::endl;
  }else Error("Factor missing.");
  return success;
}

bool Parser::ExprPrime(){
  std::cout << std::endl << "Exp'";
  // E'-> + F E' | empty
  if(token_ == Tokenizer::kToken::plus){
   
    NextToken();
    bool success = Factor();
//     if(success) std::cout << "Exp'" << std::endl;
    success &= ExprPrime();
    return success;
  } else{
    //check Follow(E')
    if(token_ == Tokenizer::kToken::eof or token_ == Tokenizer::kToken::rpar)
      return true;
    else
      Error("Expecting eof or rpar.");
  }
  return false;
}


bool Parser::Factor(){
  bool success = true;
  std::cout << std::endl << "Fact";
  // F := ( E ) | numerical
  if(token_ == Tokenizer::kToken::numerical){
    NextToken();
    return success;
  }
  if(token_ == Tokenizer::kToken::lpar){
    NextToken();
    success = Expr();
    //std::cout << "[[test )]]";
    if(token_ == Tokenizer::kToken::rpar){
      NextToken();
//       std::cout << "Fact" << std::endl;
      return true;
    }else {
      Error("Expecting rpar.");
      return false;
    }
  }
  return success;
}




} //end namespace RecDescent
