#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "BaseParser.hpp"
#include "Tokenizer.hpp"



namespace RecDescent{
  
using namespace Common;
using namespace Tokenizer;  

class Parser : public BaseParser{
  
public:  
  Parser(std::string const &file_name, Block* &programBlock);
  void Parse();
  
private:
  
  void  Prog();
  Node* Expr();
  Node* ExprPrime(Node* eprime_inht); // make it const
  Node* Term();
  Node* Factor();
  
  
  
};

} //end namespace RecDescent
