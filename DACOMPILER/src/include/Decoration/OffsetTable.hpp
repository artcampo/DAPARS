#pragma once
#include "Symbol.hpp"
#include "IR/Offset.hpp"
#include "IR/IRSubtypes.hpp"

namespace Compiler{
namespace AST{

//This represents a contigous allocation of variables  
class OffsetTable{
public:
  OffsetTable() : total_size_(0){}
  
  void StoreOffset(Symbols::SymbolId id, IR::Offset o, const size_t size){
    offsets_[id]  = o;
    total_size_  += size;
  }
  
  IR::Offset Offset(Symbols::SymbolId id) const{
    return offsets_.at(id);
  }  
  
  
private:
  std::map<Symbols::SymbolId, IR::Offset> offsets_;
  IR::AddrOffset size_;
  size_t total_size_;
};


}//end namespace AST
}//end namespace Compiler
