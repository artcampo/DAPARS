#include "Parser.hpp"
#include <iterator>
#include <fstream>
#include <iostream>
#include <exception>

namespace RecDescent{


void Parser::Error(const std::string& message){
  std::cout << message << " at: \n";
}

bool Parser::Expr(){
  // E->F E'
  if(Factor()){
    return ExprPrime();
  }else Error("Factor missing.");
  return false;
}

bool Parser::ExprPrime(){
  // E'-> + F | empty
  if(token_ == Tokenizer::kToken::plus){
    return Factor();
  } else{
    //check Follow(E')
    return true;
  }
  return false;
}


bool Parser::Factor(){
  return false;
}




} //end namespace RecDescent
