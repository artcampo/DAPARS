#pragma once
#include <stdint.h>
#include <vector>
#include <cstddef>
#include <map>
#include <iostream>
#include "VMBasicTypes.hpp"
#include "IRPrint.hpp"

namespace VM{

struct ByteCode{

  void Append(const Inst inst){ stream.push_back(inst);}

  Target NextAddress() const noexcept{ return stream.size();}

  Inst  GetInst(VM::Addr addr) const { return stream[addr];}
  Inst& GetInst(VM::Addr addr){ return stream[addr];}

  void AddFunction(const std::string& name, VM::Addr entry){
    func_index_by_addr_[entry] = functions_.size();
    functions_.push_back(FuncDesc(name, entry));
  }

  //TODO:make private
  std::vector<Inst> stream;

  size_t static_data_segment_size_;

  void SetStaticDataSegment(const size_t size){
    static_data_segment_size_ = size;
  }

  std::vector<FuncDesc>       functions_;
  std::map<VM::Addr, size_t>  func_index_by_addr_;

  void Dump(const bool extra_dump) const{
    int line = 0;
    for ( auto const inst : stream){
      if(extra_dump) std::cout << line<< ": ";
      std::cout << IRBuilder::PrintInstruction(inst);
      if(extra_dump and IsFuncEntry(line) ) std::cout << "  <- ["<< Func(line).name_ << "]";
      std::cout << "\n";
      ++line;
    }
  }

  bool  IsFuncEntry(const VM::Addr addr) const{
    auto it = func_index_by_addr_.find(addr);
    return it != func_index_by_addr_.end();
  }

  //addr must be the entry point for any function
  FuncDesc& Func(const VM::Addr addr){
    auto index = func_index_by_addr_.at(addr);
    return functions_.at(index);
  }

  const FuncDesc& Func(const VM::Addr addr) const{
    auto index = func_index_by_addr_.at(addr);
    return functions_.at(index);
  }

};

}//end namespace VM
