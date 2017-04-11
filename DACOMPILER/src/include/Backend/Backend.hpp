#pragma once
#include "CompilationUnit.hpp"
#include "IR/IRUnit.hpp"
//#include "AST/AST.hpp"
#include <map>
#include <memory>

namespace Compiler{
namespace Backend{


class Backend{
public:

  Backend(CompilationUnit& unit, IR::IRUnit& ir_unit)
  : unit_(unit), ir_unit_(ir_unit){}

protected:
  CompilationUnit&  unit_;
  IR::IRUnit&       ir_unit_;

};


}//end namespace Backend
}//end namespace Compiler
