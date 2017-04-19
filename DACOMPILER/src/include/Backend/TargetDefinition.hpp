#pragma once

namespace Compiler{
namespace Backend{


class TargetDefinition{
public:

  TargetDefinition(){}

  virtual size_t  NumRegisters() const noexcept = 0;
protected:


};


}//end namespace Backend
}//end namespace Compiler
