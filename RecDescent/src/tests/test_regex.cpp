#include "Parser.hpp"
#include <iostream>

int main(int argc, char **argv){
  
  if(argc != 2){
    std::cout << "Missing source input file\n";
    exit(1);
  }
  
  using namespace RecDescent;
  std::unique_ptr<Parser> vm(new Parser(std::string(argv[1])) );

  return 0;
}
