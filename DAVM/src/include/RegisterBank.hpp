#pragma once
#include <vector>

namespace VM{

template <typename T>
struct RegisterBank{
  
  std::vector<T>  registers;

  RegisterBank(){
    registers.resize(10);
    for(auto &it : registers)
      it = 0;
  }
};

}//end namespace VM
