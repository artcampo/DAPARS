#pragma once
#include "AST/ASTVisitor.hpp"
#include "AST/Node.hpp"

//WARNING This is outdated! used dump instead

namespace Compiler{
namespace AST{

class PrettyPrinter : public ASTVisitor{
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

  PrettyPrinter(): indent_(0){};

private:
  void Indent();
  void IncreaseIndent();
  void DecreaseIndent();

  int  indent_;
};

}//end namespace AST
}//end namespace Compiler
