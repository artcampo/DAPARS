#include "ParserLALR.hpp"
#include "GrammarLR1.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace GrammarAnalyzer;
#include "grammar_pars.cpp"

/*
 * This produces the closure set of "engineering a compiler" 2nd ed, p128 
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
