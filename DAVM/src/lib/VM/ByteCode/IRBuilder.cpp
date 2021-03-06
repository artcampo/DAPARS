#include "VM/ByteCode/IRBuilder.hpp"
#include "VM/ByteCode/IRDefinition.hpp"
#include "VM/ByteCode/IRCodification.hpp"
#include <string>
#include <iostream>
#include "VM/VMBasicTypes.hpp"

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
Inst Jump(const Target target){
  return CodeClass0(target, IR_JMP);
}

Inst Call(const Target target){
  return CodeClass0(target, IR_CALL);
}

Inst NewVar(const TypeId type_literal){
  return CodeClass0(type_literal, IR_NEW_VAR);
}

Inst Return(){
  return CodeClass0(0, IR_RET);
}
//TODO: remove constants such as 0!

////////////////////////////////////////////////////////////////////////
//CLASS 1
Inst Load(const Reg reg_dst, const SubInst literal){
  return CodeClass1(reg_dst, kIRUnusedReg, literal, IR_LOAD);
}

Inst LoadI(const Reg reg_dst, const SubInst literal){
  return CodeClass1(reg_dst, kIRUnusedReg, literal, IR_LOADI);
}

Inst LoadB(const Reg reg_dst, const Reg reg_base, const SubInst literal){
  return CodeClass1(reg_dst, reg_base, literal, IR_LOADB);
}

Inst LoadB(const Reg reg_dst, const Reg reg_base){
  return LoadB(reg_dst, reg_base, 0);
}

Inst Store(const Reg reg_src, const SubInst literal){
  return CodeClass1(reg_src, kIRUnusedReg, literal, IR_STORE);
}

Inst StoreB(const Reg reg_src, const Reg reg_base, const SubInst literal){
  return CodeClass1(reg_src, reg_base, literal, IR_STOREB);
}

Inst StoreB(const Reg reg_src, const Reg reg_base){
  return StoreB(reg_src, reg_base, 0);
}

Inst Pop (const Reg reg_dst){
  return CodeClass1(reg_dst, kIRUnusedReg, kIrUnusedLit, IR_POP);
}

Inst Push(const Reg reg_src){
  return CodeClass1(reg_src, kIRUnusedReg, kIrUnusedLit, IR_PUSH);
}

//Load Effective Address (Léa, also a song by Louise Attaque)
Inst Lea(const Reg reg_dst, const Reg reg_base, const SubInst literal){
  return CodeClass1(reg_dst, reg_base, literal, IR_LEA);
}



////////////////////////////////////////////////////////////////////////
//CLASS 2
Inst JumpIfTrue (const Reg reg_src, const Target target){
  return CodeClass2(reg_src, target, IR_JMPC, SubtypesJMPC::IR_TRUE);
}

Inst JumpIfFalse (const Reg reg_src, const Target target){
  return CodeClass2(reg_src, target, IR_JMPC, SubtypesJMPC::IR_FALSE);
}

Inst ArithI(const Reg reg_dst, const SubInst literal, const SubInst op){
  return CodeClass2(reg_dst, literal, IR_ARII, op);
}

////////////////////////////////////////////////////////////////////////
//CLASS 3
Inst Arith(const Reg reg_src1, const Reg reg_src2,
               const Reg reg_dst, const SubInst op){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, op);
}

Inst Comp(const Reg reg_src1, const Reg reg_src2,
               const Reg reg_dst, const SubInst op){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_CMP, op);
}

Inst Move (const Reg reg_src, const Reg reg_dst){
  return CodeClass3(reg_src, IR_REG0, reg_dst, IR_ARI, SubtypesArithmetic::IR_MOV);
}

Inst Logic(const Reg reg_src1, const Reg reg_src2,
               const Reg reg_dst, const SubInst op){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_LOGIC, op);
}


///////////////////////////////
/*
//TODO
Inst NewTypeId(const Reg reg_src1, const Reg reg_src2){
  return IR_NOT_IMPL;
}
//end TODO
*/
///////////////////////////////



namespace IRBuilderAPI{
using namespace SubtypesArithmetic;

Inst Add(const Reg reg_src1, const Reg reg_src2,
             const Reg reg_dst){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, IR_ADD);
}

Inst Sub(const Reg reg_src1, const Reg reg_src2,
             const Reg reg_dst){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, IR_SUB);
}

Inst Mul(const Reg reg_src1, const Reg reg_src2,
             const Reg reg_dst){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, IR_MUL);
}


Inst Div(const Reg reg_src1, const Reg reg_src2,
             const Reg reg_dst){
  return CodeClass3(reg_src1, reg_src2, reg_dst, IR_ARI, IR_DIV);
}
}; //namespace IRBuilderAPI

Inst Stop(){
  return IR_STOP;
}

bool  IsJump(const Inst inst, Target& target){
  if((inst & kClass0OpcodeBitMask) == IR_JMP){
    target = Target(DecodeClass0Literal(inst));
    return true;
  }
  if((inst & kClass2OpcodeBitMask) == IR_JMPC){
    target = Target(DecodeClass2Literal(inst));
    return true;
  }
  return false;
}

bool  IsCall(const Inst inst, Target& target){
  if((inst & kClass0OpcodeBitMask) == IR_CALL){
    target = Target(DecodeClass0Literal(inst));
    return true;
  }
  return false;
}

void PatchJump(Inst& inst, const Target target){
  if((inst & kClass0OpcodeBitMask) == IR_JMP)
    inst = CodeClass0(target, IR_JMP);

  if((inst & kClass2OpcodeBitMask) == IR_JMPC){
    Reg r; Word old_word; SubInst subt;
    DecodeClass2(inst, r, old_word, subt);
    inst = CodeClass2(r, target, IR_JMPC, subt);
  }
}

void PatchCall(Inst& inst, const Target target){
  inst = CodeClass0(target, IR_CALL);
}

}//namespace IRBuilder
}//end namespace VM
