#pragma once
#include "IR/MemAddr.hpp"
#include "BasicTypes.hpp"
#include <vector>
#include <map>
#include <memory>

namespace Compiler{
namespace Backend{



class MemAllocator{
public:

  MemAllocator(){}

  VM::Addr  Remap(const IR::MemAddr addr){
    return 0;
  }
  
  void Remap(const IR::MemAddr addr_ir, VM::Addr addr_vm){
  }
    
  void Reset(){};
private:


};


}//end namespace Backend
}//end namespace Compiler
