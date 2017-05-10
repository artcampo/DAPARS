#pragma once
#include "Symbol.hpp"
#include "SymbolTable.hpp"
#include "ErrorLog.hpp"
#include "TreeDecoration.hpp"
#include "AST/AST.hpp"
#include "Scopes/Scope.hpp"
#include "Scopes/LexicalScope.hpp"
#include "IR/Label.hpp"
#include "Module/TypeTable.hpp"
#include "Module/LabelManager.hpp"
#include "Module/FunctionManager.hpp"
#include "Module/ClassManager.hpp"
#include "Module/ScopeManager.hpp"
#include "Lang/Library.hpp"
#include <map>
#include <memory>
#include <iterator>
#include <stack>
#include <cassert>

namespace Compiler{

using AST::Ast;
using AST::LexicalScope;
using AST::Type;
using AST::Node;
using AST::Var;
using AST::Expr;
using AST::ProgBody;
using AST::ScopeId;
using AST::SymbolTable;
using AST::DeclarationTable;
using namespace Compiler::AST::Ptrs;
// class Compiler::AST::Dump;
// class ASTVisitor;
// namespace AST{ class Dump; class Function;};
namespace AST{ class Dump;};

using AST::PtrLexicalScope;
using AST::Scope;
// using AST::AddressTable;
using IR::Label;

class CompilationUnit : public TreeDecoration, public TypeTable
  , public LabelManager, public ScopeManager, public FunctionManager
  , public ClassManager
  {
public:

  CompilationUnit():
      TypeTable(error_log_)
    , ScopeManager()
    , FunctionManager()
    , ClassManager(this, this, this, this)
    , lang_lib_(*this)
    , ast_()
    , free_symbol_id_(AST::Symbols::Symbol::InitialFreeId())
    , module_scope_(std::move(GlobalLexicalScope(symbol_table_, module_declaration_table_)))
    , module_declaration_table_()
    {
      scope_by_id_[GlobalScopeId()] = module_scope_.get();
      current_scope_ = module_scope_.get();
      lang_lib_.InitCompilationUnit();
    }

  //TODO: get rid of these
  const ScopeId NewFunction(const std::string& name
    , const AST::Symbols::SymbolId symbol_id){
    const ScopeId func_scope = NewNestedScopeFunction(name);
    const Label entry        = NewFunctionEntryLabel(name);
    Label local;
    if(name == "main")  local = GetLabelMainLocals();
    else                local = NewFunctionARLabel(name);
    FunctionManager::NewFunction(name, symbol_id, ModuleAddressTable()
      , entry, local, GetLScope(name));
    return func_scope;
  }


  const ScopeId NewFunction(const std::string& name
    , const AST::Symbols::SymbolId symbol_id
    , const std::string& class_name){
    const ScopeId func_scope       = NewNestedScopeFunction(name);
    const std::string mangled_name = Function::MangledName(name, class_name);
    const Label entry = NewFunctionEntryLabel(mangled_name);
    Label local       = NewFunctionARLabel(mangled_name);
    Function& f = FunctionManager::NewFunction(name, symbol_id, class_name
      , ModuleAddressTable(), entry, local, GetLScope(name));
    return func_scope;
  }

  const Type& GetType(const std::string& name, const ScopeId scope_id){
//     if(ScopeIsLexical(scope_id))
    return GetScope(scope_id)->GetType(name);
  }
  const Type& GetType(const TypeId id) const{ return TypeTable::GetType(id);}

  const AST::Symbols::SymbolId FreeSymbolId() noexcept{return free_symbol_id_++;}

  //Used while parsing
  bool RegisterDecl(const std::string& name, const Type& type
    , const ScopeId scope_id, const AST::Symbols::SymbolId symbol_id){
    bool registered = GetScope(scope_id)->RegisterDecl(name, type, symbol_id);
//     std::cout << "Register: " << name << " in " << GetScope(scope_id)->str()<<"\n";
    return registered;
  }

  //TODO: should StoreDecl instead be called from the given scope,
  //or forbid direct calls to scope's register
  //Used outside of parsing, when we want to force variables into a given function
  bool ForceRegisterDecl(const std::string& function_name, const std::string& name, const Type& type
    , const ScopeId scope_id, const AST::Symbols::SymbolId symbol_id){
    bool registered = GetScope(scope_id)->RegisterDecl(name, type, symbol_id);
    return registered;
  }

  void UpdatePrePasses(){
    lang_lib_.UpdateCompilationUnitPrePasses();
  }

  //TODO: const?
  ScopeId GlobalScopeId() { return module_scope_->GetScopeId();}

private:
  AST::Symbols::SymbolId  free_symbol_id_;
  SymbolTable       symbol_table_;
  DeclarationTable  module_declaration_table_;
  ErrorLog          error_log_;
  Library::LangLib  lang_lib_;
  Ast               ast_;
  PtrLexicalScope   module_scope_;

public:
  friend class AST::Dump;

  using iterator = std::vector<PtrFunction>::iterator;
  using const_iterator = std::vector<PtrFunction>::const_iterator;

  iterator begin() { return functions_.begin(); }
  iterator end()   { return functions_.end(); }
  const_iterator begin()  const { return functions_.begin(); }
  const_iterator end()    const { return functions_.end(); }
  const_iterator cbegin() const { return functions_.cbegin(); }
  const_iterator cend()   const { return functions_.cend(); }

  const bool ValidAst() const noexcept { return ast_.prog_ != nullptr; }

  void InitAst(PtrProgBody& prog){
    ast_.prog_ = std::move(prog);
  }

//   const Prog* GetAstProg() const noexcept{ return *ast_.prog_;}
  ProgBody* GetAstProg() noexcept{ return ast_.prog_.get();}

  void SetFileData(std::vector<char>* file_data){
    error_log_.SetFileData(file_data);
  }

  void Error(const std::string& message, const Locus& locus){
    error_log_.Error(message, locus);
  }

  void Error(const std::string& message){
    error_log_.Error(message);
  }

  bool HasErrors() const noexcept {return error_log_.NumErrors() > 0;}
};

// namespace CompilationUnitInfo{
  enum class CompUnitInfo : size_t{
      kAst = 0
    , kTypeOfNode = 1
    , kLnessRnessOfNode = 2
    , kLocalVarOffsets = 3
    , kVarAccessIsReadOrWrite = 4
    , kVarAccessIsValOrAddress = 5
    , kIR = 6
    , kAstIncomplete1 = 7
    , kAstIncomplete2 = 8
    , kTypeCheck = 9
    , kVarIsMember = 10
    , kLexicalScopeOfNode = 11
  };
// }

}//end namespace Compiler
