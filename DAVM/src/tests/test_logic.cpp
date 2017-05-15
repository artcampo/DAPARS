#include "VM/Execution/VM.hpp"
#include "VM/ByteCode/ByteCode.hpp"
#include "VM/ByteCode/IRCodification.hpp"
#include "VM/ByteCode/IRBuilder.hpp"
#include "VM/ByteCode/IRDefinition.hpp"
#include "VM/ByteCode/Utils.hpp"
#include <iostream>

int main(){
  using namespace VM;
  std::unique_ptr<ByteCode> bc(new ByteCode());

  using namespace IRCodification;
  using namespace IRBuilder;
  using namespace IRDefinition;
  using namespace IRBuilderAPI;
  using namespace SubtypesLogic;

  bc->stream = std::vector<uint32_t> {
    LoadI(IR_REG0, 0),
    LoadI(IR_REG1, 1),

    Logic(IR_REG0, IR_REG1, IR_REG2, IR_OR),//1
    Logic(IR_REG0, IR_REG0, IR_REG3, IR_OR),//0

    Logic(IR_REG0, IR_REG1, IR_REG4, IR_AND),//0
    Logic(IR_REG1, IR_REG1, IR_REG5, IR_AND),//1

    Stop()
  };


  if(!checkIRCodification()){
    std::cout << "IR sizes invalid\n";
    return 1;
  }

  VMUtils::printRaw(*bc);
  VMUtils::print(*bc);


  //Execute bytecode
  std::unique_ptr<VirtualMachine> vm(new VirtualMachine(*bc) );
  vm->ExecProcess();
  vm->DumpExecutionContext();


  return 0;
}
