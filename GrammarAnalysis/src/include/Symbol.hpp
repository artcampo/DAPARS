#pragma once
#include <string>

namespace GrammarAnalyzer{

class Symbol{
  
public:  
  explicit Symbol(const std::string& name, const std::string& value, 
         const bool is_terminal, const bool is_empty = false)
  : name_(name), value_(value), is_terminal_(is_terminal), is_empty_(is_empty)
  {}

  std::string str(){
    if(is_empty_) return std::string("{empty}");
    else          return value_;
  }
  
  const std::string name_;
  const std::string value_;
  const bool is_terminal_; 
  const bool is_empty_;
  
  const Symbol static Empty(){
    return Symbol("EMPTY","{empty}", true, true);
  }
  
private:

};

} //end namespace GrammarAnalyzer
