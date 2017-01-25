#pragma once
#include "Rule.hpp"
#include "Symbol.hpp"

#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <set>


namespace GrammarAnalyzer{

class Grammar{
  
public:  
  Grammar();
  
  void AddSymbol(const Symbol& symbol) noexcept;
  void AddRule(const Rule& rule)  noexcept;
  void AddStartingRule(const Rule& rule)  noexcept;
  
  void Analyze() noexcept;
  
  size_t NumSymbols() const noexcept;
  bool IsBackTrackFree() noexcept;
  
  void DumpFirst() const noexcept;
  void DumpFollow() const noexcept;
  
  
protected:
  std::vector<Rule> rules_;
  bool analized_;
  std::set<Symbol> symbols_;
  std::map<Symbol, std::set<Symbol>> first_;
  std::map<Symbol, std::set<Symbol>> follow_;
  Symbol start_symbol_;
  
  void ComputeFirstSets() noexcept;
  void ComputeFollowSets() noexcept;
  void ComputeFirstPlusSets() noexcept;
  
  
  //Helpers
  std::set<Symbol> First(const std::vector<Symbol>& derived) const;
  std::set<Symbol> First(const std::vector<Symbol>& derived, const Symbol& s) const;
};

} //end namespace GrammarAnalyzer
