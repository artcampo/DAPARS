#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "ParserLR1.hpp"
#include "BaseParser.hpp"
#include "Tokenizer.hpp"
#include "GrammarLR1.hpp"
#include "Identifiers.hpp"


namespace LALR{

using namespace Common;
using namespace Tokenizer;
using namespace GrammarAnalyzer;  
  
class ParserLALR : public LR1::ParserLR1{
  
public:  
  ParserLALR(std::string const &file_name, Block* &programBlock, GrammarLR1& grammar);
//   void Parse();
  
private:
  
  
};

} //end namespace LALR
