#include "ParserLL1RecDescSourceHeaders.hpp"

namespace RecDescent{

// Returns:
//  nullptr <=> Argm is empty and previous name is a var
//  !nullptr <=> pointer to expression with function call
PtrExpr ParserLL1RecDesc::Argm(const std::string& name_inht, const ScopeId scope_inht
            , const Locus& locus_inht){
  //ARGM -> {empty}  => , {empty} = + ) ;
  if(Check({kToken::comma, kToken::equality, kToken::plus, kToken::rpar
              , kToken::semicolon}))
    return std::move(nullptr);

  //ARGM -> ( ARGL )  => (
  if(TryAndAccept(kToken::lpar)){
    std::vector<PtrExpr> args;
    PtrExpr argm_synt = ArgList(name_inht, args, scope_inht, locus_inht);
    Accept(kToken::rpar, kErr35);

    return std::move(argm_synt);
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
PtrExpr ParserLL1RecDesc::ArgList(const std::string& name_inht, std::vector<PtrExpr>& args_inht
  , const ScopeId scope_inht, const Locus& locus_inht){
  //ARGL -> {empty}  => {empty} )
  if(Check({kToken::rpar}))
    return BuildFunctionCall(name_inht, args_inht, scope_inht, locus_inht);

  //ARGL -> E ARGLP  => & * false ( {nam} {num}  true
  if(Check(set_expr_)){
    PtrExpr e = Exprs(scope_inht);
    if(e) args_inht.push_back(std::move(e));
    return ArgListPrime(name_inht, args_inht, scope_inht, locus_inht);
  }

  //Error recovery
  return RecoveryArgList(name_inht, args_inht, scope_inht, locus_inht);
}

//  nullptr <=> arguments were defective, function call is ignored
//  !nullptr <=> pointer to expression with function call
PtrExpr ParserLL1RecDesc::ArgListPrime(const std::string& name_inht, std::vector<PtrExpr>& args_inht
  , const ScopeId scope_inht, const Locus& locus_inht)
{
  //ARGLP -> {empty}  => {empty} )
  if(Check({kToken::rpar}))
    return BuildFunctionCall(name_inht, args_inht, scope_inht, locus_inht);

  //ARGLP -> , E ARGLP  => ,
  if(TryAndAccept({kToken::comma}) and Check(set_expr_)){
    PtrExpr e = Exprs(scope_inht);
    if(e) args_inht.push_back(std::move(e));
    return ArgListPrime(name_inht, args_inht, scope_inht, locus_inht);
  }

  //Error recovery
  return RecoveryArgList(name_inht, args_inht, scope_inht, locus_inht);
}

PtrExpr ParserLL1RecDesc::BuildFunctionCall(const std::string& name_inht, std::vector<PtrExpr>& args_inht
  , const ScopeId scope_inht, const Locus& locus_inht){
  PtrExpr argm_synt(nullptr);
  if(args_inht.empty()) return std::move(argm_synt);

  //TODO: build missing arguments as default expresions
  // per type

  //Check that name's symbol does exists
  if(not unit_.HasDecl(name_inht)) { Error(kErr37); return std::move(nullptr); }

  const AST::FuncType& type_func =
    dynamic_cast<const AST::FuncType&>(unit_.GetType(name_inht));

  //Check if it is a function
  if(not type_func.IsFunc()) { Error(kErr38); return std::move(nullptr); }

  //Check number of arguments VS number of parameters
  const size_t num_args = args_inht.size();
  if( num_args < type_func.NumPars()) { Error(kErr39); return std::move(nullptr); }

  if( num_args > type_func.NumPars()){
    Error(kErr40);
    size_t elements_to_remove = type_func.NumPars() - num_args;
    for(size_t i = 0; i < elements_to_remove; ++i) args_inht.pop_back();
    //continue to produce function call
  }

  for(const auto& arg : type_func){}
}

PtrExpr ParserLL1RecDesc::RecoveryArgList(const std::string& name_inht, std::vector<PtrExpr>& args_inht
  , const ScopeId scope_inht, const Locus& locus_inht){
  Error(kErr36);
  ConsumeTokensUntil(kToken::rpar);
  return BuildFunctionCall(name_inht, args_inht, scope_inht, locus_inht);
}

} //end namespace RecDescent

