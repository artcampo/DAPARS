#include "Parser.hpp"
#include "Node.hpp"
#include <iterator>
#include <fstream>
#include <iostream>
#include <exception>
#include <stack>

namespace LR1{

  /*
class ExceptionNotEndFile: public exception{
  virtual const char* what() const throw()
  {
    return "Parsed last expression, but more ";
  }
};
*/

  
Parser::Parser(std::string const &file_name, Block* &programBlock
              , GrammarLR1& grammar) 
  : BaseParser(file_name, programBlock)
  , grammar_(grammar)
{
 
}


void Parser::Parse(){
  
  class ShiftedSymbol{
  public:  
    ShiftedSymbol(const Symbol& symbol, const StateId& state)
      : symbol_(symbol), state_(state){};
    Symbol  symbol_;
    StateId state_;
  };
  
  std::stack<ShiftedSymbol> context;
  context.push( ShiftedSymbol(Symbol::Eof(), 0));
  bool finished = false;
  NextToken();
  
  while(not finished){
    const StateId state = context.top().state_;
    std::cout << "State: " << state << "\n";
  }
}

  
} //end namespace LR1
 
