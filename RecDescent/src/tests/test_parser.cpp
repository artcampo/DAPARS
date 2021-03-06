/*
 * Multiple tests for basic constructs.
 */

#include "ParserLL1RecDesc.hpp"
#include "Grammar.hpp"
#include "ASTVisitors/Dump.hpp"
#include "ASTVisitors/PrettyPrinter.hpp"
#include "ASTVisitors/Scopes.hpp"
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
  /*
  //Pre passes ASTdump, for when attributes lead to sigsev
  if(unit.GetAstProg() != nullptr){
    std::cout << "\nAST dump (without extra info):\n";
    Dump visitor_dump(unit);
    visitor_dump.Visit(*unit.GetAstProg());
  }
  */

  if(unit.ValidAst()){
    PassManager pm(unit);
    pm.Run();
  }
  if(unit.GetAstProg() != nullptr){
    /*
    std::cout << "\nAST pretty:\n";
    PrettyPrinter visitor;
    visitor.Visit(*unit.GetAstProg());
    */
    std::cout << "\nAST dump:\n";
    Dump visitor_dump(unit, true);
    visitor_dump.Visit(*unit.GetAstProg());
    if(unit.NumScopes()>1){
      Scopes v(unit);
      v.Visit(*unit.GetAstProg());
    }

  }

  std::cout << "\n";
}

int main()
{
  {
  //Grammar
  Grammar g;
  CreateGrammarExpr(g);
  std::cout << g;

  //pass
  parse<Grammar,ParserLL1RecDesc>( "void main(){int a;a=1;a=2;a=3; }", g);
  parse<Grammar,ParserLL1RecDesc>( "void main(){int a;a=2+3+4; }", g); std::cout << "\n";
  parse<Grammar,ParserLL1RecDesc>( "void main(){int a;a=2++3+4; }", g); std::cout << "\n";
  parse<Grammar,ParserLL1RecDesc>( "void main(){int a;a=1+2;a=3+4; }" , g); std::cout << "\n";
  parse<Grammar,ParserLL1RecDesc>( "void main(){int a;a=1+2;a=3+4;a=5+6; }", g); std::cout << "\n";
  }

  {
  Grammar g;
  CreateGrammarExpr(g);
  std::cout << g;

  parse<Grammar,ParserLL1RecDesc>( "void main(){int a;a=1; if(2){a=3;} }", g);
  parse<Grammar,ParserLL1RecDesc>( "void main(){int a;a=1; if(2){a=3;} a=4; }", g);
  parse<Grammar,ParserLL1RecDesc>( "void main(){int a;a=1; if(2){a=3+4+5;} }", g);
  parse<Grammar,ParserLL1RecDesc>( "void main(){int a;a=1; if(2){a=3+4+5;} a=6; }", g);
  parse<Grammar,ParserLL1RecDesc>( "void main(){int a;a=1; if(2){if(3){a=4;if(5){a=6;}}} }", g);
  parse<Grammar,ParserLL1RecDesc>( "void main(){int a;a=1; if(2){if(3){a=4+5;if(6){a=7+8;}}} }", g);
  parse<Grammar,ParserLL1RecDesc>( "void main(){int a;a=1; if(2){a=3;}if(4){a=5;}else{a=6;} }", g);


  parse<Grammar,ParserLL1RecDesc>(
    "void main(){int a; int b; bool c; }", g);
  parse<Grammar,ParserLL1RecDesc>(
    "void main(){int b, c, d; bool e f; }", g);

  parse<Grammar,ParserLL1RecDesc>(
    "void main(){int a; a=1; }", g);

  parse<Grammar,ParserLL1RecDesc>(
    "void main(){int a; a=(1+2+3); }", g);
  parse<Grammar,ParserLL1RecDesc>(
    "void main(){int a b c; b=1; c=2; a=(1+b+c); }", g);

  parse<Grammar,ParserLL1RecDesc>(
    "void main(){ int a; int b; int a; }", g);

  parse<Grammar,ParserLL1RecDesc>(
    "void main(){ int a; if(1){int b;} }", g);

  parse<Grammar,ParserLL1RecDesc>(
    "void main(){ int a; if(1){int b;}else{int c;} }", g);

  parse<Grammar,ParserLL1RecDesc>(
    "void main(){ int a; while(1 + 2){ int c; c = 1+2; } }", g);

  parse<Grammar,ParserLL1RecDesc>(
    "void main(){ bool a, b; a = true; b = false; }", g);

  parse<Grammar,ParserLL1RecDesc>(
    "void main(){ int* a; int* b; }", g);

  parse<Grammar,ParserLL1RecDesc>(
    "void main(){ int* a; int b, c; b = 2; a = &b; c = *a; }", g);

  parse<Grammar,ParserLL1RecDesc>(
    "void main(){int a; a= 2;} void f(){bool a; a=3;}", g);

  parse<Grammar,ParserLL1RecDesc>(
    "class O1{} void main(){ O1 a;}", g);

  parse<Grammar,ParserLL1RecDesc>(
    "class O1{int a;} void main(){ O1 inst; int b; b = inst.a;}", g);

  parse<Grammar,ParserLL1RecDesc>(
    "class O1{int a;} class O2{ O1 o1;} void main(){ O2 inst; int b; b = inst.o1.a;}", g);

  parse<Grammar,ParserLL1RecDesc>(
    "class O1{int f(int a, int b){int c; c = a+b; return c;}} void main(){ O1 inst; int b; b = inst.f(1,2);}", g);

  parse<Grammar,ParserLL1RecDesc>(
    "class O1{int f(int a){return a + a_;} int a_;} void main(){ O1 inst; }", g);

  parse<Grammar,ParserLL1RecDesc>(
    "class O1{int f(){return 1;} } class O2{int g(){return inst_.f() + 1;} O1 inst_; } void main(){ O2 inst; }", g);

  parse<Grammar,ParserLL1RecDesc>(
    "class O1{int f(){return g();} int g(){return 1;} } void main(){ O1 inst;}", g);
  
  parse<Grammar,ParserLL1RecDesc>(
    "class A{int a;} class B : A{int b;} void main(){ B inst;}", g);
  
  parse<Grammar,ParserLL1RecDesc>(
    "class A{int a;} class B : A{int b;} void main(){ B inst; inst.a = 2;}", g);
  /*
  //hangs at lvalrval
  parse<Grammar,ParserLL1RecDesc>(
    "class A{int a;} class B : A{int b;} void main(){ B inst; inst.b = 2;}", g);
    */
  /*
  //these mess with the parser, big time
  parse<Grammar,ParserLL1RecDesc>( "void main(){int a; (a=1); }", g);

  parse<Grammar,ParserLL1RecDesc>( "void main(){int a;a=1; if(2){a=3;}else{4;} }", g);
    */
  }

  return 0;
}
