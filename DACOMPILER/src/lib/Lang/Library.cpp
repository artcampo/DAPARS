#include "Lang/Library.hpp"
#include "Symbol.hpp"
#include "AST/ASTNodeCreation.hpp"
#include "Locus.hpp"
#include "AST/Node.hpp"
#include <vector>

namespace Compiler{
namespace Library{

void InitCompilationUnit(CompilationUnit& unit){
  return;

  AST::Symbols::SymbolId func_id;

  std::string name = "test";

  /*
  (unit.GetAstProg())->AddFunction(
    NewFuncDef(
      NewFuncDecl(name
                , ret_type
                ,

    */

  ScopeId func_scope_id;
  const AST::Symbols::SymbolId sid = unit.FreeSymbolId();
  func_scope_id = unit.NewFunction(name, func_id);

  const ScopeId global_scope_id = unit.GetGlobalLexicalScope().GetScopeId();
  const Locus l;
//   const Node& node;

  const Type& ret_type = unit.GetTypeVoid();
  const std::vector<const Type*> pars;
  //( &unit.GetTypeBool() );
  const Type& function_type = unit.GetFuncType(ret_type, pars);

  std::vector<PtrVarDecl> par_list;

  PtrFuncDecl decl = std::make_unique<AST::FuncDecl>(name, ret_type, par_list, global_scope_id, l);

//   func_id = unit.GetGlobalLexicalScope()
//     .RegisterDecl( name, node, function_type
//   unit.NewFunction( "test",



}

}//end namespace Library
}//end namespace Compiler
