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

  std::string name      = "__test";
  std::string name_par  = "__test_condition_par";
  const Type& ret_type = unit_.GetTypeVoid();
  const Type& par_type = unit_.GetTypeBool();

  const ScopeId global_scope_id = unit_.GetGlobalLexicalScope().GetScopeId();


  std::vector<const Type*> pars;
  pars.push_back( &par_type );
  const Type& function_type = unit_.GetFuncType(ret_type, pars);

  ScopeId func_scope_id;
  const AST::Symbols::SymbolId sid_func = unit_.FreeSymbolId();
  func_scope_id = unit_.NewFunction(name, func_id);


  par_decl_.push_back(std::move(NewVarDecl(name_par, par_type, func_scope_id, l_)));
  const AST::Symbols::SymbolId sid_par = unit_.FreeSymbolId();
  unit_.RegisterDecl(name_par, par_type, *par_decl_[0], func_scope_id, sid_par);
  decl_ = NewFuncDecl(name, ret_type, par_decl_, global_scope_id, l_);

  unit_.EnterFunctionDefinition(decl_.get());

  lhs_     = NewVar(name_cond_, unit_.GetTypeBool(), cond_sid_, func_scope_id, l_);
  op_lhs_  = NewVar(name_cond_, unit_.GetTypeBool(), cond_sid_, func_scope_id, l_);
  op_rhs_  = NewVar(name_par,   unit_.GetTypeBool(), cond_sid_, func_scope_id, l_);
  rhs_     = NewBinaryOp(op_lhs_, BinaryOp::kOr, op_rhs_, func_scope_id, l_);
  stmt_    = NewAssignStmt(lhs_, rhs_, func_scope_id, l_ );

  stmts_.push_back(std::move(stmt_));
  block_  = NewBlock(stmts_, func_scope_id, l_ );

  func_def_ = NewFuncDef(decl_, block_, global_scope_id, l_);

  unit_.SetFuncOriginNode(*func_def_);
  unit_.ExitFunctionDefinition();
  unit_.RestoreScope();
  unit_.RegisterDecl(name, function_type, *decl_, global_scope_id, sid_func);
//   std::cout << "func: " << func_def->str() << "\n";
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
