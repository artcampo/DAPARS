#pragma once
#include <string>
#include <memory>
#include <cstddef>
#include "IR/Label.hpp"
#include "IR/Offset.hpp"

namespace Compiler{
namespace IR{


struct MemAddr{
  MemAddr() = default;
  MemAddr(const Label l, const Offset o)
    : l_(l), o_(o){}

//   std::string str() const noexcept {return l_.str() + o_.str;}
  std::string str() const noexcept {return l_.str() + ":"+ o_.str();}

  const bool operator<  ( const MemAddr &rhs )const noexcept{
    if(l_ == rhs.l_)  return o_ < rhs.o_;
    else              return l_ < rhs.l_;
  }
    
  Offset  GetOffset() noexcept { return o_;}
  const Offset  GetOffset() const noexcept { return o_;}
  Label  GetLabel() noexcept { return l_;}
  const Label  GetLabel() const noexcept { return l_;}  
private:
  Label   l_;
  Offset  o_;
};


}//end namespace IR
}//end namespace Compiler
