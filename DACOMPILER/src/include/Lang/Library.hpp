#pragma once
#include "AST/Node.hpp"
#include "Locus.hpp"
#include "Symbol.hpp"

namespace Compiler{

class CompilationUnit;

namespace Library{

class LangLib{

public:
  LangLib(CompilationUnit& unit) : unit_(unit), name_cond_("__test_condition"){}
  void InitCompilationUnit();

private:
  CompilationUnit&  unit_;
  AST::PtrVarDecl   cond_test_var_decl_;
  Locus             l_;

  std::string name_cond_;
  AST::Symbols::SymbolId cond_sid_;

  void InitRTVars();
  void InitRTFunctionTest();
};

}//end namespace Library
}//end namespace Compiler
