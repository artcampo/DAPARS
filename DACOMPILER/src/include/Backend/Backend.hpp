#pragma once
#include "CompilationUnit.hpp"
#include "IR/IRUnit.hpp"
#include "Backend/TargetDefinition.hpp"
//#include "AST/AST.hpp"
#include <map>
#include <memory>

namespace Compiler{
namespace Backend{


class Backend{
public:

  Backend(CompilationUnit& unit, IR::IRUnit& ir_unit, TargetDefinition&& target_definition)
  : unit_(unit), ir_unit_(ir_unit), target_definition_(target_definition){}

protected:
  CompilationUnit&  unit_;
  IR::IRUnit&       ir_unit_;
  TargetDefinition& target_definition_;

};


}//end namespace Backend
}//end namespace Compiler
