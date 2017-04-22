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
  
  IR::MemAddr GetMemAddr(Symbols::SymbolId id) const{
    return offsets_.at(id);
  }  
  
  int     NumVars() const noexcept{ return offsets_.size(); }
  size_t  Size() const noexcept{ return total_size_; }
  
  using iterator = std::map<Symbols::SymbolId, IR::MemAddr>::iterator;
  using const_iterator = std::map<Symbols::SymbolId, IR::MemAddr>::const_iterator;

  iterator begin() { return offsets_.begin(); }
  iterator end()   { return offsets_.end(); }
  const_iterator begin()  const { return offsets_.begin(); }
  const_iterator end()    const { return offsets_.end(); }
  const_iterator cbegin() const { return offsets_.cbegin(); }
  const_iterator cend()   const { return offsets_.cend(); }    
  
private:
  std::map<Symbols::SymbolId, IR::MemAddr> offsets_;
  size_t total_size_;
};


}//end namespace AST
}//end namespace Compiler
