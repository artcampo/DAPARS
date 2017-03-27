#pragma once
#include <stdint.h>
#include "RegisterBank.hpp"

struct ExecutionContext{
  
  uint32_t                instruction_pointer_;
  RegisterBank<uint32_t>  registers_;

  ExecutionContext():instruction_pointer_(0), registers_(){};
};