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
  g.DumpFirst();
  std::cout << "-----------------------------" << std::endl;
  g.DumpFollow();
  
  return 0;
}
