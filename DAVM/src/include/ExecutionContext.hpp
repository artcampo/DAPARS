#pragma once
#include <stdint.h>
#include "RegisterBank.hpp"
#include "VMBasicTypes.hpp"

namespace VM{
namespace Internal{

struct ExecutionContext : public RegisterBank<Word>{

  ExecutionContext() : instruction_pointer_(0){};

  Target             instruction_pointer_;
};

}//end namespace Internal
}//end namespace VM
