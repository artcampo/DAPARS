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
    return "Parsed last Expr, but more ";
  }
};
*/


ParserLALR::ParserLALR(std::string const &file_name, CompilationUnit& unit
              , GrammarLALR& grammar)
  : ParserLR1(file_name, unit, grammar)
{

}

} //end namespace LALR

