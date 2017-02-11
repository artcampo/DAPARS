#pragma once
#include <vector>
#include <string>
#include <cstddef>
#include "Symbol.hpp"


namespace GrammarAnalyzer{

class Rule{
  
public:  
  Rule(){};
  Rule(const Symbol& head, const std::vector<Symbol>& derived,
       const bool is_initial_rule = false)
  : head_(head), derived_(derived), is_initial_rule_(is_initial_rule)
  {}
  
  Symbol        head_;
  std::vector<Symbol> derived_;
  bool          is_initial_rule_;
  
  const bool IsInitialRule() const noexcept{ return is_initial_rule_;}
  
  std::string str() const noexcept{
    std::string s(head_.str());
    s += " -> ";
    for(const auto &symbol : derived_) { s += symbol.str(); s+= " ";} 
    if(is_initial_rule_) s+= " init ";
    return s;
  }
    
  const bool operator< ( const Rule &s ) const{ 
    if( head_ == s.head_) return derived_ < s.derived_;
    else                  return head_ < s.head_;
  }
  
  const bool operator== ( const Rule &s ) const{
    return ( head_ == s.head_ and derived_ == s.derived_ );
  }
  
  const size_t NumberDerivedSymbols() const{return derived_.size();}
  
  const bool FirstDerivedCanBeEmpty() const{
    return derived_[0] == Symbol::Empty();
  }
  
private:

};

} //end namespace GrammarAnalyzer
