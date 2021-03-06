#include "ParserLR1.hpp"
#include "AST/ASTNodes.hpp"
#include "Action.hpp"
#include <iterator>
#include <fstream>
#include <iostream>
#include <exception>
#include <stack>

namespace LR1{

using namespace GrammarAnalyzer;

  class ShiftedSymbol{
  public:
    ShiftedSymbol(const SymbolId& symbol, const StateId& state)
      : symbol_(symbol), state_(state){};
    SymbolId  symbol_;
    StateId   state_;

    std::string str() const{
      return "<" + std::to_string(state_) + "," + std::to_string(symbol_) + ">";
    }
  };

  /*
class ExceptionNotEndFile: public exception{
  virtual const char* what() const throw()
  {
    return "Parsed last Expr, but more ";
  }
};
*/

ParserLR1::ParserLR1(std::string const &file_name, CompilationUnit& unit
              , GrammarLR1& grammar)
  : BaseParser(file_name, unit)
  , grammar_(grammar){}

ParserLR1::ParserLR1(const std::vector<char>& parse_data, CompilationUnit& unit
              , GrammarLR1& grammar)
  : BaseParser(parse_data, unit)
  , grammar_(grammar){}

void printStackRec(std::stack<ShiftedSymbol>& c){
  if(c.empty()) return;
  ShiftedSymbol s = c.top();
  c.pop();
  printStackRec(c);
  std::cout << s.str();
}

void printStack(std::stack<ShiftedSymbol>& context){
  std::stack<ShiftedSymbol> c = context;
  printStackRec(c);
}



void ParserLR1::Parse(){
  using kAction = Action::kAction;

  std::stack<ShiftedSymbol> context;
  context.push( ShiftedSymbol( grammar_.GetSymbolId(Symbol::Eof()), 0));
  bool finished = false;
  NextToken();

  while(not finished){
    //printStack(context); std::cout << "\n";
    const StateId  state  = context.top().state_;
    const SymbolId symbol = grammar_.GetSymbolId(token_);
    const Action action   = grammar_.GetAction(state, symbol);

    /*
    std::cout << " State: " << state
              << " token: " << str(token_)
              << " symbol: " << symbol
              << " action: " << action.str()
              << "\n";
              */

    if(action.action_ == kAction::shift){
      context.push( ShiftedSymbol(symbol, action.next_state_));
      NextToken();

    }else if(action.action_ == kAction::reduce){
      const Rule& r = grammar_.GetRule(action.rule_id_);
      std::cout << "Parsed: " << r.str() << "\n";
      for(size_t i = 0; i < r.NumberDerivedSymbols(); ++i)
        context.pop();
      const SymbolId& a_id    = grammar_.GetSymbolId(r.head_);
      const StateId&  state   = context.top().state_;
      const StateId&  state_a = grammar_.GetGoto(state, a_id);
      context.push( ShiftedSymbol( a_id, state_a));
    }else if(action.action_ == kAction::accept){
      finished = true;
    }else{
      Error("eof not found");
      finished = true;
    }
  }
}


} //end namespace LR1

