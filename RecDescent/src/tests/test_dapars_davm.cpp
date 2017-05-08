/*
 * Parse a file, generate code, and execute it in DAVM
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


int main(int argc, char **argv)
{
  if(argc != 2){
    std::cout << "Missing source input file\n";
    exit(1);
  }

  CompilationUnit unit;

  using namespace RecDescent;
  std::unique_ptr<ParserLL1RecDesc> parser(
    new ParserLL1RecDesc(std::string(argv[1]), unit));

  parser->Parse();

  if(unit.GetAstProg() != nullptr and not unit.HasErrors()){
    IRGenerator visitor_irgen(unit);
    visitor_irgen.Visit(*unit.GetAstProg(), nullptr);

    std::cout << "Backend::Davm\n";
    using namespace Compiler::Backend::BackendDAVM;
    BackendDAVM b(unit, visitor_irgen.GetIRUnit() );
    b.Run();
    std::cout << "Static data segment size: " << b.GetByteCode().StaticDataSegmentSize() << "\n";
  } else
    std::cout << "Program block is empty!\n";

  return 0;
}

