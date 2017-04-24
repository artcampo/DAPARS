#pragma once
#include <stdint.h>
#include "RegisterBank.hpp"

namespace VM{

struct ExecutionContext{
  
  Target             instruction_pointer_;
  RegisterBank<Word> registers_;

  ExecutionContext():instruction_pointer_(0), registers_(){};
};

}//end namespace VM
