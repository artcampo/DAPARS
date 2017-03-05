#include "ParserLL1RecDescSourceHeaders.hpp"

namespace RecDescent{


void ParserLL1RecDesc::FuncDefList(std::vector<PtrFuncDef>& fdefl_inht,
    const ScopeId scope_inht){
  PtrFuncDef fdef(nullptr);
  if(Check(set_types_ )){
    fdef = FuncDef_(scope_inht);
    if(fdef){
      fdefl_inht.push_back( std::move(fdef));
      FuncDefList(fdefl_inht, scope_inht);
    }
    return;
  }

  if(not Check({kToken::eof})) Error(kErr33);
}

//FDECL :=  name (){ STMTS }
PtrFuncDef ParserLL1RecDesc::FuncDef_(const ScopeId scope_inht){
//   std::cout << "fdef\n";
  PtrFuncDef func_decl_synth(nullptr);
  std::string name("func_name");

  Locus       l = CurrentLocus();


  const AST::Type& ret_type = this->Type();

  if(Accept(kToken::name, kErr26)) name = prev_token_string_value_;


  Accept(kToken::lpar, kErr27);

  scope_owner_id_.push( unit_.NewScopeOwner() );
  std::vector<PtrVarDecl> par_list_inht;
  ParList(par_list_inht);

  Accept(kToken::rpar, kErr28);


  std::vector<AST::Type*> arg_types;
  const AST::Type& type_func = unit_.GetFuncType(ret_type, arg_types);
//   std::cout << "Type: " << type_func.str() << "\n";

  if(not unit_.IsDeclValid(name, type_func)){
    Error(kErr32);
    return std::move(func_decl_synth);
  }

  Accept(kToken::lcbr, kErr29);
  const ScopeId id = unit_.NewFunction(name, scope_owner_id_.top());

  //STMTS
  std::vector<PtrStatement> stmts_inht;
  PtrBlock stmts_synt = Stmts(stmts_inht, id);
  if(not stmts_synt) Error(kErr31);

  Accept(kToken::rcbr, kErr30);
  scope_owner_id_.pop();

  func_decl_synth = NewFuncDef(name, stmts_synt, ret_type, scope_inht, l);


  unit_.SetFuncOriginNode(*func_decl_synth);
  unit_.ExitFunctionDefinition();
  unit_.RegisterDecl(name, type_func, *func_decl_synth);


  return std::move(func_decl_synth);
}


void ParserLL1RecDesc::ParList(std::vector<PtrVarDecl>& par_list_inht){
  //PARL -> PAR PARLP  => bool int void
  if(Check(set_types_)){
//     par_list_inht.push_back( std::move( Par()) );
    return;
  }

  //PARL -> {empty}  => {empty} )
  if(Check({kToken::rpar})) return;
}
/*
PtrVarDecl Par
*/
const AST::Type&  ParserLL1RecDesc::Type(){
//   std::cout << "Type\n";
  if(TryAndAccept(kToken::kwd_void))  return unit_.GetTypeVoid();

  if(TryAndAccept(kToken::kwd_int)){
    if(TryAndAccept(kToken::astk)) return unit_.GetTypePtrToInt();
    else                           return unit_.GetTypeInt();
  }

  if(TryAndAccept(kToken::kwd_bool)){
    if(TryAndAccept(kToken::astk))  return unit_.GetTypePtrToBool();
    else                            return unit_.GetTypeBool();
  }

  //Error recover
  Error("Type missing");
  return unit_.GetTypeInt();
}


} //end namespace RecDescent

