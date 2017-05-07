#pragma once
#include <stdint.h>
#include "VM/VMBasicTypes.hpp"
#include "VM/Execution/MMU.hpp"
#include "VM/VMSpec.hpp"

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

  Word StackRegisterInitAddress() const noexcept{
    return Word((uint64_t(1) << Spec::kWordBits) - Spec::kWordSize);
  }

private:


};

}//end namespace Internal
}//end namespace VM
