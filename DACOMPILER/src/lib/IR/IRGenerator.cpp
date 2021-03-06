#include "IR/IRGenerator.hpp"
#include "IR/MemAddr.hpp"
#include "IR/IRSubtypes.hpp"
#include "VM/ByteCode/IRDefinition.hpp"

namespace Compiler{

using Compiler::IR::Offset;
using Compiler::IR::Label;
using Compiler::IR::MemAddr;
using namespace Compiler::IR::Inst;

namespace AST{

/////////////////////////////////////////////////////////////////////////////
void IRGenerator::Visit(ProgBody const& p, const Node* successor){
//   std::cout << "P\n";
  p.GetProgInit().Accept (*this, nullptr);



  //Process main
  for(auto& it : p) if(it->Name()=="main"){
    NewStream(unit_.GetFunctionEntryLabel("main"), unit_.GetFunc(it->Name()));
    it->Accept(*this, successor);
    CurrentStream().AppendReturnMain();
  }

  //Process classes
  for(auto& it : p.GetClassDefs() ) it->Accept(*this, nullptr);

  //Process the rest of the functions
  for(auto& it : p) if(it->Name()!="main"){
    NewStream(unit_.GetFunctionEntryLabel(it->Name()), unit_.GetFunc(it->Name()));
    it->Accept(*this, successor);
    CurrentStream().AppendReturn();
  }

  ir_unit_.ComputeBeginBB();

  p.GetProgEnd().Accept  (*this, nullptr );
}

/////////////////////////////////////////////////////////////////////////////
void IRGenerator::Visit(ClassDef const& p, const Node* successor){
  for(auto& it : p){
    class_label_inht_ = unit_.GetClass(p.Name()).ThisLabel();
    class_inht_ = &unit_.GetClass(p.Name());
    NewStream(unit_.GetFunctionEntryLabel(
        unit_.GetFunc(*it).MangledName())
      , unit_.GetFunc(it->Name()) );
    inside_member_func_def_ = true;
    it->Accept(*this, successor);
    inside_member_func_def_ = false;
    CurrentStream().AppendReturn();
  }
}

/////////////////////////////////////////////////////////////////////////////
void IRGenerator::Visit(FuncDef const& p, const Node* successor){
//   Function& f = unit_.GetFunc(p);
//   std::cout << "in f: " << f.str();
//   const IR::Label l = f.LocalsLabel();
//   std::cout << " with label: " << l.str();
  local_label_inht_ = unit_.GetFunc(p).LocalsLabel();
//   std::cout << "Using label: " <<unit_.LabelStr( local_label_inht_.Id());
  p.GetBody().Accept(*this, successor);
}

void IRGenerator::Visit(ProgInit const& p, const Node* successor){}
void IRGenerator::Visit(ProgEnd const& p, const Node* successor){}


/////////////////////////////////////////////////////////////////////////////
// Sucessor for an statement is either:
// - next statetement in current block
// - first node of block following enclosing block
void IRGenerator::Visit(Block const& n, const Node* successor) {

//   std::cout << "B" << n.str()<< " with successor: " << successor->str() << "\n";
  BackPatch(n, CurrentStream().NextAddress());

  for (std::vector<PtrStatement>::const_iterator stmt = n.statements_.cbegin();
        stmt != n.statements_.cend(); ++stmt){
    const Node* actual_successor = successor;
    if(stmt != n.statements_.cend() - 1 )
      actual_successor = (stmt + 1)->get();

    BackPatch(**stmt, CurrentStream().NextAddress());

    (*stmt)->Accept(*this, actual_successor);
  }

}

void IRGenerator::Visit(DeclStmt const& p, const Node* successor){
//   std::cout << "D"<< p.str()<<" with successor: " << successor->str() << "\n";
  p.GetVarDeclList().Accept(*this, successor);
}

void IRGenerator::Visit(VarDeclList const& p, const Node* successor){
  for(const auto& d : p.GetVarDeclVector()) d->Accept(*this, successor);
}

void IRGenerator::Visit(VarDecl const& p, const Node* successor){
//   CurrentStream().Append( NewVar(0) );
}

void IRGenerator::Visit(IfStmt const& p, const Node* successor){

//   std::cout << "If"<< p.str()<< " with successor: " << successor->str() << "\n";

  p.GetCond().Accept(*this, successor);

  const IR::Addr current_addr = CurrentStream().NextAddress();
  const IR::Reg  reg_src      = reg_dst_of_expr_[&p.GetCond()];

  CurrentStream().AppendJumpIfTrue (reg_src);
  CurrentStream().AppendJumpIfFalse(reg_src);

  AddToBackPatch(p.GetThen(), current_addr + 0);

  if(p.HasElse()) AddToBackPatch(p.GetElse(), current_addr + 1);
  else            AddToBackPatch(*successor,  current_addr + 1);


//   std::cout << "-Store backp to: " << p.GetThen().str()<<"("<<(void*)p.GetThen()<<")"<<"\n";
//   std::cout << "-Store backp to: " << successor->str()<<"("<<(void*)successor<<")"<<"\n";

  p.GetThen().Accept(*this, successor);

  //exit from then stmt
  CurrentStream().AppendJumpInconditional();
  AddToBackPatch(*successor, CurrentStream().NextAddress() - 1);

  p.GetElse().Accept(*this, successor);

  //exit from else stmt
  CurrentStream().AppendJumpInconditional();
  AddToBackPatch(*successor, CurrentStream().NextAddress() - 1);

//   std::cout << "-Store backp to: " << successor->str()<<"\n";
//   std::cout <<"\n";
//   PrintBackPatch();

}

void IRGenerator::Visit(WhileStmt const& p, const Node* successor){

  //
  const IR::Addr reentry_addr = CurrentStream().NextAddress();
  p.GetCond().Accept(*this, successor);
  const IR::Addr current_addr = CurrentStream().NextAddress();
  const IR::Reg reg_src       = reg_dst_of_expr_[&p.GetCond()];

  CurrentStream().AppendJumpIfTrue(reg_src);
  CurrentStream().AppendJumpIfFalse(reg_src);
  AddToBackPatch(p.GetBody(), current_addr + 0);
  AddToBackPatch(*successor,   current_addr + 1);

  p.GetBody().Accept(*this, successor);
  CurrentStream().AppendJumpInconditional(reentry_addr);
}


void IRGenerator::Visit(AssignStmt const& p, const Node* successor){
  p.Rhs().Accept(*this, successor);
  reg_src_of_assignment_[&p.Lhs()] = reg_dst_of_expr_[&p.Rhs()];
  p.Lhs().Accept(*this, successor);
}


/////////////////////////////////////////////////////////////////////////////
void IRGenerator::Visit(Literal const& n, const Node* successor){
  const IR::Reg r  = CurrentStream().AppendLoadI( n.Value() );
  reg_dst_of_expr_[&n] = r;
}


/////////////////////////////////////////////////////////////////////////////
void IRGenerator::Visit(BinaryOp const& n, const Node* successor){
  n.Lhs().Accept(*this, successor);
  n.Rhs().Accept(*this, successor);

  const IR::Reg reg_src1 = reg_dst_of_expr_[&n.Lhs()];
  const IR::Reg reg_src2 = reg_dst_of_expr_[&n.Rhs()];
  const int op           = n.op;
  IR::Reg r;


  //TODO: worth the switch?
  if(op == BinaryOp::kAdd){
    const IR::ArithType op = IR::ArithType::kAdd;
    r = CurrentStream().AppendArith(reg_src1, reg_src2, op);
  }
  if(op == BinaryOp::kOr){
    const IR::LogicType op = IR::LogicType::kOr;
    r = CurrentStream().AppendLogic(reg_src1, reg_src2, op);
  }
  if(op == BinaryOp::kAnd){
    const IR::LogicType op = IR::LogicType::kAnd;
    r = CurrentStream().AppendLogic(reg_src1, reg_src2, op);
  }
  if(op == BinaryOp::kLessThan){
    const IR::CompType op = IR::CompType::kLessThan;
    r = CurrentStream().AppendComparison(reg_src1, reg_src2, op);
  }
  if(op == BinaryOp::kEqualTo){
    const IR::CompType op = IR::CompType::kEqualTo;
    r = CurrentStream().AppendComparison(reg_src1, reg_src2, op);
  }

  reg_dst_of_expr_[&n]   = r;
//   std::cout << "OP: " << op << "\n";

}

void IRGenerator::Visit(RefOp const& n, const Node* successor){
  n.Rhs().Accept(*this, successor);
  const MemAddr a       = addr_of_var_[&n.Rhs()];
  const IR::Reg r       = CurrentStream().AppendPtrElem(a);
  reg_dst_of_expr_[&n]  = r;
}

void IRGenerator::Visit(DerefOp const& n, const Node* successor){
  n.Rhs().Accept(*this, successor);
  const IR::Reg reg_src_addr = reg_dst_of_expr_[&n.Rhs()];
  if(unit_.IsRead(n)){
    const IR::Reg r       = CurrentStream().AppendLoadReg(reg_src_addr);
    reg_dst_of_expr_[&n]  = r;
  }else{
    const IR::Reg src_value = reg_src_of_assignment_[&n];
    CurrentStream().AppendStoreReg(src_value, reg_src_addr);
  }
//   const IR::Reg r       = CurrentStream().AppendAddrUnary(reg_src, AddrUnaryType::kDereference);
//   reg_dst_of_expr_[&n]  = r;
}

void IRGenerator::Visit(Var const& p, const Node* successor){

  if(p.GetType().IsFunc()){
    const MemAddr a = MemAddr( unit_.GetFunc(p.Name()).EntryLabel(), 0);
    addr_of_var_[&p] = a;
    return;
  }

  Offset o;
  MemAddr a;
  if(unit_.IsMemberVar(p)){
    AST::Symbols::SymbolId sid = class_inht_->GetHScope().DeclId(p.Name());
    o = class_inht_->MemberVarOffset(sid);
    a = MemAddr(class_label_inht_, o);
  }else{
    a = unit_.LocalVarMemAddr(p);
  }

  //Load to an argument which is in a register
  if(a.GetLabel() == unit_.GetLabelArgumentInReg()){
    const IR::Reg r_dst   = CurrentStream().AppendGetArg(a.GetOffset().GetAddr());
    reg_dst_of_expr_[&p]  = r_dst;
    return;
  }

  //regular memory access
  if(unit_.IsRead(p)){
    //Var, Read
    if(unit_.IsValueAccess(p)){
      const IR::Reg r  = CurrentStream().AppendLoad(a);
      reg_dst_of_expr_[&p] = r;
    }else{
      addr_of_var_[&p] = a;
      if(emit_addr_var_inht_){
        const IR::Reg r_dst   = CurrentStream().AppendPtrElem(a);
        reg_dst_of_expr_[&p]  = r_dst;
      }
    }
  }else{
    //Var, Write
    const IR::Reg r_src = reg_src_of_assignment_[&p];
    CurrentStream().AppendStore(r_src, a);
  }
}

/////////////////////////////////////////////////////////////////////////////
void IRGenerator::Visit(FuncCall& p, const Node* successor){

  if(inside_member_func_def_ and is_member_call_inht_){
    //TODO: adjust offset to parent's objects
    MemAddr a = MemAddr(class_label_inht_, 0);
    const IR::Reg r  = CurrentStream().AppendLoad(a);
    CurrentStream().AppendSetArg(r);
  }

  //generate arguments
  for(const auto& it : p) it->Accept(*this, successor);
  //generate set prior to call
  for(const auto& it : p) CurrentStream().AppendSetArg( reg_dst_of_expr_[&*it] );

  //Process receiver to get its addr
  p.Receiver().Accept(*this, successor);
  const MemAddr a = addr_of_var_[&p.Receiver()];
  //Create address of function
//   MemAddr a;

  //1) When function is a variable
//   ExprVar* e  = &p.GetExprVar();
//   Var* e_var  = dynamic_cast<Var*>(e);
//   if(e_var) a = MemAddr(unit_.GetFunc(e_var->Name()).EntryLabel(), 0);
  CurrentStream().AppendCall(a);

}

/////////////////////////////////////////////////////////////////////////////
void IRGenerator::Visit(FuncRet& p, const Node* successor){
  is_member_call_inht_ = unit_.IsMemberVar(p);
  p.GetCall().Accept(*this, successor);
//   std::cout << p.str() << " - " << p.GetType().str()<<"\n";
  if( p.GetType() != unit_.GetTypeVoid() ){
    const IR::Reg r_dst = CurrentStream().AppendGetRetVal();
    reg_dst_of_expr_[&p] = r_dst;
  }
}

/////////////////////////////////////////////////////////////////////////////
void IRGenerator::Visit(ReturnStmt const& p, const Node* successor){
   p.RetExpr().Accept(*this, successor);

   if( unit_.GetTypeOfNode(p.RetExpr()) != unit_.GetTypeVoid() ){
    const IR::Reg r_src = reg_dst_of_expr_[&p.RetExpr()];
    CurrentStream().AppendSetRetVal(r_src);
   }
}


/////////////////////////////////////////////////////////////////////////////
void IRGenerator::Visit(VarName const& p, const Node* successor){

}

/////////////////////////////////////////////////////////////////////////////
void IRGenerator::Visit(DotOp const& p, const Node* successor){
  emit_addr_var_inht_ = true;
  p.Lhs().Accept(*this, successor);
  emit_addr_var_inht_ = false;
  p.Rhs().Accept(*this, successor);

  const Type& t_dot       = unit_.GetTypeOfNode(p);
  const ClassType& t_lhs  = dynamic_cast<const ClassType&>(unit_.GetTypeOfNode(p.Lhs()));
  const Class& c          = unit_.GetClass(t_lhs);
  const std::string fname = p.Rhs().Name();

  if(t_dot.IsFunc()){
    //Dot with function

    //pass pointer this
    IR::Reg r_src  = reg_dst_of_expr_[&p.Lhs()];
    reg_dst_of_expr_[&p] = r_src;

    //adjust this if needed
    if(c.IsCallToParent(fname)){
      const IR::Offset o    = c.FuncPtrThisAdjust(fname);
      const IR::Reg r_src2  = CurrentStream().AppendLoadI(o.GetAddr());
      const IR::Reg r_dst   = CurrentStream().AppendArith(r_src, r_src2, IR::ArithType::kAdd);
      reg_dst_of_expr_[&p] = r_dst;
      r_src = r_dst;
    }

    //load this as first argument
    CurrentStream().AppendSetArg(r_src);

    //give the addr of the call
    const MemAddr a = MemAddr( c.GetFunction(fname).EntryLabel(), 0);
    addr_of_var_[&p] = a;
  }else{
    //Dot with var

    AST::Symbols::SymbolId sid = c.GetHScope().DeclId(p.Rhs().Name());
    IR::Offset o = c.MemberVarOffset(sid);
    const IR::Reg r_src = reg_dst_of_expr_[&p.Lhs()];

    const IR::Reg r_dst = CurrentStream().AppendLoadRegOffs(r_src, o);
    reg_dst_of_expr_[&p] = r_dst;
  }
}



/////////////////////////////////////////////////////////////////////////////
void IRGenerator::Print() const noexcept{
  for(const auto& it : ir_unit_.streams_) it->Print();
}

/////////////////////////////////////////////////////////////////////////////
void IRGenerator::EndOfProgram(){
//   CurrentStream().stream.push_back( IRBuilder::Stop());
  Print();
}



void IRGenerator::BackPatch(const Node& n, const IR::Addr position){
//   std::cout << "**Backpatch try ["<< n->str()<< "]:" <<"("<<(void*)n<<")";
  std::map<const Node*, std::vector<IR::Addr>>::iterator it = back_patch_.find(&n);
  if(it != back_patch_.end()){
    for(const auto& address : it->second){
      Jump& inst = dynamic_cast<Jump&>(CurrentStream().GetInst(address));
//       std::cout << "Backpatch: "
//         << IRBuilder::Print(inst)
//         << " with node: " << n->str() << "\n";
      inst.PatchJump(position);
    }
    back_patch_.erase(it);
  }
//   else std::cout << " no entries";

//   std::cout << "\n";

//   PrintBackPatch();
}


void IRGenerator::AddToBackPatch(const Node& n, const IR::Addr position){
//   std::cout << "**Backpatch insert ["<< n->str()<< "] has to patch:"
//             << IRBuilder::Print(CurrentStream().GetInst(position))
//             << "\n";
  //<<"("<<(void*)n<<")\n";
  back_patch_[&n].push_back(position);
}

void IRGenerator::PrintBackPatch() {
  std::cout << "-----------------------------------\n";
  for(auto e : back_patch_){
    std::cout << "** entry: ";
    std::cout << e.first->str()<<": ";
    for(const auto& address : e.second) std::cout << address<<" ";
    std::cout << "\n";
  }
  std::cout << "-----------------------------------\n";
}

}//end namespace AST
}//end namespace Compiler
