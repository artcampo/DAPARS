#pragma once
#include "Symbol.hpp"
#include "IR/Offset.hpp"
#include "IR/Label.hpp"
#include "IR/MemAddr.hpp"
#include "IR/IRSubtypes.hpp"

namespace Compiler{
namespace AST{

//This represents a contigous allocation of variables  
class AddressTable{
public:
  AddressTable() : total_size_(0){}
  
  void StoreOffset(Symbols::SymbolId id, const IR::Offset o
                 , const IR::Label l,  const size_t size){
    offsets_[id]  = IR::MemAddr(l, o);
    total_size_  += size;
  }
  
  IR::Offset Offset(Symbols::SymbolId id) const{
    return offsets_.at(id).GetOffset();
  }  
  
  size_t Size() const noexcept{ return total_size_; }
  
private:
  std::map<Symbols::SymbolId, IR::MemAddr> offsets_;
  size_t total_size_;
};


}//end namespace AST
}//end namespace Compiler
