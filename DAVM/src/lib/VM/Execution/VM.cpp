#include "VM/Execution/VM.hpp"
#include "VM/ByteCode/IRDefinition.hpp"
#include "VM/ByteCode/IRCodification.hpp"
#include "VM/ByteCode/IRBuilder.hpp"
#include "VM/Execution/RegisterBank.hpp"
#include <stdint.h>
#include <iostream>
#include <memory>

namespace VM{

bool VirtualMachine::ExecProcess(){
  ColdBoot();
  bool executing  = true;
  int instruction = 0;

  std::cout << "EXEC\n";
  while(executing and not error_log_.HasErrors()){
//     std::cout << instruction <<", " <<process_->GetIP() << "; ";
//               << process_->StackAddr() << "/" <<byte_code_.StackRegisterLow()<<": ";
    ++instruction;
    if( not process_->NextOpCodeIsValid() ){
      executing = false;
      error_log_.Exception("Next opcode invalid");
    }else{
      using namespace IRCodification;
      using namespace IRDefinition;

      const Inst    current_instruction  = process_->GetCurrentOpCode();
      const SubInst current_class        = DecodeClass(current_instruction);
      const SubInst current_type         = DecodeType(current_instruction, current_class);
      const SubInst current_op_code      = DecodeOpCode(current_class, current_type);

      Reg     reg_src1, reg_src2, reg_dst,reg_base;
      SubInst sub_type, op_offset;
      Word    literal;

      if (current_op_code == IR_STOP){
        //TODO: put within main switches (switchii? :))
        std::cout << "STOP\n";
        executing = false;
      }else{

        std::cout << "OP: ";
        ip_modified_ = false;

        switch(current_class){
          ////////////////////////////////////////////////////////////
          case InstClassLit:
            DecodeClass0(current_instruction, literal);
            switch(current_op_code){
              case IR_JMP:  Jump(Target(literal));  break;
              case IR_CALL: Call(Target(literal));  break;
              case IR_RET:  Return();               break;
              default:      error_log_.Exception("op not found (c0)"); break;
            }
            break;

          ////////////////////////////////////////////////////////////
          case InstClassRegLit:
            DecodeClass1(current_instruction, reg_dst, reg_base, literal);
            switch(current_op_code){
              case IR_LOADI: LoadI (reg_dst, literal);            break;
              case IR_LOAD:  Load  (reg_dst, literal);            break;
              case IR_LOADB: LoadB (reg_dst, reg_base, literal);  break;
              case IR_STORE: Store (reg_dst, literal);            break;
              case IR_STOREB:StoreB(reg_dst, reg_base, literal);  break;
              case IR_PUSH:  Push  (reg_dst);                     break;
              case IR_POP:   Pop   (reg_dst);                     break;
              default:       error_log_.Exception("op not found (c1)"); break;
            }
            break;

          ////////////////////////////////////////////////////////////
          case InstClassRegLitSub:
            DecodeClass2(current_instruction, reg_dst, literal, sub_type);
            switch(current_op_code){
              case IR_JMPC: JumpC (reg_dst, Target(literal), sub_type); break;
              case IR_ARII: ArithI(reg_dst, literal, sub_type);         break;
              default:      error_log_.Exception("op not found (c2)");  break;
            }
            break;

          ////////////////////////////////////////////////////////////
          case InstClassRegRegRegSub:
            DecodeClass3(current_instruction, reg_src1, reg_src2, reg_dst, sub_type);
            switch(current_op_code){
              case IR_ARI:  InstTypeArihmetic (reg_src1, reg_src2, reg_dst, sub_type); break;
              case IR_CMP:  InstTypeComparison(reg_src1, reg_src2, reg_dst, sub_type); break;
              case IR_LOGIC:InstTypeLogic     (reg_src1, reg_src2, reg_dst, sub_type); break;
              default:      error_log_.Exception("op not found (c3)");                 break;
            }
            break;

          ////////////////////////////////////////////////////////////
          default:      error_log_.Exception("class not found"); break;

        }


        if(not ip_modified_ ) process_->NextOpCode();
      }// end of if (current_op_code == IR_STOP){

      //Dump registers after every instruction
//       DumpExecutionContext();
    }
  }

  if(byte_code_.PerformCompilerTest()) CompilerTest();

  return error_log_.HasErrors();  //TODO: needed?
}

// VirtualMachine::VirtualMachine(std::string const &file_name){
//   byte_code_ =
// }

VirtualMachine::VirtualMachine(ByteCode const &byte_code)
  : byte_code_(byte_code)
  , error_log_(ErrorLog::GetInstance())
  , process_(std::make_unique<Internal::Process>(byte_code_)){
}

ByteCode* VirtualMachine::ReadByteCode(const std::string &file_name){
  ByteCode* byte_code = new ByteCode();
  return byte_code;
}


void VirtualMachine::DumpExecutionContext(int const registers_num) const{
  if(not error_log_.HasErrors())
    process_->DumpExecutionContext(registers_num);
  else{
    std::cout << "Errors in execution:" << std::endl;
    error_log_.Dump();
  }
}

// int VirtualMachine::LoadProcess(const std::string &file_name){
//   std::unique_ptr<ByteCode> byte_code(ReadByteCode(file_name));
//   process_->Allocate( mem_stack_);
// }

//inits the machine to a blank state TODO: not finished!! (not used either)
void VirtualMachine::ColdBoot(){
  process_->StackReg() = byte_code_.StackRegisterInitAddress();
//   std::cout << "Set SR: " << Addr(byte_code_.StackRegisterInitAddress())<< "\n";
}

void VirtualMachine::CompilerTest(){
  Load(IRDefinition::IRRegisters::IR_REG0, Spec::CompilerMemory::kTestBoolAddress);
  if(process_->registers_[0] == 0) error_log_.Exception("Compiler injected tests failed");
}

}//end namespace VM
