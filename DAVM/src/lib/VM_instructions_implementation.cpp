#include "VM.hpp"
#include "IRDefinition.hpp"
#include "IRCodification.hpp"
#include "IRBuilder.hpp"
#include "RegisterBank.hpp"
#include <stdint.h>
#include <iostream>

namespace VM{

using namespace IRCodification;
using namespace IRBuilder;

void VirtualMachine::InstLoad(const Reg &reg_dst, const SubInst &literal){
  std::cout << "LOAD R"<< reg_dst <<"="<<literal<<"\n";
  process_->execution_context_.registers_.registers[reg_dst] = literal;
}

/////////////////////////////////////////////////////////////////////////////
//  Class 3: type ARI
bool VirtualMachine::InstTypeArihmetic (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst, const SubInst &sub_type){
  bool error = false;
  using namespace IRDefinition;
  using namespace SubtypesArithmetic;
  switch(sub_type){
    case IR_ADD:  InstAdd(reg_src1, reg_src2, reg_dst); break;
    case IR_SUB:  InstSub(reg_src1, reg_src2, reg_dst); break;
    case IR_MUL:  InstMul(reg_src1, reg_src2, reg_dst); break;
    case IR_DIV:  InstDiv(reg_src1, reg_src2, reg_dst); break;
    case IR_MOV:  InstMov(reg_src1, reg_src2, reg_dst); break;
    default:      error_log_->errors.push_back("ari :: subtype not found");
                  error = true;                          break;
  }
  return error;
}

void VirtualMachine::InstAdd (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "ADD R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1]
    + process_->execution_context_.registers_.registers[reg_src2];
}

void VirtualMachine::InstSub (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "SUB R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1]
    - process_->execution_context_.registers_.registers[reg_src2];
}

void VirtualMachine::InstMul (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "MUL R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1]
    * process_->execution_context_.registers_.registers[reg_src2];
}

void VirtualMachine::InstDiv (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "DIV R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1]
    / process_->execution_context_.registers_.registers[reg_src2];
}



/////////////////////////////////////////////////////////////////////////////
//  Class 3: type CMP
bool VirtualMachine::InstTypeComparison (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst, const SubInst &sub_type){
  bool error = false;
  using namespace IRDefinition;
  using namespace SubtypesComparison;
  switch(sub_type){
    case IR_NOT:  InstNot(reg_src1, reg_src2, reg_dst); break;
    case IR_EQT:  InstEql(reg_src1, reg_src2, reg_dst); break;
    case IR_LST:  InstLst(reg_src1, reg_src2, reg_dst); break;
    case IR_LTE:  InstLte(reg_src1, reg_src2, reg_dst); break;
    default:      error_log_->errors.push_back("ari :: subtype not found");
                  error = true;                          break;
  }
  return error;
}

void VirtualMachine::InstNot (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "NOT R"<<reg_dst<<"= !R"<<reg_src1<<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      not process_->execution_context_.registers_.registers[reg_src1];
}

void VirtualMachine::InstEql (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "EQL R"<<reg_dst<<"=R"<<reg_src1<<" == R"<< reg_src2 <<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1]
   == process_->execution_context_.registers_.registers[reg_src2];
}

void VirtualMachine::InstLst (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "LST R"<<reg_dst<<"=R"<<reg_src1<<" < R"<< reg_src2 <<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1]
    < process_->execution_context_.registers_.registers[reg_src2];
}

void VirtualMachine::InstLte (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "LTE R"<<reg_dst<<"=R"<<reg_src1<<" <= R"<< reg_src2 <<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1]
   <= process_->execution_context_.registers_.registers[reg_src2];
}

void VirtualMachine::InstMov (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "MOV R"<<reg_dst<<"=R"<<reg_src1<<" \n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1];
}

}//end namespace VM
