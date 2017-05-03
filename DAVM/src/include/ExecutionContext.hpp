#pragma once
#include <stdint.h>
#include "RegisterBank.hpp"
#include "VMBasicTypes.hpp"

namespace VM{

struct ExecutionContext : public RegisterBank<Word>{

  ExecutionContext() : instruction_pointer_(0){};

  Target             instruction_pointer_;
};

}//end namespace VM
