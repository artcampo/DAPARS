#include "ParserLL1RecDescSourceHeaders.hpp"

namespace RecDescent{

// Returns: pointer to expr_var
PtrExpr ParserLL1RecDesc::Argm(PtrExprVar& var_inht, const ScopeId scope_inht
            , const Locus& locus_inht){
  //ARGM -> {empty}  => , {empty} = + ) ;
  if(Check(set_argm_)) return std::move(var_inht);

  //ARGM -> ( ARGL )  => (
  if(TryAndAccept(kToken::lpar)){
    std::vector<PtrExpr> args;
    PtrExpr argm_synt = ArgList(var_inht, args, scope_inht, locus_inht);
    Accept(kToken::rpar, kErr35);

    return std::move(argm_synt);
  }

  //ARGM -> . {nam} ARGM  => .
  if(TryAndAccept(kToken::dot)){
  }

  //Error detection
  Error(kErrUn1);
  return std::move(nullptr);
}


//TODO: ignoring function call will lead to function call being interpreted
//as a variable access, thus as the base address of it.
// ex: void f(int a){..}, called with f() will be parsed as f
//  nullptr <=> arguments were defective, function call is ignored
//  !nullptr <=> pointer to expression with function call
PtrExpr ParserLL1RecDesc::ArgList(PtrExprVar& var_inht, std::vector<PtrExpr>& args_inht
  , const ScopeId scope_inht, const Locus& locus_inht){
  //ARGL -> {empty}  => {empty} )
  if(Check({kToken::rpar}))
    return BuildFunctionCall(var_inht, args_inht, scope_inht, locus_inht);

  //ARGL -> E ARGLP  => & * false ( {nam} {num}  true
  if(Check(set_expr_)){
    PtrExpr e = Exprs(scope_inht);
    if(e) args_inht.push_back(std::move(e));
    return ArgListPrime(var_inht, args_inht, scope_inht, locus_inht);
  }

  //Error recovery
  return RecoveryArgList(var_inht, args_inht, scope_inht, locus_inht);
}

//  nullptr <=> arguments were defective, function call is ignored
//  !nullptr <=> pointer to expression with function call
PtrExpr ParserLL1RecDesc::ArgListPrime(PtrExprVar& var_inht, std::vector<PtrExpr>& args_inht
  , const ScopeId scope_inht, const Locus& locus_inht)
{
  //ARGLP -> {empty}  => {empty} )
  if(Check({kToken::rpar}))
    return BuildFunctionCall(var_inht, args_inht, scope_inht, locus_inht);

  //ARGLP -> , E ARGLP  => ,
  if(TryAndAccept({kToken::comma}) and Check(set_expr_)){
    PtrExpr e = Exprs(scope_inht);
    if(e) args_inht.push_back(std::move(e));
    return ArgListPrime(var_inht, args_inht, scope_inht, locus_inht);
  }

  //Error recovery
  return RecoveryArgList(var_inht, args_inht, scope_inht, locus_inht);
}

PtrExpr ParserLL1RecDesc::BuildFunctionCall(PtrExprVar& var_inht, std::vector<PtrExpr>& args_inht
  , const ScopeId scope_inht, const Locus& locus_inht){
  //TODO: build missing arguments as default expresions
  // per type

  //Check that name's symbol does exists
  //if(not unit_.HasDecl(name_inht)) { Error(kErr37); return std::move(nullptr); }


  const AST::FuncType& type_func =
    dynamic_cast<const AST::FuncType&>(var_inht.GetType());
  std::cout << "Using function: " << type_func.str() << std::endl;

  //Check if it is a function
  if(not type_func.IsFunc()) { Error(kErr38); return std::move(nullptr); }

  //Check number of arguments VS number of parameters
  const size_t num_args = args_inht.size();
  if( num_args < type_func.NumPars()) { Error(kErr39); return std::move(nullptr); }

  if( num_args > type_func.NumPars()){
    Error(kErr40);
    size_t elements_to_remove = num_args - type_func.NumPars();
    for(size_t i = 0; i < elements_to_remove; ++i) args_inht.pop_back();
    //continue to produce function call
  }

//   for(const auto& arg : type_func){}
  const AST::Type& ret_type = unit_.GetType(type_func.RetTypeId());
  PtrFuncCall call =
    NewFuncCall(var_inht, type_func, args_inht, scope_inht, locus_inht);
  PtrFuncRet ret =
    NewFuncRet(ret_type, call, scope_inht, locus_inht);

  return std::move(ret);
}

PtrExpr ParserLL1RecDesc::RecoveryArgList(PtrExprVar& var_inht, std::vector<PtrExpr>& args_inht
  , const ScopeId scope_inht, const Locus& locus_inht){
  Error(kErr36);
  ConsumeTokensUntil(kToken::rpar);
  return BuildFunctionCall(var_inht, args_inht, scope_inht, locus_inht);
}

} //end namespace RecDescent

