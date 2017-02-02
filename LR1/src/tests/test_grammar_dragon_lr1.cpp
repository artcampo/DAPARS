#include "ParserLR1.hpp"
#include "GrammarLR1.hpp"
#include <iostream>
#include <memory>
#include <string>

/*
 * This produces the closure, goto and tables of "Compilers, principles and
 * techniques" 2nd ed, p263
 */

int main(){
  
  using namespace GrammarAnalyzer;
  
  //Terminals
  const Symbol sc("c", "c", true);
  const Symbol sd("d", "d", true);
  
  //Non-terminals
  const Symbol sp("S'", "S'", false);
  const Symbol s ("S",  "S",  false);
  const Symbol c ("C",  "C",  false);
  
  
  
  //Grammar
  Rule start(sp,  {s}, true);
  
  GrammarLR1 g;
  g.AddStartingRule(start);
  
  g.AddRule(Rule(s, {c, c}));
  g.AddRule(Rule(c, {sc, c}));
  g.AddRule(Rule(c, {sd}));
  
  //Anaylze
  g.Analyze();
  
  //Print
  std::cout << "-- FIRST" << std::endl;
  g.DumpFirst();
  

  //Test initial closure
  std::set<LR1_Item> set {g.InitLR1_Item( start )};
  std::set<LR1_Item> closure = g.Closure(set);
  
  std::cout << "-- INITIAL CLOSURE" << std::endl;
  for(const auto &i : closure){
    std::cout << i.str() << "\n";
  }
  
  //Test goto
  std::set<LR1_Item> set_goto = g.Goto(closure, c);
  std::cout << "-- GOTO" << std::endl;
  for(const auto &i : set_goto){
    std::cout << i.str() << "\n";
  }  
  
  //Test CC
  g.BuildTables();
  
  std::cout << "-- CC" << std::endl;
  g.DumpCC();
  
  std::cout << "-- TABLES" << std::endl;
  g.DumpTables();
  
  return 0;
}
