#pragma once
#include <map
#include <stdint.h>
#include "BasicTypes.hpp"
#include "MMU.hpp"

namespace VM{

class VirtualMachine;

class Memory : public MMU {

public:

  void Store(const Addr addr, const Word word){
    const Word* physical_addr = LogicalToPhysical(addr);
    *physical_addr = word;
  }

  Word Load(const Addr addr){
    const Word* physical_addr = LogicalToPhysical(addr);
    return *physical_addr;
  }

private:

};

}//end namespace VM
