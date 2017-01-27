#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "BaseParser.hpp"
#include "Tokenizer.hpp"


namespace RecDescent{
  
using namespace Common;  

class Parser : public BaseParser{
  
public:  
  Parser(std::string const &file_name, Block* &programBlock);
  void Parse();
  
private:
  
  bool  Prog();
  Node* Expr();
  //Node* ExprPrime(const Node* lhs);
  Node* ExprPrime(Node* lhs);
  Node* Factor();
  
  
};

} //end namespace RecDescent
