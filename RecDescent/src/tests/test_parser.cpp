#include "ParserLL1RecDesc.hpp"
#include "Grammar.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace RecDescent;
using namespace GrammarAnalyzer;
#include "../../../Common/src/lib/grammars/grammar_expr.cpp"


/*
 * Uses pars grammar of "engineering a compiler" 2nd ed, p128.
 * Parses multiple strings, one at a time.
 */

template <class G, class P>
void parse(const std::string& str, G& g)
{
  Block* programBlock = nullptr;
  
  
  std::unique_ptr<P> parser(new 
                P(std::vector<char> (str.begin(), str.end()), programBlock));

  parser->Parse();
}

int main()
{
    
  //Grammar
  Grammar g;
  CreateGrammar(g);
  std::cout << g;
  
  //pass
  parse<Grammar,ParserLL1RecDesc>( std::string("(1)"), g); std::cout << "\n";
  parse<Grammar,ParserLL1RecDesc>( std::string("2+3+4"), g); std::cout << "\n";
  parse<Grammar,ParserLL1RecDesc>( std::string("2++3+4"), g); std::cout << "\n";
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
