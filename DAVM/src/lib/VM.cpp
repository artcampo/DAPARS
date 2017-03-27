#include "VM.hpp"
#include "IRDefinition.hpp"
#include "IRCodification.hpp"
#include "IRBuilder.hpp"
#include "RegisterBank.hpp"
#include <stdint.h>
#include <iostream>

bool VirtualMachine::ExecProcess(){
  bool executing  = true;
  bool error      = false;

  std::cout << "EXEC\n";
  while(executing and not error){
    if( not process_->NextOpCodeIsValid() ){
      error     = true;
      executing = false;
      error_log_->errors.push_back("Next opcode invalid");
    }else{
      using namespace IRCodification;
      using namespace IRBuilder;
      using namespace IRDefinition;

      uint32_t const current_instruction  = process_->GetCurrentOpCode();
      uint32_t const current_class        = DecodeClass(current_instruction);
      uint32_t const current_type         = DecodeType(current_instruction, current_class);
      uint32_t const current_op_code      = DecodeOpCode(current_class, current_type);
      uint32_t reg_src1, reg_src2, reg_dst, sub_type, literal, op_offset;

      if (current_op_code == IR_STOP){
        std::cout << "STOP\n";
        executing = false;
      }else{

        std::cout << "OP: ";
        bool instructionHasJump = false;

        switch(current_class){
          ////////////////////////////////////////////////////////////
          case InstClassNoReg:
            DecodeClass0(current_instruction, literal);
            switch(current_op_code){
              default:      error_log_->errors.push_back(
                                          "op not found (c0)");
                            error = true; break;
            }
            break;

          ////////////////////////////////////////////////////////////
          case InstClassRegLit:
            DecodeClass1(current_instruction, reg_dst, literal);
            switch(current_op_code){
              case IR_LOAD: InstLoad(reg_dst, literal); break;
              default:      error_log_->errors.push_back(
                                          "op not found (c1)");
                            error = true; break;
            }
            break;

          ////////////////////////////////////////////////////////////
          case InstClassRegLitSub:
            switch(current_op_code){
              default:      error_log_->errors.push_back(
                                          "op not found (c2)");
                            error = true; break;
            }
            break;

          ////////////////////////////////////////////////////////////
          case InstClassRegRegRegSub:
            DecodeClass3(current_instruction, reg_src1, reg_src2, reg_dst,
                         sub_type);
            switch(current_op_code){
              case IR_ARI:  error = InstTypeArihmetic(reg_src1, reg_src2,
                                              reg_dst, sub_type);
                            break;
              case IR_CMP:  error = InstTypeComparison(reg_src1, reg_src2,
                                              reg_dst, sub_type);
                            break;
              default:      error_log_->errors.push_back(
                                          "op not found (c3)");
                            error = true; break;
            }
            break;

          ////////////////////////////////////////////////////////////
          default:      error_log_->errors.push_back(
                                          "class not found");
                        error = true; break;

        }


        if (not instructionHasJump )
          process_->NextOpCode();
        else
          process_->ModifyIP(op_offset);
      }// end of if (current_op_code == IR_STOP){

      //Dump registers after every instruction
//       DumpExecutionContext();
    }
  }

  return error;
}

// VirtualMachine::VirtualMachine(std::string const &file_name){
//   byte_code_ =
// }

VirtualMachine::VirtualMachine(ByteCode const &byte_code)
  : byte_code_(byte_code)
  , process_(new Process(byte_code_))
  , error_log_( new ErrorLog()){
}

ByteCode* VirtualMachine::ReadByteCode(const std::string &file_name){
  ByteCode* byte_code = new ByteCode();
  return byte_code;
}


void VirtualMachine::DumpExecutionContext(int const registers_num) const{
  if(error_log_->errors.size() == 0)
    process_->DumpExecutionContext(registers_num);
  else{
    std::cout << "Errors in execution:" << std::endl;
    for(auto it : error_log_->errors)
      std::cout << it << "\n";
  }
}

int VirtualMachine::LoadProcess(const std::string &file_name){
  std::unique_ptr<ByteCode> byte_code(ReadByteCode(file_name));
}
