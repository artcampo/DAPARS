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

namespace RecDescent{

using namespace Common;
using namespace Tokenizer;

class ParserLL1RecDesc : public BaseParser{

public:
  ParserLL1RecDesc(std::string const &file_name, CompilationUnit& unit );
  ParserLL1RecDesc(const std::vector<char>& parse_data, CompilationUnit& unit );
  void Parse();

private:
//   Grammar& grammar_;

  void  Prog();

  //Exprs
  Expr* Exprs(const ScopeId scope_inht);
  Expr* ExprPrime(Expr* eprime_inht, const ScopeId scope_inht); // make it const
  Expr* Term(const ScopeId scope_inht);
  Expr* Factor(const ScopeId scope_inht);

  //Control Flow
  Block*      Stmts(std::vector<Statement*>& stmts_inht, const ScopeId scope_inht);
  Statement*  Stmt(const ScopeId scope_inht);
  Block*      IfElse(const ScopeId scope_inht);

  //Variables declaration
  VarDeclList*  Decl(const ScopeId scope_inht);
  VarDeclList*  NameList(std::vector<VarDecl*>& name_list_inht
                       , const TypeId& type_inht, const ScopeId scope_inht
                       , const Locus& locus_inht);
  VarDeclList*  NameListPrime(std::vector<VarDecl*>& name_list_inht
                       , const TypeId& type_inht, const ScopeId scope_inht
                       , const Locus& locus_inht);
  const TypeId  Type();


  Block* ParseSubBlock(const ScopeId scope_inht, const std::string& error);

};

} //end namespace RecDescent
