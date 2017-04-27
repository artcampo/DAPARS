#pragma once
#include "IRCodification.hpp"
#include "BasicTypes.hpp"

namespace VM{
namespace IRBuilder{


bool        checkIRCodification();
std::string PrintInstruction(const Inst& instruction);

//if it is, return its target
bool  IsJump(const Inst instruction, Target& target);
bool  IsCall(const Inst instruction, Target& target);

void PatchJump(Inst& inst, const Target target);
void PatchCall(Inst& inst, const Target target);

Inst Stop();
Inst LoadI (const Reg &reg_dst, const SubInst& literal);  //TODO
Inst Store(const Reg &reg_src, const SubInst& literal);  //TODO
Inst Move (const Reg &reg_src, const Reg &reg_dst);       //TODO
Inst Call(const Target& target);//TODO
Inst Return();
Inst LoadB(const Reg&reg_dst, const Reg&reg_base, const SubInst& literal);  //TODO
Inst StoreB(const Reg &reg_src, const Reg&reg_base, const SubInst& literal); //TODO
Inst ArithI(const Reg &reg_dst, const Reg& literal, const SubInst& op);
Inst Pop (const Reg &reg_dst);  //TODO
Inst Push(const Reg &reg_src);  //TODO

//
Inst Arith(const Reg &reg_src1, const Reg &reg_src2,
               const Reg &reg_dst, const SubInst& op);

Inst Comp(const Reg &reg_src1, const Reg &reg_src2,
               const Reg &reg_dst, const SubInst& op);


Inst Jump       (const Target& target);
Inst JumpIfTrue (const Reg& reg_src1, const Target& target);
Inst JumpIfFalse(const Reg& reg_src1, const Target& target);


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

