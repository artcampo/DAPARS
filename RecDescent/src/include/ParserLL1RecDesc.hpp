#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "BaseParser.hpp"
#include "Tokenizer.hpp"
#include "Grammar.hpp"


namespace RecDescent{
  
using namespace Common;
using namespace Tokenizer;  
using namespace GrammarAnalyzer;
using namespace DebugLog;

template<class PolicyDebugLog = DebugLogNull>
class ParserLL1RecDesc 
  : public BaseParser<PolicyDebugLog>{
  
public:  
  ParserLL1RecDesc(std::string const &file_name, Block* &programBlock);
  ParserLL1RecDesc(const std::vector<char>& parse_data, Block* &programBlock);
  void Parse();
  
private:
//   Grammar& grammar_;
  
  void  Prog();
  
  //Expressions
  Node* Expr();
  Node* ExprPrime(Node* eprime_inht); // make it const
  Node* Term();
  Node* Factor();
  
  //Control Flow
  Block*      Stmts(std::vector<Statement*>& stmts_inht);
  Statement*  Stmt();
  Block*      IfElse();

  
};

} //end namespace RecDescent
