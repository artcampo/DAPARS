/*
 * Multiple tests for basic constructs.
 */

#include "ParserLL1RecDesc.hpp"
#include "Grammar.hpp"
#include "ASTVisitors/ASTVisitorDump.hpp"
#include "ASTVisitors/ASTVisitorPrettyPrinter.hpp"
#include "ASTVisitors/ASTVisitorScopes.hpp"
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
    PassManager pm(unit);
    pm.Run();
  }
  if(unit.GetAstProg() != nullptr){
    std::cout << "\nAST pretty:\n";
    ASTVisitorPrettyPrinter visitor;
    visitor.Visit(*unit.GetAstProg());
    std::cout << "\nAST dump:\n";
    ASTVisitorDump visitor_dump(unit, true);
    visitor_dump.Visit(*unit.GetAstProg());
    if(unit.NumScopes()>1){
      ASTVisitorScopes v(unit);
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
  parse<Grammar,ParserLL1RecDesc>( std::string("int a; (a=1);"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("int a;a=1;a=2;a=3;"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("int a;a=2+3+4;"), g); std::cout << "\n";
  parse<Grammar,ParserLL1RecDesc>( std::string("int a;a=2++3+4;"), g); std::cout << "\n";
  parse<Grammar,ParserLL1RecDesc>( std::string("int a;a=1+2;a=3+4;"), g); std::cout << "\n";
  parse<Grammar,ParserLL1RecDesc>( std::string("int a;a=1+2;a=3+4;a=5+6;"), g); std::cout << "\n";
  }

  {
  Grammar g;
  CreateGrammarExpr(g);
  std::cout << g;
  parse<Grammar,ParserLL1RecDesc>( std::string("int a;a=1; if(2){a=3;}"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("int a;a=1; if(2){a=3;} a=4;"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("int a;a=1; if(2){a=3+4+5;}"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("int a;a=1; if(2){a=3+4+5;} a=6;"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("int a;a=1; if(2){if(3){a=4;if(5){a=6;}}}"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("int a;a=1; if(2){if(3){a=4+5;if(6){a=7+8;}}}"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string("int a;a=1; if(2){a=3;}else{4;}"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("int a;a=1; if(2){a=3;}if(4){a=5;}else{a=6;}"), g);


  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int a; int b; bool c;"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int b, c, d; bool e f;"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int a; a=1;"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int a; a=(1+2+3);"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int a b c; b=1; c=2; a=(1+b+c);"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int a; int b; int a;"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int a; if(1){int b;}"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int a; if(1){int b;}else{int c;}"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int a; while(1 + 2){ int c; c = 1+2; }"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "bool a, b; a = true; b = false;"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int* a; int* b;"), g);

    parse<Grammar,ParserLL1RecDesc>( std::string(
    "int* a; int b, c; b = 2; a = &b; c = *a;"), g);

  /*
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "bool b1,b2,b3; b1=true; b2=false; b3=b1 and b2;"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "bool b1; int i1,i2; i1=1; i2=2; b1=i1==i2;"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "bool b1; int i1; b1=2; i1=true;"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "bool b1; int i1; bool b1;"), g);
    */
  }

  return 0;
}
