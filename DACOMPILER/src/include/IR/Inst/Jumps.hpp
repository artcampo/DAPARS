namespace Compiler{
namespace IR{
namespace Inst{

struct Jump: public Inst, InstTarget{
  Jump() : InstTarget(0){};
  Jump(const Addr target) : InstTarget(target){};
  ~Jump() = default;

  virtual std::string str() const noexcept{
    return std::string("Jump to ") + std::to_string(target_);
  }
};

struct JumpCond : public Jump{
  JumpCond(const Reg cond) : JumpCond(cond, 0){};
  JumpCond(const Reg cond, const Addr target) : Jump(target), cond_(cond){};
  ~JumpCond() = default;

  virtual std::string str() const noexcept{
    return std::string("JumpCond ");
  }
  
protected:
  Reg cond_;
};

struct JumpCondFalse : public JumpCond{
  JumpCondFalse(const Reg cond) : JumpCond(cond){};
  JumpCondFalse(const Reg cond, const Addr target) : JumpCond(cond, target){};
  ~JumpCondFalse() = default;

  virtual std::string str() const noexcept{
    return std::string("JumpCondFalse %") + std::to_string(cond_)
         + std::string(" to:")  + std::to_string(target_);
  }
  
  void Accept(IRVisitor& v) override { v.Visit(*this); }
};

struct JumpCondTrue : public JumpCond{
  JumpCondTrue(const Reg cond) : JumpCond(cond){};
  JumpCondTrue(const Reg cond, const Addr target) : JumpCond(cond, target){};
  ~JumpCondTrue() = default;

  virtual std::string str() const noexcept{
    return std::string("JumpCondTrue %") + std::to_string(cond_)
         + std::string(" to:")  + std::to_string(target_);
  }
  
  void Accept(IRVisitor& v) override { v.Visit(*this); }
};

struct JumpIncond : public Jump{
  JumpIncond() : Jump(){};
  ~JumpIncond() = default;

  virtual std::string str() const noexcept{
    return std::string("JumpIncond") + std::to_string(target_);
  }
  
  void Accept(IRVisitor& v) override { v.Visit(*this); }
};

}//end namespace Inst
}//end namespace IR
}//end namespace Compiler
