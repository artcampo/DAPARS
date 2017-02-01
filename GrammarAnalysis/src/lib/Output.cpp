#include "Output.hpp"
#include "Symbol.hpp"
#include "Tokenizer.hpp"
#include "GrammarLR1.hpp"
#include "LR1_Item.hpp"
#include <string>
#include <iostream>


namespace GrammarAnalyzer{

std::ostream&
operator<<(std::ostream &os, const LR1_Item& c) { 
  return os << c.str();
}

std::ostream&
operator<<(std::ostream &os, const SetLR1_Item& c) { 
  for(const auto& it : c)
    os << it << "\n";
  return os;
}

std::ostream&
operator<<(std::ostream &os, const LR0_Item& c) { 
  return os << c.str();
}

std::ostream&
operator<<(std::ostream &os, const SetLR0_Item& c) { 
  for(const auto& it : c)
    os << it << "\n";
  return os;
}

std::ostream& operator<<(std::ostream& os, const LR_Tables& t){
  os << "\nAction table " << t.action_table_.size() <<"\n";
  for(const auto &it : t.action_table_){
    for(const auto &it2 : it)
      os << "| " <<  it2.str() << " " ;
    os << "|\n";
  }
  
  os << "\nGoto table " << t.goto_table_.size() <<"\n";
  for(const auto &it : t.goto_table_){
    for(const auto &it2 : it)
      os << "| " << it2<< " " ;
    os << "|\n";
  }   
  return os;
}

} //end namespace GrammarAnalyzer
