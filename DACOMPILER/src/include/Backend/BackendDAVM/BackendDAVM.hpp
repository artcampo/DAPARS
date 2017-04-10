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
  
  void Visit(IR::IRStream& stream){
    for(auto& it : stream) Visit(*it);
  }

  void Visit(IR::Inst::Inst& inst){
    std::cout << "Not implemented: " << inst.str() << "\n";
  }
  void Visit(IR::Inst::Load& inst){
    std::cout << "yuhu" << inst.str() << "\n";
  }


};


}//end namespace Backend
}//end namespace Backend
}//end namespace Compiler
