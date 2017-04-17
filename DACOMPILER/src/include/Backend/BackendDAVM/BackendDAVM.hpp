#pragma once
#include "Backend/Backend.hpp"
#include "ByteCode.hpp"
#include "Utils.hpp"  //move to VM/
#include "IRBuilder.hpp"
#include "IR/IRVisitor.hpp"
#include "Backend/RegisterAllocator.hpp"
#include "Backend/BackendDAVM/MemAllocator.hpp"

#include <map>
#include <memory>

namespace Compiler{
namespace Backend{
namespace BackendDAVM{


class BackendDAVM : public Backend, IR::IRVisitor{
public:

  BackendDAVM(CompilationUnit& unit, IR::IRUnit& ir_unit)
  : Backend(unit, ir_unit), reg_alloc_(10), mem_alloc_(){} //TODO: machine description

  void Run(){
    ComputeMainDataSegment();
    for(auto& it : ir_unit_.streams_) Visit(*it);
    
    std::cout << "---------\nBytecode:\n";
    VM::VMUtils::print(byte_code_);
  }
  
private:
  VM::ByteCode      byte_code_;
  RegisterAllocator reg_alloc_;  
  MemAllocator      mem_alloc_;
  
  void Visit(IR::IRStream& stream){
    for(auto& it : stream) it->Accept(*this);
  }

  void Visit(const IR::Inst::Inst& inst) override{
    std::cout << "Not implemented: " << inst.str() << "\n";
  }
  void Visit(const IR::Inst::JumpCond& inst) override{
    std::cout << inst.str() << "\n";
  }
  void Visit(const IR::Inst::JumpIncond& inst) override{
    std::cout << inst.str() << "\n";
  }
  
  void Visit(const IR::Inst::LoadI& inst) override{
    std::cout << inst.str() << "\n";
    RegMap rd( inst.RegDst() );
    reg_alloc_.GetRegLoadI(rd);
    byte_code_.Append( VM::IRBuilder::Load(rd.mreg_, inst.Value()));
  }
  
  void Visit(const IR::Inst::Load& inst) override{
    std::cout << inst.str() << "\n";
  }
  void Visit(const IR::Inst::LoadReg& inst) override{
    std::cout << inst.str() << "\n";
  }
  void Visit(const IR::Inst::LoadRegOffs& inst) override{
    std::cout << inst.str() << "\n";
  }
  void Visit(const IR::Inst::Store& inst) override{
    std::cout << inst.str() << "\n";
    RegMap rs ( inst.RegSrc() );
    reg_alloc_.GetRegStore(rs);
    byte_code_.Append( VM::IRBuilder::Store(rs.mreg_, mem_alloc_.Remap(inst.Addr())));
  }
  void Visit(const IR::Inst::StoreReg& inst) override{
    std::cout << inst.str() << "\n";
  }
  void Visit(const IR::Inst::Arith& inst) override{
    std::cout << inst.str() << "\n";
    RegMap rd ( inst.RegDst() );
    RegMap rs1( inst.RegSrc1() );
    RegMap rs2( inst.RegSrc2() );
    reg_alloc_.GetRegArith(rd, rs1, rs2);
    
    byte_code_.Append( VM::IRBuilder::Arith(rs1.mreg_, rs2.mreg_, rd.mreg_, int(inst.Op())));
  }
  void Visit(const IR::Inst::Comparison& inst) override{
    std::cout << inst.str() << "\n";
  }  
  void Visit(const IR::Inst::PtrElem& inst) override{
    std::cout << inst.str() << "\n";
  }  
  void Visit(const IR::Inst::GetRetVal& inst) override{
    std::cout << inst.str() << "\n";
  }  
  void Visit(const IR::Inst::SetRetVal& inst) override{
    std::cout << inst.str() << "\n";
  }  
  void Visit(const IR::Inst::SetPar& inst) override{
    std::cout << inst.str() << "\n";
  }  
  void Visit(const IR::Inst::Return& inst) override{
    std::cout << inst.str() << "\n";
  }  
  
  void Visit(const IR::Inst::ReturnMain& inst) override{
    std::cout << inst.str() << "\n";
    byte_code_.Append( VM::IRBuilder::Stop());
  }  
  
  void Visit(const IR::Inst::Call& inst) override{
    std::cout << inst.str() << "\n";
  }    
  
  void ComputeMainDataSegment(){
    auto size = unit_.GetFunc("main").LocalVars().Size();
    byte_code_.SetStaticDataSegment(size);
  }
  
};


}//end namespace Backend
}//end namespace Backend
}//end namespace Compiler
