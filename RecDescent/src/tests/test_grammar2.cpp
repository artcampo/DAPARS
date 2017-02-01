#include "Parser.hpp"
#include "Grammar.hpp"
#include <iostream>
#include <memory>
#include <string>

//#include "grammar_expr.cpp"

int main(){
  
  using namespace GrammarAnalyzer;
  
  /*
   * This is the grammar used in "Engineering a compiler", Cooper, Torczon
   * page 101
   */
  
  //Terminals
  const Symbol plus("PLUS", "+", true);
  const Symbol minu("MINU", "-", true);
  const Symbol mult("MULT", "*", true);
  const Symbol divi("DIVI", "/", true);
  const Symbol lpar("LPAR", "(", true);
  const Symbol rpar("RPAR", ")", true);
  const Symbol numr("NUM", "{num}", true);
  const Symbol name("NAM", "{nam}", true);
  
  //Non-terminals
  const Symbol p ("PROG", "PROG", false);
  const Symbol e ("EXPR", "EXPR", false);
  const Symbol ep("EXP'", "EXP'", false);
  const Symbol t ("TERM", "TERM", false);
  const Symbol tp("TER'", "TER'", false);
  const Symbol f ("FACT", "FACT", false);
  
  //Grammar
  Grammar g;
  g.AddStartingRule(Rule(p,  {e}, true));
  
  g.AddRule(Rule(e,  {t, ep}));
  g.AddRule(Rule(ep, {plus, t, ep}));
  g.AddRule(Rule(ep, {minu, t, ep}));
  g.AddRule(Rule(ep, {Symbol::Empty()}));
  
  g.AddRule(Rule(t,  {f, tp}));
  g.AddRule(Rule(tp, {mult, f, tp}));
  g.AddRule(Rule(tp, {divi, f, tp}));
  g.AddRule(Rule(tp, {Symbol::Empty()}));  
  
  g.AddRule(Rule(f,  {lpar, e, rpar}));
  g.AddRule(Rule(f,  {numr}));
  g.AddRule(Rule(f,  {name}));
  
  
  
  //Anaylze
  g.Analyze();
  
  //Print
  g.DumpFirst();
  std::cout << "-----------------------------" << std::endl;
  g.DumpFollow();
  
  return 0;
}
