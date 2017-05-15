#pragma once
#include <stdint.h>
#include <vector>
#include <cstddef>
#include <map>
#include <iostream>
#include "VM/VMBasicTypes.hpp"
#include "VM/ByteCode/IRPrint.hpp"
#include "VM/ByteCode/MemChunk.hpp"

namespace VM{

struct ByteCode{
  ByteCode() : perform_compiler_test_(false){}

  void Append(const Inst inst){ stream.push_back(inst);}

  Target NextAddress() const noexcept{ return stream.size();}

  Inst  GetInst(VM::Addr addr) const { return stream[addr];}
  Inst& GetInst(VM::Addr addr){ return stream[addr];}

  void AddFunction(const std::string& name, VM::Addr entry){
    func_index_by_addr_[entry] = functions_.size();
    functions_.push_back(FuncDesc(name, entry));
  }


  size_t StaticDataSegmentSize() const noexcept{
    if(mem_user_space_.empty()) return 0;
    else return mem_user_space_[0].Size();
  }

  void SetMemUser(const MemChunk m){
    mem_user_space_.push_back(m);
  }

  void SetMemStack(const MemChunk m){
    mem_stack_ = m;
  }

  Word StackRegisterInitAddress() const noexcept{
    return mem_stack_.high_ + 1 - Spec::kWordSize;
  }

  Addr StackRegisterHigh() const noexcept{
    return StackRegisterInitAddress();
  }

  Addr StackRegisterLow() const noexcept{
    return mem_stack_.low_;
  }

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

//   friend ByteCode* readByteCode(std::string const &file_name);
//   friend void writeByteCode(ByteCode const &byte_code, std::string const &file_name);

  bool PerformCompilerTest() const noexcept{ return perform_compiler_test_; }

  //TODO:make private
  std::vector<Inst> stream;
  std::vector<MemChunk> mem_user_space_;
  MemChunk              mem_stack_;
  bool  perform_compiler_test_;

private:


  std::vector<FuncDesc>       functions_;
  std::map<VM::Addr, size_t>  func_index_by_addr_;


};

}//end namespace VM
