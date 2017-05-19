#pragma once
#include "IR/IRStream.hpp"

#include <vector>
#include <memory>

namespace Compiler{
namespace IR{


struct IRUnit{
public:
  IRUnit() = default;
  ~IRUnit() = default;

  void NewStream(const Label& entry_label, AST::Function& function){
    streams_.push_back( std::move(
      std::make_unique<IR::IRStream>(entry_label, function) ));
  }

  std::vector<PtrIRStream> streams_;

  void ComputeBeginBB(){
    for(auto& it : streams_) it->ComputeBeginBB();
  }


};


}//end namespace IR
}//end namespace Compiler
