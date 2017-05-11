#pragma once
#include "AST/ASTVisitor.hpp"
#include "AST/Node.hpp"
#include "Function.hpp"
#include "IR/IRSubtypes.hpp"
#include "IR/Offset.hpp"
#include <map>

namespace Compiler{
namespace AST{

//This visitor is only instantiated on functions, not on Prog

// static const size_t kMaxAddress = 0xFFFF;

class LocalVarOffsets : public ASTVisitor{
public:

  LocalVarOffsets(CompilationUnit& unit, Function& func)
    : unit_(unit), func_(func), offset_(0), comp_private_offset_(0){};



  virtual void Visit(FuncDef const& p){
//     std::cout << "Local offset\n";
    p.GetBody().Accept(*this);

    IR::AddrOffset k            = p.NumPars() - 1;
    IR::AddrOffset arg_position = 0;
    IR::AddrOffset args_base    = 2;  //offset from ARP to to beginning of args in stack

    if(func_.HasLocals()) args_base += 1;
    if(func_.IsMember())  args_base += 1;

    for(auto& it : p.ParList()){
      const Symbols::Symbol& s = unit_.GetSymbolDecl(*it, it->GetScopeId());
      auto size = s.Size();
      if(arg_position < IR::kNumArgsInRegister){
        IR::AddrOffset offset = arg_position;
        func_.StoreSymbolAddress( s.Id(), IR::Offset(offset, s.BareName())
                                , unit_.GetLabelArgumentInReg(), size, func_.Params());
      }else{
        IR::AddrOffset offset    =  (k - arg_position) + args_base;
        func_.StoreSymbolAddress( s.Id(), IR::Offset(offset, s.BareName())
                                , func_.LocalsLabel(), size, func_.Params());
        std::cout << s.str() << " to offset: " << offset << std::endl;
      }
      ++arg_position;
    }


  }

  virtual void Visit(VarDecl const& p){
    const Symbols::Symbol& s = unit_.GetSymbolDecl(p, p.GetScopeId());
    auto size = s.Size();
    IR::Offset o;

    //Offset for a compiler private local var
    if(p.IsCompilerPrivate()){
      o = IR::Offset(comp_private_offset_ - size, s.BareName());
      comp_private_offset_ -= size;
      func_.StoreSymbolAddress( s.Id(), o, func_.LocalsLabel(), size, func_.LocalVars());
      return;
    }

    //Offset for a regular private local var
    if(func_.IsMain())  o = IR::Offset(offset_, s.BareName());
    else                o = IR::Offset(-offset_, s.BareName());

//     std::cout << p.str() << " to offset: " << offset_ << std::endl;
//     std::cout << s.str() << std::endl;

    func_.StoreSymbolAddress( s.Id(), o, func_.LocalsLabel(), size, func_.LocalVars());
    offset_ += size;
  };

  virtual void Visit(IfStmt const& p){
//     std::cout << unit_.GetScope(p.GetThen()->GetScopeId())->str() <<"\n";

    IR::Addr start_offset = offset_;
    p.GetThen().Accept(*this);

    if(p.HasElse()){
      IR::Addr then_offset = offset_;
      offset_              = start_offset;
      p.GetElse().Accept(*this);
      IR::Addr else_offset = offset_;
      offset_ = std::max<IR::Addr>(then_offset, else_offset);
//       std::cout << unit_.GetScope(p.GetElse()->GetScopeId())->str() <<"\n";
    }
  };

  //Traversal
  virtual void Visit(WhileStmt const& p){
    p.GetBody().Accept(*this);
  }
  virtual void Visit(DeclStmt const& p){
    p.GetVarDeclList().Accept(*this);
  }
  virtual void Visit(VarDeclList const& p){
    for(auto& it : p) it->Accept(*this);
  }
  virtual void Visit(Block const& p){
    for(auto& c : p.statements_) c->Accept(*this);
  };
  virtual void Visit(ClassDef const& p){ for(const auto& it : p) it->Accept(*this); }

  //Nothing to do
  virtual void Visit(ProgInit const& p){}
  virtual void Visit(ProgEnd const& p){}
  virtual void Visit(ProgBody const& p){}
  virtual void Visit(BinaryOp const& p){}
  virtual void Visit(AssignStmt const& p){}
  virtual void Visit(Literal const& p){}
  virtual void Visit(Var& p)    {}
  virtual void Visit(RefOp const& p){}
  virtual void Visit(DerefOp const& p){}
  virtual void Visit(FuncCall& p){}
  virtual void Visit(FuncRet& p){}
  virtual void Visit(ReturnStmt const& p){}
  virtual void Visit(VarName const& p){}
  virtual void Visit(DotOp const& p){}

private:
  CompilationUnit&  unit_;
  Function&         func_;
  IR::AddrOffset    offset_;
  IR::AddrOffset    comp_private_offset_;

};


}//end namespace AST
}//end namespace Compiler
