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


  //////////////////////////////////////////////////////////////////////
  //Expr
  PtrExpr Expr(const ScopeId scope_inht);
  PtrExpr ExprPrime(PtrExpr& eprime_inht, const ScopeId scope_inht);
  
  PtrExpr RelExpr(const ScopeId scope_inht);
  PtrExpr RelExprPrime(PtrExpr& eprime_inht, const ScopeId scope_inht); // make it const
  
  PtrExpr NumExpr(const ScopeId scope_inht);
  PtrExpr NumExprPrime(PtrExpr& eprime_inht, const ScopeId scope_inht); // make it const
  
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
  PtrExpr Argm(const std::string& name_inht, PtrExprVar& var_inht
    , const Compiler::AST::Type& type_inht, const ScopeId scope_inht
    , const Locus& locus_inht);

  //////////////////////////////////////////////////////////////////////
  //Function
  PtrFuncDef FuncDef_(const ScopeId scope_inht);
  void ParList(std::vector<PtrVarDecl>& par_list_inht, const ScopeId scope_inht);
  void ParListPrime(std::vector<PtrVarDecl>& par_list_inht, const ScopeId scope_inht);
  PtrVarDecl Par(const ScopeId scope_inht);

  PtrExpr
  ArgList(const std::string& name_inht, PtrExprVar& var_inht
    , const Compiler::AST::Type& type_inht, std::vector<PtrExpr>& args_inht
    , const ScopeId scope_inht, const Locus& locus_inht);
  PtrExpr
  ArgListPrime(const std::string& name_inht, PtrExprVar& var_inht
    , const Compiler::AST::Type& type_inht, std::vector<PtrExpr>& args_inht
    , const ScopeId scope_inht, const Locus& locus_inht);

  //////////////////////////////////////////////////////////////////////
  //Classes
  PtrClassDef     ClassDef_(const ScopeId scope_inht);

  void MemberList(std::vector<PtrVarDecl>& var_decl_inht
    , std::vector<PtrFuncDef>& func_def_inht
    , const ScopeId scope_inht);

  void Member(const Compiler::AST::Type& type_inht
    , const std::string& name_inht
    , std::vector<PtrVarDecl>& var_decl_inht
    , std::vector<PtrFuncDef>& func_def_inht
    , const Locus& locus_inht
    , const ScopeId scope_inht );
  
  void InhtList(std::vector<ClassType*>& parents_inht);
  void InhtListPrime(std::vector<ClassType*>& parents_inht);

  //////////////////////////////////////////////////////////////////////
  //Helpers not associated to a rule
  PtrBlock    ParseSubBlock(const ScopeId scope_inht, const std::string& error);
  PtrVarDecl  NameDecl(const Compiler::AST::Type& type_inht
                      , const ScopeId scope_inht
                      , const Locus& locus_inht);
  PtrFuncDef ParseFuncDef(const Compiler::AST::Type& ret_type_inht
                      , const std::string& name_inht
                      , const Locus& locus_inht
                      , const ScopeId scope_inht);

  PtrExpr
  BuildFunctionCall(const std::string& name_inht, PtrExprVar& var_inht
    , const Compiler::AST::Type& type_inht, std::vector<PtrExpr>& args_inht
    , const ScopeId scope_inht, const Locus& locus_inht);

  PtrExpr
  BuildIncompleteFunctionCall(const std::string& name_inht, PtrExprVar& var_inht
    , const Compiler::AST::Type& type_inht, std::vector<PtrExpr>& args_inht
    , const ScopeId scope_inht, const Locus& locus_inht);

  PtrExpr         RecoveryArgList(const std::string& name_inht
                                , PtrExprVar& var_inht
                                , const Compiler::AST::Type& type_inht
                                , std::vector<PtrExpr>& args_inht
                                , const ScopeId scope_inht
                                , const Locus& locus_inht);

  PtrStatement    DeclStmt(const ScopeId scope_inht);
  PtrVarDeclList  DeclList(const ScopeId scope_inht);

  PtrVarDecl      BuildNameDecl( const std::string& name_inht
                        , const Compiler::AST::Type& type_inht
                        , const ScopeId scope_inht
                        , const Locus& locus_inht);

  PtrExprVar
  BuildVar(const std::string& var_name, const Compiler::AST::Type*& var_type
    , const ScopeId scope_inht);

  void BuildParent(std::vector<ClassType*>& parents_inht);
  
private:
  std::string undeclared_name_;

  //special vars to manage class' member functions
  bool  inside_member_function_definition_;
  ScopeId member_scope_id_; //only valid if inside_member_function_definition_ is true
  std::string class_name_inht_; //only valid if inside_member_function_definition_ is true
  bool defer_build_call_;

  //const sets of accepting tokens
  void BuildTokenVectors();
  std::vector<kToken> set_types_;
  std::vector<kToken> set_expr_;
  std::vector<kToken> set_stmts_;
  std::vector<kToken> set_ifelse_;
  std::vector<kToken> set_argm_;

  std::vector<kToken> set_eprime_;
  std::vector<kToken> empty_eprime_;
  std::vector<kToken> set_releprime_;
  std::vector<kToken> empty_releprime_;
  std::vector<kToken> set_numeprime_;
  std::vector<kToken> empty_numeprime_;  

};

} //end namespace RecDescent
