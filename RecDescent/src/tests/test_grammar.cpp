#include "Parser.hpp"
#include "Grammar.hpp"
#include <iostream>
#include <memory>
#include <string>

int main(){
  
  
  using namespace GrammarAnalyzer;
  
  //Terminals
  const Symbol plus("PLUS", "+", true);
  const Symbol lpar("LPAR", "(", true);
  const Symbol rpar("RPAR", ")", true);
  const Symbol numr("NUM", "{num}", true);
  
  //Non-terminals
  const Symbol p ("PROG", "PROG", false);
  const Symbol e ("EXPR", "EXPR", false);
  const Symbol ep("EXP'", "EXP'", false);
  const Symbol f ("FACT", "FACT", false);
  
  //Grammar
  Grammar g;
  g.AddRule(Rule(p,  {e}));
  g.AddRule(Rule(e,  {f, ep}));
  g.AddRule(Rule(ep, {plus, f}));
  g.AddRule(Rule(ep, {Symbol::Empty()}));
  g.AddRule(Rule(f,  {lpar, e, rpar}));
  g.AddRule(Rule(f,  {numr}));
  
  //Anaylze
  g.Analyze();
  
  g.DumpFirst();
  
  return 0;
}
