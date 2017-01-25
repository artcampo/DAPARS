#pragma once
#include "Rule.hpp"
#include "Symbol.hpp"

#include <vector>
#include <string>
#include <algorithm> 



namespace GrammarAnalyzer{

class LR1_Item{
  
public:  
  LR1_Item(const Rule& rule, const Symbol& symbol)
  : rule_(rule), symbol_(symbol)
  {}
  
  const Rule                rule_;
  const Symbol              symbol_;
  
  
  std::string str() const noexcept{
    std::string s("[");
    s += rule_.str();
    s += ", ";
    s += symbol_.str();
    s += std::string ("]");
    return s;
  }
  
  const bool operator< ( const LR1_Item &s ) const{ 
    return ( rule_ < s.rule_ and symbol_ < s.symbol_);
  }
  
  
  bool HasSymbolAfterStackTop() const noexcept{
    auto it = std::find( rule_.derived_.begin()
                        , rule_.derived_.end()
                        , Symbol::StackTop() );
    ++it;
    return it != rule_.derived_.cend();
  }
  
  Symbol SymbolAfterStackTop() const noexcept{
    auto it = std::find( rule_.derived_.begin()
                        , rule_.derived_.end()
                        , Symbol::StackTop() );
    ++it;
    return *it;    
  }
  
  std::vector<Symbol> SymbolsAfterC() const noexcept{
    auto it = std::find( rule_.derived_.begin()
                        , rule_.derived_.end()
                        , Symbol::StackTop() );
    ++it; ++it;
    return std::vector<Symbol>(it, rule_.derived_.end());
  }
  
private:

};

} //end namespace GrammarAnalyzer
