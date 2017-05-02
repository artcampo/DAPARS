#include "ParserLL1RecDescSourceHeaders.hpp"

namespace RecDescent{

////////////////////////////////////////////////////////////////////////////

PtrExpr ParserLL1RecDesc::Expr(const ScopeId scope_inht){

  PtrExpr term_synth = AndExpr(scope_inht);

  if(not term_synth) return std::move(nullptr);

  if(not Check(set_eprime_))
    return std::move(term_synth);

  PtrExpr eprime_synt = ExprPrime(term_synth, scope_inht);
  return std::move(eprime_synt);
}


// E'    := + T E'     ** E'1.inht = new Node(+, E'.inht, T.node)
//                        E'.synt  = E'1.synt
//       |  empty      ** E'.synt  = E'1.synt
PtrExpr ParserLL1RecDesc::ExprPrime(PtrExpr& eprime_inht, const ScopeId scope_inht){
//   std::cout << "Exp'\n";
  PtrExpr eprime_synt(nullptr);
  Locus l = CurrentLocus();

  if(TryAndAccept(kToken::kwd_or)){
    PtrExpr t_synt = AndExpr(scope_inht);
    PtrExpr eprime1_inht = NewBinaryOp(eprime_inht, BinaryOp::kOr, t_synt, scope_inht, l);

    //A new E' will op against current op+
    eprime_synt = ExprPrime(eprime1_inht, scope_inht);
    if(eprime_synt.get() == nullptr) Error("[err:?] operand to or missing");
    return std::move(eprime_synt);
  }

  if(AcceptEmpty(empty_eprime_, "Expecting expression delimiter"))
    eprime_synt = std::move(eprime_inht);

  return std::move(eprime_synt);
}

////////////////////////////////////////////////////////////////////////////

PtrExpr ParserLL1RecDesc::AndExpr(const ScopeId scope_inht){

  PtrExpr term_synth  = RelExpr(scope_inht);

  if(not term_synth) return std::move(nullptr);

  if(not Check(set_andeprime_))
    return std::move(term_synth);

  PtrExpr eprime_synt = AndExprPrime(term_synth, scope_inht);
  return std::move(eprime_synt);
}


// E'    := + T E'     ** E'1.inht = new Node(+, E'.inht, T.node)
//                        E'.synt  = E'1.synt
//       |  empty      ** E'.synt  = E'1.synt
PtrExpr ParserLL1RecDesc::AndExprPrime(PtrExpr& eprime_inht, const ScopeId scope_inht){
//   std::cout << "Exp'\n";
  PtrExpr eprime_synt(nullptr);
  Locus l = CurrentLocus();

  if(TryAndAccept(kToken::kwd_and)){
    PtrExpr t_synt = RelExpr(scope_inht);
    PtrExpr eprime1_inht = NewBinaryOp(eprime_inht, BinaryOp::kAnd, t_synt, scope_inht, l);

    //A new E' will op against current op+
    eprime_synt = AndExprPrime(eprime1_inht, scope_inht);
    if(eprime_synt.get() == nullptr) Error("[err:?] operand to and missing");
    return std::move(eprime_synt);
  }

  if(AcceptEmpty(empty_andeprime_, "Expecting expression delimiter"))
    eprime_synt = std::move(eprime_inht);

  return std::move(eprime_synt);
}


////////////////////////////////////////////////////////////////////////////


PtrExpr ParserLL1RecDesc::RelExpr(const ScopeId scope_inht){

  PtrExpr term_synth  = NumExpr(scope_inht);

  if(not term_synth) return std::move(nullptr);

  if(not Check(set_releprime_))
    return std::move(term_synth);

  PtrExpr eprime_synt = RelExprPrime(term_synth, scope_inht);
  return std::move(eprime_synt);
}


PtrExpr ParserLL1RecDesc::RelExprPrime(PtrExpr& eprime_inht, const ScopeId scope_inht){
//   std::cout << "Exp'\n";
  PtrExpr eprime_synt(nullptr);
  Locus l = CurrentLocus();

  BinaryOp::Op op;
  bool op_accepted = false;
  if(TryAndAccept(kToken::lessthan)){
    op = BinaryOp::kLessThan;
    op_accepted = true;
  }else if(TryAndAccept(kToken::equalto)){
    op = BinaryOp::kEqualTo;
    op_accepted = true;
  }

  if(op_accepted){
    PtrExpr t_synt = Term(scope_inht);
    PtrExpr eprime1_inht = NewBinaryOp(eprime_inht, op, t_synt, scope_inht, l);

    eprime_synt = RelExprPrime(eprime1_inht, scope_inht);
    if(eprime_synt.get() == nullptr){
      if(op == BinaryOp::kLessThan) Error("[] operand to < missing");
      if(op == BinaryOp::kEqualTo) Error("[] operand to == missing");
    }
    return std::move(eprime_synt);
  }

  if(AcceptEmpty(empty_releprime_,"Expecting expression delimiter"))
    eprime_synt = std::move(eprime_inht);

  return std::move(eprime_synt);
}


////////////////////////////////////////////////////////////////////////////

//NE -> T NE'  => & * false ( {nam} {num}  true
PtrExpr ParserLL1RecDesc::NumExpr(const ScopeId scope_inht){

  PtrExpr term_synth  = Term(scope_inht);

  if(not term_synth) return std::move(nullptr);

  if(not Check(set_numeprime_))
    return std::move(term_synth);

  PtrExpr eprime_synt = NumExprPrime(term_synth, scope_inht);
  return std::move(eprime_synt);
}


// E'    := + T E'     ** E'1.inht = new Node(+, E'.inht, T.node)
//                        E'.synt  = E'1.synt
//       |  empty      ** E'.synt  = E'1.synt
PtrExpr ParserLL1RecDesc::NumExprPrime(PtrExpr& eprime_inht, const ScopeId scope_inht){
//   std::cout << "Exp'\n";
  PtrExpr eprime_synt(nullptr);
  Locus l = CurrentLocus();

  if(TryAndAccept(kToken::plus)){
    PtrExpr t_synt = Term(scope_inht);
    PtrExpr eprime1_inht = NewBinaryOp(eprime_inht, BinaryOp::kAdd, t_synt, scope_inht, l);
    //TODO: do not use VM's definition IR_ADD

    //A new E' will op against current op+
    eprime_synt = NumExprPrime(eprime1_inht, scope_inht);
    if(eprime_synt.get() == nullptr)
      Error("[err:12] operand to + missing");
    return std::move(eprime_synt);

  }

  //E' -> {empty}  => , {empty} = ) ;
  if(AcceptEmpty(empty_numeprime_,
//       , kToken::rcbr},  //not in original set but better error detection
      "Expecting expression delimiter")){
    eprime_synt = std::move(eprime_inht);
  }

//   std::cout << "<-Exp'\n";
  return std::move(eprime_synt);
}

////////////////////////////////////////////////////////////////////////////

PtrExpr ParserLL1RecDesc::Term(const ScopeId scope_inht){
//   std::cout << "T\n";
  return std::move(Factor(scope_inht));
}


////////////////////////////////////////////////////////////////////////////

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
    std::string var_name(prev_token_string_value_);
    const AST::Type* var_type;
    PtrExprVar var = BuildVar(var_name, var_type, scope_inht);
    return std::move(Argm(var_name, var, *var_type, scope_inht, l));
  }

  //F := ( E )
  if(TryAndAccept(kToken::lpar)){
    fp_synt = Expr(scope_inht);
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


////////////////////////////////////////////////////////////////////////////

//This function has to consider when are we inside a class definition,
//as in that case access to yet not declared members is valid
//var_type is an output of the function, which could be type_unknown
PtrExprVar ParserLL1RecDesc::BuildVar(const std::string& var_name
  , const AST::Type*& var_type, const ScopeId scope_inht){
  Locus l = CurrentLocus();
  bool found_in_lexical_scope = false;
  bool found_in_hierarchical_scope = false;
  Symbols::SymbolId dec_sid;

  if(unit_.HasDecl(var_name, scope_inht)){
    found_in_lexical_scope = true;
  } else if(not inside_member_function_definition_){
    //Error depends on what we think the name refers to
    if(Check({kToken::lpar}))      Error(kErr46);
    else if(Check({kToken::dot}))  Error(kErr85);
    else Error(kErr16);

    //Todo: this recovery will be insufficiente for calls/objects
    //Error recovery: insert it as int
    //TODO: this ptr will leak
    BuildNameDecl(var_name, unit_.GetTypeInt(), scope_inht, l);
    found_in_lexical_scope = true;
  }else{
    //inside_member_function_definition_ = true
    if(unit_.HasDecl(var_name, member_scope_id_))
      found_in_hierarchical_scope = true;
    else{
      //Var could be member still not declared
      var_type  = &unit_.GetTypeUnknown();
      dec_sid   = Symbols::Symbol::UnknownSymbol();
    }
  }

  if(found_in_lexical_scope){
    var_type = &unit_.GetType(var_name, scope_inht);
    dec_sid  = unit_.DeclId(var_name, scope_inht);
  }
  if(found_in_hierarchical_scope){
    var_type = &unit_.GetType(var_name, member_scope_id_);
    dec_sid  = unit_.DeclId(var_name, member_scope_id_);
  }

  //const std::string name = prev_token_string_value_;

  return std::move( NewVar(var_name
                  , *var_type
                  , dec_sid
                  , scope_inht, l) );
}

} //end namespace RecDescent

