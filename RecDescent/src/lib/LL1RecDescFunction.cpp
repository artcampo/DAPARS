#include "ParserLL1RecDescSourceHeaders.hpp"

namespace RecDescent{


//FDECL :=  name (){ STMTS }
PtrFuncDef ParserLL1RecDesc::FuncDef_(const ScopeId scope_inht){
//   std::cout << "fdef\n";
  std::string name("func_name");
  Locus       l = CurrentLocus();
  const AST::Type& ret_type = this->Type();
  if(Accept(kToken::name, kErr26)) name = prev_token_string_value_;
  const std::string& n =  name;

  Accept(kToken::lpar, kErr27);
  return std::move( ParseFuncDef(ret_type, n, l, scope_inht) );

}

PtrFuncDef ParserLL1RecDesc::ParseFuncDef(const Compiler::AST::Type& ret_type_inht
    , const std::string& name_inht, const Locus& locus_inht, const ScopeId scope_inht){


  //Scope for
  scope_owner_id_.push( unit_.NewScopeOwner() );
  const ScopeId id = unit_.NewFunction(name_inht, scope_owner_id_.top());

  std::vector<PtrVarDecl> par_list;
  ParList(par_list, id);

  Accept(kToken::rpar, kErr28);

  std::vector<const AST::Type*> par_types;
  for(const auto& it : par_list) par_types.push_back( &it->GetType() );


  const AST::Type& type_func = unit_.GetFuncType(ret_type_inht, par_types);
//   std::cout << "Type: " << type_func.str() << "\n";

  if(not unit_.IsDeclValid(name_inht, scope_inht)){
    Error(kErr32);
    return std::move(nullptr);
  }
  PtrFuncDecl decl = NewFuncDecl(name_inht, ret_type_inht, par_list, scope_inht, locus_inht);
  unit_.EnterFunctionDefinition(decl.get());

  //Process function definition
  Accept(kToken::lcbr, kErr29);

  //STMTS
  std::vector<PtrStatement> stmts_inht;
  PtrBlock stmts_synt = Stmts(stmts_inht, id);
  if(not stmts_synt) FatalError(kErr31);

  Accept(kToken::rcbr, kErr30);
  scope_owner_id_.pop();


  PtrFuncDef func_def_synth = NewFuncDef(decl, stmts_synt, scope_inht, locus_inht);
//   std::cout << "funcdef with " << ret_type_inht.str()<<"\n";

  unit_.SetFuncOriginNode(*func_def_synth);
  unit_.ExitFunctionDefinition();
  unit_.RestoreScope();
  unit_.RegisterDecl(name_inht, type_func, *decl, scope_inht);

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



} //end namespace RecDescent

