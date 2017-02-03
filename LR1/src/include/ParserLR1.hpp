#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "BaseParser.hpp"
#include "Tokenizer.hpp"
#include "GrammarLR1.hpp"
#include "Identifiers.hpp"



namespace LR1{

using namespace Common;
using namespace Tokenizer;
using namespace GrammarAnalyzer;  
  
class ParserLR1 : public BaseParser{
  
public:  
  ParserLR1(std::string const &file_name, Block* &programBlock, GrammarLR1& grammar);
  ParserLR1(const std::vector<char>& parse_data, Block* &programBlock, GrammarLR1& grammar);
  void Parse();
  
protected:
  GrammarLR1& grammar_;
  
private:  
  void NextWord();
  
};

} //end namespace LR1
