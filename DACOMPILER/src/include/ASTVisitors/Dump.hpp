#pragma once
#include "AST/ASTVisitor.hpp"
#include "AST/Node.hpp"
#include "CompilationUnit.hpp"

namespace Compiler{
namespace AST{

class Dump : public ASTVisitor{
public:
  virtual void Visit(ProgBody const& p);
  virtual void Visit(ProgInit const& p);
  virtual void Visit(ProgEnd const& p);
  virtual void Visit(FuncDef const& p);
  virtual void Visit(Block const& p);
  virtual void Visit(IfStmt const& p);
  virtual void Visit(WhileStmt const& p);
  virtual void Visit(Literal const& p);
  virtual void Visit(BinaryOp const& p);
  virtual void Visit(DeclStmt const& p);
  virtual void Visit(VarDeclList const& p);
  virtual void Visit(VarDecl const& p);
  virtual void Visit(AssignStmt const& p);
  virtual void Visit(Var& p);
  virtual void Visit(RefOp const& p);
  virtual void Visit(DerefOp const& p);
  virtual void Visit(FuncCall& p);
  virtual void Visit(FuncRet& p);
  virtual void Visit(ReturnStmt const& p);
  virtual void Visit(ClassDef const& p);
  virtual void Visit(VarName const& p);
  virtual void Visit(DotOp const& p);


  Dump(CompilationUnit& unit, const bool show_attributes = false, const bool ast_has_type_info = true)
    :  unit_(unit), indent_(0), show_attributes_(show_attributes), ast_has_type_info_(ast_has_type_info){};

private:
  void Indent();
  void IncreaseIndent();
  void DecreaseIndent();
  void DisplayAttributes(Node const& p);
  void DumpSymbolTable();

  int  indent_;
  bool show_attributes_;
  bool ast_has_type_info_; //user for better dumping
  CompilationUnit&  unit_;
};

}//end namespace AST
}//end namespace Compiler
