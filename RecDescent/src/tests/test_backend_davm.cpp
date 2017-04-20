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
    PassManager pm(unit);
    pm.Run();
  }

  if(unit.GetAstProg() != nullptr){
    IRGenerator visitor_irgen(unit);
    visitor_irgen.Visit(*unit.GetAstProg(), nullptr);

    std::cout << "Backend::Davm\n";
    using namespace Compiler::Backend::BackendDAVM;
    BackendDAVM b(unit, visitor_irgen.GetIRUnit() );
    b.Run();
    std::cout << "Static data segment size: " << b.GetByteCode().static_data_segment_size_ << "\n";
  }

  std::cout << "\n";
}

int main()
{

  Grammar g;
  CreateGrammarExpr(g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a; a = 1+2; }"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a, b, c; a = 4; b = 3; c = a + b; }"), g);
  
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(int p){ return p; }") + 
    "void main(){int a; a = 1; a = f(a); }"
    , g);  
  
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(int p0, int p1){ return p0 + p1; }") + 
    "void main(){int a; a = f(9,8); }"
    , g);    


  return 0;
}
