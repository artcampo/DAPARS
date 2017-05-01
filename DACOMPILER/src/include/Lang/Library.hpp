#pragma once
#include "AST/Node.hpp"
#include "Locus.hpp"
#include "Symbol.hpp"

namespace Compiler{

class CompilationUnit;

namespace Library{

class LangLib{

public:
  LangLib(CompilationUnit& unit) : unit_(unit), name_cond_("__test_result"){}
  void InitCompilationUnit();
  void UpdateCompilationUnitPrePasses();

private:
  CompilationUnit&  unit_;

  Locus             l_;

  std::string name_cond_;
  AST::Symbols::SymbolId cond_sid_;
  AST::PtrVarDecl   cond_test_var_decl_;
  AST::PtrFuncDecl  decl_;
  AST::PtrFuncDef   func_def_;
  AST::PtrBlock block_;
  std::vector<AST::PtrVarDecl> par_decl_;
  std::vector<AST::PtrStatement> stmts_;
  AST::PtrExpr lhs_;
  AST::PtrExpr op_lhs_;
  AST::PtrExpr op_rhs_;
  AST::PtrExpr rhs_;
  AST::PtrExpr ret_;
  AST::PtrStatement stmt_a_;
  AST::PtrStatement stmt_r_;
  AST::PtrVarDeclList decl_list_;
  AST::PtrStatement decl_stmt_;

  void InitRTVarsDecl();
  void InitRTVarsDef();
  void InitRTFunctionTest();
};

}//end namespace Library
}//end namespace Compiler
