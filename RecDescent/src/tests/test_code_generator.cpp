/*
 * Multiple tests for basic constructs.
 */

#include "ParserLL1RecDesc.hpp"
#include "Grammar.hpp"
#include "ASTVisitors/Dump.hpp"
#include "ASTVisitors/PrettyPrinter.hpp"
#include "ASTVisitors/Scopes.hpp"
#include "IR/IRGenerator.hpp"
#include "CompilationUnit.hpp"
#include "Passes/PassManager.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace RecDescent;
// using namespace GrammarAnalyzer;
using GrammarAnalyzer::Grammar;
using Compiler::PassManager;
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
    if(unit.NumScopes()>1){
      Scopes v(unit);
      v.Visit(*unit.GetAstProg());
      std::cout << std::endl;
    }

    PassManager pm(unit);
    pm.Run();
  }
  if(unit.GetAstProg() != nullptr){
    /*
    std::cout << "\nAST dump:\n";
    Dump visitor_dump(unit, true);
    visitor_dump.Visit(*unit.GetAstProg());
*/

    IRGenerator visitor_irgen(unit);
    visitor_irgen.Visit(*unit.GetAstProg(), nullptr);

    std::cout << "\nIR dump:\n";
    visitor_irgen.EndOfProgram();
  }

  std::cout << "\n";
}

int main()
{

  Grammar g;
  CreateGrammarExpr(g);
  std::cout << g;


  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a,b,c; while(true){b=2+3;} c=3+4; }"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){ int a, b; if(true){int c; a=2+3;} else{ int d, e,f; d = 4; } int g; b=5+6; }"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a,b; int* p; a=*p; b=2; p = &b; a = b + *p;}"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a; a= 2;} void f(int p1){int a; a=p1;}"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(int p1){int a; a=p1; return a;} void main(){int a; a=f(2);}"), g);

  /*
  //Causes sigsev (hahah)
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a; a= *&a;}"), g);
*/

  return 0;
}
