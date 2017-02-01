#pragma once
#include "Rule.hpp"
#include "Symbol.hpp"
#include "LR1_Item.hpp"
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


} //end namespace GrammarAnalyzer
