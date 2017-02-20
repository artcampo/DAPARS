/*
 * Multiple tests for basic constructs.
 */

#include "ParserLL1RecDesc.hpp"
#include "Grammar.hpp"
#include "ASTVisitorDump.hpp"
#include "ASTVisitorPrettyPrinter.hpp"
#include "ASTVisitorScopes.hpp"
#include "ASTVisitorCodeGenerator.hpp"
#include "CompilationUnit.hpp"
#include "Passes/PassManager.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace RecDescent;
// using namespace GrammarAnalyzer;
using GrammarAnalyzer::Grammar;
#include "../../../Common/src/lib/grammars/grammar_expr.cpp"
#include "../../../Common/src/lib/grammars/grammar_control_flow.cpp"

/*
 * Uses pars grammar of "engineering a compiler" 2nd ed, p128.
 * Parses multiple strings, one at a time.
 */

template <class G, class P>
void parse(const std::string& str, G& g)
{
  std::cout << "---------------------------------------------------\n";

  CompilationUnit unit;

  std::unique_ptr<P> parser(new
                P(std::vector<char> (str.begin(), str.end()), unit));

  parser->Parse();
  if(unit.ValidAst()){
    PassManager pm(unit);
    pm.Run();
  }
  if(unit.ast_.block_ != nullptr){
    std::cout << "\nAST dump:\n";
    ASTVisitorDump visitor_dump;
    visitor_dump.Visit(*unit.ast_.block_);
    CodeGen visitor_codegen;
    visitor_codegen.Visit(*unit.ast_.block_, nullptr);
//     visitor_codegen.EndOfProgram();
  }

  std::cout << "\n";
}

int main()
{

  Grammar g;
  CreateGrammarExpr(g);
  std::cout << g;

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "if(1){int a;} int a;"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "if(1){int a;}else{int a;} int a;"), g);


  return 0;
}
