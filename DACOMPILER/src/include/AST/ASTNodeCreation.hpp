#pragma once
#include <memory>
#include <vector>
#include <string>
#include "AST/AST.hpp"
#include "CompilationUnit.hpp"
#include "Locus.hpp"
#include "Symbol.hpp"
#include "AST/ASTNodes.hpp"
#include "Types.hpp"
#include "Locus.hpp"


namespace Common{

// using namespace Tokenizer;
using namespace Compiler::AST;
using Compiler::CompilationUnit;
using Compiler::Locus;
using namespace Compiler::AST::Ptrs;


PtrBinaryOp
NewBinaryOp(PtrExpr& lhs, const int op
  , PtrExpr& rhs, const ScopeId id, const Locus& locus);


PtrVar
NewVar(const std::string& name, const Type& type
  , Compiler::AST::Symbols::SymbolId id
  , const ScopeId scope_id, const Locus& locus);

PtrLiteral
NewLiteral(const uint32_t &value, const Type& type
  , const ScopeId id, const Locus& locus);

PtrBlock
NewBlock(std::vector<PtrStatement>& stmts_inht
  , const ScopeId id, const Locus& locus);

PtrIfStmt
NewIfStmt(PtrExpr& condition, PtrBlock& block1
  , const ScopeId id, const Locus& locus);

PtrIfStmt
NewIfStmt(PtrExpr& condition, PtrBlock& block1
  , PtrBlock& block2, const ScopeId id, const Locus& locus);

PtrDeclStmt
NewDeclStmt(PtrVarDeclList& list, const ScopeId id
  , const Locus& locus);

PtrVarDeclList
NewVarDeclList(std::vector<PtrVarDecl>& list
  , const ScopeId id, const Locus& locus);

PtrVarDecl
NewVarDecl(const std::string& name
                              , const Type& type
                              , const ScopeId id
                               , const Locus& locus);

PtrAssignStmt
NewAssignStmt(PtrExpr& lhs
                                    , PtrExpr& rhs
                                     , const ScopeId id
                                     , const Locus& locus);

PtrWhileStmt
NewWhileStmt(PtrExpr& condition, PtrBlock& body
    , const ScopeId id, const Locus& locus);

PtrNotOp
NewNotOp(PtrExpr& rhs, const ScopeId id, const Locus& locus);

PtrRefOp
NewRefOp(PtrExpr& rhs, const ScopeId id, const Locus& locus);

PtrDerefOp
NewDerefOp(PtrExpr& rhs, const ScopeId id, const Locus& locus);


PtrFuncDef
NewFuncDef(PtrFuncDecl& decl, PtrBlock& block
            , const ScopeId id, const Locus& locus);

PtrFuncDecl
NewFuncDecl(const std::string& name
            , const Type& ret_type
            , std::vector<PtrVarDecl>& par_list
            , const ScopeId id, const Locus& locus);

PtrFuncRet
NewFuncRet(const Type& ret_type
    , PtrFuncCall& call
    , const ScopeId id
    , const Locus& locus);

PtrFuncCall
NewFuncCall(const std::string& name
    , PtrExprVar& expr_var
    , const FuncType& function_type
    , std::vector<PtrExpr>& arg_list
    , const ScopeId id
    , const Locus& locus);

PtrReturnStmt
NewReturnStmt(PtrExpr& ret_expr, FuncDecl& func_decl, const ScopeId id, const Locus& locus);

PtrClassDef
NewClassDef(const std::string& name
    , std::vector<PtrVarDecl>& var_decl
    , std::vector<PtrFuncDef>& func_def
    , const ScopeId id
    , const Locus& locus);

PtrVarName
NewVarName(const std::string& name
    , const ScopeId id
    , const Locus& locus);

PtrDotOp
NewDotOp(PtrExprVar& lhs, PtrVarName& rhs, const ScopeId id
  , const Locus& locus);


} //end namespace Common
