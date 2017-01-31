#include "ParserLALR.hpp"
#include "Node.hpp"
#include "Action.hpp"
#include <iterator>
#include <fstream>
#include <iostream>
#include <exception>
#include <stack>

namespace LALR{

  /*
class ExceptionNotEndFile: public exception{
  virtual const char* what() const throw()
  {
    return "Parsed last expression, but more ";
  }
};
*/

  
ParserLALR::ParserLALR(std::string const &file_name, Block* &programBlock
              , GrammarLALR& grammar) 
  : ParserLR1(file_name, programBlock, grammar)
{
 
}
  
} //end namespace LALR
 
