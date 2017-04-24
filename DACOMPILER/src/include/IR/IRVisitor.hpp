#pragma once
#include <memory>

namespace Compiler{
namespace IR{

namespace Inst{
#include "IR/IRForwardDeclarations.inc"
}  
  
class IRVisitor{
public:
  virtual void Visit(Inst::Inst const& p)       = 0;  //TODO: delete: only for debug
  virtual void Visit(Inst::JumpCond const& p)   = 0;
  virtual void Visit(Inst::JumpIncond const& p) = 0;
  virtual void Visit(Inst::LoadI const& p)      = 0;
  virtual void Visit(Inst::Load const& p)       = 0;
  virtual void Visit(Inst::LoadReg const& p)    = 0;
  virtual void Visit(Inst::LoadRegOffs const& p)= 0;
  virtual void Visit(Inst::Store const& p)      = 0;
  virtual void Visit(Inst::StoreReg const& p)   = 0;
  virtual void Visit(Inst::Arith const& p)      = 0;
  virtual void Visit(Inst::Comparison const& p) = 0;
  virtual void Visit(Inst::PtrElem const& p)    = 0;
  virtual void Visit(Inst::GetRetVal const& p)  = 0;
  virtual void Visit(Inst::SetRetVal const& p)  = 0;
  virtual void Visit(Inst::SetArg const& p)     = 0;
  virtual void Visit(Inst::GetArg const& p)     = 0;
  virtual void Visit(Inst::Return const& p)     = 0;
  virtual void Visit(Inst::ReturnMain const& p) = 0;
  virtual void Visit(Inst::Call const& p)       = 0;
  
};  
  
}//end namespace IR
}//end namespace Compiler

