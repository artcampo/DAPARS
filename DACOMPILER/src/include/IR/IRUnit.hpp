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
  std::vector<bool> is_begin_bb_;

bool IsInstBeginBB(const Addr addr) const noexcept{ return is_begin_bb_[addr];}

void ComputeBeginBB(){
//   std::cout << "CompBB " << stream_.size()<<"\n";
  size_t size = 0;
  for(const auto& s : streams_) size += s->Size();
  is_begin_bb_.resize(size, false);

  IR::Addr offset = 0;
  for(const auto& s : streams_){
    is_begin_bb_[offset] = true;
    for(const auto& inst : *s){

      if(inst->IsJump()){
        const Inst::Jump& jump = dynamic_cast<const Inst::Jump&>(*inst);
        is_begin_bb_[jump.GetTarget()] = true;
      }
      offset++;
    }
  }
//   DumpBeginBB();
}

void DumpBeginBB(){
  IR::Addr offset = 0;
  for(const auto& s : streams_)
    for(const auto& inst : *s){
      if(is_begin_bb_[offset]) std::cout << "BeginBB: " << offset << " : " << inst->str() << "\n";
      ++offset;
    }
}


};


}//end namespace IR
}//end namespace Compiler
