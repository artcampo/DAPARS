#include "GrammarLR1.hpp"
#include <exception>
#include <vector>
#include <iostream>

namespace GrammarAnalyzer{

GrammarLR1::GrammarLR1(){
  AddSymbol(Symbol::StackTop());
}

size_t GrammarLR1::NumSymbols() const noexcept{
  return symbols_.size() - 1;
}  

LR1_Item GrammarLR1::InitLR1_Item(const Rule& rule) const noexcept{
  Rule r = rule;
  r.derived_.insert( r.derived_.begin(), Symbol::StackTop() );
  return LR1_Item(r, Symbol::Eof());
}

std::set<LR1_Item> GrammarLR1::Closure(const std::set<LR1_Item>& set) const noexcept{
}

void GrammarLR1::BuildTables() noexcept{

}



void GrammarLR1::DumpTables() const noexcept{

}


} //end namespace GrammarAnalyzer
