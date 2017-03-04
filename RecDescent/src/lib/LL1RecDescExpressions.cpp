#include "ParserLL1RecDescSourceHeaders.hpp"

namespace RecDescent{


//TODO: should return PtrExpr
PtrExpr ParserLL1RecDesc::Exprs(const ScopeId scope_inht){
//   std::cout << "Exp\n";
  PtrExpr eprime_synt(nullptr);
  PtrExpr term_synth  = Term(scope_inht);

  if(term_synth.get() != nullptr){
    eprime_synt = ExprPrime(term_synth, scope_inht);

//     if(eprime_synt == nullptr)
//       Error("E' missing");
  }else {
    Error("[err:13] Term missing.");
  }
//   std::cout << "<-Exp\n";
  return std::move(eprime_synt);
}


PtrExpr ParserLL1RecDesc::Term(const ScopeId scope_inht){
//   std::cout << "T\n";
  return std::move(Factor(scope_inht));
}



// E'    := + T E'     ** E'1.inht = new Node(+, E'.inht, T.node)
//                        E'.synt  = E'1.synt
//       |  empty      ** E'.synt  = E'1.synt
PtrExpr ParserLL1RecDesc::ExprPrime(PtrExpr& eprime_inht, const ScopeId scope_inht){
//   std::cout << "Exp'\n";
  PtrExpr eprime_synt(nullptr);
  Locus l = CurrentLocus();

  if(TryAndAccept(kToken::plus)){
    PtrExpr t_synt = Term(scope_inht);
    PtrExpr eprime1_inht = NewBinaryOp(eprime_inht, IR_ADD, t_synt, scope_inht, l);

    //A new E' will op against current op+
    eprime_synt = ExprPrime(eprime1_inht, scope_inht);
    if(eprime_synt.get() == nullptr)
      Error("[err:12] operand to + missing");
    return std::move(eprime_synt);

  }

  //E' -> {empty}  => {empty} = ) ;
  if(AcceptEmpty({kToken::equality, kToken::rpar, kToken::semicolon},
      "Expecting +")){
    eprime_synt = std::move(eprime_inht);
  }

//   std::cout << "<-Exp'\n";
  return std::move(eprime_synt);
}


PtrExpr ParserLL1RecDesc::Factor(const ScopeId scope_inht){
  PtrExpr f_synt;
  Locus l = CurrentLocus();

  //F := *F'
  if(TryAndAccept(kToken::astk)){
//     f_synt = New
    PtrExpr fp_synt = FactorPrime(scope_inht);
    f_synt = NewDerefOp(fp_synt, scope_inht, l);
    return std::move(f_synt);
  }

  //F := &F'
  if(TryAndAccept(kToken::ampersand)){
    PtrExpr fp_synt = FactorPrime(scope_inht);
    f_synt = NewRefOp(fp_synt, scope_inht, l);
    return std::move(f_synt);
  }

  return std::move(FactorPrime(scope_inht));
}

// F := ( E ) | numerical
PtrExpr ParserLL1RecDesc::FactorPrime(const ScopeId scope_inht){
//   std::cout << "Fact\n";
  PtrExpr fp_synt(nullptr);
  Locus l = CurrentLocus();

  //F' := numerical
  if(TryAndAccept(kToken::numerical)){
    const AST::Type& t = unit_.GetType(kBasicTypeId::kInt);
    fp_synt = NewLiteral(prev_token_int_value_, t
                      , scope_inht, l);
    return std::move(fp_synt);
  }

  //F' := true
  if(TryAndAccept(kToken::kwd_true)){
    fp_synt = NewLiteral(1, unit_.GetType(kBasicTypeId::kBool) , scope_inht, l);
    //fp_synt = NewLiteral(1, AST::Type::Bool() , scope_inht, l);
    return std::move(fp_synt);
  }

  //F' := false
  if(TryAndAccept(kToken::kwd_false)){
    fp_synt = NewLiteral(0, unit_.GetType(kBasicTypeId::kBool) , scope_inht, l);
    return std::move(fp_synt);
  }

  //F' := name
  if(TryAndAccept(kToken::name)){
    if(not unit_.Scope().IsDecl(prev_token_string_value_)){
      Error(kErr16);
      //Error recovery: insert it as int
      VarDecl* n = new VarDecl(undeclared_name_, unit_.GetType(kBasicTypeId::kInt)
                           , scope_inht, l);
      unit_.RegisterDecl(prev_token_string_value_, unit_.GetType(kBasicTypeId::kInt), *n);
    }
    fp_synt = NewVar(prev_token_string_value_
                  , unit_.Scope().GetType(prev_token_string_value_)
                  , unit_.Scope().DeclId(prev_token_string_value_)
                  , scope_inht, l);
    return std::move(fp_synt);
  }

  //F := ( E )
  if(TryAndAccept(kToken::lpar)){
    fp_synt = Exprs(scope_inht);
    Accept(kToken::rpar, "[err:14] Expecting rpar.");
  }else{
    //Error recover
    Error("Expecting numerical or lpar");
    if(not ContinueParsing())
      return std::move(fp_synt);
    NextToken();
    return std::move(Factor(scope_inht));
  }

//   std::cout << "<-Fact\n";
  return std::move(fp_synt);
}


} //end namespace RecDescent

