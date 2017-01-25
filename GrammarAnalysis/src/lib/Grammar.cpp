#include "Grammar.hpp"
#include <exception>
#include <vector>
#include <iostream>

namespace GrammarAnalyzer{



  
Grammar::Grammar()
  : analized_(false)
{
  AddSymbol(Symbol::Empty());
  AddSymbol(Symbol::Eof());
}

void Grammar::AddSymbol(const Symbol& symbol) noexcept{
  symbols_.insert(symbol);
}

size_t Grammar::NumSymbols() const noexcept{
  return symbols_.size();
}

void Grammar::AddRule(const Rule& rule) noexcept{
  rules_.push_back(rule);
  AddSymbol(rule.head_);
  for(const auto &s : rule.derived_)
    AddSymbol(s);
}

void Grammar::AddStartingRule(const Rule& rule)  noexcept{
  AddRule(rule);
  start_symbol_ = rule.head_;
}


void Grammar::Analyze() noexcept{
  if(not analized_){
    ComputeFirstSets();
    ComputeFollowSets();
    ComputeFirstPlusSets();
    analized_ = true;
  }
}

bool HasEmptySymbol(const std::set<Symbol>& set){
  return set.find(Symbol::Empty())!= set.end();
}

//returns first(b1,b2..bk)
std::set<Symbol> Grammar::First(const std::vector<Symbol>& derived){
  std::set<Symbol> first_set;
  bool all_have_empty = true;
  
  for(const auto &symbol : derived){
    
    std::cout << "asking: " << symbol.str() << std::endl;
    bool symbol_has_empty_symbol = HasEmptySymbol(first_[symbol]);
    all_have_empty = all_have_empty and symbol_has_empty_symbol;
    std::set<Symbol> set = first_[symbol];
    set.erase(Symbol::Empty());
    first_set.insert(set.cbegin(), set.cend());
    if(not symbol_has_empty_symbol){
      break;
    }
  }
  
  if(all_have_empty)
    first_set.insert(Symbol::Empty());  
  return first_set;
}

//returns first(b1,b2..bk,s)
std::set<Symbol> Grammar::First(const std::vector<Symbol>& derived, const Symbol& s){
  if(derived.size() == 0){
    return std::set<Symbol>({s});
  }else{
    std::set<Symbol> first = First(derived);
    if(HasEmptySymbol(first)) first.insert(s);
    return first;
  }
}

void Grammar::ComputeFirstSets() noexcept{
  for(const auto &s : symbols_)
    if(s.IsTerminal())
      first_[s] = {s};
  
  bool hasChanged = true;
  while(hasChanged){
    hasChanged = false;
    for(const auto &r : rules_){  
      std::set<Symbol> new_set = First(r.derived_);
      
      //check if set stays the same, and if not, update it
      for(const auto &symbol : new_set)
        if(first_[r.head_].find(symbol) == first_[r.head_].end()){
          first_[r.head_].insert(symbol);
          hasChanged = true;
        }
      
    }
  }
}

void Grammar::ComputeFollowSets() noexcept{
//   std::set<Symbol> start_set;
//   start_set.insert(Symbol::Eof());
//   follow_[start_symbol_] = start_set;
  follow_[start_symbol_].insert(Symbol::Eof());
  
  bool hasChanged = true;
  while(hasChanged){
    hasChanged = false;
    for(const auto &r : rules_){  
      bool all_have_empty = true;
      const Symbol& a = r.head_;
      std::set<Symbol> trailer = follow_[a];
      
      for (auto it = r.derived_.rbegin(); it != r.derived_.rend(); ++it){
        const Symbol& b = *it;
        if(not b.IsTerminal()){
          std::set<Symbol>& b_set = follow_[b];
          //check if set stays the same, and if not, update it
          for(const auto &symbol : trailer){
            if(b_set.find(symbol) == b_set.end()){
              b_set.insert(symbol);
              hasChanged = true;
            }
          }
          std::set<Symbol>& b_first_set = first_[b];
          if(b_first_set.find(Symbol::Empty()) != b_first_set.end()){
            //trailer <- trailer U (first(b) - empty)
            for(const auto &symbol : b_first_set)
              if(symbol != Symbol::Empty())
                trailer.insert(symbol);
                
          }else{
            trailer = first_[b];
          }
        }else{
          trailer = first_[b];
        }
      }// for(const auto &symbol : r.derived_)
    } // end for(const auto &r : rules_){  
  }// end while   
}

void Grammar::ComputeFirstPlusSets() noexcept{
}

bool Grammar::IsBackTrackFree() noexcept{
  if(not analized_) Analyze();
}

void Grammar::DumpFirst() const noexcept{
  for(const auto &entry : first_){
    if(not entry.first.IsTerminal()){
      std::cout << entry.first.str() << " ";
      
      for(const auto &s : entry.second){
        std::cout << s.str() << " ";
      }
      std::cout << std::endl;
    }
  }
}

void Grammar::DumpFollow() const noexcept{
  for(const auto &entry : follow_){
    if(not entry.first.IsTerminal()){
      std::cout << entry.first.str() << " ";
      
      for(const auto &s : entry.second){
        std::cout << s.str() << " ";
      }
      std::cout << std::endl;
    }
  }
}


} //end namespace GrammarAnalyzer
