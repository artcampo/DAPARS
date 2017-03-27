#pragma once
#include "Types.hpp"
#include "Symbol.hpp"
#include "AST/AST.hpp"
#include "AST/Node.hpp"
#include "IR/Label.hpp"
#include "IR/Offset.hpp"
#include "Module/TypeTable.hpp"
#include "Scopes/Scope.hpp"
#include "Scopes/ScopeId.hpp"
#include "Scopes/HierarchicalScope.hpp"
// #include "Module/FunctionManager.hpp"
#include <map>
#include <memory>

namespace Compiler{

class FunctionManager;

namespace AST{

using AST::Ast;
using AST::Type;
using AST::Node;
using AST::ClassDef;
using AST::ScopeId;
using AST::HierarchicalScope;
using namespace Compiler::AST::Ptrs;
using IR::Label;
using IR::Offset;

class Class;
using PtrClass = std::unique_ptr<Class>;



class Class{
public:

  Class(const std::string& name
    , const AST::Symbols::SymbolId symbol_id
    , const ScopeOwnerId scope_owner_id
    , const ScopeId scope_id
    , HierarchicalScope& scope
    , const IR::Label this_label
    , std::vector<Class*>& parents
    , ClassDef& class_def
    , FunctionManager* func_manager)
  : name_(name)
    , symbol_id_(symbol_id)
    , scope_owner_id_(scope_owner_id)
    , scope_(scope)
    , this_label_(this_label)
    , parents_(parents)
    , class_def_(class_def)
    , func_manager_(func_manager){
    BuildObjectRecord(class_def_);
    BuildFunctionsReferences();
  }

  HierarchicalScope& GetHScope() const noexcept{ return scope_;}

  const IR::Label ThisLabel() const noexcept{ return this_label_; }
  const IR::Offset MemberVarOffset(AST::Symbols::SymbolId& sid) const noexcept{
//     std::cout << "Asking: " << sid << "\n" << "in ";
//     for(const auto& it : offset_of_var_) std::cout << it.first << ": " << it.second.str() << "\n";
    return offset_of_var_.at(sid);
  }


  Function& GetFunction(const std::string& name)const{
    return *function_by_name_.at(name);
  }

  std::string Name()  const noexcept{return name_;}
  
  std::string str()  const noexcept{ 
    if(parents_.empty()) return name_;
    std::string n = name_ + ":"; 
    bool first = true;
    for(auto& parent : parents_){
      if(not first) n += ",";
      n += parent->str();
    }
    return n;
  }
  const size_t Size() const noexcept{ return class_size_;}
private:
  ScopeOwnerId            scope_owner_id_;
  std::string             name_;
  AST::Symbols::SymbolId  symbol_id_;
  HierarchicalScope&      scope_;
  IR::Label               this_label_;
  std::vector<Class*>     parents_;
  ClassDef&               class_def_;
  
  //Data computed
  size_t  class_size_;
  std::map<AST::Symbols::SymbolId, IR::Offset> offset_of_var_;
  //Each entry is a triplet (sid,offset,size)
  std::vector<std::pair<AST::Symbols::SymbolId, std::pair<IR::Offset, size_t>>> object_record_;
  //TODO: should use function sid instead
  std::map<std::string, Function*> function_by_name_;


  void InsertObjectRecord(const AST::Symbols::SymbolId sid, const Offset offset
    , const size_t size){
    offset_of_var_[sid] = offset;
    object_record_.push_back( {sid, {offset, size} });    
  }
  void BuildObjectRecord(const ClassDef& class_def){
    size_t offset       = RtiSize();
    size_t next_offset  = offset; //offset after processing current parent
    
    //Insert parent's variables
    for(const auto& p : parents_){
      for(const auto& sid_offset_size : p->object_record_){
        const size_t size = sid_offset_size.second.second;
        const Offset orig = sid_offset_size.second.first;
        const Offset o    = Offset(orig.GetAddr() + offset, orig.Name());
        InsertObjectRecord( sid_offset_size.first, o, size);
        next_offset += size;
      }
      offset = next_offset;
    }
    //Insert own variables
    for( const auto& it : class_def.GetVarDecl()){
      const std::string name  = it->Name();
      const Type& type        = it->GetType();
      const size_t size       = type.Size();
      
      InsertObjectRecord(scope_.DeclId(name), Offset(offset, name), size);
      offset += size;
    }
    class_size_ = offset;
  }

  void BuildFunctionsReferences(){
    for( const auto& it : class_def_){
      Function& f  = func_manager_->GetFunc(*it);
      AddFunction(f);
    }
  }
  void AddFunction(Function& f){
    function_by_name_[std::string(f.Name())] = &f;
  }
  
  const size_t RtiSize() const noexcept{ return 0;}

  FunctionManager* func_manager_;
  /*
  Symbols::Symbol& GetSymbolDecl(const Node& n) const{
//     std::cout << "Asking n: " << &n << " " << n.str() << std::endl;
    return *symbol_decl_of_node_.at(&n);
  }

  void StoreDecl(Symbols::Symbol& s, const Node& n){
    symbol_decl_of_node_[&n] = &s;
  }
  */

/*
  void SetOriginNode(const FuncDef& n){
    origin_node_ = const_cast<FuncDef*>(&n);
  }
  */

};


}//end namespace AST
}//end namespace Compiler
