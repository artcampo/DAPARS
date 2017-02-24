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

  //Exprs
  std::unique_ptr<Expr> Exprs(const ScopeId scope_inht);
  std::unique_ptr<Expr> ExprPrime(std::unique_ptr<Expr>& eprime_inht
                          , const ScopeId scope_inht); // make it const
  std::unique_ptr<Expr> Term(const ScopeId scope_inht);
  std::unique_ptr<Expr> Factor(const ScopeId scope_inht);
  std::unique_ptr<Expr> FactorPrime(const ScopeId scope_inht);

  //Control Flow
  PtrBlock      Stmts(std::vector<std::unique_ptr<Statement>>& stmts_inht, const ScopeId scope_inht);
  std::unique_ptr<Statement>  Stmt(const ScopeId scope_inht);
  PtrBlock      IfElse(const ScopeId scope_inht);

  //Variables declaration
  std::unique_ptr<VarDeclList>  Decl(const ScopeId scope_inht);
  std::unique_ptr<VarDeclList>  NameList(std::vector<std::unique_ptr<VarDecl>>& name_list_inht
                       , const Compiler::AST::Type& type_inht, const ScopeId scope_inht
                       , const Locus& locus_inht);
  std::unique_ptr<VarDeclList>  NameListPrime(std::vector<std::unique_ptr<VarDecl>>& name_list_inht
                       , const Compiler::AST::Type& type_inht, const ScopeId scope_inht
                       , const Locus& locus_inht);
  const Compiler::AST::Type&  Type_();


  PtrBlock ParseSubBlock(const ScopeId scope_inht, const std::string& error);

};

} //end namespace RecDescent
