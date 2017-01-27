#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "BaseParser.hpp"
#include "Tokenizer.hpp"
#include "GrammarLR1.hpp"
#include "Identifiers.hpp"

using namespace Common;
using namespace Tokenizer;
using namespace GrammarAnalyzer;

namespace LR1{

  
class Parser : public BaseParser{
  
public:  
  Parser(std::string const &file_name, Block* &programBlock, GrammarLR1& grammar);
  void Parse();
  
private:
  GrammarLR1& grammar_;
  
  void NextWord();
  
};

} //end namespace LR1
