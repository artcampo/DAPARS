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

void GrammarLR1::BuildTables() noexcept{

}



void GrammarLR1::DumpTables() const noexcept{

}


} //end namespace GrammarAnalyzer
