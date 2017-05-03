#pragma once
#include <vector>
#include "VMBasicTypes.hpp"

namespace VM{

template <typename T>
struct RegisterBank{

  std::vector<T>  registers_;

  RegisterBank(){
    registers_.resize(Spec::kNumberRegisters);
    for(auto &it : registers_) it = 0;
  }
};

}//end namespace VM
