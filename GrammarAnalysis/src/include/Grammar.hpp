#pragma once
#include "Rule.hpp"
#include "Symbol.hpp"
#include "Tokenizer.hpp"

#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <set>

/*
 * Symbols depending on if they are terminals or not terminals, have unique 
 * integer IDs. Eof has ID = 0 and Empty has no ID.
 * 
 * The terminals are mapped in their order of appearance, to make it
 * convenient to map between tokens and symbols IDs.
 */

namespace GrammarAnalyzer{

using namespace Common;
using namespace Tokenizer;  
  
class Grammar{
  
public:  
  
  using SymbolId              = size_t;
  
  Grammar();
  
  void AddSymbol(const Symbol& symbol, const kToken& tokenId);
  void AddSymbol(const Symbol& symbol);
  void AddRule(const Rule& rule)  noexcept;
  void AddStartingRule(const Rule& rule)  noexcept;
  
  void Analyze() noexcept;
  
  size_t NumSymbols() const noexcept;
  bool IsBackTrackFree() noexcept;
  
  void DumpFirst() const noexcept;
  void DumpFollow() const noexcept;
  
  SymbolId GetSymbolId(const Symbol& symbol);
  SymbolId GetSymbolId(const kToken& token);
  
protected:
  std::vector<Rule> rules_;
  bool analized_;
  std::set<Symbol> symbols_;
  std::map<Symbol, std::set<Symbol>> first_;
  std::map<Symbol, std::set<Symbol>> follow_;
  Symbol start_symbol_;
  Rule   starting_rule_;
  
  SymbolId free_term_id_;
  SymbolId free_non_term_id_;
  std::map<Symbol,SymbolId>  symbol_id_;
  std::map<kToken, SymbolId> tokenId_of_symbolId_;
  
  void ComputeFirstSets() noexcept;
  void ComputeFollowSets() noexcept;
  void ComputeFirstPlusSets() noexcept;
  
  
  
  
  //Helpers
  std::set<Symbol> First(const std::vector<Symbol>& derived);
  std::set<Symbol> First(const std::vector<Symbol>& derived, const Symbol& s);
};

} //end namespace GrammarAnalyzer
