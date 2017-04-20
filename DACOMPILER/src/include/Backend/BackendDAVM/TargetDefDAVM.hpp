#pragma once
#include "Backend/TargetDefinition.hpp"

namespace Compiler{
namespace Backend{


class TargetDefDAVM : public TargetDefinition{
public:

  TargetDefDAVM(){}

  size_t  NumRegisters() const noexcept{ return 16; }
protected:


};


}//end namespace Backend
}//end namespace Compiler
