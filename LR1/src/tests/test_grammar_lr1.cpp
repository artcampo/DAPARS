#include "Parser.hpp"
#include "GrammarLR1.hpp"
#include "IRDefinition.hpp"
#include "Node.hpp"
#include "Utils.hpp"
#include "ASTVisitorCodeGenerator.hpp"
#include "ASTVisitorPrettyPrinter.hpp"
#include <iostream>
#include <memory>
#include <string>

/*
 * This parses the pars grammar of "engineering a compiler" 2nd ed, p128 
 */


int main(int argc, char **argv)
{
  if(argc != 2){
    std::cout << "Missing source input file\n";
    exit(1);
  }
  
  using namespace GrammarAnalyzer;
  
  //Terminals
  const Symbol lpar("LPAR", "(", true);
  const Symbol rpar("RPAR", ")", true);
  
  //Non-terminals
  const Symbol prog ("PROG", "PROG", false);
  const Symbol l    ("LIST", "LIST", false);
  const Symbol p    ("PAIR", "PAIR", false);
  
  
  //Grammar
  GrammarLR1 g;
  g.AddStartingRule(Rule(prog,  {l}));
  
  g.AddRule(Rule(l, {l, p}));
  g.AddRule(Rule(l, {p}));
  g.AddRule(Rule(p, {lpar, p, rpar}));
  g.AddRule(Rule(p, {lpar, rpar}));
  
  //Anaylze
  g.Analyze();
  g.BuildTables();  
  
  
  //Create parser
  Block* programBlock = nullptr;
  
  using namespace RecDescent;
  std::unique_ptr<Parser> parser(new Parser(std::string(argv[1]), programBlock));

  parser->Parse();
  
  if(programBlock == nullptr)
    std::cout << "Program block is empty!" << std::endl;
  
  std::cout << "\nPrint AST\n";
  ASTVisitorPrettyPrinter visitor;
  visitor.Visit(*programBlock);
    
  return 0;
}
