#pragma once
#include "Symbol.hpp"
#include "IR/Offset.hpp"

namespace Compiler{
namespace AST{

class OffsetTable{
public:
  void StoreOffset(Symbols::SymbolId id, IR::Offset o){
    offsets_[id] = o;
  }
  
  IR::Offset Offset(Symbols::SymbolId id) const{
    return offsets_.at(id);
  }  
  
private:
  std::map<Symbols::SymbolId, IR::Offset> offsets_;

};


}//end namespace AST
}//end namespace Compiler
