#pragma once
#include "Rule.hpp"
#include "Symbol.hpp"
#include "Identifiers.hpp"

#include <vector>
#include <string>
#include <algorithm> 
#include <iostream>
#include <set>

namespace GrammarAnalyzer{

  
class LR0_Item{  
public:  
  LR0_Item(const Rule& rule, const RuleId& rule_id)
  : rule_(rule), rule_id_(rule_id){}  
  
  const Rule                rule_;
  const RuleId              rule_id_; 
  
  
  std::string str() const noexcept{
    std::string s("[");
    s += rule_.str();
    s += std::string ("]");
    return s;
  }
  
  const bool operator< ( const LR0_Item &s ) const{
    return  rule_ < s.rule_;
  }  
  
  const bool operator== ( const LR0_Item &s ) const{
    return rule_ == s.rule_;
  }  
  
  bool IsStackTopAtBeginning() const noexcept{
    auto it = std::find( rule_.derived_.begin()
                        , rule_.derived_.end()
                        , Symbol::StackTop() );
    return it == rule_.derived_.cbegin();
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

  
  const bool IsInitialRule() const noexcept{
    return rule_.IsInitialRule();
  }
  
  Rule OriginalRule() const noexcept{
    std::vector<Symbol> d = rule_.derived_;
    d.erase(std::remove(d.begin(), d.end(), Symbol::StackTop()), d.end());
    return Rule(rule_.head_, d, rule_.IsInitialRule());
  }  
  
protected:  
};
  

using SetLR0_Item           = std::set<LR0_Item>;



} //end namespace GrammarAnalyzer
