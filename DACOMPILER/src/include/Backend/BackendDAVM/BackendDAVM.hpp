#pragma once
#include "Backend/Backend.hpp"
#include "ByteCode.hpp"
#include "Utils.hpp"  //move to VM/
#include "IRBuilder.hpp"
#include "Function.hpp"
#include "IR/IRVisitor.hpp"
#include "Backend/RegisterAllocator.hpp"
#include "Backend/BackendDAVM/MemAllocator.hpp"
#include "Backend/BackendDAVM/TargetDefDAVM.hpp"

#include <map>
#include <memory>

namespace Compiler{
namespace Backend{
namespace BackendDAVM{


class BackendDAVM : public Backend, IR::IRVisitor{
public:

  BackendDAVM(CompilationUnit& unit, IR::IRUnit& ir_unit)
  : Backend(unit, ir_unit, TargetDefDAVM())
  , reg_alloc_(TargetDefDAVM().NumRegisters())
  , mem_alloc_(){} //TODO: machine description

  void Run(){
    ComputeMainDataSegment();
    for(auto& it : ir_unit_.streams_) Visit(*it);
    
    std::cout << "---------\nBytecode:\n";
    VM::VMUtils::print(byte_code_);
  }
  
  VM::ByteCode& GetByteCode() noexcept { return byte_code_; }
  const VM::ByteCode& GetByteCode() const noexcept { return byte_code_; }
  
private:
  VM::ByteCode      byte_code_;
  RegisterAllocator reg_alloc_;  
  MemAllocator      mem_alloc_;
  
  void Visit(IR::IRStream& stream){
    //Account for stack_ptr
    int basic_register_usage = 1;
    //Account for arp_ptr
    if(stream.GetFunction().Name() != "main") basic_register_usage++;
    //Account for this_ptr
    if(stream.GetFunction().IsMember()) basic_register_usage++;
      
    reg_alloc_.Reset( stream.MaxRegUsed(), basic_register_usage );
    for(auto& it : stream) it->Accept(*this);
  }

  void Visit(const IR::Inst::Inst& inst) override{
    std::cout << "Not implemented: " << inst.str() << "\n";
  }
  void Visit(const IR::Inst::JumpCond& inst) override{
    std::cout << inst.str() << " !!!\n";
  }
  void Visit(const IR::Inst::JumpIncond& inst) override{
    std::cout << inst.str() << " !!!\n";
  }
  
  void Visit(const IR::Inst::LoadI& inst) override{
    std::cout << inst.str() << "\n";
    RegMap rd = reg_alloc_.IRReg( inst.RegDst() );
    reg_alloc_.GetRegLoadI(rd);
    byte_code_.Append( VM::IRBuilder::Load(rd.mreg_, inst.Value()));
  }
  
  void Visit(const IR::Inst::Load& inst) override{
    std::cout << inst.str() << "\n";
    RegMap rd = reg_alloc_.IRReg    (inst.RegDst());
    RegMap rs = reg_alloc_.IRMemAddr(inst.Addr());
    reg_alloc_.GetRegLoad(rd, rs);
    //TODO:issue load if rs is not in register
    
  }
  void Visit(const IR::Inst::LoadReg& inst) override{
    std::cout << inst.str() << " !!!LoadReg\n";
  }
  void Visit(const IR::Inst::LoadRegOffs& inst) override{
    std::cout << inst.str() << " !!!LoadRegOffs\n";
  }
  void Visit(const IR::Inst::Store& inst) override{
    std::cout << inst.str() << "\n";
    RegMap rs = reg_alloc_.IRReg    (inst.RegSrc());
    RegMap rd = reg_alloc_.IRMemAddr(inst.Addr());
    reg_alloc_.GetRegStore(rs, rd);
    byte_code_.Append( VM::IRBuilder::Store(rs.mreg_, mem_alloc_.Remap(inst.Addr())));
  }
  void Visit(const IR::Inst::StoreReg& inst) override{
    std::cout << inst.str() << " !!!\n";
  }
  void Visit(const IR::Inst::Arith& inst) override{
    std::cout << inst.str() << "\n";
    RegMap rd  = reg_alloc_.IRReg( inst.RegDst() );
    RegMap rs1 = reg_alloc_.IRReg( inst.RegSrc1() );
    RegMap rs2 = reg_alloc_.IRReg( inst.RegSrc2() );
    reg_alloc_.GetRegArith(rd, rs1, rs2);
    
    byte_code_.Append( VM::IRBuilder::Arith(rs1.mreg_, rs2.mreg_, rd.mreg_, int(inst.Op())));
  }
  void Visit(const IR::Inst::Comparison& inst) override{
    std::cout << inst.str() << " !!!\n";
  }  
  void Visit(const IR::Inst::PtrElem& inst) override{
    std::cout << inst.str() << " !!!\n";
  }  
  void Visit(const IR::Inst::GetRetVal& inst) override{
    std::cout << inst.str() << " !!!\n";
  }  
  void Visit(const IR::Inst::SetRetVal& inst) override{
    std::cout << inst.str() << " !!!\n";
  }  
  void Visit(const IR::Inst::SetPar& inst) override{
    std::cout << inst.str() << " !!!\n";
  }  
  void Visit(const IR::Inst::Return& inst) override{
    std::cout << inst.str() << " !!!\n";
  }  
  
  void Visit(const IR::Inst::ReturnMain& inst) override{
    std::cout << inst.str() << "\n";
    byte_code_.Append( VM::IRBuilder::Stop());
  }  
  
  void Visit(const IR::Inst::Call& inst) override{
    std::cout << inst.str() << "\n";
  }
  
  void ComputeMainDataSegment(){
    auto main = unit_.GetFunc("main");
    auto vars = main.LocalVars();
    auto size = vars.Size();
    byte_code_.SetStaticDataSegment(size);
    for(const auto& it : vars){
      IR::MemAddr addr_ir = it.second;
      mem_alloc_.ComputeRemap(addr_ir);
    }
    //mem_alloc_.Remap();
  }
  
};


}//end namespace Backend
}//end namespace Backend
}//end namespace Compiler
