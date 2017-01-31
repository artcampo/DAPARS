#include "ParserLR1.hpp"
#include "GrammarLR1.hpp"
#include "LR1_Item.hpp"
#include "Rule.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace GrammarAnalyzer;

void print(const std::set<LR1_Item>& s){
  for(const auto &setIt : s){
    std::cout << "** " << setIt.str() << std::endl;
  }
}

int main(){
  
  std::set<LR1_Item> set;
  
  //Terminals
  const Symbol lpar("LPAR", "(", true);
  const Symbol rpar("RPAR", ")", true);
  
  //Non-terminals
  const Symbol prog ("PROG", "PROG", false);
  const Symbol l    ("LIST", "LIST", false);
  const Symbol p    ("PAIR'", "PAIR'", false);  
  
  Rule(prog,  {l});
  
  Rule r0(l, {l, p});
  Rule r1(l, {p});
  Rule r2(p, {lpar, p, rpar});
  Rule r3(p, {lpar, rpar});  
  
  
  LR1_Item item1 ( Rule(p, {Symbol::StackTop(), l} ),     Symbol::Eof(), 0);
  LR1_Item item2 ( Rule(l, {Symbol::StackTop(), l, p} ),  Symbol::Eof(), 1);
  LR1_Item item3 ( Rule(l, {Symbol::StackTop(), p} ),     Symbol::Eof(), 2);
  LR1_Item item4 ( Rule(l, {Symbol::StackTop(), l, p} ),  Symbol::Eof(), 3);
  
  std::cout << "find before insert: ";
  if(set.find(item1) == set.end())
    std::cout << " not found.\n";
  else std::cout << " found.\n";     
  set.insert(item1);
  print(set);
  std::cout << "\n";  

  std::cout << "find before insert: ";
  if(set.find(item2) == set.end())
    std::cout << " not found.\n";
  else std::cout << " found.\n";     
  set.insert(item2);
  print(set);
  std::cout << "\n";  

  std::cout << "find before insert: ";
  if(set.find(item3) == set.end())
    std::cout << " not found.\n";
  else std::cout << " found.\n";     
  set.insert(item3);
  print(set);
  std::cout << "\n";    
  
  std::cout << "find before insert: ";
  if(set.find(item4) == set.end())
    std::cout << " not found.\n";
  else std::cout << " found.\n";     
  set.insert(item4);
  print(set);
  std::cout << "\n";  
  
  std::cout << "find before insert: ";
  if(set.find(item3) == set.end())
    std::cout << " not found.\n";
  else std::cout << " found.\n";     
  set.insert(item3);
  print(set);
  std::cout << "\n";      
   
  
  /*
-----------------------------
Insert: [LIST -> {·} LIST PAIR' , {eof}]
** [LIST -> {·} LIST PAIR' , {eof}]
** [PROG -> {·} LIST , {eof}]
Insert: [LIST -> {·} PAIR' , {eof}]
** [LIST -> {·} LIST PAIR' , {eof}]
** [LIST -> {·} PAIR' , {eof}]
** [PROG -> {·} LIST , {eof}]
Insert: [LIST -> {·} LIST PAIR' , {eof}]
** [LIST -> {·} LIST PAIR' , {eof}]
** [LIST -> {·} PAIR' , {eof}]
** [PROG -> {·} LIST , {eof}]
Insert: [LIST -> {·} PAIR' , {eof}]
** [LIST -> {·} LIST PAIR' , {eof}]
** [LIST -> {·} PAIR' , {eof}]
** [PROG -> {·} LIST , {eof}]
   
   */
/*  
  Symbol s1("1","1",true);
  Symbol s2("2","2",true);
  Symbol se("e","e",true);
  Symbol se2("e2","e2",true);
  
  
  std::vector<Symbol> d1 = { Symbol("a","a",true), Symbol("b","b",true) };
  std::vector<Symbol> d2 = { Symbol("d","d",true), Symbol("a","a",true), Symbol("b","b",true) };
  
  LR1_Item item1 ( Rule(s1, d1 ), se);
  LR1_Item item2 ( Rule(s2, d1 ), se);
  LR1_Item item3 ( Rule(s2, d1 ), se2);
  
  
  set.insert(item1);
  print(set);
  std::cout << "\n";
  
  set.insert(item2);
  print(set);
  std::cout << "\n";  
  
  set.insert(item3);
  print(set);
  std::cout << "\n"; 
  
  std::cout << "find 2: ";
  if(set.find(item2) == set.end())
    std::cout << " not found.\n";
  else std::cout << " found.\n";
  
  std::cout << "find 3: ";
  if(set.find(item3) == set.end())
    std::cout << " not found.\n";
  else std::cout << " found.\n";  
  */
  return 0;
}
