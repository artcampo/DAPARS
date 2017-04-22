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

std::string ArithString(const uint32_t reg_src1, const uint32_t reg_src2
  , const uint32_t reg_dst, const uint32_t sub_type){
  using namespace std;
  using namespace SubtypesArithmetic;
  std::string s;
  bool three_regs = true;
  switch(sub_type){
    case IR_ADD: s = string("ADD, rs"); break;
    case IR_SUB: s = string("SUB, rs"); break;
    case IR_MUL: s = string("MUL, rs"); break;
    case IR_DIV: s = string("DIV, rs"); break;
    case IR_MOV: s = string("MOV, rs"); three_regs = false; break;
    default:     s = string(" - ERROR in print decode -"); break;
  }
  if(three_regs)
    s = s + to_string(reg_src1) + string(" rs") +
        to_string(reg_src2) + string(" rd") + to_string(reg_dst);
  else
    s = s + to_string(reg_src1) + string(" rd") + to_string(reg_dst);  
  return s;
}

std::string ArithString(const uint32_t reg_dst, const uint32_t literal, const uint32_t sub_type){
  using namespace std;
  using namespace SubtypesArithmetic;
  std::string s;
  bool three_regs = true;
  switch(sub_type){
    case IR_ADD: s = string("ADDI, rd"); break;
    case IR_SUB: s = string("SUBI, rd"); break;
    case IR_MUL: s = string("MULI, rd"); break;
    case IR_DIV: s = string("DIVI, rd"); break;
    case IR_MOV: s = string("MOVI, rd"); three_regs = false; break;
    default:     s = string(" - ERROR in print decode -"); break;
  }
  s += to_string(reg_dst) + string(" ") + to_string(literal);  
  return s;
}

std::string PrintInstruction(const uint32_t& instruction){
  const uint32_t current_class   = DecodeClass(instruction);
  const uint32_t current_type    = DecodeType(instruction, current_class);
  const uint32_t current_op_code = DecodeOpCode(current_class, current_type);
  uint32_t reg_src1, reg_src2, reg_dst, reg_base, sub_type, literal, op_offset;
  std::string s;

//   std::cout << "Op: " << current_op_code <<"\n";
  //Decode operans
  switch(current_class){
    case InstClassLit:
      DecodeClass0(instruction, literal);  break;
    case InstClassRegLit:
      DecodeClass1(instruction, reg_dst, reg_base, literal);  break;
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
    case IR_CALL:
      s = string("CALL: @") + to_string(literal);
      break;      
    case IR_NEW_VAR:
      s = string("NEWVAR: TypeId") + to_string(literal);
      break;
    case IR_RET:
      s = string("RETURN");
      break;      
      

    //Class 1
    case IR_LOADI:
      s = string("LOADI, r") + to_string(reg_dst) + " val: " +
          to_string(literal);
      break;
    case IR_STORE:  
      s = string("STORE, r") + to_string(reg_dst) + " to [@" +
          to_string(literal) + "]";
      break;
    case IR_LOADB:
      s = string("LOADB, r") + to_string(reg_dst) + " <- [@ " +
          "r" + to_string(reg_base) + " + " + to_string(literal) + "]";
      break;
    case IR_STOREB:  
      s = string("STORE, r") + to_string(reg_dst) + " -> [@" +
          "r" + to_string(reg_base) + " + " + to_string(literal) + "]";
      break;      

    //Class 2
    case IR_JMPC:
      s = string("JUMPIF ");
      if(sub_type == SubtypesJMPC::IR_TRUE ) s += string("true");
      if(sub_type == SubtypesJMPC::IR_FALSE) s += string("false");
      s += string(" to:") + to_string(literal);
      break;
    case IR_ARII:
      s = ArithString(reg_dst, literal, sub_type);
      break;      

    //Class 3
    case IR_ARI:
      s = ArithString(reg_src1, reg_src2, reg_dst, sub_type);
      break;//case IR_ARI
    case IR_CMP:
      using namespace SubtypesComparison;
      switch(sub_type){
        case IR_NOT: s = string("NOT, rs"); break;
        case IR_EQL: s = string("EQL, rs"); break;
        case IR_LST: s = string("LST, rs"); break;
        case IR_LTE: s = string("LTE, rs"); break;
        default:     s = string(" - ERROR in print decode -"); break;
      }
      s = s + to_string(reg_src1) + string(" rs") +
          to_string(reg_src2) + string(" rd") + to_string(reg_dst);
      break;//case IR_CMP
    default: s = string(" - ERROR in print decode -"); break;
  };

  return s;
}


}//namespace IRBuilder
}//end namespace VM
