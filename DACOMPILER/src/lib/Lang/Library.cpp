#include "Lang/Library.hpp"
#include "Symbol.hpp"
#include "AST/ASTNodeCreation.hpp"
#include "Locus.hpp"
#include "AST/Node.hpp"
#include <vector>

namespace Compiler{

using namespace Common;

namespace Library{

void InitCompilationUnit(CompilationUnit& unit){
  return;

  AST::Symbols::SymbolId func_id;

  std::string name      = "test";
  std::string name_par  = "condition";


  /*
  (unit.GetAstProg())->AddFunction(
    NewFuncDef(
      NewFuncDecl(name
                , ret_type
                ,

    */

  const ScopeId global_scope_id = unit.GetGlobalLexicalScope().GetScopeId();
  const Locus l;
  const Type& ret_type = unit.GetTypeVoid();
  const Type& par_type = unit.GetTypeBool();
  std::vector<const Type*> pars;
  pars.push_back( &par_type );
  const Type& function_type = unit.GetFuncType(ret_type, pars);

  ScopeId func_scope_id;
  const AST::Symbols::SymbolId sid = unit.FreeSymbolId();
  func_scope_id = unit.NewFunction(name, func_id);

  std::vector<PtrVarDecl> par_decl;
  par_decl.push_back(std::move(NewVarDecl(name_par, par_type, global_scope_id, l)));
  PtrFuncDecl decl = NewFuncDecl(name, ret_type, par_decl, global_scope_id, l);

//   const Node& node;




  //( &unit.GetTypeBool() );



//   std::vector<PtrVarDeclList> par_list;
//   par_list.push_back(std::move(NewVarDeclList(par_decl, global_scope_id, l)));



//   func_id = unit.GetGlobalLexicalScope()
//     .RegisterDecl( name, node, function_type
//   unit.NewFunction( "test",



}

}//end namespace Library
}//end namespace Compiler
