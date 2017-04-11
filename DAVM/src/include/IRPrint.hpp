#pragma once
#include "IRCodification.hpp"
//TODO:
#include "BasicTypes.hpp"
namespace VM{
namespace IRBuilder{

using VM::Addr;
using VM::Inst;
using VM::Target;
using VM::Reg;

std::string PrintInstruction(const uint32_t& instruction);

} //namespace IRBuilder
}//end namespace VM
