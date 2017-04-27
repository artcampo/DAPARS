#pragma once
#include <stdint.h>
#include <vector>
#include <cstddef>
#include "BasicTypes.hpp"

namespace VM{

struct ByteCode{

  void Append(const Inst inst){ stream.push_back(inst);}

  Target NextAddress() const noexcept{ return stream.size();}

  Inst  GetInst(VM::Addr addr) const { return stream[addr];}
  Inst& GetInst(VM::Addr addr){ return stream[addr];}

  //TODO:make private
  std::vector<Inst> stream;
  
  size_t static_data_segment_size_;
  
  void SetStaticDataSegment(const size_t size){
    static_data_segment_size_ = size;
  }
};

}//end namespace VM
