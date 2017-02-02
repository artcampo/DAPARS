#pragma once
#include "Rule.hpp"
#include "Symbol.hpp"
#include "LR1_Item.hpp"
#include "LR0_Item.hpp"
#include "LR_Tables.hpp"
#include <string>
#include <iostream>
#include <map>

/*
 * Provides overloads of <<
 */

namespace GrammarAnalyzer{

std::ostream& operator<<(std::ostream &os, const Symbol& s);
std::ostream& operator<<(std::ostream &os, const Rule& r);
  
std::ostream& operator<<(std::ostream &os, const LR1_Item& c);
std::ostream& operator<<(std::ostream &os, const LR0_Item& c);

std::ostream& operator<<(std::ostream &os, const SetLR1_Item& c);
std::ostream& operator<<(std::ostream &os, const SetLR0_Item& c);

std::ostream& operator<<(std::ostream& os, const LR_Tables& t);

std::ostream&
operator<<(std::ostream &os, const std::vector<Symbol>& c);

std::ostream&
operator<<(std::ostream &os, const std::set<Symbol>& c);

std::ostream&
operator<<(std::ostream &os, const std::map<Symbol, std::set<Symbol>>& c);


} //end namespace GrammarAnalyzer
