#pragma once
#include <string>
#include <stdint.h>

namespace VM{

using Addr    = uint32_t;  //regular memory address
using Target  = uint32_t;  //address to code
using Inst    = uint32_t;  //a single instruction
using Reg     = uint32_t;  //id of a register
using Word    = int32_t;   //a machine word (thus content of register/ram unit)
using SubInst = uint32_t;  //sub-values of an instruction

using TypeId = uint32_t;

struct FuncDesc{
  FuncDesc(const std::string& name, Addr entry) : name_(name), entry_(entry){};

  std::string name_;
  Addr        entry_;
};

} //end namespace VM


