/*
 * A test for each of the possible error messages.
 */

#include "ParserLL1RecDesc.hpp"
#include "Grammar.hpp"
#include "ASTVisitors/Dump.hpp"
#include "ASTVisitors/PrettyPrinter.hpp"
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
  /*
  if(unit.GetAstProg() != nullptr){
    std::cout << "\nAST dump:\n";
    Dump visitor_dump(unit, true);
    visitor_dump.Visit(*unit.GetAstProg());
  }
*/
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

  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){    }"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){ @ }"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){ 1; ) }"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){ 1 ) }"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){ int a }"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){ if }"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){ if( }"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){ if(int }"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){ if(1) }"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){ if(1){ }"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){ if(1){2; }"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){ 1+; }"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){ 1+(; }"), g);

  //error:15
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){ int a; int b; int a; }"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a; a=1+b; }"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a; bool b; a=a+b; }"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a; bool b; a=b+b; }"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a b c; }"), g);

  //22
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){int a,b,c; a= 2+1; a = b+c; 2 = 2; }"), g);


  //err39
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(int p1){int a; a=p1;} void main(){int a; a=1;  a=f();}"), g);

  //err40
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(int p1){int a; a=p1;} void main(){int a; a=f(2,3);}"), g);

  //err41
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(int p1, int p2){int a; a=p1;} void main(){int a; a=f(2,true);}"), g);

  //err43
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(){ return 1} void main(){ int a;}"), g);

  //kErr45
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(){ return true;} void main(){ int a;}"), g);

  //kErr84
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "int f(){ int a;} class f{} void main(){ int a;}"), g);

  //Test that should be applied
  /*
  //should say that main is being defined inside a class
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "class O1{int f(int a, int b){int c; c = a+b; return c;} void main(){ O1 inst; int b; b = inst.f(1,2);}"), g);
   */

  //tests that sigsev
  /*
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){ int a, b; if(true){int c; a=2+3;} else{ int d, e,f; d = 4 } int g; b=5+6; }"), g);

   */

  //Chaotic cluster fuck
  /*
  parse<Grammar,ParserLL1RecDesc>( std::string(
    "void main(){bool a; int b; a = true; b = 1; a = b; b = true; a = 2; }"), g);

  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){1+(2)); }"), g);
  parse<Grammar,ParserLL1RecDesc>( std::string("void main(){1++2; }"), g);
*/

  return 0;
}
