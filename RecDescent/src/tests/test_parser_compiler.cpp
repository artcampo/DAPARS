#include "Parser.hpp"
#include "IRDefinition.hpp"
#include "Node.hpp"
#include "Utils.hpp"
#include "ASTVisitorCodeGenerator.hpp"
#include "ASTVisitorPrettyPrinter.hpp"
#include <iostream>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>


int main(int argc, char **argv)
{
  if(argc != 2){
    std::cout << "Missing source input file\n";
    exit(1);
  }
  
  Block* programBlock = nullptr;
  
  using namespace RecDescent;
  std::unique_ptr<Parser> parser(new Parser(std::string(argv[1]), programBlock));

  parser->Parse();
  
  if(programBlock != nullptr)

  {
  std::cout << "Print AST\n";
  ASTVisitorPrettyPrinter visitor;
  visitor.Visit(*programBlock);
  }
  
  /*
  std::cout << "--------------------------\n";
  std::cout << "ByteCode:\n";
  ASTVisitorCodeGenerator visitor;
  visitor.Visit(*programBlock);
  visitor.EndOfProgram();
  
  visitor.Print();
  
  if(argc == 2){
    VMUtils::writeByteCode( visitor.byte_code(), std::string(argv[1]) );
    
//       std::cout << "--------------------------\n";
//       std::cout << "Bytecode written to: " << argv[1] << "\n";
  }
  */
    
    return 0;
}
