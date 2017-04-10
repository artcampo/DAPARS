#pragma once
#include "Backend/Backend.hpp"

#include <map>
#include <memory>

namespace Compiler{
namespace Backend{
namespace BackendDAVM{


class BackendDAVM : public Backend{
public:

  BackendDAVM(CompilationUnit& unit, IR::IRUnit& ir_unit)
  : Backend(unit, ir_unit){}

  void Run(){
    for(auto& it : ir_unit_.streams_) Visit(*it);
  }
private:
  
  void Visit(IR::IRStream& stream){}
  

};


}//end namespace Backend
}//end namespace Backend
}//end namespace Compiler
