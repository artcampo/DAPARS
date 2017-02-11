#include "Grammar.hpp"
#include <exception>
#include <vector>
#include <iostream>
#include <cassert>

namespace GrammarAnalyzer{


  
Grammar::Grammar()
  : analized_(false),free_term_id_(0), free_non_term_id_(0), free_rule_id_(0)
  , num_terminals_(0), num_nonterminals_(0)
{
  AddTerminal(Symbol::Empty(), Tokenizer::kToken::eof);
  AddTerminal(Symbol::Eof(), Tokenizer::kToken::eof);
}


RuleId Grammar::GetRuleId(const Rule& rule) const{
  return ruleId_of_rule_.at(rule);
}

SymbolId Grammar::GetSymbolId(const kToken& token) const{
  return symbolId_of_tokenId_.at(token);
}

std::string Grammar::str() const noexcept{
  std::string s("Grammar:\n");
  for(const auto it : rules_) {s += it.str(); s+= std::string("\n");}
  return s;
}

void Grammar::AddTerminal(const Symbol& symbol, const kToken& tokenId){
  if(symbol != Symbol::Empty() and symbol != Symbol::StackTop()){
    ++num_terminals_;
//     std::cout << "added terminal: "<< symbol << " " << num_terminals_ <<"\n";
  }
  AddSymbol(symbol);
  if(symbol != Symbol::Empty()){
    symbolId_of_tokenId_[tokenId] = GetSymbolId(symbol);
  }
}

void Grammar::AddNonTerminal(const Symbol& symbol){
  ++num_nonterminals_;
  AddSymbol(symbol);
}

void Grammar::AddSymbol(const Symbol& symbol){
//   std::cout << "Adding symbol: " << symbol ;
  symbols_.insert(symbol);
  if(symbol != Symbol::Empty() and symbol != Symbol::StackTop()){
    //assign id 
    CreateSymbolId(symbol);
  }
//   std::cout << "\n";
}


void Grammar::AddRule(const Rule& rule) noexcept{
  rules_.push_back(rule);
  ruleId_of_rule_[rule] = free_rule_id_;
  ++free_rule_id_;
}

void Grammar::AddStartingRule(const Rule& rule)  noexcept{
  assert(rule.IsInitialRule() == true);
  AddRule(rule);
  start_symbol_  = rule.head_;
  starting_rule_ = rule;
//   std::cout << "initial rule:" << rule << "\n";
}


void Grammar::Analyze() noexcept{
  if(not analized_){
    Initialize();
    ComputeFirstSets();
    ComputeFollowSets();
    ComputeFirstPlusSets();
    is_back_track_free_ = ComputeBackTrackFree();
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
//   std::cout << "Computing first(" << derived << "): ";
  
  for(const auto &symbol : derived){
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
//   std::cout << first_set << "\n";
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
//     std::cout << "Already on first:" << first_ <<"\n";
    for(const auto &r : rules_){  
      std::set<Symbol> new_set = First(r.derived_);
      
      //check if set stays the same, and if not, update it
      for(const auto &symbol : new_set)
        if(first_[r.head_].find(symbol) == first_[r.head_].end()){
          first_[r.head_].insert(symbol);
//           std::cout << "first(" << r.head_ << ") += " << symbol << "\n";
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


bool Grammar::IsBackTrackFree() noexcept{
  if(not analized_) Analyze();
}

void Grammar::DumpFirst() const noexcept{
  std::cout << "First Table\n";  
  for(const auto &entry : first_){
    if(not entry.first.IsTerminal()){
      std::cout << entry.first.str() << ": ";
      
      for(const auto &s : entry.second){
        std::cout << s.str() << " ";
      }
      std::cout << std::endl;
    }
  }
}

void Grammar::DumpFollow() const noexcept{
  std::cout << "Follow Table\n";
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


void Grammar::DumpFirstPlus() const noexcept{
}


void Grammar::CreateSymbolId(const Symbol& symbol){
  if(symbol.IsTerminal()){
    auto it = symbol_id_.find(symbol);
    if(it == symbol_id_.end()){
      symbol_id_[symbol] = free_term_id_;
      id_to_terminal_[free_term_id_] = symbol;
      free_term_id_++;
//       std::cout << " new terminal";
    } 
  }else{
    auto it = symbol_id_.find(symbol);
    if(it == symbol_id_.end()){
      symbol_id_[symbol] = free_non_term_id_;
      id_to_nonterminal_[free_non_term_id_] = symbol;
      free_non_term_id_++;
//       std::cout << " new non terminal";
    } 
  }  
}

SymbolId Grammar::GetSymbolId(const Symbol& symbol){
  if(symbol == Symbol::Empty() or symbol == Symbol::StackTop()){
    std::cout << "GetSymbolId of empty or stacktop\n";
    exit(1);
  }
    
  if(symbol.IsTerminal()){
    auto it = symbol_id_.find(symbol);
    if(it == symbol_id_.end()){
      std::cout << "terminal not registered" << symbol;
      exit(1);
    }else return it->second;
  }else{
    auto it = symbol_id_.find(symbol);
    if(it == symbol_id_.end()){
      std::cout << "nonterminal not registered" << symbol;
      exit(1);
    }else return it->second;
  }
}

const Symbol 
Grammar::AddTerminalKeyword(const std::string& name, 
                     const std::string& display_name, const kToken& tokenId){
  const Symbol s(name, display_name, true);
  AddTerminal(s, tokenId);
  //TODO: extra information about kwds
  std::cout << "KEYWORD EXTRA INFO NOT IMPLEMENTED\n";//exit(1);
  return s;
}

const Symbol 
Grammar::AddTerminal(const std::string& name, 
                     const std::string& display_name, const kToken& tokenId){
  const Symbol s(name, display_name, true);
  AddTerminal(s, tokenId);
  return s;
}

const Symbol
Grammar::AddNonTerminal(const std::string& name){
  const Symbol s(name, name, false);
  AddNonTerminal(s);
  return s;
}


void Grammar::DumpPropierties() noexcept{
  std::cout << "Is backtrack free: " << IsBackTrackFree()<<"\n";
}


void Grammar::ComputeFirstPlusSets() noexcept{
  for(const auto &r : rules_){
    const Symbol& b = r.derived_[0];
    const Symbol& a = r.head_;    
    first_plus_[r] = first_[b];

    if(r.FirstDerivedCanBeEmpty()){
      for(const auto &symbol : follow_[a])
        first_plus_[r].insert(symbol);
    }
  }
}

void Grammar::Initialize(){
  //std::map<Symbol, Rule*> rule_of_head_;
  for(const auto &r : rules_){
    rules_of_head_[r.head_].push_back(&r);
  }  
}

bool Grammar::ComputeBackTrackFree(){
  bool   is_back_track_free_ = true;
  for(const auto &it_heads : rules_of_head_){
    const std::vector<Rule const*>& rules = it_heads.second;

    for(std::vector<Rule const*>::const_iterator it_ri = rules.cbegin()
      , it_ri_end = rules.cend(); it_ri != it_ri_end; ++it_ri){
      
      const std::set<Symbol>& ri_set = first_plus_.at(**it_ri);
      for(std::vector<Rule const*>::const_iterator it_rj = it_ri + 1
        ; it_rj != it_ri_end; ++it_rj){
          const std::set<Symbol>& rj_set = first_plus_.at(**it_rj);
          
          //check if sets are disjoint
          for(const auto &symbol : ri_set){
            if(rj_set.find(symbol) != rj_set.end()){
              is_back_track_free_ = false;
              std::cout << "Backtrack free violation. Rules:\n";
              std::cout << **it_ri <<"\n";
              std::cout << **it_rj <<"\n";
              std::cout << "Offending symbol: " << symbol << "\n";
              break;
            }
          }
        }
      }
  }
  return is_back_track_free_;  
}

} //end namespace GrammarAnalyzer
