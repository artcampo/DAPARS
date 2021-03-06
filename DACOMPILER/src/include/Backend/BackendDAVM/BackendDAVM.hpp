#pragma once
#include "Backend/Backend.hpp"
#include "VM/ByteCode/ByteCode.hpp"
#include "VM/ByteCode/Utils.hpp"
#include "VM/ByteCode/IRBuilder.hpp"
#include "VM/ByteCode/IRDefinition.hpp"
#include "Function.hpp"
#include "IR/IRSubtypes.hpp"
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
    ComputeCompilerDataSegment();
    ComputeStackDataSegment();
    for(auto& it : ir_unit_.streams_) Visit(*it);
    BackpathCallTargets();

    byte_code_.SetPerformCompilerTest();
    std::cout << "---------\nBytecode:\n";
    VM::VMUtils::print(byte_code_, true);
  }

  void WriteBytecode(const std::string& file_name){
    VM::VMUtils::writeByteCode(byte_code_, file_name);
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
    if(addr.GetLabel().IsLinkTime())
      byte_code_.Append( VM::IRBuilder::Load(reg_dst, mem_alloc_.Remap(addr)));
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
    if(addr.GetLabel().IsLinkTime())
      byte_code_.Append( VM::IRBuilder::Store(reg_src, mem_alloc_.Remap(addr)));
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
    const std::string& name = stream.GetFunction().Name();
    const std::string& mname = stream.GetFunction().MangledName();
    const bool is_main = (name == "main");
    //Account for stack_ptr
    int basic_register_usage = 1;
    //Account for arp_ptr
    if(not is_main) basic_register_usage++;
    //Account for this_ptr
    if(stream.GetFunction().IsMember()) basic_register_usage++;

    //Save address to backpatch calls to this function
    CallBackPatchTranslation( stream.EntryMemAddr()
                            , byte_code_.NextAddress());

    //Add function desc to BC
    byte_code_.AddFunction(mname, byte_code_.NextAddress());

    //Translate
    if(is_main) InitTest();
    reg_alloc_.Reset( stream.MaxRegUsed(), basic_register_usage );
    if(not is_main) FuncPrologue(stream.GetFunction());

    for(auto& it : stream){
      //flush reg alloc at start of BB
      if(ir_unit_.IsInstBeginBB(ir_inst_addr_)){
//         std::cout << "RegAlloc::flush::BB\n";
        reg_alloc_.Flush();
      }

      //Store translation of address from IR to VM's bytecode
      VM::Addr address_of_vm_inst = byte_code_.NextAddress();
      JumpBackPatchTranslation(ir_inst_addr_, address_of_vm_inst);


//       std::cout << it->str() <<" IR " <<ir_inst_addr_ << "-> VM " <<address_of_vm_inst<<"\n";
      std::cout << it->str();
      it->Accept(*this);
//       std::cout << " => "; byte_code_.DumpLast();
      std::cout <<"\n";
//       reg_alloc_.Dump();
      ir_inst_addr_++;  //keep track of current IR's instruction offset
    }
    if(not is_main) FuncEpilogue(stream.GetFunction());
  }

  //TODO: delete this one
  void Visit(const IR::Inst::Inst& inst) override{
    std::cout << "Not implemented: " << inst.str() << "\n";
  }

  void Visit(const IR::Inst::JumpCond& inst) override{
    IssueWriteBack();
    RegMap rc = reg_alloc_.IRReg(inst.RegCond());
//     reg_alloc_.Dump2();
    reg_alloc_.GetRegRead(rc);
//     std::cout << "jump cond " << inst.RegCond() << " to " << rc.mreg_ << "\n";
    VM::Target target_id = BackpatchId(inst.GetTarget());
    if(inst.GetJumpCondType() == IR::JumpCondType::kTrue)
      byte_code_.Append( VM::IRBuilder::JumpIfTrue (rc.mreg_, target_id));
    else
      byte_code_.Append( VM::IRBuilder::JumpIfFalse(rc.mreg_, target_id));
//     std::cout << VM::IRBuilder::Print(byte_code_.stream.back()) << "\n";
  }

  void Visit(const IR::Inst::JumpIncond& inst) override{
    IssueWriteBack();
    VM::Target target_id = BackpatchId(inst.GetTarget());
    byte_code_.Append( VM::IRBuilder::Jump(target_id));
  }

  void Visit(const IR::Inst::LoadI& inst) override{
    RegMap rd = reg_alloc_.IRReg( inst.RegDst() );
    reg_alloc_.GetRegLoadI(rd);
    byte_code_.Append( VM::IRBuilder::LoadI(rd.mreg_, inst.Value()));
  }

  void Visit(const IR::Inst::Load& inst) override{
    RegMap rd = reg_alloc_.IRReg(inst.RegDst());
    reg_alloc_.GetRegLoadI(rd);
    LoadCallBack(rd.mreg_, inst.Addr());
  }

  void Visit(const IR::Inst::LoadReg& inst) override{
    RegMap rd = reg_alloc_.IRReg(inst.RegDst());
    RegMap rb = reg_alloc_.IRReg(inst.RegSrc());
    reg_alloc_.GetRegLoadI(rd);
    reg_alloc_.GetRegRead(rb);
    byte_code_.Append( VM::IRBuilder::LoadB(rd.mreg_, rb.mreg_) );
  }

  void Visit(const IR::Inst::LoadRegOffs& inst) override{
    RegMap rd = reg_alloc_.IRReg(inst.RegDst());
    RegMap rb = reg_alloc_.IRReg(inst.RegSrc());
    reg_alloc_.GetRegLoadI(rd);
    reg_alloc_.GetRegRead(rb);
    byte_code_.Append( VM::IRBuilder::LoadB(rd.mreg_, rb.mreg_, inst.GetOffset().GetAddr() ));
  }

  void Visit(const IR::Inst::Store& inst) override{
    RegMap rs = reg_alloc_.IRReg    (inst.RegSrc());
    RegMap rd = reg_alloc_.IRMemAddr(inst.Addr());
    reg_alloc_.GetRegStore(rs, rd);
    StoreCallBack(rs.mreg_, inst.Addr());
  }

  void Visit(const IR::Inst::StoreReg& inst) override{
    RegMap rs = reg_alloc_.IRReg(inst.RegSrc1());
    RegMap rb = reg_alloc_.IRReg(inst.RegSrc2());
    reg_alloc_.GetRegStoreThroughPointer(rs);
    reg_alloc_.GetRegRead(rb);
    std::cout << "*** StoreB " << rs.mreg_ << ", " <<rb.mreg_ << "\n";
    byte_code_.Append( VM::IRBuilder::StoreB(rs.mreg_, rb.mreg_ ));
  }

  void Visit(const IR::Inst::Arith& inst) override{
    RegMap rd  = reg_alloc_.IRReg( inst.RegDst() );
    RegMap rs1 = reg_alloc_.IRReg( inst.RegSrc1() );
    RegMap rs2 = reg_alloc_.IRReg( inst.RegSrc2() );
    reg_alloc_.GetRegArith(rd, rs1, rs2);

    int op = VM::IRDefinition::SubtypesArithmetic::IR_ADD;
    byte_code_.Append( VM::IRBuilder::Arith(rs1.mreg_, rs2.mreg_, rd.mreg_, op));
  }

  void Visit(const IR::Inst::Logic& inst) override{
    RegMap rd  = reg_alloc_.IRReg( inst.RegDst() );
    RegMap rs1 = reg_alloc_.IRReg( inst.RegSrc1() );
    RegMap rs2 = reg_alloc_.IRReg( inst.RegSrc2() );
    reg_alloc_.GetRegArith(rd, rs1, rs2);

    int op;
    if(inst.Op() == IR::LogicType::kOr)  op = VM::IRDefinition::SubtypesLogic::IR_OR;
    if(inst.Op() == IR::LogicType::kAnd) op = VM::IRDefinition::SubtypesLogic::IR_AND;

    byte_code_.Append( VM::IRBuilder::Logic(rs1.mreg_, rs2.mreg_, rd.mreg_, op));
  }

  void Visit(const IR::Inst::Comparison& inst) override{
    RegMap rd  = reg_alloc_.IRReg( inst.RegDst() );
    RegMap rs1 = reg_alloc_.IRReg( inst.RegSrc1() );
    RegMap rs2 = reg_alloc_.IRReg( inst.RegSrc2() );
    reg_alloc_.GetRegArith(rd, rs1, rs2);

    int op;
    if(inst.Op() == IR::CompType::kLessThan) op = VM::IRDefinition::SubtypesComparison::IR_LST;
    if(inst.Op() == IR::CompType::kEqualTo ) op = VM::IRDefinition::SubtypesComparison::IR_EQT;

    byte_code_.Append( VM::IRBuilder::Comp(rs1.mreg_, rs2.mreg_, rd.mreg_, op));
  }

  void Visit(const IR::Inst::PtrElem& inst) override{
    const IR::MemAddr addr = inst.Addr();
    RegMap rd  = reg_alloc_.IRReg( inst.RegDst() );
    reg_alloc_.GetRegGetRetVal(rd);

    if(addr.GetLabel().IsRunTime()){
      MReg reg_base;
      if(addr.GetLabel().IsArp())     reg_base = reg_alloc_.MRegArpPtr();
      if(addr.GetLabel().IsThisPtr()) reg_base = reg_alloc_.MRegThisPtr();
      byte_code_.Append( VM::IRBuilder::Lea(rd.mreg_, reg_base, addr.GetOffset().GetAddr()));
    }
    if(addr.GetLabel().IsLinkTime())
      byte_code_.Append( VM::IRBuilder::LoadI(rd.mreg_, mem_alloc_.Remap(addr)));
  }

  void Visit(const IR::Inst::GetRetVal& inst) override{
    RegMap rd     = reg_alloc_.IRReg( inst.RegDst() );
    MReg rs_mreg  = reg_alloc_.MRegRetValue();
    reg_alloc_.GetRegGetRetVal(rd);
    byte_code_.Append( VM::IRBuilder::Move(rs_mreg, rd.mreg_));
  }

  void Visit(const IR::Inst::SetRetVal& inst) override{
    RegMap rs = reg_alloc_.IRReg(inst.RegSrc());
    reg_alloc_.GetRegRead(rs);
      byte_code_.Append( VM::IRBuilder::Move(rs.mreg_, reg_alloc_.MRegRetValue()));
  }

  void Visit(const IR::Inst::SetArg& inst) override{
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
    RegMap rd = reg_alloc_.IRReg( inst.RegDst() );
    rd.mreg_  = inst.Value ();
    reg_alloc_.SetRegGetArg(rd);
  }

  void Visit(const IR::Inst::Return& inst) override{
    IssuePreBasicBlocEnding();
    //Return emitted alongside epilogue
  }

  void Visit(const IR::Inst::ReturnMain& inst) override{
    IssuePreBasicBlocEnding();
    byte_code_.Append( VM::IRBuilder::Stop());
  }

  void Visit(const IR::Inst::Call& inst) override{
    IssueWriteBack();

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
private:

  void FuncPrologue(const Function& f){
    bool uses_arp = f.HasLocals() or f.HasArguments();
    //Push used registers
    if(f.IsMember())
      byte_code_.Append( VM::IRBuilder::Push( reg_alloc_.MRegThisPtr() ));

    //Set ARP
    if(uses_arp){
      byte_code_.Append( VM::IRBuilder::Push( reg_alloc_.MRegArpPtr() ));
      byte_code_.Append( VM::IRBuilder::Move( reg_alloc_.MRegStackPtr()
                                            , reg_alloc_.MRegArpPtr() ));
    }

    //Alloc locals in satck
    if(f.HasLocals()){
      const size_t  locals_size = f.LocalVars().Size();
      const MReg    rd = reg_alloc_.MRegStackPtr();
      byte_code_.Append( VM::IRBuilder::ArithI(rd
                                , locals_size
                                , VM::IRDefinition::SubtypesArithmetic::IR_SUB));
    }
  }

  void FuncEpilogue(const Function& f){
    bool uses_arp = f.HasLocals() or f.HasArguments();
    if(uses_arp){
      byte_code_.Append( VM::IRBuilder::Move( reg_alloc_.MRegArpPtr()
                                            , reg_alloc_.MRegStackPtr()));
      byte_code_.Append( VM::IRBuilder::Pop( reg_alloc_.MRegArpPtr() ));
    }
    if(f.IsMember())
      byte_code_.Append( VM::IRBuilder::Pop( reg_alloc_.MRegThisPtr() ));
    byte_code_.Append( VM::IRBuilder::Return());
  }

  void IssuePreBasicBlocEnding(){
    reg_alloc_.WriteBack();
  }

  void IssueWriteBack(){
    reg_alloc_.WriteBack();
  }

  //Other functions that do not handle sections of IR
  void ComputeMainDataSegment(){
    auto main = unit_.GetFunc("main");
    auto vars = main.LocalVars();
    auto size = vars.Size();
    byte_code_.SetMemUser(VM::MemChunk(0, size));
    for(const auto& it : vars){
      IR::MemAddr addr_ir = it.second;
      mem_alloc_.ComputeRemap(addr_ir);
    }
    //mem_alloc_.Remap();
  }

  void ComputeStackDataSegment(){
    VM::MemChunk m_last = VM::MemChunk::LastPage();
    VM::MemChunk m(m_last.low_ - TargetDefDAVM().StackReservedSpace(), m_last.low_ - 1);
    byte_code_.SetMemStack(m);
//     std::cout << "Setting stack : " << m.low_<< ", " << m.high_ << "\n";
  }

  void ComputeCompilerDataSegment(){
    auto main = unit_.GetFunc("__test");
    auto vars = main.LocalVars();
    auto size = vars.Size();
    byte_code_.SetMemUser(VM::MemChunk::LastPage());
    for(const auto& it : vars){
      IR::MemAddr addr_ir = it.second;
      mem_alloc_.ComputeRemap(addr_ir);
    }
    //mem_alloc_.Remap();
  }

  //This doesn't need regalloc in place
  void InitTest(){
    byte_code_.Append( VM::IRBuilder::LoadI(0, 1));
    byte_code_.Append( VM::IRBuilder::Store( 0, VM::MemChunk::LastWord()));
  }

  //Jumps refer to their IR address, and will have to be translated later
  //to the VM address where the original instruction was placed
  VM::Target  BackpatchId(const IR::Addr addr){return addr;}
  void JumpBackPatchTranslation(const IR::Addr addr, VM::Target target){
    jump_backpatch_translation_[addr] = target;
  }

  VM::Target JumpPatch(const IR::Addr addr){
//     std::cout << "Patch IR: " << addr << " to VM: " << jump_backpatch_translation_.at(addr) <<"\n";
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
