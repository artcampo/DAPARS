/*
 * RecDesc parser with Backend-DAVM.
 * Gets input file and generates ByteCode
 */

#include "ParserLL1RecDesc.hpp"
#include "Grammar.hpp"
#include "ASTVisitors/Dump.hpp"
#include "ASTVisitors/PrettyPrinter.hpp"
#include "ASTVisitors/Scopes.hpp"
#include "IR/IRGenerator.hpp"
#include "CompilationUnit.hpp"
#include "Passes/PassManager.hpp"
#include "Backend/BackendDAVM/BackendDAVM.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace RecDescent;
using namespace GrammarAnalyzer;
using Compiler::PassManager;


int main(int argc, char **argv)
{
  if(argc != 3){
    std::cout << "Missing args. Usage: "<< std::string(argv[0]) << " input_file output.bc\n";
    exit(1);
  }

  CompilationUnit unit;

  using namespace RecDescent;
  std::unique_ptr<ParserLL1RecDesc> parser(
    new ParserLL1RecDesc(std::string(argv[1]), unit));

  parser->Parse();

  if(unit.ValidAst()){
    PassManager pm(unit);
    pm.Run();
  }

  if(unit.GetAstProg() != nullptr and not unit.HasErrors()){
    IRGenerator visitor_irgen(unit);
    visitor_irgen.Visit(*unit.GetAstProg(), nullptr);

    using namespace Compiler::Backend::BackendDAVM;
    BackendDAVM b(unit, visitor_irgen.GetIRUnit() );
    b.Run();

    b.WriteBytecode(std::string(argv[2]));

  }

  return 0;
}

