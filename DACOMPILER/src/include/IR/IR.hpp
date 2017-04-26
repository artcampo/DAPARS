#pragma once
#include <string>
#include <memory>
#include <cstddef>
#include "IR/IRSubtypes.hpp"
#include "IR/Label.hpp"
#include "IR/Offset.hpp"
#include "IR/MemAddr.hpp"
#include "IR/IRVisitor.hpp"
#include "AST/ASTNodes.hpp"

namespace Compiler{
namespace IR{
namespace Inst{

#include "IR/IRForwardDeclarations.inc"

//Interface
struct Inst{
  Inst(){};
  virtual ~Inst() = default;

  virtual std::string str() const noexcept = 0;
  
  //non-conventional, but while developing I want to see unimplemented instructions
  virtual void Accept(IRVisitor& v){ v.Visit(*this);};
};

//Policies 
struct InstAddress{
  InstAddress(const MemAddr addr)
    :addr_(addr){}
  ~InstAddress() = default;
  MemAddr Addr() const noexcept{ return addr_;}
protected:
  MemAddr addr_;
};

struct InstSrc{
  InstSrc(const Reg src) : src_(src){}
  ~InstSrc() = default;
  Reg RegSrc() const noexcept { return src_;}
protected:
  Reg src_;
};

struct InstSrcSrc{
  InstSrcSrc(const Reg src1, const Reg src2) : src1_(src1), src2_(src2){}
  ~InstSrcSrc() = default;
  Reg RegSrc1() const noexcept { return src1_;}
  Reg RegSrc2() const noexcept { return src2_;}
protected:
  Reg src1_;
  Reg src2_;
};

struct InstDst{
  InstDst(const Reg reg_dst) : dst_(reg_dst){};
  ~InstDst() = default;

  Reg RegDst() const noexcept { return dst_;}
protected:
  Reg dst_;
};

struct InstVal{
  InstVal(const NodeValue val) : val_(val){};
  ~InstVal() = default;

  NodeValue Value() const noexcept { return val_;}
protected:
  NodeValue val_;
};

struct InstOffset{
  InstOffset(const Offset offset) : offset_(offset){};
  ~InstOffset() = default;

  const Offset GetOffset() const noexcept { return offset_;}
protected:
   Offset offset_;
};

struct InstTarget{
  InstTarget() : target_(kAddrUninitialized){};
  InstTarget(const Addr target) : target_(target){};
  ~InstTarget() = default;

  void PatchJump(const Addr target){ target_ = target;}
  
protected:
  Addr target_;
};


//Extended interfaces
struct BinaryOp : public Inst, public InstDst, public InstSrcSrc{
  BinaryOp(const Reg reg_dst, const Reg src1, const Reg src2)
  : InstDst(reg_dst), InstSrcSrc(src1, src2){};
  virtual ~BinaryOp() = default;

  virtual std::string str() const noexcept = 0;
  
  
};

struct UnaryOp : public Inst, public InstDst, public InstSrc{
  UnaryOp(const Reg reg_dst, const Reg src1)
  : InstDst(reg_dst), InstSrc(src1){};
  virtual ~UnaryOp() = default;

  virtual std::string str() const noexcept = 0;
};


}//end namespace Inst
}//end namespace IR
}//end namespace Compiler

#include "Inst/Jumps.hpp"
#include "Inst/MemAccess.hpp"
#include "Inst/Ops.hpp"
#include "Inst/Functions.hpp"
