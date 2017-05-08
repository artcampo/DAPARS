#pragma once
#include "VM/VMBasicTypes.hpp"
#include "VM/VMSpec.hpp"
#include <iostream>

namespace VM{
/*
struct MemChunk;

namespace VMUtils{
  std::istream& operator>>(std::istream &is, MemChunk& m);
  std::ostream& operator<<(std::ostream& os, const MemChunk& m);
};*/


struct MemChunk{
  MemChunk() = default;
  MemChunk(const Addr low, const Addr high) : low_(low), high_(high){}

  Addr  Size() const noexcept{ return high_ - low_;}

  Addr low_;
  Addr high_;

  static MemChunk LastPage(){
    return MemChunk(Spec::kMaxAddress - Spec::kPageSize + 1, Spec::kMaxAddress);
  }

  friend std::istream& operator>>(std::istream &is, MemChunk& m);
  friend std::ostream& operator<<(std::ostream& os, const MemChunk& m);
};



}//end namespace VM
