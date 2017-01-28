#pragma once
#include <vector>
#include <string>
#include <cstddef>
#include "Symbol.hpp"


namespace GrammarAnalyzer{

class Rule{
  
public:  
  Rule(){};
  Rule(const Symbol& head, const std::vector<Symbol>& derived)
  : head_(head), derived_(derived)
  {}
  
  Symbol        head_;
  std::vector<Symbol> derived_;
  
  std::string str() const noexcept{
    std::string s(head_.str());
    s += " -> ";
    for(const auto &symbol : derived_) { s += symbol.str(); s+= " ";} 
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
  
private:

};

} //end namespace GrammarAnalyzer
