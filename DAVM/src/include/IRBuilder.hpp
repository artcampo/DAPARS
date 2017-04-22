#pragma once
#include "IRCodification.hpp"
//TODO:
#include "BasicTypes.hpp"
namespace VM{

namespace IRBuilder{

using VM::Addr;
using VM::Inst;
using VM::Target;
using VM::Reg;

bool        checkIRCodification();
std::string PrintInstruction(const uint32_t& instruction);



Inst Stop();
Inst LoadI (const Reg &reg_dst, const uint32_t& literal);  //TODO
Inst Store(const Reg &reg_src, const uint32_t& literal);  //TODO
Inst Move (const Reg &reg_src, const Reg &reg_dst);       //TODO
Inst Call(const Target& target);//TODO
Inst Return();
Inst LoadB(const uint32_t&reg_dst, const uint32_t&reg_base, const uint32_t& literal);  //TODO
Inst StoreB(const Reg &reg_src, const uint32_t&reg_base, const uint32_t& literal); //TODO
Inst ArithI(const Reg &reg_dst, const uint32_t& literal, const uint32_t& op);


//
Inst Arith(const Reg &reg_src1, const Reg &reg_src2,
               const Reg &reg_dst, const uint32_t& op);

Inst Comp(const Reg &reg_src1, const Reg &reg_src2,
               const Reg &reg_dst, const uint32_t& op);




Inst Jump       (const Target& target);
Inst JumpIfTrue (const Reg& reg_src1, const Target& target);
Inst JumpIfFalse(const Reg& reg_src1, const Target& target);

void PatchJump(Inst& inst, const Target& target);

//Creates new variable, reg_src1 = TypeId
Inst NewVar(const Reg&reg_src1);

//Register a new typeeId
// reg_src1, typeId
// reg_src2, size of type
Inst NewTypeId(const Reg&reg_src1, const Reg&reg_src2);


namespace IRBuilderAPI{
// These should not be used by compiler, only direct API
Inst Add(const Reg &reg_src1, const Reg &reg_src2,
             const Reg &reg_dst);
Inst Sub(const Reg &reg_src1, const Reg &reg_src2,
             const Reg &reg_dst);
Inst Mul(const Reg &reg_src1, const Reg &reg_src2,
             const Reg &reg_dst);
Inst Div(const Reg &reg_src1, const Reg &reg_src2,
             const Reg &reg_dst);

} //namespace IRBuilderAPI

} //namespace IRBuilder
}//end namespace VM

