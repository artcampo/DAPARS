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

std::string PrintInstruction(const uint32_t& instruction){
  const uint32_t current_class   = DecodeClass(instruction);
  const uint32_t current_type    = DecodeType(instruction, current_class);
  const uint32_t current_op_code = DecodeOpCode(current_class, current_type);
  uint32_t reg_src1, reg_src2, reg_dst, sub_type, literal, op_offset;
  std::string s;

//   std::cout << "Op: " << current_op_code <<"\n";
  //Decode operans
  switch(current_class){
    case InstClassNoReg:
      DecodeClass0(instruction, literal);  break;
    case InstClassRegLit:
      DecodeClass1(instruction, reg_dst, literal);  break;
    case InstClassRegLitSub:
      DecodeClass2(instruction, reg_dst, literal, sub_type);  break;
    case InstClassRegRegRegSub:
      DecodeClass3(instruction, reg_src1, reg_src2, reg_dst, sub_type);break;

    default: break;
  }

  //Produce string
  using namespace std;
  switch(current_op_code){
    //Class 0
    case IR_NOP:
      s = string("NOP");
      break;
    case IR_STOP:
      s = string("STOP"); break;
    case IR_NOT_IMPL:
      s = string("[Not implemented]"); break;
    case IR_JMP:
      s = string("JUMP: @") + to_string(literal);
      break;
    case IR_NEW_VAR:
      s = string("NewVar: TypeId") + to_string(literal);
      break;

    //Class 1
    case IR_LOAD:
      s = string("Load, r:") + to_string(reg_dst) + " val: " +
          to_string(literal);
      break;
    case IR_STORE:  
      s = string("Store, r:") + to_string(reg_dst) + " to [@" +
          to_string(literal) + "]";
      break;

    //Class 2
    case IR_JMPC:
      s = string("jump if ");
      if(sub_type == SubtypesJMPC::IR_TRUE) s += string("true");
      if(sub_type == SubtypesJMPC::IR_FALSE) s += string("false");
      s += string("to:") + to_string(literal);
      break;

    //Class 3
    case IR_ARI:
      using namespace SubtypesArithmetic;
      switch(sub_type){
        case IR_ADD: s = string("ADD, rs1:"); break;
        case IR_SUB: s = string("SUB, rs1:"); break;
        case IR_MUL: s = string("MUL, rs1:"); break;
        case IR_DIV: s = string("DIV, rs1:"); break;
        default:     s = string(" - ERROR in print decode -"); break;
      }
      s = s + to_string(reg_src1) + string(" rs2: ") +
          to_string(reg_src2) + string(" rd:") + to_string(reg_dst);
      break;//case IR_ARI
    case IR_CMP:
      using namespace SubtypesComparison;
      switch(sub_type){
        case IR_NOT: s = string("NOT, rs1:"); break;
        case IR_EQL: s = string("EQL, rs1:"); break;
        case IR_LST: s = string("LST, rs1:"); break;
        case IR_LTE: s = string("LTE, rs1:"); break;
        default:     s = string(" - ERROR in print decode -"); break;
      }
      s = s + to_string(reg_src1) + string(" rs2: ") +
          to_string(reg_src2) + string(" rd:") + to_string(reg_dst);
      break;//case IR_CMP
    default: s = string(" - ERROR in print decode -"); break;
  };

  return s;
}


}//namespace IRBuilder
}//end namespace VM
