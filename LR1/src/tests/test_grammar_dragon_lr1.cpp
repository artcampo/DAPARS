#include "ParserLR1.hpp"
#include "GrammarLR1.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace GrammarAnalyzer;
#include "../../../Common/src/lib/grammars/grammar_dragon_4_55.cpp"

/*
 * This produces the closure, goto and tables of "Compilers, principles and
 * techniques" 2nd ed, p263
 */

int main(){
  
  //Grammar
  GrammarLR1 g;
  CreateGrammar(g);
  
  
  std::cout << "-- CC" << std::endl;
  g.DumpCC();
  
  std::cout << "-- TABLES" << std::endl;
  g.DumpTables();  
  
  return 0;
}
