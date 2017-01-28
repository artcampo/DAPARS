#include "GrammarLR1.hpp"
#include <exception>
#include <vector>
#include <iostream>
#include <stack>

namespace GrammarAnalyzer{

GrammarLR1::GrammarLR1(): free_symbol_id_(0){
  AddSymbol(Symbol::StackTop());
}

size_t GrammarLR1::NumSymbols() const noexcept{
  return symbols_.size() - 1;
}  

LR1_Item GrammarLR1::InitLR1_Item(const Rule& rule) const noexcept{
  return InitLR1_Item(rule, Symbol::Eof());
}

LR1_Item GrammarLR1::InitLR1_Item(const Rule& rule, const Symbol& symbol) const noexcept{
  Rule r = rule;
  r.derived_.insert( r.derived_.begin(), Symbol::StackTop() );
  return LR1_Item(r, symbol, GetRuleId(rule));
}

std::set<LR1_Item> GrammarLR1::Closure(const std::set<LR1_Item>& set){
  std::set<LR1_Item> s = set;
  bool has_changed = true;
  while(has_changed){
//     std::cout << "another spin\n";
    has_changed = false;
    for(const LR1_Item &i : s){
//       std::cout << "Inspect " << i.str() << "\n";
      //determine C
      if(i.HasSymbolAfterStackTop()){
        Symbol c = i.SymbolAfterStackTop();
        std::vector<Symbol> delta = i.SymbolsAfterC();
        std::set<Symbol> first_Ca = First(delta, i.symbol_);
        
        //For each rule C->Y
        for(const auto &r : rules_){
          if(r.head_ == c){
//             std::cout << "For head " << c.str() << " b has " << first_Ca.size() << "\n";
            for(const auto &b : first_Ca){
              LR1_Item item = InitLR1_Item(r, b);
              if(s.find(item) == s.end()){
//                 std::cout << "Insert: " << item.str() << std::endl;
                has_changed = true;
                s.insert(item);
//              for(const auto &setIt : s)std::cout << "** " << setIt.str() << std::endl;
                
                break;
              }//end if(s.find(item)
            }//end for(const auto &b : first_Ca)
            
            }
          }//for(const auto &r : rules_)
      }
    }
  }
  return s;
}

std::set<LR1_Item> GrammarLR1::Goto(const std::set<LR1_Item>& set, const Symbol& symbol){
  std::set<LR1_Item> moved;
  
  for(const LR1_Item &i : set)
    if(i.HasSymbolAfterStackTop() and i.SymbolAfterStackTop() == symbol){
      LR1_Item advanced = i.SwapSymbolAfterStackTop();
      moved.insert(advanced);
    }
  return Closure(moved);
}

void GrammarLR1::AssignId(const std::set<LR1_Item>& set){
  set_id_[set] = free_symbol_id_;
//   set_by_id_[free_symbol_id_] = set;
  ++free_symbol_id_;  
}

void GrammarLR1::NewCC(const std::set<LR1_Item>& set){
  marked_[set] = false;
  AssignId(set);
  cc_.insert(set);
}

void GrammarLR1::InitTables(){
  goto_table_.resize(free_symbol_id_);
  for(auto& it : goto_table_) it.resize(free_non_term_id_);
  
  transition_table_.resize(free_symbol_id_);
  for(auto& it : transition_table_) it.resize(free_term_id_);
  
  //indexed with: StateId, SymbolId (term)
  action_table_.resize(free_symbol_id_);
  for(auto& it : action_table_) it.resize(free_term_id_);
  
    
}

void GrammarLR1::CanonicalCollection(){
  SetLR1_Item initial_set {InitLR1_Item( starting_rule_ )};
  SetLR1_Item cc0 = Closure(initial_set);
  NewCC(cc0);
  bool has_changed = true;

  class TableItem{
  public:   
    TableItem(const StateId& cci, const SymbolId& sym_id, const StateId& ccj):
      cci_(cci), sym_id_(sym_id), ccj_(ccj){};
    TableItem(){};
    
    StateId   cci_;
    SymbolId  sym_id_;
    StateId   ccj_;
  };    
  
  std::stack<TableItem> goto_items;
  std::stack<TableItem> transition_items;
  
  while(has_changed){
    has_changed = false;
    
    for(const auto &set : cc_){
      const StateId cci = set_id_[set];
      if(marked_[set] == false){
        marked_[set] = true;
        
        for(const auto &item : set){
          if(item.HasSymbolAfterStackTop()){
            const Symbol symbol = item.SymbolAfterStackTop();
            SetLR1_Item temp  = Goto(set, symbol);
            
//             if(temp.size() > 0)
            if(cc_.find(temp) == cc_.end()){
//               std::cout<< "for Symbol: " << symbol.str() << "\n";
              has_changed = true;
              NewCC(temp);

              const StateId ccj       = set_id_[temp];   
              const SymbolId sym_id = GetSymbolId(symbol);
              if(not symbol.IsTerminal())
                goto_items.push(TableItem(cci, sym_id, ccj));
              else{
                transition_items.push(TableItem(cci, sym_id, ccj));
//                 std::cout << "-- Transition " << cci <<","<<symbol.str()<<" -> " << ccj << "\n";
              }
              
//               std::cout << "New set: \n";
//               for(const auto &i : temp)
//                 std::cout << i.str() << "\n";
              
          }
        } 
        }//end for(const auto &item : set){
        
      }// end if(marked[set] == false){
    }// end for(const auto &set : cc_)
  }
  
  InitTables();
  while(not goto_items.empty()){
    TableItem item = goto_items.top();
    goto_items.pop();
    goto_table_[item.cci_][item.sym_id_] = item.ccj_;
  }
  while(not transition_items.empty()){
    TableItem item = transition_items.top();
    transition_items.pop();
    transition_table_[item.cci_][item.sym_id_] = item.ccj_;
  }  
}

void GrammarLR1::BuildTables() noexcept{
  InitSymbolsIds();
  CanonicalCollection();
  BuildActionTable();
}


void GrammarLR1::BuildActionTable() noexcept{
  std::cout << "build\n";
  for(const auto &set : cc_){
    const StateId cci = set_id_[set];
    for(const auto &item : set){
      //Case 1: shift
      if(item.HasSymbolAfterStackTop()){
        const Symbol symbol = item.SymbolAfterStackTop();
        if(symbol.IsTerminal()){
          //std::cout << "Asking for " << cci << " " << symbol.str() << "\n";
          
          SetLR1_Item temp      = Goto(set, symbol);
          const StateId ccj       = set_id_[temp];
          const SymbolId sym_id = GetSymbolId(symbol);
          action_table_[cci][sym_id] = Action( Action::kAction::shift, ccj);
        } 
      }
      else{
        if(item.IsInitialRule(starting_rule_) and item.symbol_ == Symbol::Eof()){
          const SymbolId sym_id = GetSymbolId(Symbol::Eof());
          action_table_[cci][sym_id] = Action( Action::kAction::accept, 0);
        }
        else{
          const RuleId r_id = item.rule_id_;
          const Rule&  r    = GetRule(r_id);
          if(std::find(rules_.begin(), rules_.end(), r) != rules_.end()){
            const Symbol& a       = item.symbol_;

            const SymbolId sym_id = GetSymbolId(a);
            action_table_[cci][sym_id] = Action( Action::kAction::reduce, 0, r_id);
          }
        }
      }
         
    }//end for(const auto &item : set){
  }//end for(const auto &set : cc_){
  
}


void GrammarLR1::InitSymbolsIds() noexcept{
  GetSymbolId(Symbol::Eof());
  for(const auto &r : rules_)
    for (const auto& it : r.derived_)
      GetSymbolId(it);
}

void GrammarLR1::DumpCC() const noexcept{
  std::cout << "\nCC sets: " << cc_.size() << "\n";
  for(const auto &set : cc_){
    const StateId cci = set_id_.at(set);
    std::cout << "cc"<< cci<<"\n";
    for(const auto &item : set){ 
      std::cout << item.str() << "\n";
    }
  }
}


void GrammarLR1::DumpTables() const noexcept{
  std::cout << "\nAction table " << action_table_.size() <<"\n";
  for(const auto &it : action_table_){
    for(const auto &it2 : it)
      std::cout << "| " <<  it2.str() << " " ;
    std::cout << "|\n";
  }
  
  std::cout << "\nGoto table " << goto_table_.size() <<"\n";
  for(const auto &it : goto_table_){
    for(const auto &it2 : it)
      std::cout << "| " << it2<< " " ;
    std::cout << "|\n";
  }  
}


} //end namespace GrammarAnalyzer
