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
              , &Backend::StoreCallBack
              , this)
  , mem_alloc_()
  , call_backpatch_free_id_(0)
  , ir_inst_addr_(0){}

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
  
  void StoreCallBack(const MReg reg_src, const IR::MemAddr addr) override {
//     std::cout << "Load callback\n" << reg_dst <<"\n";
    if(addr.GetLabel().IsRunTime()){
      MReg reg_base;
      if(addr.GetLabel().IsArp())     reg_base = reg_alloc_.MRegArpPtr();
      if(addr.GetLabel().IsThisPtr()) reg_base = reg_alloc_.MRegThisPtr();
//       std::cout << "Base: " << reg_base << addr.GetLabel().str()<<"\n";
      byte_code_.Append( VM::IRBuilder::StoreB(reg_src, reg_base, addr.GetOffset().GetAddr()));
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
  int               pushed_args_for_current_call_;
  std::map<IR::MemAddr, VM::Target> call_backpatch_ids_;
  std::map<VM::Target, IR::MemAddr> call_backpatch_ids_inverse_;
  std::map<IR::MemAddr, VM::Target> call_backpatch_translation_;
  std::map<IR::Addr, VM::Target>    jump_backpatch_translation_;
  VM::Target  call_backpatch_free_id_;
  IR::Addr    ir_inst_addr_;
  
  void Visit(IR::IRStream& stream){
    is_first_arg_ = true;
    pushed_args_for_current_call_ = 0;
    bool is_main  = stream.GetFunction().Name() == "main";
    //Account for stack_ptr
    int basic_register_usage = 1;
    //Account for arp_ptr
    if(not is_main) basic_register_usage++;
    //Account for this_ptr
    if(stream.GetFunction().IsMember()) basic_register_usage++;
      
    //Save address to backpatch calls to this function
    CallBackPatchTranslation( stream.EntryMemAddr()
                            , byte_code_.NextAddress());
    
    //Translate
    reg_alloc_.Reset( stream.MaxRegUsed(), basic_register_usage );
    if(not is_main) FuncPrologue(stream.GetFunction());
    for(auto& it : stream){
      //Store translation of address from IR to VM's bytecode
      VM::Addr address_of_vm_inst = byte_code_.NextAddress();
      JumpBackPatchTranslation(ir_inst_addr_, address_of_vm_inst);
      
      it->Accept(*this);
      ir_inst_addr_++;  //keep track of current IR's instruction offset
    }
    if(not is_main) FuncEpilogue(stream.GetFunction());
  }

  void Visit(const IR::Inst::Inst& inst) override{
    std::cout << "Not implemented: " << inst.str() << "\n";
  }
  
  void Visit(const IR::Inst::JumpCond& inst) override{
    std::cout << inst.str() << "\n";
    RegMap rc = reg_alloc_.IRReg(inst.RegCond());
    reg_alloc_.GetRegRead(rc);
    VM::Target target_id = BackpatchId(inst.GetTarget());
    if(inst.GetJumpCondType() == IR::JumpCondType::kTrue)
      byte_code_.Append( VM::IRBuilder::JumpIfTrue (rc.mreg_, target_id));
    else
      byte_code_.Append( VM::IRBuilder::JumpIfFalse(rc.mreg_, target_id));
  }
  
  void Visit(const IR::Inst::JumpIncond& inst) override{
    std::cout << inst.str() << "\n";
    VM::Target target_id = BackpatchId(inst.GetTarget());
    byte_code_.Append( VM::IRBuilder::Jump(target_id));
  }
  
  void Visit(const IR::Inst::LoadI& inst) override{
    std::cout << inst.str() << "\n";
    RegMap rd = reg_alloc_.IRReg( inst.RegDst() );
    reg_alloc_.GetRegLoadI(rd);
    byte_code_.Append( VM::IRBuilder::LoadI(rd.mreg_, inst.Value()));
  }
  
  void Visit(const IR::Inst::Load& inst) override{
    std::cout << inst.str() << "\n";
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
    
    int op = VM::IRDefinition::SubtypesArithmetic::IR_ADD;
    byte_code_.Append( VM::IRBuilder::Arith(rs1.mreg_, rs2.mreg_, rd.mreg_, op));
  }
  
  void Visit(const IR::Inst::Logic& inst) override{
    std::cout << inst.str() << " !!!\n";
  }  
  
  void Visit(const IR::Inst::Comparison& inst) override{
    std::cout << inst.str() << "\n";
    RegMap rd  = reg_alloc_.IRReg( inst.RegDst() );
    RegMap rs1 = reg_alloc_.IRReg( inst.RegSrc1() );
    RegMap rs2 = reg_alloc_.IRReg( inst.RegSrc2() );
    reg_alloc_.GetRegArith(rd, rs1, rs2);
    
    int op = VM::IRDefinition::SubtypesComparison::IR_LST;
    //VM::IRDefinition::SubtypesArithmetic::IR_ADD;
    byte_code_.Append( VM::IRBuilder::Comp(rs1.mreg_, rs2.mreg_, rd.mreg_, op));    
  }  
  
  void Visit(const IR::Inst::PtrElem& inst) override{
    std::cout << inst.str() << " !!!\n";
  }  
  
  void Visit(const IR::Inst::GetRetVal& inst) override{
    std::cout << inst.str() << "\n";
    RegMap rd     = reg_alloc_.IRReg( inst.RegDst() );
    MReg rs_mreg  = reg_alloc_.MRegRetValue();
    reg_alloc_.GetRegGetRetVal(rd);
    byte_code_.Append( VM::IRBuilder::Move(rs_mreg, rd.mreg_));    
  }  
  
  void Visit(const IR::Inst::SetRetVal& inst) override{
    std::cout << inst.str() << "\n";
    RegMap rs = reg_alloc_.IRReg(inst.RegSrc());
    reg_alloc_.GetRegRead(rs);
    byte_code_.Append( VM::IRBuilder::Move(rs.mreg_, reg_alloc_.MRegRetValue()));
  }  
  
  void Visit(const IR::Inst::SetArg& inst) override{
    std::cout << inst.str() << "\n";
    if(is_first_arg_){
      RegMap rs = reg_alloc_.IRReg(inst.RegSrc());
      reg_alloc_.GetRegRead(rs);
      //If it's not already on mreg=0, move it
      if(rs.mreg_ != reg_alloc_.MRegRetValue()){
        reg_alloc_.FlushMReg( reg_alloc_.MRegRetValue());
        RegMap rd = reg_alloc_.ForceReg( reg_alloc_.MRegRetValue() );
        byte_code_.Append( VM::IRBuilder::Move(rs.mreg_, rd.mreg_));
      }
    
    }else{
      //push arg to the stack
      RegMap rs = reg_alloc_.IRReg(inst.RegSrc());
      reg_alloc_.GetRegRead(rs);
      byte_code_.Append( VM::IRBuilder::Push(rs.mreg_));
      ++pushed_args_for_current_call_;
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
    //Return emitted alongside epilogue
  }  
  
  void Visit(const IR::Inst::ReturnMain& inst) override{
    std::cout << inst.str() << "\n";
    byte_code_.Append( VM::IRBuilder::Stop());
  }  
  
  void Visit(const IR::Inst::Call& inst) override{
    std::cout << inst.str() << "\n";
    
    if(inst.Addr().GetLabel().IsLinkTime()){
//       std::cout << "call to: " << inst.Addr().str()<<"\n";
      VM::Target target_id = BackpatchId(inst.Addr());
      byte_code_.Append( VM::IRBuilder::Call(target_id));
    }else{
      //TODO: call through computed register
    }
    
    if(pushed_args_for_current_call_ > 0){
      //effectively pop all pushed args at once
      byte_code_.Append( VM::IRBuilder::ArithI(reg_alloc_.MRegStackPtr()
                                , pushed_args_for_current_call_
                                , VM::IRDefinition::SubtypesArithmetic::IR_ADD));      
    }
    
    //prepare for next call
    is_first_arg_ = true; 
    pushed_args_for_current_call_ = 0;
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
    byte_code_.Append( VM::IRBuilder::Return());
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
  
  //Jumps refer to their IR address, and will have to be translated later
  //to the VM address where the original instruction was placed
  VM::Target  BackpatchId(const IR::Addr addr){return addr;}
  void        JumpBackPatchTranslation(const IR::Addr addr, VM::Target target){
    jump_backpatch_translation_[addr] = target;
//     for(const auto& it : jump_backpatch_ids_) std::cout << it.first <<":" <<it.second << " ";
//     std::cout << "\n";
  }
  VM::Target JumpPatch(const IR::Addr addr){
//     for(const auto& it : jump_backpatch_ids_) std::cout << it.first <<":" <<it.second << " ";
//     std::cout << "\n"; 
//     std::cout << "ask : " << addr;
    return jump_backpatch_translation_.at(addr);
  }
  
  //Calls refer to MemAddr (label,offset) and thus need an intermediate
  //identifier, which after producing the VM's function can be changed
  VM::Target BackpatchId(const IR::MemAddr addr){
    auto it = call_backpatch_ids_.find(addr);
    if(it == call_backpatch_ids_.end()){
      call_backpatch_ids_[addr] = call_backpatch_free_id_;
      call_backpatch_ids_inverse_[call_backpatch_free_id_] = addr;
      return call_backpatch_free_id_++;
    }
    return it->second;
  }
  void CallBackPatchTranslation(const IR::MemAddr addr, VM::Target target){
    call_backpatch_translation_[addr] = target;
  }
  VM::Target CallPatch(VM::Target target){
    const IR::MemAddr addr = call_backpatch_ids_inverse_.at(target);
    return call_backpatch_translation_.at(addr);
  }
  
  
  void BackpathCallTargets(){
    for(auto& inst : byte_code_.stream){
      VM::Target target;
      if(VM::IRBuilder::IsJump(inst, target))
        VM::IRBuilder::PatchJump(inst, JumpPatch(IR::Addr(target)));
      if(VM::IRBuilder::IsCall(inst, target))
        VM::IRBuilder::PatchCall(inst, CallPatch(target));
    }
  }
  
};


}//end namespace Backend
}//end namespace Backend
}//end namespace Compiler
