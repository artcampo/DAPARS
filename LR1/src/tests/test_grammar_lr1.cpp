#include "Parser.hpp"
#include "GrammarLR1.hpp"
#include <iostream>
#include <memory>
#include <string>

/*
 * This produces the closure set of "engineering a compiler" 2nd ed, p128 
 */

int main(){
  
  using namespace GrammarAnalyzer;
  
  //Terminals
  const Symbol lpar("LPAR", "(", true);
  const Symbol rpar("RPAR", ")", true);
  
  //Non-terminals
  const Symbol prog ("PROG", "PROG", false);
  const Symbol l    ("LIST", "LIST", false);
  const Symbol p    ("PAIR", "PAIR", false);
  
  std::cout << "-----------------------------" << std::endl;
  //Grammar
  GrammarLR1 g;
  g.AddStartingRule(Rule(prog,  {l}));
  
  g.AddRule(Rule(l, {l, p}));
  g.AddRule(Rule(l, {p}));
  g.AddRule(Rule(p, {lpar, p, rpar}));
  g.AddRule(Rule(p, {lpar, rpar}));
  
  //Anaylze
  g.Analyze();
  g.BuildTables();
  
  //Print
  std::cout << "-----------------------------" << std::endl;
  g.DumpFirst();
  std::cout << "-----------------------------" << std::endl;

  //Test initial closure
  std::set<LR1_Item> set {g.InitLR1_Item( Rule(prog, {l}) )};
  std::set<LR1_Item> closure = g.Closure(set);
  
  for(const auto &i : closure){
    std::cout << i.str() << "\n";
  }
  
  return 0;
}
