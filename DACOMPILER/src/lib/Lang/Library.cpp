#include "Lang/Library.hpp"
#include "AST/ASTNodeCreation.hpp"


#include <vector>

namespace Compiler{

using namespace Common;

namespace Library{

void LangLib::InitCompilationUnit(){
  InitRTVarsDecl();
  InitRTFunctionTest();
}

void LangLib::UpdateCompilationUnitPrePasses(){
  if(unit_.HasErrors()) return;
  InitRTVarsDef();
  (unit_.GetAstProg())->AddFunction(func_def_);
}


void LangLib::InitRTFunctionTest(){

  AST::Symbols::SymbolId func_id;

  std::string name      = "__test";
  std::string name_par  = "__test_condition_par";
  const Type& ret_type = unit_.GetTypeBool();
  const Type& par_type = unit_.GetTypeBool();

  const ScopeId global_scope_id = unit_.GetGlobalLexicalScope().GetScopeId();


  std::vector<const Type*> pars;
  pars.push_back( &par_type );
  const Type& function_type = unit_.GetFuncType(ret_type, pars);

  ScopeId func_scope_id;
  const AST::Symbols::SymbolId sid_func = unit_.FreeSymbolId();
  func_scope_id = unit_.NewFunction(name, func_id);
  unit_.RegisterDecl(name, function_type, global_scope_id, sid_func);

  par_decl_.push_back(std::move(NewVarDecl(name_par, par_type, func_scope_id, l_)));
  const AST::Symbols::SymbolId sid_par = unit_.FreeSymbolId();
  unit_.RegisterDecl(name_par, par_type, func_scope_id, sid_par);
  decl_ = NewFuncDecl(name, ret_type, par_decl_, global_scope_id, l_);

  unit_.EnterFunctionDefinition(decl_.get());

  lhs_     = NewVar(name_cond_, unit_.GetTypeBool(), cond_sid_, func_scope_id, l_);
  op_lhs_  = NewVar(name_cond_, unit_.GetTypeBool(), cond_sid_, func_scope_id, l_);
  op_rhs_  = NewVar(name_par,   unit_.GetTypeBool(), sid_par,   func_scope_id, l_);
  rhs_     = NewBinaryOp(op_lhs_, BinaryOp::kAnd, op_rhs_, func_scope_id, l_);
  stmt_a_  = NewAssignStmt(lhs_, rhs_, func_scope_id, l_ );
  ret_     = NewVar(name_cond_, unit_.GetTypeBool(), cond_sid_, func_scope_id, l_);
//   stmt_r_  = NewReturnStmt(ret_, *decl_, func_scope_id, l_);

  stmts_.push_back(std::move(stmt_a_));
//   stmts_.push_back(std::move(stmt_r_));
  block_  = NewBlock(stmts_, func_scope_id, l_ );

  func_def_ = NewFuncDef(decl_, block_, global_scope_id, l_);

  unit_.SetFuncOriginNode(*func_def_);
  unit_.ExitFunctionDefinition();
  unit_.RestoreScope();

//   std::cout << "func: " << func_def->str() << "\n";


}

void LangLib::InitRTVarsDecl(){
  const Type& type  = unit_.GetTypeBool();
  cond_sid_         = unit_.FreeSymbolId();
}

void LangLib::InitRTVarsDef(){
  const ScopeId main_scope_id = unit_.GetLScope("main").GetScopeId();
  const Type& type                  = unit_.GetTypeBool();
  cond_test_var_decl_ = NewVarDecl(name_cond_, type, main_scope_id, l_);
  //unit_.GetGlobalLexicalScope().ForceRegisterDecl("main", name_cond_, type, cond_sid_);
  unit_.ForceRegisterDecl("main", name_cond_, type, main_scope_id, cond_sid_);

  std::vector<PtrVarDecl> l;
  l.push_back(std::move(cond_test_var_decl_));
  decl_list_ = NewVarDeclList(l, main_scope_id, l_);
  decl_stmt_ = NewDeclStmt(decl_list_, main_scope_id, l_);
  Block& b = unit_.GetFunc("main").GetFuncDefNode().GetBody();
  b.AddStatement(decl_stmt_);
  //
}

}//end namespace Library
}//end namespace Compiler
