#pragma once
#include "AST/AST.hpp"
#include "Types.hpp"
#include "Symbol.hpp"
#include "AST/Node.hpp"
#include "Symbol.hpp"
#include "Module/TypeTable.hpp"
#include "IR/Offset.hpp"
#include "IR/Label.hpp"
#include "Scopes/Scope.hpp"
#include "Scopes/LexicalScope.hpp"
#include "Decoration/AddressTable.hpp"
#include <map>
#include <memory>

namespace Compiler{
namespace AST{

using AST::Ast;
using AST::Type;
using AST::Node;
using AST::ProgBody;
using AST::ScopeId;
using namespace Compiler::AST::Ptrs;
using AST::FuncDef;
using IR::Label;

class Function;
using PtrFunction = std::unique_ptr<Function>;


class Function{
public:
  static PtrFunction NewFunction(const std::string& name
    , const AST::Symbols::SymbolId symbol_id , AddressTable& module_offset_table
    , const Label entry_label, const Label locals_label, LexicalScope& scope){
    return std::move( std::make_unique<Function>
      (name, symbol_id, "", name, module_offset_table, entry_label
      , locals_label, false, scope));
  }

  static PtrFunction NewMemberFunction(const std::string& name
    , const AST::Symbols::SymbolId symbol_id
    , const std::string& class_name
    , AddressTable& module_offset_table
    , const Label entry_label
    , const Label locals_label
    , LexicalScope& scope){
    return std::move( std::make_unique<Function>
      (name, symbol_id, class_name, MangledName(name, class_name), module_offset_table
      , entry_label, locals_label, true, scope));
  }

  const static std::string MangledName(const std::string& name
    , const std::string& class_name){
    return class_name + "::" + name;
  }

  FuncDef& GetFuncDefNode() { return *origin_node_; }
  const FuncDef& GetFuncDefNode() const { return *origin_node_; }


  const Symbols::Symbol& GetSymbolDecl(const NamedNode& n) const{
    //use scope_
    return scope_.PostParseDecl(n.Name());
//     std::cout << "Asking n: " << &n << " " << n.str() << std::endl;
//     return *symbol_decl_of_node_.at(&n);
  }

  void SetOriginNode(const FuncDef& n){
    origin_node_ = const_cast<FuncDef*>(&n);
  }


  AddressTable& LocalVars() { return locals_offset_table_; }
  const AddressTable& LocalVars() const { return locals_offset_table_; }
  AddressTable& Params() { return params_offset_table_; }
  const AddressTable& Params() const { return params_offset_table_; }


  void StoreSymbolAddress(Symbols::SymbolId id, const IR::Offset o
                        , const Label l, const size_t size, AddressTable& table){
//     std::cout << "Store: " << id << " o: " << o.str() << std::endl;
    table.StoreOffset(id, o, l, size);
    module_offset_table_.StoreOffset(id, o, l, size);
  }

  IR::MemAddr LocalVarMemAddr(Symbols::SymbolId id) const{
    return locals_offset_table_.GetMemAddr(id);
  }

  const Label      EntryLabel() const noexcept{ return entry_label_;}
  const Label      LocalsLabel() const noexcept{ return locals_label_;}

  std::string str()  const noexcept{ return name_;}

  std::string MangledName()const noexcept{ return mangled_name_;}
  const bool IsMember() const noexcept{ return is_member_;}
  const bool IsMain() const noexcept{ return is_main_;}

  std::string Name()  const noexcept{ return name_;}

  const bool HasLocals() const noexcept{ return locals_offset_table_.NumVars() > 0;}
  const bool HasArguments() const noexcept{ return params_offset_table_.NumVars() > 0;}


  bool  IsCompilerPrivate() const{ return name_.compare(0, 2, "__") == 0; }
private:

  std::string       name_;
  std::string       class_name_;
  std::string       mangled_name_;
  AST::Symbols::SymbolId symbol_id_;

  FuncDef*          origin_node_;
  const Label       entry_label_;
  const Label       locals_label_;
  bool              is_member_;
  bool              is_main_;
  LexicalScope&     scope_;

  AddressTable&      module_offset_table_;
  AddressTable       locals_offset_table_;
  AddressTable       params_offset_table_;

public:
  Function(const std::string& name
    , const AST::Symbols::SymbolId symbol_id
    , const std::string& class_name
    , const std::string& mangled_name
    , AddressTable& module_offset_table
    , const Label entry_label
    , const Label locals_label
    , const bool is_member
    , LexicalScope& scope)
  : name_(name)
    , class_name_(class_name)
    , symbol_id_(symbol_id)
    , origin_node_(nullptr)
    , module_offset_table_(module_offset_table)
    , entry_label_(entry_label)
    , locals_label_(locals_label)
    , is_member_ (is_member)
    , mangled_name_(mangled_name)
    , is_main_(name == "main")
    , scope_(scope){}

};


}//end namespace AST
}//end namespace Compiler
