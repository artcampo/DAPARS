#include "ParserLL1RecDescSourceHeaders.hpp"

namespace RecDescent{

// Returns:
//  nullptr <=> Argm is empty and previous name is a var
//  !nullptr <=> pointer to expression with function call
PtrExpr ParserLL1RecDesc::Argm(const std::string& name_inht, const ScopeId scope_inht
            , const Locus& locus_inht){
  PtrExpr argm_synt(nullptr);

  //ARGM -> ( ARGL )  => (
  if(TryAndAccept(kToken::lpar)){
    std::vector<PtrExpr> args;
    argm_synt = ArgList(args, scope_inht, locus_inht);
    Accept(kToken::rpar, kErr35);

    return std::move(argm_synt);
  }

  //ARGM -> {empty}  => , {empty} = + ) ;
  if(Check({kToken::comma, kToken::equality, kToken::plus, kToken::rpar
              , kToken::semicolon}))
    return std::move(argm_synt);

  //Error detection
  Error(kErrUn1);
  return std::move(argm_synt);
}


//TODO: ignoring function call will lead to function call being interpreted
//as a variable access, thus as the base address of it.
// ex: void f(int a){..}, called with f() will be parsed as f
//  nullptr <=> arguments were defective, function call is ignored
//  !nullptr <=> pointer to expression with function call
PtrExpr ParserLL1RecDesc::ArgList(std::vector<PtrExpr>& args_inht
  , const ScopeId scope_inht, const Locus& locus_inht){
  //ARGL -> {empty}  => {empty} )
  if(Check({kToken::rpar}))
    return BuildFunctionCall(args_inht, scope_inht, locus_inht);

  //ARGL -> E ARGLP  => & * false ( {nam} {num}  true
  if(Check(set_expr_)){
    PtrExpr e = Exprs(scope_inht);
    if(e) args_inht.push_back(std::move(e));
    return ArgListPrime(args_inht, scope_inht, locus_inht);
  }

  //Error recovery
  return RecoveryArgList(args_inht, scope_inht, locus_inht);
}

//  nullptr <=> arguments were defective, function call is ignored
//  !nullptr <=> pointer to expression with function call
PtrExpr ParserLL1RecDesc::ArgListPrime(std::vector<PtrExpr>& args_inht
  , const ScopeId scope_inht, const Locus& locus_inht)
{
  //ARGLP -> {empty}  => {empty} )
  if(Check({kToken::rpar}))
    return BuildFunctionCall(args_inht, scope_inht, locus_inht);

  //ARGLP -> , E ARGLP  => ,
  if(TryAndAccept({kToken::comma}) and Check(set_expr_)){
    PtrExpr e = Exprs(scope_inht);
    if(e) args_inht.push_back(std::move(e));
    return ArgListPrime(args_inht, scope_inht, locus_inht);
  }

  //Error recovery
  return RecoveryArgList(args_inht, scope_inht, locus_inht);
}

PtrExpr ParserLL1RecDesc::BuildFunctionCall(std::vector<PtrExpr>& args_inht
  , const ScopeId scope_inht, const Locus& locus_inht){
  PtrExpr argm_synt(nullptr);
  if(args_inht.empty()) return std::move(argm_synt);

}

PtrExpr ParserLL1RecDesc::RecoveryArgList(std::vector<PtrExpr>& args_inht
  , const ScopeId scope_inht, const Locus& locus_inht){
  Error(kErr36);
  ConsumeTokensUntil(kToken::rpar);
  return BuildFunctionCall(args_inht, scope_inht, locus_inht);
}

} //end namespace RecDescent

