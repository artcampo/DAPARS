#pragma once
#include <stdint.h>
#include "VMBasicTypes.hpp"
#include "MMU.hpp"

namespace VM{
namespace Internal{

template <class TestingPolicy>
class Memory : public MMU<TestingPolicy> {

public:

  void Store(const Addr addr, const Word word){
    Word *const physical_addr = this->LogicalToPhysical(addr);
    *physical_addr = word;
  }

  Word Load(const Addr addr){
    Word *const physical_addr = this->LogicalToPhysical(addr);
    return *physical_addr;
  }

private:


};

}//end namespace Internal
}//end namespace VM
