#pragma once
#include <stdint.h>
#include "VM/Execution/RegisterBank.hpp"
#include "VM/VMBasicTypes.hpp"

namespace VM{
namespace Internal{

struct ExecutionContext : public RegisterBank<Word>{

  ExecutionContext() : instruction_pointer_(0){};

  Word&  StackReg() noexcept { return registers_[Spec::kNumberRegisters - 1];}
  const Word&  StackReg() const noexcept { return registers_[Spec::kNumberRegisters - 1];}

  Target  GetIP() const noexcept{return instruction_pointer_;}

protected:
  Target             instruction_pointer_;
};

}//end namespace Internal
}//end namespace VM
