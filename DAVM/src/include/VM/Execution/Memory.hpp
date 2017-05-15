#pragma once
#include <stdint.h>
#include "VM/VMBasicTypes.hpp"
#include "VM/Execution/MMU.hpp"
#include "VM/VMSpec.hpp"
#include "VM/ByteCode/MemChunk.hpp"
#include <iostream>
#include <algorithm>

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


  void Allocate(const MemChunk m){
//     std::cout << "alloc " << m.low_ <<" to " << m.high_ << "\n";
    Addr a = m.low_;
    bool finished = false;
    this->AllocateLogical(a);
//     std::cout << a << " ";
    while(not finished){
      //avoid overflow
      if(a >= (Spec::kMaxAddress - Spec::kPageSize + 1)) finished = true;
      else{
        a += Spec::kPageSize;
        if(a >= m.high_){ a = m.high_; finished = true;}
        this->AllocateLogical(a);
//         std::cout << a << " ";
      }
    }
//     std::cout << "\n";
  }

private:


};

}//end namespace Internal
}//end namespace VM
