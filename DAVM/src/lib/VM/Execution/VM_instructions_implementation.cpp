#include "VM/Execution/VM.hpp"
#include "VM/ByteCode/IRDefinition.hpp"
#include "VM/ByteCode/IRCodification.hpp"
#include "VM/ByteCode/IRBuilder.hpp"
#include "VM/Execution/RegisterBank.hpp"
#include <stdint.h>
#include <iostream>

namespace VM{

using namespace IRCodification;
using namespace IRBuilder;

/////////////////////////////////////////////////////////////////////////////
//  Helpers
Word VirtualMachine::PopWord(){
  std::cout << "  -- pop @"<<  Addr(process_->StackReg() + Spec::kWordSize)<< " val: "
            << process_->Load( Addr(process_->StackReg() + Spec::kWordSize)) << "\n";
  process_->StackReg() += Spec::kWordSize;
  return process_->Load( Addr(process_->StackReg()));
}

void VirtualMachine::PushWord(const Word word){
  std::cout << "  -- push @"<< Addr(process_->StackReg())<< " val: "
            << word << "\n";
  process_->Store( Addr(process_->StackReg()), word);
  process_->StackReg() -= Spec::kWordSize;
}

/////////////////////////////////////////////////////////////////////////////
//  Class 0
void VirtualMachine::Return(){
  process_->ModifyIP(PopWord());
  ip_modified_ = true;
  std::cout << "Return "<< process_->GetIP()<<"\n";
}

void VirtualMachine::Call(const Target target){
  std::cout << "Call "<< target<< " ret@: " << process_->GetIP() + Spec::kCodeMemUnit << "\n";
  PushWord(process_->GetIP() + Spec::kCodeMemUnit);
  process_->ModifyIP(target);
  ip_modified_ = true;
}

void VirtualMachine::Jump(const Target target){
  std::cout << "JUMP "<< target<<"\n";
  process_->ModifyIP(target);
  ip_modified_ = true;
}

/////////////////////////////////////////////////////////////////////////////
//  Class 1
void VirtualMachine::LoadI(const Reg reg_dst, const Word literal){
  std::cout << "LOADI R"<< reg_dst <<" <- "<<literal<<"\n";
  process_->registers_[reg_dst] = literal;
}

void VirtualMachine::Load (const Reg reg_dst, const Word literal){
  std::cout << "LOAD R"<<reg_dst<<" <- [@"<<literal <<"]";
  process_->registers_[reg_dst] = process_->Load(literal);
  std::cout << ", val = "<< process_->registers_[reg_dst] <<" \n";
}

void VirtualMachine::LoadB(const Reg reg_dst, const Reg reg_base, const Word literal){
  std::cout << "LOADB R"<< reg_dst <<" <- [@"<<literal <<"+ R"<<reg_base<<"]";
  process_->registers_[reg_dst] =
    process_->Load(Addr(literal + process_->registers_[reg_base]));
}


void VirtualMachine::Store(const Reg reg_src, const Word literal){
  std::cout << "STORE [@" <<literal << "] <- R" << reg_src << " val: " << process_->registers_[reg_src] <<"\n";
  process_->Store(Addr(literal), process_->registers_[reg_src]);
}

void VirtualMachine::StoreB(const Reg reg_src, const Reg reg_base, const Word literal){
  std::cout << "STOREB [@" <<literal <<"+"
            << process_->registers_[reg_base]
            <<  "] <- " << process_->registers_[reg_src] <<"\n";
  process_->Store(Addr(literal + process_->registers_[reg_base]), process_->registers_[reg_src]);
}


void VirtualMachine::Pop(const Reg reg_dst){
  std::cout << "POP R"<<reg_dst<<" <- [@"<<Addr(process_->StackReg() + Spec::kWordSize) <<"]\n";
  process_->registers_[reg_dst] = PopWord();
}




void VirtualMachine::Push(const Reg reg_src){
  std::cout << "PUSH [@" <<Addr(process_->StackReg())
            <<  "] <- R" << reg_src <<"\n";
  PushWord(process_->registers_[reg_src]);
}



/////////////////////////////////////////////////////////////////////////////
//  Class 2
void VirtualMachine::ArithI(const Reg reg_dst, const Word literal, const SubInst op){

}

void VirtualMachine::JumpC (const Reg reg_src, const Target target, const SubInst op){
  using namespace IRDefinition::SubtypesJMPC;
  if(  (op == IR_TRUE  and     process_->registers_[reg_src])
    or (op == IR_FALSE and not process_->registers_[reg_src]) ){
    process_->ModifyIP(target);
    ip_modified_ = true;
  }
}

/////////////////////////////////////////////////////////////////////////////
//  Class 3: type ARI
bool VirtualMachine::InstTypeArihmetic (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst, const SubInst sub_type){
  bool error = false;
  using namespace IRDefinition::SubtypesArithmetic;
  switch(sub_type){
    case IR_ADD:  Add(reg_src1, reg_src2, reg_dst); break;
    case IR_SUB:  Sub(reg_src1, reg_src2, reg_dst); break;
    case IR_MUL:  Mul(reg_src1, reg_src2, reg_dst); break;
    case IR_DIV:  Div(reg_src1, reg_src2, reg_dst); break;
    case IR_MOV:  Move(reg_src1, reg_src2, reg_dst); break;
    default:      error_log_->Log("ari :: subtype not found");
                  error = true;                          break;
  }
  return error;
}

void VirtualMachine::Add (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "ADD R"<<reg_dst<<" <- R"<<reg_src1<<" + R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    + process_->registers_[reg_src2];
}

void VirtualMachine::Sub (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "SUB R"<<reg_dst<<" <- R"<<reg_src1<<" - R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    - process_->registers_[reg_src2];
}

void VirtualMachine::Mul (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "MUL R"<<reg_dst<<" <- R"<<reg_src1<<" * R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    * process_->registers_[reg_src2];
}

void VirtualMachine::Div (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "DIV R"<<reg_dst<<" <- R"<<reg_src1<<" / R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    / process_->registers_[reg_src2];
}

void VirtualMachine::Move(const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "MOVE R"<<reg_dst<<" <- R"<<reg_src1<<" \n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1];
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
    default:      error_log_->Log("cmp :: subtype not found");
                  error = true;                          break;
  }
  return error;
}

