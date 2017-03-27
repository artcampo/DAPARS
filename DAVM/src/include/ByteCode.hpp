#pragma once
#include <stdint.h>
#include <vector>
#include <cstddef>
#include "BasicTypes.hpp"


using VM::Addr;
using VM::Inst;

struct ByteCode{

  void Append(const Inst inst){ stream.push_back(inst);}

  Addr NextAddress() const noexcept{ return stream.size();}

  Inst  GetInst(VM::Addr addr) const { return stream[addr];}
  Inst& GetInst(VM::Addr addr){ return stream[addr];}

  //TODO:make private
  std::vector<uint32_t> stream;
};
