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
    }
  }
}

bool Parser::Expr(){
  // E->F E'
  if(Factor()){
    std::cout << "Fact" << std::endl;
    bool success = ExprPrime();
    if(success) std::cout << "Exp" << std::endl;
  }else Error("Factor missing.");
  return false;
}

bool Parser::ExprPrime(){
  // E'-> + F | empty
  if(token_ == Tokenizer::kToken::plus){
    NextToken();
    bool success = Factor();
    if(success) std::cout << "Exp'" << std::endl;
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
  // F := ( E ) | numerical
  if(token_ == Tokenizer::kToken::numerical){
    std::cout << "Fact" << std::endl;
    return true;
  }
  if(token_ == Tokenizer::kToken::lpar){
    NextToken();
    bool success = Expr();
    NextToken();
    if(token_ == Tokenizer::kToken::rpar){
      std::cout << "Fact" << std::endl;
      return true;
    }
  }
  return false;
}




} //end namespace RecDescent
