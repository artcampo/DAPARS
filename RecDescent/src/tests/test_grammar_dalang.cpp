#include "ParserLL1RecDesc.hpp"
#include "Grammar.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace RecDescent;
using namespace GrammarAnalyzer;
#include "../../../Common/src/lib/grammars/grammar_dalang.cpp"

int main(){
  Grammar g;
  CreateGrammarDalang(g);

  //Print
  std::cout << "Dalang grammar v0.3\n";
  std::cout << g;
  std::cout << "-----------------------------" << std::endl;
  g.DumpFirst();

  std::cout << "-----------------------------" << std::endl;
  g.DumpFollow();

  std::cout << "-----------------------------" << std::endl;
  g.DumpFirstPlus();

  std::cout << "-----------------------------" << std::endl;
  g.DumpPropierties();


  return 0;
}
