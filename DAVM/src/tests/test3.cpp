#include "VM.hpp"
#include "ByteCode.hpp"
#include "IRCodification.hpp"
#include "IRBuilder.hpp"
#include "IRDefinition.hpp"
#include "Utils.hpp"
#include <iostream>

int main(){
  std::unique_ptr<ByteCode> bc(new ByteCode());
  
  using namespace IRCodification;
  using namespace IRBuilder;
  using namespace IRDefinition;
  using namespace IRBuilderAPI;
  using namespace SubtypesComparison;
  
  bc->stream = std::vector<uint32_t> {
    Load( IR_REG0, 42),
    Load( IR_REG1, 42),
    Load( IR_REG2, 12),
    
    Comp( IR_REG0, IR_REG1, IR_REG3, IR_NOT),
    Comp( IR_REG0, IR_REG1, IR_REG4, IR_EQL),
    Comp( IR_REG0, IR_REG1, IR_REG5, IR_LST),
    Comp( IR_REG0, IR_REG1, IR_REG6, IR_LTE),
    
    Comp( IR_REG2, IR_REG1, IR_REG7, IR_EQL),
    Comp( IR_REG2, IR_REG1, IR_REG8, IR_LST),
    Comp( IR_REG2, IR_REG1, IR_REG9, IR_LTE),
    
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