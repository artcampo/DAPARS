#pragma once
#include "CompilationUnit.hpp"

namespace Compiler{  
namespace Library{
  
void InitCompilationUnit(CompilationUnit& unit){
  AST::Symbols::SymbolId func_id;
  func_id = unit.GetGlobalLexicalScope().RegisterDecl("test",
  unit.NewFunction( "test", 
                    
}

}//end namespace Library
}//end namespace Compiler
