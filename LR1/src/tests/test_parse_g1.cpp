#include "ParserLR1.hpp"
#include "GrammarLR1.hpp"
#include "IRDefinition.hpp"
#include "Node.hpp"
#include "Utils.hpp"
#include "ASTVisitorPrettyPrinter.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace GrammarAnalyzer;
#include "../../../Common/src/lib/grammars/grammar_pars.cpp"

/*
 * This parses the pars grammar of "engineering a compiler" 2nd ed, p128
 */

int main(int argc, char **argv)
{
  if(argc != 2){
    std::cout << "Missing source input file\n";
    exit(1);
  }

  //Grammar
  GrammarLR1 g;
  CreateGrammar(g);
  g.DumpTables();

  CompilationUnit unit;

  using namespace LR1;
  std::unique_ptr<ParserLR1> parser(new
                ParserLR1(std::string(argv[1]), unit, g));

  parser->Parse();

  /*
  if(programBlock == nullptr)
    std::cout << "Program block is empty!" << std::endl;

  std::cout << "\nPrint AST\n";
  ASTVisitorPrettyPrinter visitor;
  visitor.Visit(*programBlock);
*/

  return 0;
}
