#pragma once
#include "Backend/Backend.hpp"
#include "ByteCode.hpp"
#include "Utils.hpp"  //move to VM/
#include "IRBuilder.hpp"
#include "IR/IRVisitor.hpp"

#include <map>
#include <memory>

namespace Compiler{
namespace Backend{
namespace BackendDAVM{


class BackendDAVM : public Backend, IR::IRVisitor{
public:

  BackendDAVM(CompilationUnit& unit, IR::IRUnit& ir_unit)
  : Backend(unit, ir_unit){}

  void Run(){
    for(auto& it : ir_unit_.streams_) Visit(*it);
    
    std::cout << "---------\nBytecode:\n";
    VM::VMUtils::print(byte_code_);
  }
private:
  
  void Visit(IR::IRStream& stream){
    for(auto& it : stream) it->Accept(*this);
  }

  void Visit(const IR::Inst::Inst& inst) override{
    std::cout << "Not implemented: " << inst.str() << "\n";
  }
  void Visit(const IR::Inst::JumpCond& inst) override{
    std::cout << inst.str() << "\n";
  }
  void Visit(const IR::Inst::JumpIncond& inst) override{
    std::cout << inst.str() << "\n";
  }
  void Visit(const IR::Inst::LoadI& inst) override{
    std::cout << inst.str() << "\n";
  }
  void Visit(const IR::Inst::Load& inst) override{
    std::cout << inst.str() << "\n";
  }
  void Visit(const IR::Inst::LoadReg& inst) override{
    std::cout << inst.str() << "\n";
  }
  void Visit(const IR::Inst::LoadRegOffs& inst) override{
    std::cout << inst.str() << "\n";
  }
  void Visit(const IR::Inst::Store& inst) override{
    std::cout << inst.str() << "\n";
  }
  void Visit(const IR::Inst::StoreReg& inst) override{
    std::cout << inst.str() << "\n";
  }
  void Visit(const IR::Inst::Arith& inst) override{
    std::cout << inst.str() << "\n";
  }
  void Visit(const IR::Inst::Comparison& inst) override{
    std::cout << inst.str() << "\n";
  }  
  void Visit(const IR::Inst::PtrElem& inst) override{
    std::cout << inst.str() << "\n";
  }  
  void Visit(const IR::Inst::GetRetVal& inst) override{
    std::cout << inst.str() << "\n";
  }  
  void Visit(const IR::Inst::SetRetVal& inst) override{
    std::cout << inst.str() << "\n";
  }  
  void Visit(const IR::Inst::SetPar& inst) override{
    std::cout << inst.str() << "\n";
  }  
  void Visit(const IR::Inst::Return& inst) override{
    std::cout << inst.str() << "\n";
  }  
  
  void Visit(const IR::Inst::ReturnMain& inst) override{
    std::cout << inst.str() << "\n";
    byte_code_.Append( VM::IRBuilder::Stop());
  }  
  
  void Visit(const IR::Inst::Call& inst) override{
    std::cout << inst.str() << "\n";
  }    
  
  

  VM::ByteCode byte_code_;
  
};


}//end namespace Backend
}//end namespace Backend
}//end namespace Compiler
