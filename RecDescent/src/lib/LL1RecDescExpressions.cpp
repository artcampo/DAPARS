#include "ParserLL1RecDescSourceHeaders.hpp"

namespace RecDescent{


//TODO: should return PtrExpr
PtrExpr ParserLL1RecDesc::Exprs(const ScopeId scope_inht){

  PtrExpr term_synth  = Term(scope_inht);

  if(not term_synth) return std::move(nullptr);

  if(not Check(set_eprime_))
    return std::move(term_synth);

  PtrExpr eprime_synt = ExprPrime(term_synth, scope_inht);
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

  //E' -> {empty}  => , {empty} = ) ;
  if(AcceptEmpty(empty_eprime_,
//       , kToken::rcbr},  //not in original set but better error detection
      "Expecting expression delimiter")){
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

  //F := F'
  return std::move(FactorPrime(scope_inht));
}

// F := ( E ) | numerical
PtrExpr ParserLL1RecDesc::FactorPrime(const ScopeId scope_inht){
//   std::cout << "Fact\n";
  PtrExpr fp_synt(nullptr);
  Locus l = CurrentLocus();

  //F' := numerical
  if(TryAndAccept(kToken::numerical)){
    const AST::Type& t = unit_.GetTypeInt();
    fp_synt = NewLiteral(prev_token_int_value_, t
                      , scope_inht, l);
    return std::move(fp_synt);
  }

  //F' := true
  if(TryAndAccept(kToken::kwd_true)){
    fp_synt = NewLiteral(1, unit_.GetTypeBool(), scope_inht, l);
    return std::move(fp_synt);
  }

  //F' := false
  if(TryAndAccept(kToken::kwd_false)){
    fp_synt = NewLiteral(0, unit_.GetTypeBool() , scope_inht, l);
    return std::move(fp_synt);
  }


  //F' := name ARGM
  if(TryAndAccept(kToken::name)){
    if(not unit_.Scope().HasDecl(prev_token_string_value_)){
      Error(kErr16);
      //Error recovery: insert it as int
      VarDecl* n = new VarDecl(undeclared_name_, unit_.GetTypeInt()
                           , scope_inht, l);
      unit_.RegisterDecl(prev_token_string_value_, unit_.GetTypeInt(), *n);
    }

    const std::string name = prev_token_string_value_;
    fp_synt = Argm(name, scope_inht, l);
    if(not fp_synt)
      fp_synt = NewVar(prev_token_string_value_
                    , unit_.GetType(prev_token_string_value_)
                    , unit_.Scope().DeclId(prev_token_string_value_)
                    , scope_inht, l);
    return std::move(fp_synt);
  }

  //F := ( E )
  if(TryAndAccept(kToken::lpar)){
    fp_synt = Exprs(scope_inht);
    Accept(kToken::rpar, kErr14);
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

