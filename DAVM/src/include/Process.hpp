#pragma once
#include "ByteCode.hpp"
#include "ExecutionContext.hpp"
#include <memory>
#include <stdint.h>
  
class VirtualMachine;
  
class Process{
  
public:  
  Process(const ByteCode &byte_code) : byte_code_(byte_code),
    execution_context_(){};
    
  void NextOpCode();
  
  bool      NextOpCodeIsValid() const;
  uint32_t  GetCurrentOpCode() const;

  void      ModifyIP(uint32_t const &offset);
  
  void DumpExecutionContext(int const registers_num = 5) const;
private:
  const ByteCode    &byte_code_;
  ExecutionContext  execution_context_;
  friend class VirtualMachine;
};