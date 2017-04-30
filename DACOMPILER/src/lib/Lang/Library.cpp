#include "Lang/Library.hpp"
#include "AST/ASTNodeCreation.hpp"


#include <vector>

namespace Compiler{

using namespace Common;

namespace Library{

void LangLib::InitCompilationUnit(){
  InitRTVars();
  InitRTFunctionTest();
}

void LangLib::InitRTFunctionTest(){

  AST::Symbols::SymbolId func_id;

  std::string name      = "test";
  std::string name_par  = "condition";
  const Type& ret_type = unit_.GetTypeVoid();
  const Type& par_type = unit_.GetTypeBool();

  const ScopeId global_scope_id = unit_.GetGlobalLexicalScope().GetScopeId();


  std::vector<const Type*> pars;
  pars.push_back( &par_type );
  const Type& function_type = unit_.GetFuncType(ret_type, pars);

  ScopeId func_scope_id;
  const AST::Symbols::SymbolId sid = unit_.FreeSymbolId();
  func_scope_id = unit_.NewFunction(name, func_id);

  std::vector<PtrVarDecl> par_decl;
  par_decl.push_back(std::move(NewVarDecl(name_par, par_type, global_scope_id, l_)));
  PtrFuncDecl decl = NewFuncDecl(name, ret_type, par_decl, global_scope_id, l_);

  unit_.EnterFunctionDefinition(decl.get());

  PtrExpr lhs     = NewVar(name_cond_, unit_.GetTypeBool(), cond_sid_, func_scope_id, l_);
  PtrExpr op_lhs  = NewVar(name_cond_, unit_.GetTypeBool(), cond_sid_, func_scope_id, l_);
  PtrExpr op_rhs  = NewVar(name_par,   unit_.GetTypeBool(), cond_sid_, func_scope_id, l_);
  PtrExpr rhs     = NewBinaryOp(op_lhs, BinaryOp::kAnd, op_rhs, func_scope_id, l_);
  PtrStatement a  = NewAssignStmt(lhs, rhs, func_scope_id, l_ );
  std::vector<PtrStatement> stmts; stmts.push_back(std::move(a));
  PtrBlock block  = NewBlock(stmts, func_scope_id, l_ );

  PtrFuncDef func_def = NewFuncDef(decl, block, global_scope_id, l_);

  unit_.SetFuncOriginNode(*func_def);
  unit_.ExitFunctionDefinition();
  unit_.RestoreScope();
  unit_.RegisterDecl(name, function_type, *decl, global_scope_id, sid);
}

void LangLib::InitRTVars(){

  const Type& type                  = unit_.GetTypeBool();
  const ScopeId global_scope_id     = unit_.GetGlobalLexicalScope().GetScopeId();
  cond_sid_  = unit_.FreeSymbolId();

  cond_test_var_decl_ = NewVarDecl(name_cond_, type, global_scope_id, l_);
  unit_.GetGlobalLexicalScope().RegisterDecl(name_cond_, type, *cond_test_var_decl_, cond_sid_);
}

}//end namespace Library
}//end namespace Compiler
