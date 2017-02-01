#pragma once
#include "Rule.hpp"
#include "Symbol.hpp"
#include "Identifiers.hpp"

#include <vector>
#include <string>
#include <algorithm> 
#include <iostream>

namespace GrammarAnalyzer{

class LR1_Item{
  
public:  
  LR1_Item(const Rule& rule, const Symbol& symbol, const RuleId& rule_id)
  : rule_(rule), symbol_(symbol), rule_id_(rule_id)
  {}
  
  const Rule                rule_;
  const Symbol              symbol_;
  const RuleId              rule_id_;
  
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
    return LR1_Item(Rule(rule_.head_, derived), symbol_, rule_id_);
  }
  
  bool IsInitialRule() const noexcept{
    return rule_.IsInitialRule();
  }
  
  Rule OriginalRule() const noexcept{
    std::vector<Symbol> d = rule_.derived_;
    d.erase(std::remove(d.begin(), d.end(), Symbol::StackTop()), d.end());
    return Rule(rule_.head_, d, rule_.IsInitialRule());
  }
  
private:

};

} //end namespace GrammarAnalyzer
