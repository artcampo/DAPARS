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

void VirtualMachine::LoadI(const Reg &reg_dst, const SubInst &literal){
  std::cout << "LOAD R"<< reg_dst <<"="<<literal<<"\n";
  process_->registers_[reg_dst] = literal;
}

/////////////////////////////////////////////////////////////////////////////
//  Class 3: type ARI
bool VirtualMachine::InstTypeArihmetic (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst, const SubInst &sub_type){
  bool error = false;
  using namespace IRDefinition;
  using namespace SubtypesArithmetic;
  switch(sub_type){
    case IR_ADD:  Add(reg_src1, reg_src2, reg_dst); break;
    case IR_SUB:  Sub(reg_src1, reg_src2, reg_dst); break;
    case IR_MUL:  Mul(reg_src1, reg_src2, reg_dst); break;
    case IR_DIV:  Div(reg_src1, reg_src2, reg_dst); break;
    case IR_MOV:  Move(reg_src1, reg_src2, reg_dst); break;
    default:      error_log_->errors.push_back("ari :: subtype not found");
                  error = true;                          break;
  }
  return error;
}

void VirtualMachine::Add (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "ADD R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    + process_->registers_[reg_src2];
}

void VirtualMachine::Sub (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "SUB R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    - process_->registers_[reg_src2];
}

void VirtualMachine::Mul (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "MUL R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    * process_->registers_[reg_src2];
}

void VirtualMachine::Div (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "DIV R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    / process_->registers_[reg_src2];
}



/////////////////////////////////////////////////////////////////////////////
//  Class 3: type CMP
bool VirtualMachine::InstTypeComparison (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst, const SubInst &sub_type){
  bool error = false;
  using namespace IRDefinition;
  using namespace SubtypesComparison;
  switch(sub_type){
    case IR_NOT:  Not(reg_src1, reg_src2, reg_dst); break;
    case IR_EQT:  Eqt(reg_src1, reg_src2, reg_dst); break;
    case IR_LST:  Lst(reg_src1, reg_src2, reg_dst); break;
    case IR_LTE:  Lte(reg_src1, reg_src2, reg_dst); break;
    default:      error_log_->errors.push_back("ari :: subtype not found");
                  error = true;                          break;
  }
  return error;
}

void VirtualMachine::Not (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "NOT R"<<reg_dst<<"= !R"<<reg_src1<<"\n";
  process_->registers_[reg_dst] =
      not process_->registers_[reg_src1];
}

void VirtualMachine::Eqt (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "EQL R"<<reg_dst<<"=R"<<reg_src1<<" == R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
   == process_->registers_[reg_src2];
}

void VirtualMachine::Lst (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "LST R"<<reg_dst<<"=R"<<reg_src1<<" < R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    < process_->registers_[reg_src2];
}

void VirtualMachine::Lte (const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "LTE R"<<reg_dst<<"=R"<<reg_src1<<" <= R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
   <= process_->registers_[reg_src2];
}

void VirtualMachine::Move(const Reg &reg_src1,
  const Reg &reg_src2, const Reg &reg_dst){
  std::cout << "MOV R"<<reg_dst<<"=R"<<reg_src1<<" \n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1];
}

void VirtualMachine::Load (const Reg&reg_dst, const SubInst& literal){
  std::cout << "LOAD R"<<reg_dst<<"=[@"<<literal <<"]";
  process_->registers_[reg_dst] = process_->Load(literal);
  std::cout << ", val = "<< process_->registers_[reg_dst] <<" \n";
}

}//end namespace VM
