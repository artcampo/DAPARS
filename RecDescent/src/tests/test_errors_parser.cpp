/*
 * A test for each of the possible error messages.
 */

#include "ParserLL1RecDesc.hpp"
#include "Grammar.hpp"
#include "ASTVisitorDump.hpp"
#include "ASTVisitorPrettyPrinter.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace RecDescent;
using namespace GrammarAnalyzer;
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
  Block* programBlock = nullptr;
  
  
  std::unique_ptr<P> parser(new 
                P(std::vector<char> (str.begin(), str.end()), programBlock));

  parser->Parse();
  if(programBlock != nullptr){
    std::cout << "\nAST dump:\n";
    ASTVisitorDump visitor_dump;
    visitor_dump.Visit(*programBlock);      
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
  
  //Chaotic cluster fuck
  parse<Grammar,ParserLL1RecDesc>( std::string("1+(2));"), g); 
  parse<Grammar,ParserLL1RecDesc>( std::string("1++2;"), g); 

  return 0;
}
