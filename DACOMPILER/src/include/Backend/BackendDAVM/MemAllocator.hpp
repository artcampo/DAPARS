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
    return translation_.at(addr);
  }
  
  //Remap a variable
  void ComputeRemap(const IR::MemAddr addr_ir ){
    if(addr_ir.GetLabel().IsRunTime()){
      std::cout << "Trying to remap RT addr.\n";
      exit(1);
    }
    VM::Addr addr_vm(addr_ir.GetOffset().GetAddr());
    translation_[addr_ir] = addr_vm;
  }
    
  void Reset(){};
private:
  std::map<IR::MemAddr, VM::Addr> translation_;


};


}//end namespace Backend
}//end namespace Compiler
