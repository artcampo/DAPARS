#pragma once
#include "ByteCode.hpp"
#include "ExecutionContext.hpp"
#include <memory>
#include <stdint.h>

namespace VM{

class VirtualMachine;

class Process : public ExecutionContext{

public:
  Process(const ByteCode &byte_code) : byte_code_(byte_code){};

  void NextOpCode();

  bool      NextOpCodeIsValid() const;
  Inst      GetCurrentOpCode() const;

  void      ModifyIP(Target const &offset);

  void DumpExecutionContext(int const registers_num = 5) const;
private:
  const ByteCode    &byte_code_;
  friend class VirtualMachine;
};

}//end namespace VM
