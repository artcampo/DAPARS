#pragma once
#include <map>
#include <cstddef>
#include <memory>
#include <vector>
#include "Symbol.hpp"
#include "Types.hpp"
#include "Scopes/ScopeId.hpp"
#include "SymbolTable.hpp"


namespace Compiler{
namespace AST{

// using namespace Symbols;
using ScopeId = size_t;
//pair of name and symbol that was shadowed (or -1 if none)
using InsertedSymbol = std::pair<Symbols::SymbolString, Symbols::SymbolId>;
using InsertedDeclarations = std::pair<Symbols::SymbolId, Symbols::Symbol&>;

class LexicalScope;
class HierarchicalScope;
using PtrLexicalScope = std::unique_ptr<LexicalScope>;
using PtrHierarchicalScope = std::unique_ptr<HierarchicalScope>;


class Scope {
public:
  Scope(const ScopeId id) : id_(id){}
  virtual ~Scope() = default;

  const ScopeId GetScopeId() const noexcept{return id_;};

  virtual const Symbols::SymbolId DeclId(const std::string& name) const = 0;
  virtual bool IsDeclValid(const std::string& name) = 0;
  virtual bool HasDecl(const std::string& name) = 0;
  virtual bool RegisterDecl(const std::string& name, const Type& type
      , AST::Symbols::SymbolId symbol_id) = 0;
  virtual std::string str() const noexcept = 0;




  virtual const Type& GetType(const std::string& name) const = 0;
  virtual const Symbols::Symbol& PostParseDecl(const std::string& name) const = 0;

protected:
  ScopeId       id_;


};


namespace DumpScope{
void Dump(const std::map<Symbols::SymbolId, std::unique_ptr<Symbols::Symbol>>& dec);
}//end namespace DumpScope

}//end namespace AST
}//end namespace Compiler
