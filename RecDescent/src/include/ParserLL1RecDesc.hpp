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

  PtrFuncDecl FuncDecl_(const ScopeId scope_inht);

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
  const Compiler::AST::Type&  Type_();


  PtrBlock ParseSubBlock(const ScopeId scope_inht, const std::string& error);

  std::string undeclared_name_;

};

} //end namespace RecDescent
