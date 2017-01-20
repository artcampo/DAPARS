#include "Parser.hpp"
#include "Grammar.hpp"
#include <iostream>
#include <memory>
#include <string>

int main(){
  
  
  using namespace GrammarAnalyzer;
  
  const Symbol P("PROG", "PROG", is_terminal = false);
  
  Grammar g;
  /*
  g.AddRule(Rule( Symbol,
                  {("E", )}
  );
  */
  return 0;
}
