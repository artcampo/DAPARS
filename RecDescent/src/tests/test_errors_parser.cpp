/*
 * A test for each of the possible error messages.
 */

#include "ParserLL1RecDesc.hpp"
#include "Grammar.hpp"
#include "ASTVisitors/ASTVisitorDump.hpp"
#include "ASTVisitors/ASTVisitorPrettyPrinter.hpp"
#include "Passes/PassManager.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace RecDescent;
using namespace GrammarAnalyzer;
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
  std::cout << "------------------------------------------------------------------\n";
  CompilationUnit unit;

  std::unique_ptr<P> parser(new
                P(std::vector<char> (str.begin(), str.end()), unit));

  parser->Parse();
  if(unit.ValidAst()){
    PassManager pm(unit);
    pm.Run();
  }
  if(unit.GetAstProg() != nullptr){
    std::cout << "\nAST dump:\n";
    ASTVisitorDump visitor_dump(unit, true);
    visitor_dump.Visit(*unit.GetAstProg());
  }

  std::cout << "\n";
}

int main()
{
  std::cout << "Parsing of each possible error\n";
  //Grammar
  Grammar g;
  CreateGrammarExpr(g);
  std::cout << g;

  //Proper error identification
  parse<Grammar,ParserLL1RecDesc>( std::string("  "), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("@"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("1; )"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("1 )"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("int a"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string("if"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("if("), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("if(int"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("if(1)"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("if(1){"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("if(1){2;"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string("1+;"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string("1+(;"), g);

  //error:15
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int a; int b; int a;"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int a; a=1+b;"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int a; bool b; a=a+b;"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int a; bool b; a=b+b;"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int a b c;"), g);

  //22
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int a,b,c; a= 2+1; a = b+c; 2 = 2;"), g);



  //Chaotic cluster fuck
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "bool a; int b; a = true; b = 1; a = b; b = true; a = 2;"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string("1+(2));"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("1++2;"), g);

  return 0;
}
