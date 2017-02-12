#include "ParserLL1RecDesc.hpp"
#include "Grammar.hpp"
#include "ASTVisitorDump.hpp"
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
  std::cout << "---------------------------------------------------\n";
  Block* programBlock = nullptr;
  
  
  std::unique_ptr<P> parser(new 
                P(std::vector<char> (str.begin(), str.end()), programBlock));

  parser->Parse();
  if(programBlock != nullptr){
    std::cout << "Print AST\n";
    ASTVisitorDump visitor;
    visitor.Visit(*programBlock);  
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
  parse<Grammar,ParserLL1RecDesc>( std::string("(1);"), g); 
  parse<Grammar,ParserLL1RecDesc>( std::string("1;2;3;"), g); 
  parse<Grammar,ParserLL1RecDesc>( std::string("2+3+4;"), g); std::cout << "\n";
  parse<Grammar,ParserLL1RecDesc>( std::string("2++3+4;"), g); std::cout << "\n";
  parse<Grammar,ParserLL1RecDesc>( std::string("1+2;3+4;"), g); std::cout << "\n";
  parse<Grammar,ParserLL1RecDesc>( std::string("1+2;3+4;5+6;"), g); std::cout << "\n";
  }
  
  {
  Grammar g;
  CreateGrammarExpr(g);
  std::cout << g;
  parse<Grammar,ParserLL1RecDesc>( std::string("1; if(2){3;}"), g); 
  parse<Grammar,ParserLL1RecDesc>( std::string("1; if(2){3;} 4;"), g); 
  parse<Grammar,ParserLL1RecDesc>( std::string("1; if(2){3+4+5;}"), g); 
  parse<Grammar,ParserLL1RecDesc>( std::string("1; if(2){3+4+5;} 6;"), g); 
  parse<Grammar,ParserLL1RecDesc>( std::string("1; if(2){if(3){4;if(5){6;}}}"), g); 
  parse<Grammar,ParserLL1RecDesc>( std::string("1; if(2){if(3){4+5;if(6){7+8;}}}"), g); 
  
  parse<Grammar,ParserLL1RecDesc>( std::string("1; if(2){3;}else{4;}"), g); 
  parse<Grammar,ParserLL1RecDesc>( std::string("1; if(2){3;}if(4){5;}else{6;}"), g); 
  }
  
  /*
  parse( std::string("()()()"), g); std::cout << "\n";
  parse( std::string("(())"), g); std::cout << "\n";
  
  //no pass
  parse( std::string("((()()))"), g); std::cout << "\n";
  parse( std::string("(()"), g); std::cout << "\n";
  parse( std::string("())"), g); std::cout << "\n";
*/
  return 0;
}
