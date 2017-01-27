#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "BaseParser.hpp"
#include "Tokenizer.hpp"

using namespace Common;
using namespace Tokenizer;

namespace LR1{

class Parser : public BaseParser{
  
public:  
  Parser(std::string const &file_name, Block* &programBlock);
  void Parse();
  
private:
  
  
};

} //end namespace LR1
