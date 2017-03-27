#include "Process.hpp"
#include <iostream>

void Process::NextOpCode(){
  execution_context_.instruction_pointer_++;
}

bool Process::NextOpCodeIsValid() const{
  return (execution_context_.instruction_pointer_ >= 0 and
          execution_context_.instruction_pointer_ < byte_code_.stream.size() );
}

// PRE: NextOpCodeIsValid() is true
uint32_t Process::GetCurrentOpCode() const{
  return byte_code_.stream[execution_context_.instruction_pointer_];
}

void Process::ModifyIP(uint32_t const &offset){
  execution_context_.instruction_pointer_++;
  execution_context_.instruction_pointer_ += offset;
}

void Process::DumpExecutionContext(int const registers_num) const{
  std::cout << "Registers: ";
  for( int i = 0; i < registers_num ; ++i){
    std::cout << execution_context_.registers_.registers[i];
    if ( i != (registers_num - 1) ) std::cout << ", ";
  }
  std::cout << "\n";
}