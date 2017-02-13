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

  
template<class PolicyDebugLog>    
ParserLALR<PolicyDebugLog>::ParserLALR(std::string const &file_name, Block* &programBlock
              , GrammarLALR& grammar) 
  : LR1::ParserLR1<PolicyDebugLog>(file_name, programBlock, grammar)
{
 
}
  
template class ParserLALR<DebugLogNull>;
template class ParserLALR<DebugLogWriteToCout>;    
  
} //end namespace LALR
 
