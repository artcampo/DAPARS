#pragma once
#include "Rule.hpp"
#include "Symbol.hpp"
#include "LR1_Item.hpp"
#include "LR0_Item.hpp"
#include <string>
#include <iostream>

/*
 * Provides overloads of <<
 */

namespace GrammarAnalyzer{

std::ostream&
operator<<(std::ostream &os, const LR1_Item& c);

std::ostream&
operator<<(std::ostream &os, const SetLR1_Item& c);

std::ostream&
operator<<(std::ostream &os, const LR0_Item& c);

std::ostream&
operator<<(std::ostream &os, const SetLR0_Item& c);

} //end namespace GrammarAnalyzer
