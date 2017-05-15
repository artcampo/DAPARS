#include "VM/Execution/VM.hpp"
#include "VM/ByteCode/ByteCode.hpp"
#include "VM/ByteCode/IRCodification.hpp"
#include "VM/ByteCode/IRBuilder.hpp"
#include "VM/ByteCode/IRDefinition.hpp"
#include "VM/VMSpec.hpp"
#include "VM/ByteCode/MemChunk.hpp"
#include <iostream>

int main(){
  using namespace VM;
  std::unique_ptr<ByteCode> bc(new ByteCode());


  using namespace IRCodification;
  using namespace IRBuilder;
  using namespace IRDefinition;
  using namespace IRBuilderAPI;
  using namespace Spec;

  bc->stream = std::vector<uint32_t> {
    Load( IR_REG0, 0),
    Load( IR_REG1, 3),
    Load( IR_REG0, 0 + (1 << (kPageNumBits - kWordNumBits)) ),
    Load( IR_REG1, 3 + (1 << (kPageNumBits - kWordNumBits)) ),
    Stop ()
  };

  bc->SetMemUser(MemChunk(0, 3 + (1 << (kPageNumBits - kWordNumBits))));

  if(!checkIRCodification()){
    std::cout << "IR sizes invalid\n";
    return 1;
  }

  //Execute bytecode
  std::unique_ptr<VirtualMachine> vm(new VirtualMachine(*bc) );
  vm->ExecProcess();
  vm->DumpExecutionContext();

  return 0;
}
