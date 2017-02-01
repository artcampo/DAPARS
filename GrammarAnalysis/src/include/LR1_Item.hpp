#pragma once
#include "Rule.hpp"
#include "Symbol.hpp"
#include "Identifiers.hpp"
#include "LR0_Item.hpp"
#include <vector>
#include <string>
#include <algorithm> 
#include <iostream>
#include <set>

namespace GrammarAnalyzer{


class LR1_Item : public LR0_Item{
  
public:  
  LR1_Item(const Rule& rule, const Symbol& symbol, const RuleId& rule_id)
  : LR0_Item(rule, rule_id), symbol_(symbol){}
  

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
  
  LR0_Item ToLR0_Item() const noexcept{
    return LR0_Item(rule_, rule_id_);
  }  
  
private:

};

using SetLR1_Item           = std::set<LR1_Item>;
using SetOfSetsLR1_Item     = std::set<SetLR1_Item>;
using VectorOfSetsLR1_Item  = std::vector<SetLR1_Item>;  


} //end namespace GrammarAnalyzer
