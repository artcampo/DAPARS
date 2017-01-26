#include "GrammarLR1.hpp"
#include <exception>
#include <vector>
#include <iostream>

namespace GrammarAnalyzer{

GrammarLR1::GrammarLR1(): free_id_(0){
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
  return LR1_Item(r, symbol);
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
  set_id_[set] = free_id_;
//   set_by_id_[free_id_] = set;
  ++free_id_;  
}

void GrammarLR1::NewCC(const std::set<LR1_Item>& set){
  marked_[set] = false;
  AssignId(set);
  cc_.insert(set);
}

void GrammarLR1::CanonicalCollection(){
  SetLR1_Item initial_set {InitLR1_Item( starting_rule_ )};
  SetLR1_Item cc0 = Closure(initial_set);
  NewCC(cc0);
  bool has_changed = true;
  
  while(has_changed){
    has_changed = false;
    
    for(const auto &set : cc_){
      const SetId cci = set_id_[set];
      if(marked_[set] == false){
        marked_[set] = true;
        
        for(const auto &item : set){
          if(item.HasSymbolAfterStackTop()){
            const Symbol symbol = item.SymbolAfterStackTop();
            SetLR1_Item temp  = Goto(set, symbol);
            if(cc_.find(temp) == cc_.end()){
//               std::cout<< "for Symbol: " << symbol.str() << "\n";
              has_changed = true;
              NewCC(temp);

              const SetId ccj = set_id_[temp];                 
              if(not symbol.IsTerminal()) 
                goto_table_[cci][symbol] = ccj;
              else{
                std::cout << "-- Transition " << cci <<","<<symbol.str()<<" -> " << ccj << "\n";
                transition_table_[cci][symbol] = ccj;
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
}

void GrammarLR1::BuildTables() noexcept{
  CanonicalCollection();
  BuildActionTable();
}


void GrammarLR1::BuildActionTable() noexcept{
  std::cout << "build\n";
  for(const auto &set : cc_){
    const SetId cci = set_id_[set];
    for(const auto &item : set){
      //Case 1: shift
      if(item.HasSymbolAfterStackTop()){
        const Symbol symbol = item.SymbolAfterStackTop();
        if(symbol.IsTerminal()){
          //std::cout << "Asking for " << cci << " " << symbol.str() << "\n";
          
          //defensive
          auto it = transition_table_.find(cci);
          if(it->second.find(symbol) == it->second.end() ){
            SetLR1_Item temp  = Goto(set, symbol);
            if(set_id_.find(temp) == set_id_.end()){
              std::cout << "new set\n";
              exit(1);
              }
          }
          
          SetLR1_Item temp  = Goto(set, symbol);
          const SetId ccj = set_id_[temp];
          action_table_[cci][symbol] = Action( Action::kAction::shift, ccj);
          
        } 

      }
      else{
        
      }
         
    }//end for(const auto &item : set){
  }//end for(const auto &set : cc_){
  
}



/*
 *   std::map<SetId, std::map<Symbol,SetId>> action_table_;
  std::map<SetId, std::map<Symbol,SetId>> goto_table_;
  
 */
void GrammarLR1::DumpTables() const noexcept{
  std::cout << "Action table " << action_table_.size() <<"\n";
  std::cout << "Unique sets: " << free_id_ <<"\n";
  for(const auto &it : action_table_){
    std::cout << "For set " << it.first;
    for(const auto &it2 : it.second){
      std::cout << " - " << it2.first.str() << ": " << it2.second.str();
    }
    std::cout << "\n";
  }
  
  std::cout << "Goto table\n";
  for(const auto &it : goto_table_){
    std::cout << "For set " << it.first;
    for(const auto &it2 : it.second){
      std::cout << " - " << it2.first.str() << ": " << it2.second;
    }
    std::cout << "\n";
  }  
}


} //end namespace GrammarAnalyzer
