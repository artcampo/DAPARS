#include "Output.hpp"
#include "Symbol.hpp"
#include "Tokenizer.hpp"
#include "Grammar.hpp"
#include "GrammarLR1.hpp"
#include "LR1_Item.hpp"
#include <string>
#include <iostream>



namespace GrammarAnalyzer{

std::ostream&
operator<<(std::ostream &os, const Rule& r){ 
  return os << r.str();
}

std::ostream&
operator<<(std::ostream &os, const Symbol& s){ 
  return os << s.str();
}
  
std::ostream&
operator<<(std::ostream &os, const LR1_Item& c){ 
  return os << c.str();
}

std::ostream&
operator<<(std::ostream &os, const LR0_Item& c){ 
  return os << c.str();
}

std::ostream&
operator<<(std::ostream &os, const SetLR1_Item& c){ 
  for(const auto& it : c) os << it << "\n";
  return os;
}

std::ostream&
operator<<(std::ostream &os, const SetLR0_Item& c){ 
  for(const auto& it : c) os << it << "\n";
  return os;
}

std::ostream& operator<<(std::ostream& os, const LR_Tables& t){
  os << "\nAction table " << t.action_table_.size() <<"\n";
  for(const auto &it : t.action_table_){
    for(const auto &it2 : it) os << "| " <<  it2.str() << " " ;
    os << "|\n";
  }
  
  os << "\nGoto table " << t.goto_table_.size() <<"\n";
  for(const auto &it : t.goto_table_){
    for(const auto &it2 : it) os << "| " << it2<< " " ;
    os << "|\n";
  }   
  return os;
}

std::ostream&
operator<<(std::ostream &os, const std::vector<Symbol>& c){ 
  for(const auto& it : c) os << it << " ";
  return os;
}

std::ostream&
operator<<(std::ostream &os, const std::set<Symbol>& c){
  for(const auto& it : c) os << it << " ";
  return os;
}

std::ostream&
operator<<(std::ostream &os, const std::map<Symbol, std::set<Symbol>>& c){
  for(const auto &it : c){
    std::cout << it.first << ": ";
    for(const auto &it2 : it.second) os << it2 << ", ";
  }
  return os;
}

std::ostream&
operator<<(std::ostream &os, const Grammar& g){
  os << g.str() << "\n";
  return os;
}

} //end namespace GrammarAnalyzer
