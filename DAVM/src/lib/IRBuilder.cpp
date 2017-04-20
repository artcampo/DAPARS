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
                + kRegisterNumBits
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

////////////////////////////////////////////////////////////////////////
//CLASS 1
uint32_t Load(const uint32_t&reg_dst, const uint32_t& literal){
  return CodeClass1(reg_dst, literal, IR_LOAD);
}

uint32_t Store(const Reg &reg_src, const uint32_t& literal){
  return CodeClass1(reg_src, literal, IR_STORE);
}


////////////////////////////////////////////////////////////////////////
//CLASS 2
Inst JumpIfTrue (const Reg&reg_src1, const Target& target){
  return CodeClass2(0, target, IR_JMPC, SubtypesJMPC::IR_TRUE);
}

Inst JumpIfFalse (const Reg&reg_src1, const Target& target){
  return CodeClass2(0, target, IR_JMPC, SubtypesJMPC::IR_FALSE);
}

////////////////////////////////////////////////////////////////////////
//CLASS 3
uint32_t Arith(const uint32_t&reg_src1, const uint32_t&reg_src2,
               const uint32_t&reg_dst, const uint32_t&op){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, op);
}

uint32_t Comp(const uint32_t&reg_src1, const uint32_t&reg_src2,
               const uint32_t&reg_dst, const uint32_t&op){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_CMP, op);
}

uint32_t Move (const Reg &reg_src, const Reg &reg_dst){
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

uint32_t Add(const uint32_t&reg_src1, const uint32_t&reg_src2,
             const uint32_t&reg_dst){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, IR_ADD);
}

uint32_t Sub(const uint32_t&reg_src1, const uint32_t&reg_src2,
             const uint32_t&reg_dst){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, IR_SUB);
}

uint32_t Mul(const uint32_t&reg_src1, const uint32_t&reg_src2,
             const uint32_t&reg_dst){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, IR_MUL);
}


uint32_t Div(const uint32_t&reg_src1, const uint32_t&reg_src2,
             const uint32_t&reg_dst){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, IR_DIV);
}
}; //namespace IRBuilderAPI

uint32_t Stop(){
  return IR_STOP;
}




}//namespace IRBuilder
}//end namespace VM
