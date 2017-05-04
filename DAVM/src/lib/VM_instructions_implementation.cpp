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

/////////////////////////////////////////////////////////////////////////////
//  Class 0
void VirtualMachine::Return(){

}

void VirtualMachine::Call(const Target target){

}

void VirtualMachine::Jump(const Target target){

}

/////////////////////////////////////////////////////////////////////////////
//  Class 1
void VirtualMachine::LoadI(const Reg reg_dst, const Word literal){
  std::cout << "LOAD R"<< reg_dst <<"="<<literal<<"\n";
  process_->registers_[reg_dst] = literal;
}

void VirtualMachine::Load (const Reg reg_dst, const Word literal){
  std::cout << "LOAD R"<<reg_dst<<"=[@"<<literal <<"]";
  process_->registers_[reg_dst] = process_->Load(literal);
  std::cout << ", val = "<< process_->registers_[reg_dst] <<" \n";
}

void VirtualMachine::LoadB(const Reg reg_dst, const Reg reg_base, const Word literal){
}


void VirtualMachine::Store(const Reg reg_src, const Word literal){
}

void VirtualMachine::StoreB(const Reg reg_src, const Reg reg_base, const Word literal){
}


void VirtualMachine::Pop(const Reg reg_dst){
}

void VirtualMachine::Push(const Reg reg_src){
}


/////////////////////////////////////////////////////////////////////////////
//  Class 3: type ARI
bool VirtualMachine::InstTypeArihmetic (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst, const SubInst sub_type){
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

void VirtualMachine::Add (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "ADD R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    + process_->registers_[reg_src2];
}

void VirtualMachine::Sub (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "SUB R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    - process_->registers_[reg_src2];
}

void VirtualMachine::Mul (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "MUL R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    * process_->registers_[reg_src2];
}

void VirtualMachine::Div (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "DIV R"<<reg_dst<<"=R"<<reg_src1<<" op R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    / process_->registers_[reg_src2];
}



/////////////////////////////////////////////////////////////////////////////
//  Class 3: type CMP
bool VirtualMachine::InstTypeComparison (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst, const SubInst sub_type){
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

void VirtualMachine::Not (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "NOT R"<<reg_dst<<"= !R"<<reg_src1<<"\n";
  process_->registers_[reg_dst] =
      not process_->registers_[reg_src1];
}

void VirtualMachine::Eqt (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "EQL R"<<reg_dst<<"=R"<<reg_src1<<" == R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
   == process_->registers_[reg_src2];
}

void VirtualMachine::Lst (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "LST R"<<reg_dst<<"=R"<<reg_src1<<" < R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    < process_->registers_[reg_src2];
}

void VirtualMachine::Lte (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "LTE R"<<reg_dst<<"=R"<<reg_src1<<" <= R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
   <= process_->registers_[reg_src2];
}

void VirtualMachine::Move(const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "MOV R"<<reg_dst<<"=R"<<reg_src1<<" \n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1];
}



}//end namespace VM
