#include "VM.hpp"
#include "IRDefinition.hpp"
#include "IRCodification.hpp"
#include "IRBuilder.hpp"
#include "RegisterBank.hpp"
#include <stdint.h>
#include <iostream>

using namespace IRCodification;
using namespace IRBuilder;

void VirtualMachine::InstLoad(uint32_t const &reg_dst, uint32_t const &literal){
  std::cout << "LOAD R"<< reg_dst <<"="<<literal<<"\n";
  process_->execution_context_.registers_.registers[reg_dst] = literal;
}

/////////////////////////////////////////////////////////////////////////////
//  Class 3: type ARI
bool VirtualMachine::InstTypeArihmetic (uint32_t const &reg_src1,
  uint32_t const &reg_src2, uint32_t const &reg_dst, uint32_t const &sub_type){
  bool error = false;
  using namespace IRDefinition;
  using namespace SubtypesArithmetic;
  switch(sub_type){
    case IR_ADD:  InstAdd(reg_src1, reg_src2, reg_dst); break;
    case IR_SUB:  InstSub(reg_src1, reg_src2, reg_dst); break;
    case IR_MUL:  InstMul(reg_src1, reg_src2, reg_dst); break;
    case IR_DIV:  InstDiv(reg_src1, reg_src2, reg_dst); break;
    default:      error_log_->errors.push_back("ari :: subtype not found");
                  error = true;                          break;
  }
  return error;
}

void VirtualMachine::InstAdd (uint32_t const &reg_src1,
  uint32_t const &reg_src2, uint32_t const &reg_dst){
  std::cout << "ADD R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1]
    + process_->execution_context_.registers_.registers[reg_src2];
}

void VirtualMachine::InstSub (uint32_t const &reg_src1,
  uint32_t const &reg_src2, uint32_t const &reg_dst){
  std::cout << "SUB R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1]
    - process_->execution_context_.registers_.registers[reg_src2];
}

void VirtualMachine::InstMul (uint32_t const &reg_src1,
  uint32_t const &reg_src2, uint32_t const &reg_dst){
  std::cout << "MUL R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1]
    * process_->execution_context_.registers_.registers[reg_src2];
}

void VirtualMachine::InstDiv (uint32_t const &reg_src1,
  uint32_t const &reg_src2, uint32_t const &reg_dst){
  std::cout << "DIV R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1]
    / process_->execution_context_.registers_.registers[reg_src2];
}



/////////////////////////////////////////////////////////////////////////////
//  Class 3: type CMP
bool VirtualMachine::InstTypeComparison (uint32_t const &reg_src1,
  uint32_t const &reg_src2, uint32_t const &reg_dst, uint32_t const &sub_type){
  bool error = false;
  using namespace IRDefinition;
  using namespace SubtypesComparison;
  switch(sub_type){
    case IR_NOT:  InstNot(reg_src1, reg_src2, reg_dst); break;
    case IR_EQL:  InstEql(reg_src1, reg_src2, reg_dst); break;
    case IR_LST:  InstLst(reg_src1, reg_src2, reg_dst); break;
    case IR_LTE:  InstLte(reg_src1, reg_src2, reg_dst); break;
    default:      error_log_->errors.push_back("ari :: subtype not found");
                  error = true;                          break;
  }
  return error;
}

void VirtualMachine::InstNot (uint32_t const &reg_src1,
  uint32_t const &reg_src2, uint32_t const &reg_dst){
  std::cout << "NOT R"<<reg_dst<<"= !R"<<reg_src1<<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      not process_->execution_context_.registers_.registers[reg_src1];
}

void VirtualMachine::InstEql (uint32_t const &reg_src1,
  uint32_t const &reg_src2, uint32_t const &reg_dst){
  std::cout << "EQL R"<<reg_dst<<"=R"<<reg_src1<<" == R"<< reg_src2 <<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1]
   == process_->execution_context_.registers_.registers[reg_src2];
}

void VirtualMachine::InstLst (uint32_t const &reg_src1,
  uint32_t const &reg_src2, uint32_t const &reg_dst){
  std::cout << "LST R"<<reg_dst<<"=R"<<reg_src1<<" < R"<< reg_src2 <<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1]
    < process_->execution_context_.registers_.registers[reg_src2];
}

void VirtualMachine::InstLte (uint32_t const &reg_src1,
  uint32_t const &reg_src2, uint32_t const &reg_dst){
  std::cout << "LTE R"<<reg_dst<<"=R"<<reg_src1<<" <= R"<< reg_src2 <<"\n";
  process_->execution_context_.registers_.registers[reg_dst] =
      process_->execution_context_.registers_.registers[reg_src1]
   <= process_->execution_context_.registers_.registers[reg_src2];
}
