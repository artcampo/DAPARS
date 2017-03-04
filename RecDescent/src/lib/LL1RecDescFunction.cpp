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

  AcceptEmpty({kToken::eof}, "[err:] File not ending in eof");
}

//FDECL :=  name (){ STMTS }
PtrFuncDef ParserLL1RecDesc::FuncDef_(const ScopeId scope_inht){
  std::cout << "fdef\n";
  PtrFuncDef func_decl_synth(nullptr);
  std::string name("func_name");

  Locus       l = CurrentLocus();


  const AST::Type& ret_type = Type_();

  if(Accept(kToken::name, kErr26)) name = prev_token_string_value_;


  Accept(kToken::lpar, kErr27);
  Accept(kToken::rpar, kErr28);


//   const AST::Type& type_func;

  Accept(kToken::lcbr, kErr29);
  const ScopeId id = unit_.NewFunction(name);

  //STMTS
  std::vector<PtrStatement> stmts_inht;
  PtrBlock stmts_synt = Stmts(stmts_inht, id);
  if(not stmts_synt) Error(kErr31);

  Accept(kToken::rcbr, kErr30);
/*
  if(not unit_.IsDeclValid(name, type_func)){
    Error(kErr32);
    return std::move(func_decl_synth);
  }
*/

  func_decl_synth = NewFuncDef(name, stmts_synt, ret_type, scope_inht, l);

//   unit_.RegisterDecl(prev_token_string_value_, type_func, func_decl_synth);
  unit_.GetFunc(name).SetOriginNode(*func_decl_synth);

  unit_.ExitFunctionDefinition();


  return std::move(func_decl_synth);
}


const AST::Type&  ParserLL1RecDesc::Type_(){
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

