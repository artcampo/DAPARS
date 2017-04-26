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
  JumpCond(const JumpCondType t, const Reg cond) : JumpCond(t, cond, 0){};
  JumpCond(const JumpCondType t, const Reg cond, const Addr target) 
    : jump_type_(t),Jump(target), reg_cond_(cond){};
  ~JumpCond() = default;

  virtual std::string str() const noexcept{
    return std::string("JumpCond ");
  }
  
  JumpCondType  GetJumpCondType() const noexcept{ return jump_type_; }
  Reg           RegCond() const noexcept{ return reg_cond_; }
  
protected:
  Reg           reg_cond_;
  JumpCondType  jump_type_;
  
};

struct JumpCondFalse : public JumpCond{
  JumpCondFalse(const Reg cond) : JumpCond(JumpCondType::kFalse, cond){};
  JumpCondFalse(const Reg cond, const Addr target) : JumpCond(JumpCondType::kFalse, cond, target){};
  ~JumpCondFalse() = default;

  virtual std::string str() const noexcept{
    return std::string("JumpCondFalse %") + std::to_string(reg_cond_)
         + std::string(" to:")  + std::to_string(target_);
  }
  
  void Accept(IRVisitor& v) override { v.Visit(*this); }
};

struct JumpCondTrue : public JumpCond{
  JumpCondTrue(const Reg cond) : JumpCond(JumpCondType::kTrue, cond){};
  JumpCondTrue(const Reg cond, const Addr target) : JumpCond(JumpCondType::kTrue, cond, target){};
  ~JumpCondTrue() = default;

  virtual std::string str() const noexcept{
    return std::string("JumpCondTrue %") + std::to_string(reg_cond_)
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
