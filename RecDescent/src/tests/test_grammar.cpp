#include "Parser.hpp"
#include "Grammar.hpp"
#include <iostream>
#include <memory>
#include <string>

int main(){
  
  
  using namespace RecDescent;
  std::unique_ptr<Parser> parser(new Parser(std::string(argv[1])));

  using namespace GrammarAnalyzer;
  Grammar g;
  
  return 0;
}
