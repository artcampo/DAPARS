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
    
  void Reset(){};
private:


};


}//end namespace Backend
}//end namespace Compiler
