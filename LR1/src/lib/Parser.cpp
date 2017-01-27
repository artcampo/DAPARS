#include "Parser.hpp"
#include "Node.hpp"
#include <iterator>
#include <fstream>
#include <iostream>
#include <exception>

namespace LR1{

  /*
class ExceptionNotEndFile: public exception{
  virtual const char* what() const throw()
  {
    return "Parsed last expression, but more ";
  }
};
*/

  
Parser::Parser(std::string const &file_name, Block* &programBlock) 
  : BaseParser(file_name, programBlock)
{
 
}


void Parser::Parse(){

  if(num_errors_ != 0){
    std::cout << "Program syntactically incorrect\n";
  }
}

  
} //end namespace LR1
 
