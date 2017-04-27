#pragma once
#include "CompilationUnit.hpp"
#include "IR/IRUnit.hpp"
#include "Backend/TargetDefinition.hpp"
#include "Backend/RegMap.hpp"
#include <map>
#include <memory>

namespace Compiler{
namespace Backend{


class Backend{
public:

  Backend(CompilationUnit& unit, IR::IRUnit& ir_unit, TargetDefinition&& target_definition)
  : unit_(unit), ir_unit_(ir_unit), target_definition_(target_definition){}

  virtual void LoadCallBack (const MReg r, const IR::MemAddr addr) = 0;
  virtual void StoreCallBack(const MReg r, const IR::MemAddr addr) = 0;
protected:
  CompilationUnit&  unit_;
  IR::IRUnit&       ir_unit_;
  TargetDefinition& target_definition_;

};


}//end namespace Backend
}//end namespace Compiler
