#include "IRCodification.hpp"
#include "IRDefinition.hpp"
#include "BasicTypes.hpp"
#include <iostream>

namespace VM{

namespace IRCodification{

using namespace IRDefinition;
using namespace VM;

//TODO: change const order!!
SubInst  DecodeClass (Inst const &instruction){
  return instruction & kClassBitMask;
}


SubInst  DecodeType (Inst const &instruction, SubInst const &inst_class){
  if( inst_class == InstClassLit )
    return (instruction >> kClassNumBits) & kClass0BitMask;
  else if( inst_class == InstClassRegLit )
    return (instruction >> kClassNumBits) & kClass1BitMask;
  else if( inst_class == InstClassRegLitSub )
    return (instruction >> kClassNumBits) & kClass2BitMask;
 else //if( inst_class == InstClassRegRegRegSub )
    return (instruction >> kClassNumBits) & kClass3BitMask;
}


SubInst  DecodeOpCode(SubInst const &inst_class, SubInst const &inst_type){
  return inst_class + (inst_type << kClassNumBits);
}

VM::Inst CodeClass0(const Word& literal, const SubInst &type){
  return type
    + (Code(literal) << kClass0OpcodeNumBits);
}

VM::Inst CodeClass1(Reg const &reg_dst, const Reg&reg_base,
                    const Word& literal, SubInst const &type){
  return type
    + (reg_dst  << (kClass1OpcodeNumBits))
    + (reg_base << (kClass1OpcodeNumBits + kRegisterNumBits))
    + (Code(literal)  << (kClass1OpcodeNumBits + kRegisterNumBits*2));
}

VM::Inst CodeClass2(Reg const &reg_dst, const Word& literal,
                    SubInst const &type, SubInst const &subtype){
  return type
    + (subtype  << (kClass2OpcodeNumBits))
    + (reg_dst <<  (kClass2OpcodeNumBits + kSubtypeNumBits))
    + (Code(literal) <<  (kClass2OpcodeNumBits + kSubtypeNumBits + kRegisterNumBits));
}

VM::Inst CodeClass3(Reg const &reg_src1, Reg const &reg_src2
                   ,Reg const &reg_dst, SubInst const &type
                   ,const SubInst& subtype){
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
