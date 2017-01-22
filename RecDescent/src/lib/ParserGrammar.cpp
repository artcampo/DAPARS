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
 return true;
}

bool Parser::Expr(){
  std::cout << "Exp"<< std::endl;
  // E->F E'
  if(Factor()){
//    Accept
    bool success = ExprPrime();
//     if(success) std::cout << "Exp" << std::endl;
  }else Error("Factor missing.");
  return false;
}

bool Parser::ExprPrime(){
  std::cout << "Exp'"<< std::endl;
  // E'-> + F E' | empty
  if(token_ == Tokenizer::kToken::plus){
    std::cout << "+" << std::endl;
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
  std::cout << "Fact"<< std::endl;
  // F := ( E ) | numerical
  if(token_ == Tokenizer::kToken::numerical){
    NextToken();
    std::cout << "Fact: num" << std::endl;
    return true;
  }
  if(token_ == Tokenizer::kToken::lpar){
    NextToken();
    bool success = Expr();
    if(token_ == Tokenizer::kToken::rpar){
      NextToken();
//       std::cout << "Fact" << std::endl;
      return true;
    }
  }
  return false;
}




} //end namespace RecDescent
