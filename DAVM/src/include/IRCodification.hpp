#pragma once
#include <stdint.h>
#include <string>
#include "BasicTypes.hpp"

namespace VM{
namespace IRCodification{

SubInst  DecodeClass (const Inst &instruction);
SubInst  DecodeType  (const Inst &instruction, const SubInst &inst_class);
SubInst  DecodeOpCode(const SubInst &inst_class, const SubInst &inst_type);

Inst CodeClass0(SubInst const& literal, const SubInst &type);
void DecodeClass0(const VM::Inst instruction, SubInst& literal);

Inst CodeClass1(const Reg &reg_dst, const Reg&reg_base,
                    SubInst const& literal, const SubInst &type);
void DecodeClass1(const VM::Inst instruction, Reg&reg_base,
                  Reg& reg_dst,SubInst& literal);

Inst CodeClass2(const Reg &reg_dst, SubInst const& literal,
                    const SubInst &type, const SubInst &subtype);
void DecodeClass2(const VM::Inst instruction, Reg &reg_dst
                 ,SubInst& literal, SubInst& subtype);

Inst CodeClass3(const Reg &reg_src1, const Reg &reg_src2
                   ,const Reg &reg_dst, const SubInst &type
                   ,const SubInst &subtype);
void DecodeClass3(const VM::Inst instruction, Reg &reg_src1
                 ,Reg &reg_src2, Reg &reg_dst, SubInst &subtype);

} //namespace IRCodification
}//end namespace VM
