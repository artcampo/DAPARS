#pragma once
#include "Backend/Backend.hpp"
#include "ByteCode.hpp"
#include "Utils.hpp"  //move to VM/
#include "IRBuilder.hpp"
#include "IRDefinition.hpp"
#include "Function.hpp"
#include "IR/Label.hpp"
#include "IR/IRVisitor.hpp"
#include "Backend/RegisterAllocator.hpp"
#include "Backend/BackendCallbacks.hpp"
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
  , reg_alloc_( TargetDefDAVM().NumRegisters()
              , &Backend::LoadCallBack
              , this)
  , mem_alloc_()
  , backpatch_free_id_(0){}

  void Run(){
    ComputeMainDataSegment();
    for(auto& it : ir_unit_.streams_) Visit(*it);
    BackpathCallTargets();
    
    std::cout << "---------\nBytecode:\n";
    VM::VMUtils::print(byte_code_);
  }
  
  void LoadCallBack(const MReg reg_dst, const IR::MemAddr addr) override {
//     std::cout << "Load callback\n" << reg_dst <<"\n";
    if(addr.GetLabel().IsRunTime()){
      MReg reg_base;
      if(addr.GetLabel().IsArp())     reg_base = reg_alloc_.MRegArpPtr();
      if(addr.GetLabel().IsThisPtr()) reg_base = reg_alloc_.MRegThisPtr();
//       std::cout << "Base: " << reg_base << addr.GetLabel().str()<<"\n";
      byte_code_.Append( VM::IRBuilder::LoadB(reg_dst, reg_base, addr.GetOffset().GetAddr()));
//       std::cout << "loadOff: " << addr.GetOffset().GetAddr() << "\n";
    }
  }
  
  VM::ByteCode& GetByteCode() noexcept { return byte_code_; }
  const VM::ByteCode& GetByteCode() const noexcept { return byte_code_; }
  
private:
  VM::ByteCode      byte_code_;
  RegisterAllocator reg_alloc_;  
  MemAllocator      mem_alloc_;
  bool              is_first_arg_;
  std::map<IR::MemAddr, VM::Target> backpatch_ids_;
  VM::Target backpatch_free_id_;
  
  void Visit(IR::IRStream& stream){
    is_first_arg_ = true;
    bool is_main  = stream.GetFunction().Name() == "main";
    //Account for stack_ptr
    int basic_register_usage = 1;
    //Account for arp_ptr
    if(not is_main) basic_register_usage++;
    //Account for this_ptr
    if(stream.GetFunction().IsMember()) basic_register_usage++;
      
    reg_alloc_.Reset( stream.MaxRegUsed(), basic_register_usage );
    if(not is_main) FuncPrologue(stream.GetFunction());
    for(auto& it : stream) it->Accept(*this);
    if(not is_main) FuncEpilogue(stream.GetFunction());
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
    byte_code_.Append( VM::IRBuilder::LoadI(rd.mreg_, inst.Value()));
  }
  
  void Visit(const IR::Inst::Load& inst) override{
    std::cout << inst.str() << " **\n";
    RegMap rd = reg_alloc_.IRReg    (inst.RegDst());
    RegMap rs = reg_alloc_.IRMemAddr(inst.Addr());
    reg_alloc_.GetRegLoad(rd, rs);
    //TODO:issue load if rs is not in register
    reg_alloc_.Dump();
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
    RegMap rd     = reg_alloc_.IRReg( inst.RegDst() );
    MReg rs_mreg  = reg_alloc_.MRegRetValue();
    reg_alloc_.GetRegGetRetVal(rd);
    byte_code_.Append( VM::IRBuilder::Move(rs_mreg, rd.mreg_));    
  }  
  
  void Visit(const IR::Inst::SetRetVal& inst) override{
    std::cout << inst.str() << " !!!\n";
  }  
  void Visit(const IR::Inst::SetPar& inst) override{
    std::cout << inst.str() << "\n";
    if(is_first_arg_){
      RegMap rs = reg_alloc_.IRReg(inst.RegSrc());
      reg_alloc_.GetRegRead(rs);
      //If it's not already on mreg=0, move it
      if(rs.mreg_ != reg_alloc_.MRegRetValue()){
        reg_alloc_.Flush( reg_alloc_.MRegRetValue());
        RegMap rd = reg_alloc_.ForceReg( reg_alloc_.MRegRetValue() );
        byte_code_.Append( VM::IRBuilder::Move(rs.mreg_, rd.mreg_));
      }
    
    }else{
      //push arg to the stack
      //TODO
    }
    
    is_first_arg_ = false;
  }  
  
  void Visit(const IR::Inst::GetArg& inst) override{
    std::cout << inst.str() << "\n";
    RegMap rd = reg_alloc_.IRReg( inst.RegDst() );
    rd.mreg_  = inst.Value ();
    reg_alloc_.SetRegGetArg(rd);
  }    
  
  void Visit(const IR::Inst::Return& inst) override{
    std::cout << inst.str() << "\n";
    byte_code_.Append( VM::IRBuilder::Return());
  }  
  
  void Visit(const IR::Inst::ReturnMain& inst) override{
    std::cout << inst.str() << "\n";
    byte_code_.Append( VM::IRBuilder::Stop());
  }  
  
  void Visit(const IR::Inst::Call& inst) override{
    std::cout << inst.str() << "\n";
    
    if(inst.Addr().GetLabel().IsLinkTime()){
      VM::Target target_id = BackpatchId(inst.Addr());
      byte_code_.Append( VM::IRBuilder::Call(target_id));
    }else{
      //TODO: call through computed register
    }
    
    is_first_arg_ = true; //next set of arguments will not belong to this call
  }
  
  void FuncPrologue(const Function& f){
    byte_code_.Append( VM::IRBuilder::Move( reg_alloc_.MRegStackPtr()
                                          , reg_alloc_.MRegArpPtr() ));
    if(f.HasLocals()){
      const size_t  locals_size = f.LocalVars().Size();
      const MReg    rd = reg_alloc_.MRegStackPtr();
      byte_code_.Append( VM::IRBuilder::ArithI(rd
                                , locals_size
                                , VM::IRDefinition::SubtypesArithmetic::IR_SUB));
    }
  }
  
  void FuncEpilogue(const Function& f){
    if(f.HasLocals()){ 
      byte_code_.Append( VM::IRBuilder::Move( reg_alloc_.MRegArpPtr()
                                            , reg_alloc_.MRegStackPtr()));
    }
  }
  
  //Other functions that do not handle sections of IR
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
  
  VM::Target BackpatchId(const IR::MemAddr addr){
    auto it = backpatch_ids_.find(addr);
    if(it == backpatch_ids_.end()){
      backpatch_ids_[addr] = backpatch_free_id_;
      return backpatch_free_id_++;
    }
    return it->second;
  }
  
  void BackpathCallTargets(){
    
  }
  
};


}//end namespace Backend
}//end namespace Backend
}//end namespace Compiler
