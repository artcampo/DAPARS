#include "ParserLL1RecDesc.hpp"
#include "Grammar.hpp"
#include "IRDefinition.hpp"
#include "Node.hpp"
#include "Utils.hpp"
#include "ASTVisitorCodeGenerator.hpp"
#include "ASTVisitorPrettyPrinter.hpp"
#include <iostream>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

using namespace RecDescent;
using namespace GrammarAnalyzer;
#include "../../../Common/src/lib/grammars/grammar_expr.cpp"

int main(int argc, char **argv)
{
  if(argc != 2){
    std::cout << "Missing source input file\n";
    exit(1);
  }
  
  Block* programBlock = nullptr;
  
  using namespace RecDescent;
  std::unique_ptr<ParserLL1RecDesc<>> parser(
    new ParserLL1RecDesc<>(std::string(argv[1]), programBlock));

  parser->Parse();
  
  if(programBlock == nullptr){
    std::cout << "Program block is empty!" << std::endl;
    return 1;
  }
  
  std::cout << "Print AST\n";
  ASTVisitorPrettyPrinter visitor;
  visitor.Visit(*programBlock);
    
  return 0;
}
