#pragma once
#include <stdint.h>
#include <string>
#include "BasicTypes.hpp"

namespace IRCodification{

uint32_t  DecodeClass (const uint32_t &instruction);
uint32_t  DecodeType  (const uint32_t &instruction, const uint32_t &inst_class);
uint32_t  DecodeOpCode(const uint32_t &inst_class, const uint32_t &inst_type);

VM::Inst CodeClass0(uint32_t const& literal, const uint32_t &type);
void DecodeClass0(const VM::Inst instruction, uint32_t& literal);

VM::Inst CodeClass1(const uint32_t &reg_dst, uint32_t const& literal,
                    const uint32_t &type);
void DecodeClass1(const VM::Inst instruction, uint32_t& reg_dst,
                uint32_t& literal);

VM::Inst CodeClass2(const uint32_t &reg_dst, uint32_t const& literal,
                    const uint32_t &type, const uint32_t &subtype);
void DecodeClass2(const VM::Inst instruction, uint32_t &reg_dst
                 ,uint32_t& literal, uint32_t& subtype);

VM::Inst CodeClass3(const uint32_t &reg_src1, const uint32_t &reg_src2
                   ,const uint32_t &reg_dst, const uint32_t &type
                   ,const uint32_t &subtype);
void DecodeClass3(const VM::Inst instruction, uint32_t &reg_src1
                 ,uint32_t &reg_src2, uint32_t &reg_dst, uint32_t &subtype);

}; //namespace IRCodification

