#pragma once
#include <vector>

template <typename T>
struct RegisterBank{
  
  std::vector<T>  registers;

  RegisterBank(){
    registers.resize(10);
    for(auto &it : registers)
      it = 0;
  }
};