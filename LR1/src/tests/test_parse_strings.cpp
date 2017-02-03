#include "ParserLR1.hpp"
#include "GrammarLR1.hpp"
#include "IRDefinition.hpp"
#include "Node.hpp"
#include "Utils.hpp"
#include "ASTVisitorCodeGenerator.hpp"
#include "ASTVisitorPrettyPrinter.hpp"
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
  Block* programBlock = nullptr;
  
  
  std::unique_ptr<ParserLR1> parser(new 
                ParserLR1(str, programBlock, g));

  parser->Parse();
}

int main()
{
    
  //Grammar
  GrammarLR1 g;
  CreateGrammar(g);
  std::cout << g;
  
  parse( std::string("()"), g);
  parse( std::string("()()()"), g);
  parse( std::string("(())"), g);
//   parse( std::string("((()()))"), g);

  return 0;
}
