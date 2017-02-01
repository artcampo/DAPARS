#include "ParserLALR.hpp"
#include "GrammarLALR.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace GrammarAnalyzer;
#include "grammar_dragon_4_55.cpp"

/*
 * This produces the closure set of "engineering a compiler" 2nd ed, p128 
 */

int main(){
  
  //Grammar
  GrammarLALR g;
  CreateGrammar<GrammarLALR>(g);
  
//   std::cout << "-- CC" << std::endl;
//   g.DumpCC();
  
//   std::cout << "-- TABLES" << std::endl;
//   g.DumpTables();  
  
  std::cout << "-- TABLES" << std::endl;
  g.DumpTables();
    
  return 0;
}
