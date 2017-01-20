#pragma once
#include <string>

namespace GrammarAnalyzer{

class Symbol{
  
public:  
  explicit Symbol(const std::string& name, const std::string& value, 
         const bool is_terminal, const bool is_empty = false)
  : name_(name), value_(value), is_terminal_(is_terminal), is_empty_(is_empty)
  {}

  std::string str() const noexcept{
    if(is_empty_) return std::string("{empty}");
    else          return value_;
  }
  
  bool IsEmpty() const noexcept   { return is_empty_; }
  bool IsTerminal() const noexcept{ return is_terminal_; }
  
  const std::string name_;
  const std::string value_;
  const bool is_terminal_; 
  const bool is_empty_;
  
  const Symbol static Empty(){ return Symbol("EMPTY","{empty}", true, true); }
  const Symbol static Eof()  { return Symbol("EOF",  "{eof}",   true, false); }
  
  const bool operator< ( const Symbol &s ) const { return ( name_ < s.name_ );}
  const bool operator== ( const Symbol &s ) const { return ( name_ == s.name_ );}
  const bool operator!= ( const Symbol &s ) const { return not( name_ == s.name_ );}
  
private:

};

/*
namespace std
{
template<> struct less<GrammarAnalyzer::Symbol>
{
  bool operator() (const GrammarAnalyzer::Symbol& lhs, 
                    const GrammarAnalyzer::Symbol& rhs) const{
      return lhs.name_ < rhs.name_;
  }
};
}//end namespace std
*/

} //end namespace GrammarAnalyzer
