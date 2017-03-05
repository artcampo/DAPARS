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
#include "Node.hpp"

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

  void  Prog();

  void FuncDefList(std::vector<PtrFuncDef>& fdefl_inht, const ScopeId scope_inht);
  PtrFuncDef FuncDef_(const ScopeId scope_inht);

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

  //Function
  void ParList(std::vector<PtrVarDecl>& par_list_inht);


  //Helpers not associated to a rule
  PtrBlock ParseSubBlock(const ScopeId scope_inht, const std::string& error);
  PtrVarDecl NameDecl(const Compiler::AST::Type& type_inht, const ScopeId scope_inht
                         , const Locus& locus_inht);

  //Variables
  std::string undeclared_name_;
  void BuildTokenVectors();
  std::vector<kToken> set_types_;
  //top() points to current owner
  std::stack<ScopeOwnerId> scope_owner_id_;

};

} //end namespace RecDescent
