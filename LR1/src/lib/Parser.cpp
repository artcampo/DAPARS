#include "Parser.hpp"
#include "Node.hpp"
#include "Action.hpp"
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
  
  using kAction = Action::kAction;
  
  class ShiftedSymbol{
  public:  
    ShiftedSymbol(const SymbolId& symbol, const StateId& state)
      : symbol_(symbol), state_(state){};
    SymbolId  symbol_;
    StateId   state_;
  };
  
  std::stack<ShiftedSymbol> context;
  context.push( ShiftedSymbol( grammar_.GetSymbolId(Symbol::Eof()), 0));
  bool finished = false;
  NextToken();
  
  while(not finished){
    const StateId  state  = context.top().state_;
    const SymbolId symbol = grammar_.GetSymbolId(token_);
    const Action action   = grammar_.GetAction(state, symbol);
    std::cout << "State: " << state 
              << " token: " << str(token_)
              << " symbol: " << symbol
              << " action: " << action.str()
              << "\n";
              
    if(action.action_ == kAction::shift){
      context.push( ShiftedSymbol(symbol, state));
      NextToken();
    }else if(action.action_ == kAction::reduce){
    }else if(action.action_ == kAction::accept){
      finished = true;
    }else{
      std::cout << "Nosferatu\n";
    }
  }
}

  
} //end namespace LR1
 
