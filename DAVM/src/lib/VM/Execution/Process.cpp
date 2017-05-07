#include "VM/Execution/Process.hpp"
#include <iostream>

namespace VM{
namespace Internal{

void Process::NextOpCode(){
  instruction_pointer_++;
}

bool Process::NextOpCodeIsValid() const{
  return (instruction_pointer_ >= 0 and
          instruction_pointer_ < byte_code_.stream.size() );
}

// PRE: NextOpCodeIsValid() is true
Inst Process::GetCurrentOpCode() const{
  return byte_code_.stream[instruction_pointer_];
}

void Process::ModifyIP(const Target new_ip){
  instruction_pointer_ = new_ip;
}

void Process::DumpExecutionContext(const int registers_num) const{
  std::cout << "Registers: ";
  for( int i = 0; i < registers_num ; ++i){
    std::cout << registers_[i];
    if ( i != (registers_num - 1) ) std::cout << ", ";
  }
  std::cout << "\n";
}

}//end namespace Internal
}//end namespace VM
