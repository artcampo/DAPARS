#pragma once
#include "IR/IR.hpp"
#include "IR/IRBuilder.hpp"
#include "IR/Label.hpp"
#include "AST/Node.hpp"
#include <vector>
#include <memory>

namespace Compiler{
namespace IR{

//Each stream is uniquely associated to a function
struct IRStream;
using PtrIRStream = std::unique_ptr<IRStream>;

struct IRStream : public IRBuilder{
  IRStream(const Label& entry_label) : entry_label_(entry_label){}

  Addr NextAddress() const noexcept{ return stream_.size(); }

  Inst::Inst& GetInst(const Addr addr) const noexcept{ return *stream_[addr];}

  void AppendJumpIfTrue(const Reg cond);
  void AppendJumpIfFalse(const Reg cond);
  void AppendJumpIfFalse(const Reg cond, const Addr target);
  void AppendJumpInconditional();

  void AppendStore(const Reg src,const MemAddr addr);
  void AppendStoreReg(const Reg src1, const Reg src2);

  Reg AppendLoadI(const NodeValue val);
  Reg AppendLoad(const MemAddr addr);
  Reg AppendLoadReg(const Reg src);
  Reg AppendLoadRegOffs(const Reg src, const Offset o);
  Reg AppendArith(const Reg src1, const Reg src2, const ArithType op);
  Reg AppendPtrElem(const MemAddr addr);

  Reg AppendGetRetVal();
  void AppendSetRetVal(const Reg src);
  void AppendSetPar(const Reg src);
  void AppendReturn();
  void AppendReturnMain();
  void AppendCall(const MemAddr addr);


  void Print() const noexcept;
  
  
private:
  std::vector<Inst::PtrInst> stream_;
  Label entry_label_;

  void Append(Inst::PtrInst inst){ stream_.push_back( std::move(inst)); }

  Reg RegAssignedToPreviousInst() const;

public:  
  using iterator = std::vector<Inst::PtrInst>::iterator;
  using const_iterator = std::vector<Inst::PtrInst>::const_iterator;

  iterator begin() { return stream_.begin(); }
  iterator end()   { return stream_.end(); }
  const_iterator begin()  const { return stream_.begin(); }
  const_iterator end()    const { return stream_.end(); }
  const_iterator cbegin() const { return stream_.cbegin(); }
  const_iterator cend()   const { return stream_.cend(); }    
  
};


}//end namespace IR
}//end namespace Compiler
