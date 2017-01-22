#include "Parser.hpp"
#include <iostream>
#include <memory>
#include <string>

int main(int argc, char **argv){
  
  if(argc != 2){
    std::cout << "Missing source input file\n";
    exit(1);
  }
  
  using namespace RecDescent;
  std::unique_ptr<Parser> parser(new Parser(std::string(argv[1])));

  parser->Parse();
  
  return 0;
}
