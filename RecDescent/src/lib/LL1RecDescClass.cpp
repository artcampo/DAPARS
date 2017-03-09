#include "ParserLL1RecDescSourceHeaders.hpp"

namespace RecDescent{




//CDEF    := Class name{}
PtrClassDef ParserLL1RecDesc::ClassDef_(const ScopeId scope_inht){
//   std::cout << "cdef\n";
  if(not Accept(kToken::kwd_class, kErr83)) return std::move(nullptr);

  Locus       l = CurrentLocus();
  std::string name("class_name_not_parsed");
  if(Accept(kToken::name, kErr80)) name = prev_token_string_value_;

  if(not unit_.IsDeclValid(name)){
    Error(kErr84);
    return std::move(nullptr);
  }
  const AST::Type& type = unit_.GetClassType(name);

  Accept(kToken::lcbr, kErr81);
  Accept(kToken::rcbr, kErr82);

  PtrClassDef cdef = NewClassDef(name, scope_inht, l);
  RegNameType(name);

  return std::move(cdef);
  /*
  std::string name("func_name");




  const AST::Type& ret_type = this->Type();



  //Scope for
  scope_owner_id_.push( unit_.NewScopeOwner() );
  const ScopeId id = unit_.NewFunction(name, scope_owner_id_.top());

  Accept(kToken::lpar, kErr27);

  std::vector<PtrVarDecl> par_list;
  ParList(par_list, id);

  Accept(kToken::rpar, kErr28);

  std::vector<const AST::Type*> par_types;
  for(const auto& it : par_list) par_types.push_back( &it->GetType() );


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
  unit_.RegisterDecl(name, type_func, *decl);


  return std::move(func_def_synth);
  */
}



} //end namespace RecDescent

