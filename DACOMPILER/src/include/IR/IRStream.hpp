#pragma once
#include "Function.hpp"
#include "IR/IR.hpp"
#include "IR/IRBuilder.hpp"
#include "IR/Label.hpp"
#include "IR/MemAddr.hpp"
#include "AST/Node.hpp"
#include <vector>
#include <memory>

namespace Compiler{

// namespace AST{ class Function; }

namespace IR{

//Each stream is uniquely associated to a function
struct IRStream;

using PtrIRStream = std::unique_ptr<IRStream>;

struct IRStream : public IRBuilder{
  IRStream(const Label& entry_label, AST::Function& function)
    : entry_label_(entry_label), function_(function)
    , entry_mem_addr_(entry_label, Offset(0)){}

  MemAddr EntryMemAddr() const noexcept{ return entry_mem_addr_; }
  Addr NextAddress() const noexcept{ return stream_.size(); }

  Inst::Inst& GetInst(const Addr addr) const noexcept{ return *stream_[addr];}
  bool IsInstBeginBB(const Addr addr) const noexcept{ return is_begin_bb_[addr];}

  void  Print() const noexcept;
//   void  PrintLast() const noexcept;
  Reg   MaxRegUsed() const noexcept { return num_regs_used_;}
  const AST::Function& GetFunction()const noexcept { return function_;}

  //TODO: put these in different sub-class
  void AppendJumpIfTrue(const Reg cond);
  void AppendJumpIfFalse(const Reg cond);
  void AppendJumpIfFalse(const Reg cond, const Addr target);
  void AppendJumpInconditional(const Addr target);
  void AppendJumpInconditional();

  void AppendStore(const Reg src,const MemAddr addr);
  void AppendStoreReg(const Reg src1, const Reg src2);

  Reg  AppendLoadI(const NodeValue val);
  Reg  AppendLoad(const MemAddr addr);
  Reg  AppendLoadReg(const Reg src);
  Reg  AppendLoadRegOffs(const Reg src, const Offset o);
  Reg  AppendArith(const Reg src1, const Reg src2, const ArithType op);
  Reg  AppendLogic(const Reg src1, const Reg src2, const LogicType op);
  Reg  AppendComparison(const Reg src1, const Reg src2, const CompType op);

  Reg  AppendPtrElem(const MemAddr addr);

  Reg  AppendGetRetVal();
  void AppendSetRetVal(const Reg src);
  void AppendSetArg(const Reg src);
  Reg  AppendGetArg(const NodeValue position);
  void AppendReturn();
  void AppendReturnMain();
  void AppendCall(const MemAddr addr);


private:
  std::vector<Inst::PtrInst> stream_;
  std::vector<bool> is_begin_bb_;
  Label entry_label_;
  MemAddr entry_mem_addr_;
  AST::Function& function_;
  Reg num_regs_used_; //computed after appending return

  void Append(Inst::PtrInst inst){ stream_.push_back( std::move(inst)); }

  Reg RegAssignedToPreviousInst() const;

  void ComputeBeginBB();

public:
  using iterator = std::vector<Inst::PtrInst>::iterator;
  using const_iterator = std::vector<Inst::PtrInst>::const_iterator;

  iterator begin() { return stream_.begin(); }
  iterator end()   { return stream_.end(); }
  const_iterator begin()  const { return stream_.begin(); }
  const_iterator end()    const { return stream_.end(); }
  const_iterator cbegin() const { return stream_.cbegin(); }
  const_iterator cend()   const { return stream_.cend(); }

  friend class IRUnit;

};


}//end namespace IR
}//end namespace Compiler
