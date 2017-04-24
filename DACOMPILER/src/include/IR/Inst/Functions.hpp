namespace Compiler{
namespace IR{
namespace Inst{


struct GetRetVal : public Inst, public InstDst {
  GetRetVal(const Reg reg_dst) : InstDst(reg_dst){};
  virtual ~GetRetVal() = default;

  virtual std::string str() const noexcept{
    return "%" + std::to_string(dst_) + " = GetRetVal()";
  }
  
  void Accept(IRVisitor& v) override { v.Visit(*this); }
};


struct SetRetVal : public Inst, public InstSrc{
  SetRetVal(const Reg src) : InstSrc(src){};
  virtual ~SetRetVal() = default;

  virtual std::string str() const noexcept{
    return "SetRetVal( %"  + std::to_string(src_) + ")";
  }
  
  void Accept(IRVisitor& v) override { v.Visit(*this); }
};


struct SetArg : public Inst, public InstSrc{
  SetArg(const Reg src) : InstSrc(src){};
  virtual ~SetArg() = default;

  virtual std::string str() const noexcept{
    return "SetArg( %"  + std::to_string(src_) + ")";
  }
  
  void Accept(IRVisitor& v) override { v.Visit(*this); }
};


struct GetArg : public Inst, public InstDst, public InstVal{
  GetArg(const Reg dst, const NodeValue val) : InstDst(dst), InstVal(val){};
  virtual ~GetArg() = default;

  virtual std::string str() const noexcept{
    return "%" + std::to_string(dst_) + " = GetArg( arg" + std::to_string(val_) + " )";
  }
  
  void Accept(IRVisitor& v) override { v.Visit(*this); }
};



struct Return : public Inst{
  Return(): Inst(){};
  virtual ~Return() = default;

  virtual std::string str() const noexcept{
    return "Return";
  }
  
  void Accept(IRVisitor& v) override { v.Visit(*this); }
};

struct ReturnMain : public Inst{
  ReturnMain(): Inst(){};
  virtual ~ReturnMain() = default;

  virtual std::string str() const noexcept{
    return "ReturnMain";
  }
  
  void Accept(IRVisitor& v) override { v.Visit(*this); }
};

struct Call : public Inst, public InstAddress{
  Call(const MemAddr addr) : InstAddress(addr){};
  virtual ~Call() = default;

  virtual std::string str() const noexcept{
    return "call [" + addr_.str() + "]";
  }
  
  void Accept(IRVisitor& v) override { v.Visit(*this); }
};


}//end namespace Inst
}//end namespace IR
}//end namespace Compiler
