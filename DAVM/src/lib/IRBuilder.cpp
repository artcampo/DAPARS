#include "IRBuilder.hpp"
#include "IRDefinition.hpp"
#include "IRCodification.hpp"
#include <string>
#include <iostream>
#include "BasicTypes.hpp"

namespace VM{

namespace IRBuilder{

using namespace IRDefinition;
using namespace IRCodification;
using namespace VM;

bool checkIRCodification(){
  bool wellFormed = true;

  //Class 0
  wellFormed &= ( kClassNumBits
                + kClass0InstTypeNumBits
                + kRegisterNumBits
                + kLiteralNumBits) <= 32;
 //Class 1
  wellFormed &= ( kClassNumBits
                + kClass1InstTypeNumBits
                + kRegisterNumBits*2
                + kLiteralNumBits) <= 32;

 //Class 2
  wellFormed &= ( kClassNumBits
                + kClass2InstTypeNumBits
                + kRegisterNumBits
                + kLiteralNumBits
                + kSubtypeNumBits) <= 32;
 //Class 3
  wellFormed &= ( kClassNumBits
                + kClass3InstTypeNumBits
                + kRegisterNumBits*3
                + kSubtypeNumBits) <= 32;

  return wellFormed;
}




////////////////////////////////////////////////////////////////////////
//CLASS 0
Inst Jump(const Target& target){
  return CodeClass0(target, IR_JMP);
}

Inst Call(const Target& target){
  return CodeClass0(target, IR_CALL);
}

Inst NewVar(const TypeId &type_literal){
  return CodeClass0(type_literal, IR_NEW_VAR);
}

Inst Return(){
  return CodeClass0(0, IR_RET);
}
//TODO: remove constants such as 0!

////////////////////////////////////////////////////////////////////////
//CLASS 1
Inst LoadI(const uint32_t&reg_dst, const uint32_t& literal){
  return CodeClass1(reg_dst, kIRUnusedReg, literal, IR_LOADI);
}

Inst LoadB(const uint32_t&reg_dst, const uint32_t&reg_base, const uint32_t& literal){
  return CodeClass1(reg_dst, reg_base, literal, IR_LOADB);
}

Inst Store(const Reg &reg_src, const uint32_t& literal){
  return CodeClass1(reg_src, kIRUnusedReg, literal, IR_STORE);
}

Inst StoreB(const Reg &reg_src, const uint32_t&reg_base, const uint32_t& literal){
  return CodeClass1(reg_src, reg_base, literal, IR_STOREB);
}

Inst Pop (const Reg &reg_dst){
  return CodeClass1(reg_dst, kIRUnusedReg, kIrUnusedLit, IR_POP);
}

Inst Push(const Reg &reg_src){
  return CodeClass1(reg_src, kIRUnusedReg, kIrUnusedLit, IR_PUSH);
}


////////////////////////////////////////////////////////////////////////
//CLASS 2
Inst JumpIfTrue (const Reg&reg_src1, const Target& target){
  return CodeClass2(0, target, IR_JMPC, SubtypesJMPC::IR_TRUE);
}

Inst JumpIfFalse (const Reg&reg_src1, const Target& target){
  return CodeClass2(0, target, IR_JMPC, SubtypesJMPC::IR_FALSE);
}

Inst ArithI(const Reg &reg_dst, const uint32_t& literal, const uint32_t& op){
  return CodeClass2(reg_dst, literal, IR_ARII, op);
}

////////////////////////////////////////////////////////////////////////
//CLASS 3
Inst Arith(const uint32_t&reg_src1, const uint32_t&reg_src2,
               const uint32_t&reg_dst, const uint32_t&op){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, op);
}

Inst Comp(const uint32_t&reg_src1, const uint32_t&reg_src2,
               const uint32_t&reg_dst, const uint32_t&op){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_CMP, op);
}

Inst Move (const Reg &reg_src, const Reg &reg_dst){
  return CodeClass3(reg_src, IR_REG0, reg_dst, IR_ARI, SubtypesArithmetic::IR_MOV);
}


///////////////////////////////
//TODO
Inst NewTypeId(const Reg&reg_src1, const Reg&reg_src2){
  return IR_NOT_IMPL;
}
//end TODO
///////////////////////////////



void PatchJump(Inst& inst, const Target& target){
  if((inst & kClass0OpcodeBitMask) == IR_JMP)
    inst = CodeClass0(target, IR_JMP);

  if((inst & kClass2OpcodeBitMask) == IR_JMPC){
    uint32_t subt = (inst >> kClass2OpcodeNumBits) & kLiteraltMask;
    inst = CodeClass2(0, target, IR_JMPC, subt);
  }
}

namespace IRBuilderAPI{
using namespace SubtypesArithmetic;

Inst Add(const uint32_t&reg_src1, const uint32_t&reg_src2,
             const uint32_t&reg_dst){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, IR_ADD);
}

Inst Sub(const uint32_t&reg_src1, const uint32_t&reg_src2,
             const uint32_t&reg_dst){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, IR_SUB);
}

Inst Mul(const uint32_t&reg_src1, const uint32_t&reg_src2,
             const uint32_t&reg_dst){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, IR_MUL);
}


Inst Div(const uint32_t&reg_src1, const uint32_t&reg_src2,
             const uint32_t&reg_dst){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, IR_DIV);
}
}; //namespace IRBuilderAPI

uint32_t Stop(){
  return IR_STOP;
}




}//namespace IRBuilder
}//end namespace VM
