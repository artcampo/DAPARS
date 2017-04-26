#include "IR/IRStream.hpp"
#include "IR/IR.hpp"
#include "IR/IRSubtypes.hpp"
#include "Function.hpp"

namespace Compiler{
namespace IR{


void IRStream::AppendJumpIfTrue(const Reg cond){
  Append( JumpIfTrue(cond) );
}

void IRStream::AppendJumpIfFalse(const Reg cond){
  Append( JumpIfFalse(cond) );
}

void IRStream::AppendJumpIfFalse(const Reg cond, const Addr target){
  Append( JumpIfFalse(cond, target) );
}

void IRStream::AppendJumpInconditional(){
  Append( JumpInconditional() );
}

Reg IRStream::AppendLoadI(const NodeValue val){
  Append( LoadI(val) );
  return RegAssignedToPreviousInst();
}

Reg IRStream::AppendLoad(const MemAddr addr){
  Append( Load(addr) );
  return RegAssignedToPreviousInst();
}

Reg IRStream::AppendLoadReg(const Reg src){
  Append( LoadReg(src) );
  return RegAssignedToPreviousInst();
}

Reg IRStream::AppendLoadRegOffs(const Reg src, const Offset o){
  Append( LoadRegOffs(src, o) );
  return RegAssignedToPreviousInst();
}

void IRStream::AppendStore(const Reg src, const MemAddr addr){
  Append( Store(src, addr) );
}

void IRStream::AppendStoreReg(const Reg src1, const Reg src2){
  Append( StoreReg(src1, src2) );
}


Reg IRStream::AppendArith(const Reg src1, const Reg src2, const ArithType op){
  Append( Arith(src1, src2, op) );
  return RegAssignedToPreviousInst();
}

Reg IRStream::AppendPtrElem(const MemAddr addr){
  Append( PtrElem(addr) );
  return RegAssignedToPreviousInst();
}

Reg IRStream::AppendGetRetVal(){
  Append( GetRetVal() );
  return RegAssignedToPreviousInst();
}
void IRStream::AppendSetRetVal(const Reg src){
  Append( SetRetVal(src) );
}

void IRStream::AppendSetArg(const Reg src)    { Append( SetArg(src) );}
void IRStream::AppendCall(const MemAddr addr) { Append( Call(addr) );  };

Reg IRStream::AppendGetArg(const NodeValue position){ 
  Append( GetArg(position) );   
  return RegAssignedToPreviousInst();
}

void IRStream::AppendReturn(){ 
  Append( Return() ); 
  num_regs_used_ = UsedRegs();
}

void IRStream::AppendReturnMain(){ 
  Append( ReturnMain() );  
  num_regs_used_ = UsedRegs();
}

Reg IRStream::RegAssignedToPreviousInst() const{
  Inst::InstDst& i = dynamic_cast<Inst::InstDst&>(*(stream_[ stream_.size() - 1 ]));
  return i.RegDst();
}

void IRStream::Print() const noexcept{
 Addr a = 0;
 std::cout << entry_label_.str() << "\n";
 for(auto& it : stream_){ std::cout << a << ": "<< it->str() << "\n"; ++a;}
}

}//end namespace IR
}//end namespace Compiler
