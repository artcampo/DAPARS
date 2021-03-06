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
// #include "../../../Common/src/lib/grammars/grammar_control_flow.cpp"



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

  if(unit.GetAstProg() != nullptr and not unit.HasErrors()){
    IRGenerator visitor_irgen(unit);
    visitor_irgen.Visit(*unit.GetAstProg(), nullptr);

    std::cout << "Backend::Davm\n";
    using namespace Compiler::Backend::BackendDAVM;
    BackendDAVM b(unit, visitor_irgen.GetIRUnit() );
    b.Run();
    std::cout << "Static data segment size: " << b.GetByteCode().StaticDataSegmentSize() << "\n";
  }

  std::cout << "\n";
}

int main()
{

  Grammar g;
  CreateGrammarExpr(g);

  //basic mem access and arith
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a; a = 1; a = a + 2; }"), g);

  //different offsets to main data segment
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a, b, c; a = 4; b = 3; c = a + b; }"), g);

  //return value
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(int p){ return p; }") +
    "void main(){int a; a = 1; a = f(a); }"
    , g);

  //args mixed: reg and stack
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(int p0, int p1){ return p0 + p1; }") +
    "void main(){int a; a = f(9,8); }"
    , g);

  //3 args in stack
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(int p0, int p1, int p2, int p3){ return p0 + p1 + p2 + p3; }") +
    "void main(){int a; a = f(9,8,7,6); }"
    , g);

  //locals alloc, size = 1
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(){ int a; return a; }") +
    "void main(){int a; a = f(); }"
    , g);

  //locals alloc, size = 4
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(){ int a,b,c,d; return a + d; }") +
    "void main(){int a; a = f(); }"
    , g);

  //If/else
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a, b, c, d; a = 1; b = 0;") +
    "if((a+b) < 2){a = 2;} else {a=1;} c = a; d = b; }"
    , g);

  //patching of funcalls
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(){ return 1; }") +
    "int g(){ return f() + 2; }"
    "void main(){int a; a = f() + g();}"
    , g);

  //bools and ors
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){bool a, b, c; a = true; b = false; c = a or b;") +
    "c = false or true; }"
    , g);

  //LT labels VS RT labels
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a, b, c; a = a + 10; b = 11; c = 12;} ") +
    "int f(){int a, b, c;     a = a + 10; b = 11; c = 12;}"
    , g);


  //equal comparison
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){bool b; int a, c; a = 10; c = 11; b = a == c;} ")
    , g);

  //lang_lib: test
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "bool main(){int a, b; a = 1; b = 2; return __test(false); } ")
    , g);

  //and
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){bool a,b; a = true; b = false; a = a and b;} ")
    , g);

  return 0;
}
