#include "Grammar.hpp"
#include "Rule.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace GrammarAnalyzer;


int main(){
  
  //Terminals
  const Symbol lpar("LPAR", "(", true);
  const Symbol rpar("RPAR", ")", true);
  
  //Non-terminals
  const Symbol prog ("PROG", "PROG", false);
  const Symbol l    ("LIST", "LIST", false);
  const Symbol p    ("PAIR'", "PAIR'", false);  
  
  Rule(prog,  {l});
  
  Rule r0(l, {l, p});
  Rule r1(l, {p});
  Rule r2(p, {lpar, p, rpar});
  Rule r3(p, {lpar, rpar});  
  
  
  std::cout << "t\n";
  
  return 0;
}
