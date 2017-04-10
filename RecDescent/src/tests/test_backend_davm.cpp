/*
 * Multiple tests for: Backend-DAVM
 */

#include "ParserLL1RecDesc.hpp"
#include "Grammar.hpp"
#include "ASTVisitors/Dump.hpp"
#include "ASTVisitors/PrettyPrinter.hpp"
#include "ASTVisitors/Scopes.hpp"
#include "IR/IRGenerator.hpp"
#include "CompilationUnit.hpp"
#include "Passes/PassManager.hpp"
#include "Backend/BackendDAVM/BackendDAVM.hpp"
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

  bool dump_decorated_ast_before_IRGen = true;

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
    if(dump_decorated_ast_before_IRGen){
      std::cout << "\nAST fully decorated dump:\n";
      Dump visitor_dump(unit, true, true);
      visitor_dump.Visit(*unit.GetAstProg());
    }

    IRGenerator visitor_irgen(unit);
    visitor_irgen.Visit(*unit.GetAstProg(), nullptr);

    std::cout << "\nIR dump:\n";
    visitor_irgen.EndOfProgram();
    
    //
    using namespace Compiler::Backend::BackendDAVM;
    BackendDAVM b(unit);
    b.Run();
  }

  std::cout << "\n";
}

int main()
{

  Grammar g;
  CreateGrammarExpr(g);
  std::cout << g;

/*
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a; a = 1+2; }"), g);
*/


  return 0;
}
