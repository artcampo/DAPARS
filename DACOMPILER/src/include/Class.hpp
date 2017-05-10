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
    , const ScopeId scope_id
    , HierarchicalScope& scope
    , const IR::Label this_label
    , std::vector<Class*>& parents
    , ClassDef& class_def
    , FunctionManager* func_manager)
  : name_(name)
    , symbol_id_(symbol_id)
    , scope_(scope)
    , this_label_(this_label)
    , parents_(parents)
    , class_def_(class_def)
    , func_manager_(func_manager){
    if(not parents_.empty()) std::cout << "ObjRec for " + str() << " -> ";
    BuildFuncsNotInht();
    BuildObjectRecord(class_def_);
    if(not parents_.empty()) std::cout << "\n";
    BuildFunctionsReferences();
  }

  HierarchicalScope& GetHScope() const noexcept{ return scope_;}

  const IR::Label ThisLabel() const noexcept{ return this_label_; }
  const IR::Offset MemberVarOffset(AST::Symbols::SymbolId& sid) const noexcept{
//     std::cout << "Asking: " << sid << "\n" << "in ";
//     for(const auto& it : offset_of_var_) std::cout << it.first << ": " << it.second.str() << "\n";
    return offset_of_var_.at(sid);
  }


  const bool IsCallToParent(const std::string& name)const{
    auto it = offset_of_func_.find(name);
    return it != offset_of_func_.end();
  }
  const IR::Offset FuncPtrThisAdjust(const std::string& name)const{
    return offset_of_func_.at(name);
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

  std::vector<Function*>  GetFuncsNotInherited() noexcept{ return functions_not_inherited_;}
  const std::vector<Function*>  GetFuncsNotInherited() const noexcept{ return functions_not_inherited_;}
private:
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
  std::map<std::string, IR::Offset> offset_of_func_;
  std::map<std::string, Function*> function_by_name_;
  std::vector<Function*> functions_;
  std::vector<Function*> functions_not_inherited_;


  void InsertObjectRecord(const AST::Symbols::SymbolId sid, const Offset offset
    , const size_t size){
    offset_of_var_[sid] = offset;
    object_record_.push_back( {sid, {offset, size} });
  }
  void BuildObjectRecord(const ClassDef& class_def){

    size_t offset       = RtiSize();
    size_t next_offset  = offset; //offset after processing current parent

    //Insert parent's functions and variables
    for(const auto& p : parents_){
      //correction of this parent's function is current offset
      //this has to use explicitely the functions of parent not inherited from
      //the parent's parent
      //for(const auto& f : p->GetFuncsNotInherited()){
      for(const auto& f : *p){
        AddFunction(*f);
        offset_of_func_[f->Name()] = offset;
        std::cout <<  "(" << f->Name() << ", " << offset << ") ";
      }

      //variables adapted to child layout
      for(const auto& sid_offset_size : p->object_record_){
        const size_t size = sid_offset_size.second.second;
        const Offset orig = sid_offset_size.second.first;
        const Offset o    = Offset(orig.GetAddr() + offset, orig.Name());
        InsertObjectRecord( sid_offset_size.first, o, size);
        next_offset += size;
        std::cout << "("<< orig.Name() << ", " << o.GetAddr() << ") ";
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
    functions_.push_back(&f);
  }

  void BuildFuncsNotInht(){
    for( const auto& it : class_def_)
      functions_not_inherited_.push_back(&func_manager_->GetFunc(*it));
  }

  const size_t RtiSize() const noexcept{ return 0; }

  FunctionManager* func_manager_;

public:
  using iterator = std::vector<Function*>::iterator;
  using const_iterator = std::vector<Function*>::const_iterator;

  iterator begin() { return functions_.begin(); }
  iterator end()   { return functions_.end(); }
  const_iterator begin()  const { return functions_.begin(); }
  const_iterator end()    const { return functions_.end(); }
  const_iterator cbegin() const { return functions_.cbegin(); }
  const_iterator cend()   const { return functions_.cend(); }


/*
  void SetOriginNode(const FuncDef& n){
    origin_node_ = const_cast<FuncDef*>(&n);
  }
  */

};


}//end namespace AST
}//end namespace Compiler