void VirtualMachine::Not (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "NOT R"<<reg_dst<<" <-  !R"<<reg_src1<<"\n";
  process_->registers_[reg_dst] = not process_->registers_[reg_src1];
}

void VirtualMachine::Eqt (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "EQL R"<<reg_dst<<" <- R"<<reg_src1<<" == R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
   == process_->registers_[reg_src2];
}

void VirtualMachine::Lst (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "LST R"<<reg_dst<<" <- R"<<reg_src1<<" < R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
    < process_->registers_[reg_src2];
}

void VirtualMachine::Lte (const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst){
  std::cout << "LTE R"<<reg_dst<<" <- R"<<reg_src1<<" <= R"<< reg_src2 <<"\n";
  process_->registers_[reg_dst] =
      process_->registers_[reg_src1]
   <= process_->registers_[reg_src2];
}

/////////////////////////////////////////////////////////////////////////////
//  Class 3: type Logic
bool VirtualMachine::InstTypeLogic(const Reg reg_src1,
  const Reg reg_src2, const Reg reg_dst, const SubInst sub_type){
  bool error = false;
  using namespace IRDefinition;
  using namespace SubtypesLogic;
  switch(sub_type){
    case IR_OR:   Or (reg_src1, reg_src2, reg_dst); break;
    case IR_AND:  And(reg_src1, reg_src2, reg_dst); break;
    default:      error_log_->Log("logic :: subtype not found");
                  error = true; break;
  }
  return error;
}

void VirtualMachine::Or(const Reg reg_src1, const Reg reg_src2, const Reg reg_dst){

}

void VirtualMachine::And(const Reg reg_src1, const Reg reg_src2, const Reg reg_dst){

}




}//end namespace VM
