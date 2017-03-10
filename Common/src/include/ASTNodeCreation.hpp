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

using namespace Tokenizer;
using namespace Compiler::AST;
using Compiler::CompilationUnit;
using Compiler::Locus;
using namespace Compiler::AST::Ptrs;


class ASTNodeCreation{

public:
  ASTNodeCreation() = default;
  ~ASTNodeCreation() = default;


protected:

PtrBinaryOp
NewBinaryOp(PtrExpr& lhs, const int op
  , PtrExpr& rhs, const ScopeId id, const Locus& locus){
//   if(lhs == nullptr) ErrorCritical("lhs invalid");
//   if(rhs == nullptr) ErrorCritical("rhs invalid");
  if(lhs.get() == nullptr or rhs.get() == nullptr)
    return PtrBinaryOp(nullptr);

  return std::make_unique<BinaryOp>(lhs, op, rhs, id, locus);
}


PtrVar
NewVar(const std::string& name, const Type& type
  , Compiler::AST::Symbols::SymbolId id
  , const ScopeId scope_id, const Locus& locus){
  return std::make_unique<Var>(name, type, id, scope_id, locus);
}

PtrLiteral
NewLiteral(const uint32_t &value, const Type& type
  , const ScopeId id, const Locus& locus){
  return std::make_unique<Literal>(value, type, id, locus);
}

PtrBlock
NewBlock(std::vector<PtrStatement>& stmts_inht
  , const ScopeId id, const Locus& locus){
  if(stmts_inht.empty()) return PtrBlock(nullptr);
  for(auto& stmt : stmts_inht)
    if(stmt.get() == nullptr) return PtrBlock(nullptr);

  PtrBlock new_block = std::make_unique<Block>(id, locus);
  for(auto& stmt : stmts_inht){
    new_block->AddStatement(stmt);
  }
  return new_block;
}

PtrIfStmt
NewIfStmt(PtrExpr& condition, PtrBlock& block1
  , const ScopeId id, const Locus& locus){
  if(condition.get() == nullptr or block1.get() == nullptr)
    return PtrIfStmt(nullptr);

  return std::make_unique<IfStmt>(condition, block1, id, locus);
}

PtrIfStmt
NewIfStmt(PtrExpr& condition, PtrBlock& block1
  , PtrBlock& block2, const ScopeId id, const Locus& locus){

  return std::make_unique<IfStmt>(condition, block1, block2, id, locus);
}

PtrDeclStmt
NewDeclStmt(PtrVarDeclList& list, const ScopeId id
  , const Locus& locus){
  if(list.get() == nullptr)
    return PtrDeclStmt(nullptr);

  return std::make_unique<DeclStmt>(list, id, locus);
}

PtrVarDeclList
NewVarDeclList(std::vector<PtrVarDecl>& list
  , const ScopeId id, const Locus& locus){
  if(list.empty()) return PtrVarDeclList(nullptr);
  for(auto& dec : list) if(dec.get() == nullptr) return PtrVarDeclList(nullptr);

  return std::make_unique<VarDeclList>(list, id, locus);
}

PtrVarDecl
NewVarDecl(const std::string& name
                              , const Type& type
                              , const ScopeId id
                               , const Locus& locus){
  return std::make_unique<VarDecl>(name, type, id, locus);
}

PtrAssignStmt
NewAssignStmt(PtrExpr& lhs
                                    , PtrExpr& rhs
                                     , const ScopeId id
                                     , const Locus& locus){
  if(lhs.get() == nullptr or rhs.get() == nullptr)
    return PtrAssignStmt(nullptr);
  return std::make_unique<AssignStmt>(lhs,rhs, id, locus);
}

PtrWhileStmt
NewWhileStmt(PtrExpr& condition, PtrBlock& body
    , const ScopeId id, const Locus& locus){
  if(condition.get() == nullptr or body.get() == nullptr){
    std::cout << "Null NewWhileStmt\n";
    return PtrWhileStmt(nullptr);
  }

  return std::make_unique<WhileStmt>( condition, body, id, locus);
}

PtrRefOp
NewRefOp(PtrExpr& rhs, const ScopeId id, const Locus& locus){
  if(rhs.get() == nullptr) return PtrRefOp(nullptr);
  return std::make_unique<RefOp>(rhs, id, locus);
}

PtrDerefOp
NewDerefOp(PtrExpr& rhs, const ScopeId id, const Locus& locus){
  if(rhs.get() == nullptr) return PtrDerefOp(nullptr);
  return std::make_unique<DerefOp>(rhs, id, locus);
}



PtrFuncDef
NewFuncDef(PtrFuncDecl& decl, PtrBlock& block
            , const ScopeId id, const Locus& locus){
  if(not block or not decl) return PtrFuncDef(nullptr);
  return std::make_unique<FuncDef>(decl, block, id, locus);
}

PtrFuncDecl
NewFuncDecl(const std::string& name
            , const Type& ret_type
            , std::vector<PtrVarDecl>& par_list
            , const ScopeId id, const Locus& locus){
  return std::make_unique<FuncDecl>(name, ret_type, par_list, id, locus);
}

PtrFuncRet
NewFuncRet(const Type& ret_type
    , PtrFuncCall& call
    , const ScopeId id
    , const Locus& locus){
  if(not call) return PtrFuncRet(nullptr);
  return std::make_unique<FuncRet>(ret_type, call, id, locus);
}

PtrFuncCall
NewFuncCall(const std::string& name
    , const FuncType& function_type
    , std::vector<PtrExpr>& arg_list
    , const ScopeId id
    , const Locus& locus){
  return std::make_unique<FuncCall>(name, function_type, arg_list, id, locus);
}

PtrReturnStmt
NewReturnStmt(PtrExpr& ret_expr, FuncDecl& func_decl, const ScopeId id, const Locus& locus){
  if(not ret_expr)return PtrReturnStmt(nullptr);
  return std::make_unique<ReturnStmt>(ret_expr, func_decl, id, locus);
}

PtrClassDef
NewClassDef(const std::string& name
    , const ScopeId id
    , const Locus& locus){
  return std::make_unique<ClassDef>(name, id, locus);
}

};

} //end namespace Common
