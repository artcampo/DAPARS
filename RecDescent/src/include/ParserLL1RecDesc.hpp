#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "BaseParser.hpp"
#include "Tokenizer.hpp"
#include "Grammar.hpp"
#include "CompilationUnit.hpp"
#include "Types.hpp"
#include "AST/Node.hpp"

namespace RecDescent{

using namespace Common;
using namespace Common::Tokenizer;
using namespace Compiler::AST::Ptrs;

class ParserLL1RecDesc : public BaseParser{

public:
  ParserLL1RecDesc(std::string const &file_name, CompilationUnit& unit );
  ParserLL1RecDesc(const std::vector<char>& parse_data, CompilationUnit& unit );
  void Parse();

private:
//   Grammar& grammar_;

  //Top level
  void  Prog();

  void DefList( std::vector<PtrFuncDef>& fdefl_inht
              , std::vector<PtrClassDef>& cdefl_inht
              , const ScopeId scope_inht);


  //Exprs
  PtrExpr Exprs(const ScopeId scope_inht);
  PtrExpr ExprPrime(PtrExpr& eprime_inht
                          , const ScopeId scope_inht); // make it const
  PtrExpr Term(const ScopeId scope_inht);
  PtrExpr Factor(const ScopeId scope_inht);
  PtrExpr FactorPrime(const ScopeId scope_inht);

  //Control Flow
  PtrBlock      Stmts(std::vector<PtrStatement>& stmts_inht, const ScopeId scope_inht);
  PtrStatement  Stmt(const ScopeId scope_inht);
  PtrBlock      IfElse(const ScopeId scope_inht);

  //Variables declaration
  PtrVarDeclList  Decl(const ScopeId scope_inht);
  PtrVarDeclList  NameList(std::vector<PtrVarDecl>& name_list_inht
                       , const Compiler::AST::Type& type_inht, const ScopeId scope_inht
                       , const Locus& locus_inht);
  PtrVarDeclList  NameListPrime(std::vector<PtrVarDecl>& name_list_inht
                       , const Compiler::AST::Type& type_inht, const ScopeId scope_inht
                       , const Locus& locus_inht);
  const Compiler::AST::Type&  Type();

  //Extension of name_list_inht
  PtrExpr Argm(PtrExprVar& var_inht, const ScopeId scope_inht
            , const Locus& locus_inht);

  //Function
  PtrFuncDef FuncDef_(const ScopeId scope_inht);
  void ParList(std::vector<PtrVarDecl>& par_list_inht, const ScopeId scope_inht);
  void ParListPrime(std::vector<PtrVarDecl>& par_list_inht, const ScopeId scope_inht);
  PtrVarDecl Par(const ScopeId scope_inht);

  PtrExpr
  ArgList(PtrExprVar& var_inht, std::vector<PtrExpr>& args_inht
    , const ScopeId scope_inht, const Locus& locus_inht);
  PtrExpr
  ArgListPrime(PtrExprVar& var_inht, std::vector<PtrExpr>& args_inht
    , const ScopeId scope_inht, const Locus& locus_inht);

  //Helpers not associated to a rule
  PtrBlock
  ParseSubBlock(const ScopeId scope_inht, const std::string& error);
  PtrVarDecl
  NameDecl(const Compiler::AST::Type& type_inht, const ScopeId scope_inht
                         , const Locus& locus_inht);

  PtrExpr
  BuildFunctionCall(PtrExprVar& var_inht, std::vector<PtrExpr>& args_inht
    , const ScopeId scope_inht , const Locus& locus_inht);

  PtrExpr
  RecoveryArgList(PtrExprVar& var_inht, std::vector<PtrExpr>& args_inht, const ScopeId scope_inht
    , const Locus& locus_inht);

  //Classes
  PtrClassDef ClassDef_(const ScopeId scope_inht);

private:
  std::string undeclared_name_;

  //top() points to current owner
  std::stack<ScopeOwnerId> scope_owner_id_;

  //const sets of accepting tokens
  void BuildTokenVectors();
  std::vector<kToken> set_types_;
  std::vector<kToken> set_expr_;
  std::vector<kToken> set_stmts_;
  std::vector<kToken> set_ifelse_;
  std::vector<kToken> set_argm_;

  std::vector<kToken> set_eprime_;

  std::vector<kToken> empty_eprime_;

};

} //end namespace RecDescent
