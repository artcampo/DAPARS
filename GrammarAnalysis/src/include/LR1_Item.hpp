#pragma once
#include "Rule.hpp"
#include "Symbol.hpp"

#include <vector>
#include <string>
#include <algorithm> 

#include <iostream>

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
    if(rule_ == s.rule_) return symbol_ < s.symbol_;
    else return  rule_ < s.rule_;
  }
  
  const bool operator== ( const LR1_Item &s ) const{
    return (rule_ == s.rule_) and (symbol_ == s.symbol_);
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
  
  LR1_Item SwapSymbolAfterStackTop() const noexcept{
    std::vector<Symbol> derived = rule_.derived_;
    std::vector<Symbol>::iterator it = std::find( derived.begin()
                       , derived.end()
                       , Symbol::StackTop() );
    std::vector<Symbol>::iterator next = it; 
    ++next;
    std::iter_swap(it, next);
    return LR1_Item(Rule(rule_.head_, derived), symbol_);
  }
  
  bool IsInitialRule(const Rule& initial_rule) const{
    return rule_.head_ == initial_rule.head_;
  }
  
private:

};

} //end namespace GrammarAnalyzer
