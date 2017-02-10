#pragma once
#include "Rule.hpp"
#include "Symbol.hpp"
#include "Tokenizer.hpp"
#include "Identifiers.hpp"
#include "Output.hpp"
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
  
  
  Grammar();
  
  const Symbol AddTerminal(const std::string& name, const std::string& display_name, const kToken& tokenId);
  const Symbol AddTerminalKeyword(const std::string& name, const std::string& display_name, const kToken& tokenId);
  const Symbol AddNonTerminal(const std::string& name);
  
  void AddRule(const Rule& rule)  noexcept;
  void AddStartingRule(const Rule& rule)  noexcept;
  
  //TODO:hide those two
  void AddTerminal(const Symbol& symbol, const kToken& tokenId);
  void AddNonTerminal(const Symbol& symbol);  
  
  void Analyze() noexcept;
  
  size_t NumRules() const noexcept {return rules_.size();};
  size_t NumSymbols() const noexcept {return symbols_.size();};
  size_t NumTerminals() const noexcept {return num_terminals_;};
  size_t NumNonTerminals() const noexcept {return num_nonterminals_;};
  
  bool IsBackTrackFree() noexcept; //todo
  
  void DumpFirst() const noexcept;
  void DumpFollow() const noexcept;
  
  SymbolId GetSymbolId(const Symbol& symbol);
  SymbolId GetSymbolId(const kToken& token) const;
  RuleId   GetRuleId(const Rule& rule) const;
  
  const Rule& GetRule(const RuleId& rule_id) const {return rules_[rule_id];};
  Rule&       GetRule(const RuleId& rule_id) {return rules_[rule_id];};
  
  std::string str() const noexcept;
  
protected:
  size_t num_terminals_;
  size_t num_nonterminals_;  
  
  std::vector<Rule> rules_;
  bool analized_;
  std::set<Symbol> symbols_;
  std::map<Symbol, std::set<Symbol>> first_;
  std::map<Symbol, std::set<Symbol>> follow_;
  Symbol start_symbol_;
  Rule   starting_rule_;
  
  SymbolId free_term_id_;
  SymbolId free_non_term_id_;
  RuleId   free_rule_id_;
  std::map<Symbol,SymbolId>  symbol_id_;
  std::map<kToken, SymbolId> symbolId_of_tokenId_;
  std::map<Rule, RuleId>     ruleId_of_rule_;
  std::map<SymbolId, Symbol>  id_to_terminal_;
  std::map<SymbolId, Symbol>  id_to_nonterminal_;
  

  
  void ComputeFirstSets() noexcept;
  void ComputeFollowSets() noexcept;
  void ComputeFirstPlusSets() noexcept;
  
  void CreateSymbolId(const Symbol& symbol);
  void AddSymbol(const Symbol& symbol);
  
  
  //Helpers
  std::set<Symbol> First(const std::vector<Symbol>& derived);
  std::set<Symbol> First(const std::vector<Symbol>& derived, const Symbol& s);
};

std::ostream&
operator<<(std::ostream &os, const Grammar& g);

} //end namespace GrammarAnalyzer
