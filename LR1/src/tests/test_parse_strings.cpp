#include "ParserLR1.hpp"
#include "GrammarLR1.hpp"
#include "IRDefinition.hpp"
#include "AST/Node.hpp"
#include "Utils.hpp"
#include "ASTVisitors/PrettyPrinter.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace LR1;
using namespace GrammarAnalyzer;
#include "../../../Common/src/lib/grammars/grammar_pars.cpp"

/*
 * Uses pars grammar of "engineering a compiler" 2nd ed, p128.
 * Parses multiple strings, one at a time.
 */

template <class G>
void parse(const std::string& str, G& g)
{
  CompilationUnit unit;


  std::unique_ptr<ParserLR1> parser(new
                ParserLR1(std::vector<char> (str.begin(), str.end()), unit, g));

  parser->Parse();
}

int main()
{

  //Grammar
  GrammarLR1 g;
  CreateGrammar(g);
  std::cout << g;

  //pass
  parse( std::string("()"), g); std::cout << "\n";
  parse( std::string("()()()"), g); std::cout << "\n";
  parse( std::string("(())"), g); std::cout << "\n";

  //no pass
  parse( std::string("((()()))"), g); std::cout << "\n";
  parse( std::string("(()"), g); std::cout << "\n";
  parse( std::string("())"), g); std::cout << "\n";

  return 0;
}
