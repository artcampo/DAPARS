#include "IRCodification.hpp"
#include "IRDefinition.hpp"
#include <iostream>

namespace IRCodification{

using namespace IRDefinition;
using namespace VM;

uint32_t  DecodeClass (uint32_t const &instruction){
  return instruction & kClassBitMask;
}


uint32_t  DecodeType (uint32_t const &instruction, uint32_t const &inst_class){
  if( inst_class == InstClassNoReg )
    return (instruction >> kClassNumBits) & kClass0BitMask;
  else if( inst_class == InstClassRegLit )
    return (instruction >> kClassNumBits) & kClass1BitMask;
  else if( inst_class == InstClassRegLitSub )
    return (instruction >> kClassNumBits) & kClass2BitMask;
 else //if( inst_class == InstClassRegRegRegSub )
    return (instruction >> kClassNumBits) & kClass3BitMask;
}


uint32_t  DecodeOpCode(uint32_t const &inst_class, uint32_t const &inst_type){
  return inst_class + (inst_type << kClassNumBits);
}

VM::Inst CodeClass0(uint32_t const& literal, const uint32_t &type){
  return type
    + (literal << kClass0OpcodeNumBits);
}

VM::Inst CodeClass1(uint32_t const &reg_dst, uint32_t const& literal,
                    uint32_t const &type){
  return type
    + (reg_dst << (kClass1OpcodeNumBits))
    + (literal << (kClass1OpcodeNumBits+ kRegisterNumBits));
}

VM::Inst CodeClass2(uint32_t const &reg_dst, uint32_t const& literal,
                    uint32_t const &type, uint32_t const &subtype){
  return type
    + (subtype  << (kClass2OpcodeNumBits))
    + (reg_dst <<  (kClass2OpcodeNumBits + kSubtypeNumBits))
    + (literal <<  (kClass2OpcodeNumBits + kSubtypeNumBits + kRegisterNumBits));
}

VM::Inst CodeClass3(uint32_t const &reg_src1, uint32_t const &reg_src2
                   ,uint32_t const &reg_dst, uint32_t const &type
                   ,uint32_t const &subtype){
  return type
    + (subtype  << (kClass3OpcodeNumBits))
    + (reg_src1 << (kClass3OpcodeNumBits + kSubtypeNumBits))
    + (reg_src2 << (kClass3OpcodeNumBits + kSubtypeNumBits
                    + kRegisterNumBits))
    + (reg_dst  << (kClass3OpcodeNumBits + kSubtypeNumBits
                    + kRegisterNumBits*2));
}


void DecodeClass0(const VM::Inst instruction, uint32_t& literal){
  literal = (instruction >> kClass0OpcodeNumBits)
            & kLiteraltMask;
}

void DecodeClass1(const VM::Inst instruction, uint32_t &reg_dst,
                uint32_t &literal){
  reg_dst = (instruction >> kClass1OpcodeNumBits)
            & kRegistertMask;
  literal = (instruction >> (kClass1OpcodeNumBits + kRegisterNumBits))
            & kLiteraltMask;
}


void DecodeClass2(const VM::Inst instruction, uint32_t& reg_dst
                 ,uint32_t& literal, uint32_t& subtype){
  subtype  = (instruction >> (kClass2OpcodeNumBits))
           & kSubtypeMask;
  reg_dst  = (instruction >> (kClass2OpcodeNumBits
                              + kSubtypeNumBits))
           & kRegistertMask;
  literal  = (instruction >> (kClass2OpcodeNumBits
                              + kSubtypeNumBits
                              + kRegisterNumBits))
           & kLiteraltMask;
}

void DecodeClass3(const VM::Inst instruction, uint32_t &reg_src1
                 ,uint32_t &reg_src2, uint32_t &reg_dst, uint32_t &subtype){

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

}; //namespace IRCodification
