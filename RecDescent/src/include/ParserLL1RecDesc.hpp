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
  Expr* Exprs();
  Expr* ExprPrime(Expr* eprime_inht); // make it const
  Expr* Term();
  Expr* Factor();

  //Control Flow
  Block*      Stmts(std::vector<Statement*>& stmts_inht);
  Statement*  Stmt();
  Block*      IfElse();

  //Variables declaration
  VarDeclList*  Decl();
  VarDeclList*  NameList(std::vector<VarDecl*>& name_list_inht, const TypeId& type_inht );
  const TypeId  Type();


};

} //end namespace RecDescent
