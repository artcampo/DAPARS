#include "VM/ByteCode/IRCodification.hpp"
#include "VM/ByteCode/IRDefinition.hpp"
#include "VM/VMBasicTypes.hpp"
#include <iostream>

namespace VM{

namespace IRCodification{

using namespace IRDefinition;
using namespace VM;

//TODO: change const order!!
SubInst  DecodeClass (const Inst instruction){
  return instruction & kClassBitMask;
}


SubInst  DecodeType (const Inst instruction, const SubInst inst_class){
  if( inst_class == InstClassLit )
    return (instruction >> kClassNumBits) & kClass0BitMask;
  else if( inst_class == InstClassRegLit )
    return (instruction >> kClassNumBits) & kClass1BitMask;
  else if( inst_class == InstClassRegLitSub )
    return (instruction >> kClassNumBits) & kClass2BitMask;
 else //if( inst_class == InstClassRegRegRegSub )
    return (instruction >> kClassNumBits) & kClass3BitMask;
}


SubInst  DecodeOpCode(const SubInst inst_class, const SubInst inst_type){
  return inst_class + (inst_type << kClassNumBits);
}

VM::Inst CodeClass0(const Word literal, const SubInst type){
  return type
    + (Code(literal) << kClass0OpcodeNumBits);
}

VM::Inst CodeClass1(const Reg reg_dst, const Reg reg_base,
                    const Word literal, const SubInst type){
  return type
    + (reg_dst  << (kClass1OpcodeNumBits))
    + (reg_base << (kClass1OpcodeNumBits + kRegisterNumBits))
    + (Code(literal)  << (kClass1OpcodeNumBits + kRegisterNumBits*2));
}

VM::Inst CodeClass2(const Reg reg_dst, const Word literal,
                    const SubInst type, const SubInst subtype){
  return type
    + (subtype  << (kClass2OpcodeNumBits))
    + (reg_dst <<  (kClass2OpcodeNumBits + kSubtypeNumBits))
    + (Code(literal) <<  (kClass2OpcodeNumBits + kSubtypeNumBits + kRegisterNumBits));
}

VM::Inst CodeClass3(const Reg reg_src1, const Reg reg_src2
                   ,const Reg reg_dst, const SubInst type
                   ,const SubInst subtype){
  return type
    + (subtype  << (kClass3OpcodeNumBits))
    + (reg_src1 << (kClass3OpcodeNumBits + kSubtypeNumBits))
    + (reg_src2 << (kClass3OpcodeNumBits + kSubtypeNumBits
                    + kRegisterNumBits))
    + (reg_dst  << (kClass3OpcodeNumBits + kSubtypeNumBits
                    + kRegisterNumBits*2));
}

void DecodeClass0(const VM::Inst instruction, Word& literal){
  literal = Decode(
              (instruction >> kClass0OpcodeNumBits)
              & kLiteraltMask);
}

void DecodeClass1(const VM::Inst instruction, Reg&reg_dst,
                  Reg &reg_base, Word& literal){
  reg_dst = (instruction >> kClass1OpcodeNumBits)
            & kRegistertMask;
  reg_base = (instruction >> (kClass1OpcodeNumBits + kRegisterNumBits))
            & kRegistertMask;
  literal = Decode(
              (instruction >> (kClass1OpcodeNumBits + kRegisterNumBits*2))
              & kLiteraltMask);
}


void DecodeClass2(const VM::Inst instruction, Reg& reg_dst
                 ,Word& literal, SubInst& subtype){
  subtype  = (instruction >> (kClass2OpcodeNumBits))
           & kSubtypeMask;
  reg_dst  = (instruction >> (kClass2OpcodeNumBits
                              + kSubtypeNumBits))
           & kRegistertMask;
  literal  = Decode(
              (instruction >> (kClass2OpcodeNumBits
                              + kSubtypeNumBits
                              + kRegisterNumBits))
              & kLiteraltMask);
}

void DecodeClass3(const VM::Inst instruction, Reg &reg_src1
                 ,Reg &reg_src2, Reg &reg_dst, SubInst &subtype){

  subtype  = (instruction >> (kClass3OpcodeNumBits))
           & kSubtypeMask;
  reg_src1 = (instruction >> (kClass3OpcodeNumBits
                              + kSubtypeNumBits))
           & kRegistertMask;
  reg_src2 = (instruction >> (kClass3OpcodeNumBits
                              + kSubtypeNumBits + kRegisterNumBits))
           & kRegistertMask;
  reg_dst  = (instruction >> (kClass3OpcodeNumBits
                              + kSubtypeNumBits + kRegisterNumBits*2))
           & kRegistertMask;
}

//extract subtype
SubInst DecodeClass3Subtype(const VM::Inst instruction){
  SubInst subtype; Reg r;
  DecodeClass3(instruction, r, r, r, subtype);
  return subtype;
}

SubInst DecodeClass2Subtype(const VM::Inst instruction){
  SubInst subtype; Reg r; Word l;
  DecodeClass2(instruction, r, l, subtype);
  return subtype;
}

Word DecodeClass2Literal(const VM::Inst instruction){
  SubInst subtype; Reg r; Word l;
  DecodeClass2(instruction, r, l, subtype);
  return l;
}

Word DecodeClass0Literal(const VM::Inst instruction){
  Word l;
  DecodeClass0(instruction, l);
  return l;
}

SubInst Code(const Word literal){
  if(literal < 0){
    SubInst l = SubInst((-literal)) & kLiteralUnsignedMask;
    l += 1 << kLiteralSignPosition;
    return l;
  }else{
    SubInst l = SubInst(literal & kLiteralUnsignedMask);
    return l;
  }
}

Word Decode(const SubInst literal){
  SubInst sign = (literal >> kLiteralSignPosition) & 1;
  if(not sign)  return  Word(literal & kLiteralUnsignedMask);
  else          return -Word(literal & kLiteralUnsignedMask);
}


}//namespace IRCodification
}//end namespace VM
