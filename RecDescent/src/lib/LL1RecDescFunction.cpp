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
  std::string name("func_name");

  Locus       l = CurrentLocus();


  const AST::Type& ret_type = this->Type();

  if(Accept(kToken::name, kErr26)) name = prev_token_string_value_;

  //Scope for
  scope_owner_id_.push( unit_.NewScopeOwner() );
  const ScopeId id = unit_.NewFunction(name, scope_owner_id_.top());

  Accept(kToken::lpar, kErr27);

  std::vector<PtrVarDecl> par_list;
  ParList(par_list, id);

  Accept(kToken::rpar, kErr28);


  std::vector<const AST::Type*> par_types;
  for(const auto& it : par_list){
    par_types.push_back( &it->GetType() );
  }

  const AST::Type& type_func = unit_.GetFuncType(ret_type, par_types);
//   std::cout << "Type: " << type_func.str() << "\n";

  if(not unit_.IsDeclValid(name)){
    Error(kErr32);
    return std::move(nullptr);
  }
  PtrFuncDecl decl = NewFuncDecl(name, ret_type, par_list, scope_inht, l);
  unit_.EnterFunctionDefinition(decl.get());

  //Process function definition
  Accept(kToken::lcbr, kErr29);

  //STMTS
  std::vector<PtrStatement> stmts_inht;
  PtrBlock stmts_synt = Stmts(stmts_inht, id);
  if(not stmts_synt) FatalError(kErr31);

  Accept(kToken::rcbr, kErr30);
  scope_owner_id_.pop();


  PtrFuncDef func_def_synth = NewFuncDef(decl, stmts_synt, scope_inht, l);
//   std::cout << "funcdef with " << ret_type.str()<<"\n";

  unit_.SetFuncOriginNode(*func_def_synth);
  unit_.ExitFunctionDefinition();
  unit_.RegisterDecl(name, type_func, *func_def_synth);


  return std::move(func_def_synth);
}


void ParserLL1RecDesc::ParList(std::vector<PtrVarDecl>& par_list_inht
  , const ScopeId scope_inht){
  //PARL -> PAR PARLP  => bool int void
  if(Check(set_types_)){
    PtrVarDecl var_decl = Par(scope_inht);
    if(not var_decl) return;
    par_list_inht.push_back( std::move(var_decl));
    ParListPrime(par_list_inht, scope_inht);
    return;
  }

  //PARL -> {empty}  => {empty} )
  if(Check({kToken::rpar})) return;
}

void ParserLL1RecDesc::ParListPrime(std::vector<PtrVarDecl>& par_list_inht
  , const ScopeId scope_inht){
  //PARLP -> {empty}  => {empty} )
  if(Check({kToken::rpar})) return;

  //PARLP -> , PAR PARLP  => ,
  if(not TryAndAccept(kToken::comma)) Error(kErr34);

  if(Check(set_types_)){
    PtrVarDecl var_decl = Par(scope_inht);
    if(not var_decl) return;
    par_list_inht.push_back( std::move(var_decl));
    ParListPrime(par_list_inht, scope_inht);
    return;
  }
}

PtrVarDecl ParserLL1RecDesc::Par(const ScopeId scope_inht){
  const Locus l = CurrentLocus();
  const AST::Type& type = this->Type();
  return NameDecl(type, scope_inht, l);
}

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

