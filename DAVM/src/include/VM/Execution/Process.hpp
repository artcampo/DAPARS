#pragma once
#include "VM/ByteCode/ByteCode.hpp"
#include "VM/Execution/ExecutionContext.hpp"
#include "VM/Execution/Memory.hpp"
#include "VM/Execution/Policies/MemoryTesting.hpp"
#include <memory>
#include <stdint.h>

namespace VM{

class VirtualMachine;


namespace Internal{


class Process : public Internal::ExecutionContext
  , public Internal::Memory<Internal::BasicPatternTestingPolicy>{

public:
  Process(const ByteCode &byte_code) : byte_code_(byte_code){};

  void NextOpCode();

  bool      NextOpCodeIsValid() const;
  Inst      GetCurrentOpCode() const;

  void      ModifyIP(const Target new_ip);

  void      DumpExecutionContext(int const registers_num = 5) const;
private:
  const ByteCode    &byte_code_;
  friend class VirtualMachine;

};

}//end namespace Internal
}//end namespace VM
