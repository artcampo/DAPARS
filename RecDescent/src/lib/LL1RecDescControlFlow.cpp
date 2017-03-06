#include "ParserLL1RecDescSourceHeaders.hpp"

namespace RecDescent{

PtrStatement ParserLL1RecDesc::Stmt(const ScopeId scope_inht){
//   std::cout << "stmt\n";
//   if(not ContinueParsing()) return nullptr;
  PtrStatement stmt_synt(nullptr);
  Locus l = CurrentLocus();

  //STMT -> if ( E ) { STMTS } IFELSE  => if
  if(TryAndAccept(kToken::kwd_if)){

//      std::cout << "stmt::if\n";
    if(not Accept(kToken::lpar, "[err:6] if missing lpar.")) return std::move(stmt_synt);
    PtrExpr expr_synt = Exprs(scope_inht);
    if(expr_synt.get() == nullptr) Error("[err:7] if condition wrong.");

    if(not Accept(kToken::rpar, "[err:8] if missing rpar.")) return std::move(stmt_synt);

    if(not Accept(kToken::lcbr, "[err:9] if missing lcbr.")) return std::move(stmt_synt);
    const ScopeId nested_id = unit_.NewNestedScope( scope_owner_id_.top());
    PtrBlock stmts_synt = ParseSubBlock(nested_id, "[err:10] if missing then statement.");
    if(stmts_synt.get() == nullptr) return std::move(stmt_synt);
    if(not Accept(kToken::rcbr, "[err:11] if missing rcbr.")) return std::move(stmt_synt);

    unit_.RestoreScope();
    PtrBlock ifelse_synt = IfElse(scope_inht);

    if(ifelse_synt.get() == nullptr)
      stmt_synt = NewIfStmt(expr_synt, stmts_synt, scope_inht, l);
    else
      stmt_synt = NewIfStmt(expr_synt, stmts_synt, ifelse_synt, scope_inht, l);
//     std::cout << "<-stmt::if\n";
    return std::move(stmt_synt);

  }

  //STMT -> while ( E ) { STMTS }  => while
  //while(E){STMTS}
  if(TryAndAccept(kToken::kwd_while)){
    if(not Accept(kToken::lpar, "[err:] while missing lpar.")) return std::move(stmt_synt);
    PtrExpr expr_synt = Exprs(scope_inht);
    if(expr_synt.get() == nullptr) Error("[err:] while condition wrong.");
    if(not Accept(kToken::rpar, "[err:] while missing rpar.")) return std::move(stmt_synt);

    if(not Accept(kToken::lcbr, "[err:] while missing lcbr.")) return std::move(stmt_synt);
    const ScopeId nested_id = unit_.NewNestedScope(scope_owner_id_.top());
    PtrBlock stmts_synt = ParseSubBlock(nested_id, "[err:] while missing body.");
    if(stmts_synt.get() == nullptr) return std::move(stmt_synt);
    if(not Accept(kToken::rcbr, "[err:] while missing rcbr.")) return std::move(stmt_synt);

    unit_.RestoreScope();
    stmt_synt = NewWhileStmt(expr_synt, stmts_synt, scope_inht, l);
    return std::move(stmt_synt);
  }

  //STMT -> DECL ;  => bool int
  if(Check({kToken::kwd_int, kToken::kwd_bool})){
//     std::cout << "stmt::decl stmt\n";
    PtrVarDeclList decl_synt = Decl(scope_inht);
    stmt_synt = NewDeclStmt(decl_synt, scope_inht, l);
    Accept(kToken::semicolon, "[err:5] Expecting semicolon after variable declaration.");
    return std::move(stmt_synt);
  }

  //STMT -> E = E ;  => & * false ( {nam} {num}  true
  if(Check(set_expr_)){
//     std::cout << "stmt::assign stmt\n";
    PtrExpr expr_lhs  = Exprs(scope_inht);
    if(not Accept(kToken::equality, "[err:] assignment missing '='")){
      return std::move(stmt_synt);
    }
    PtrExpr expr_rhs  = Exprs(scope_inht);
    stmt_synt       = NewAssignStmt(expr_lhs, expr_rhs, scope_inht, l);

    Accept(kToken::semicolon, "[err:4] Expecting semicolon after Expr.");
  }else{
    Error("Expr wrong");
  }
//   std::cout << "<-stmt\n";
  return std::move(stmt_synt);
}



PtrBlock ParserLL1RecDesc::IfElse(const ScopeId scope_inht){
//   std::cout << "IfElse\n";
  PtrBlock ifelse_synt(nullptr);
  if(not ContinueParsing())
    return std::move(ifelse_synt);


  if(TryAndAccept(kToken::kwd_else)){

    Accept(kToken::lcbr, "else missing lcbr.");
    std::vector<PtrStatement> stmts_inht;
    const ScopeId nested_id = unit_.NewNestedScope(scope_owner_id_.top());
    PtrBlock stmts_synt = Stmts(stmts_inht, nested_id);
    if(stmts_synt.get() == nullptr) Error("Statements within else wrong.");
    ifelse_synt = std::move(stmts_synt);
    Accept(kToken::rcbr, "else missing rcbr.");
    unit_.RestoreScope();
    return std::move(ifelse_synt);
  }

  //IFELSE -> {empty}  => & * bool {empty} false if int ( {nam} {num} }  true void while
  AcceptEmpty( { kToken::astk, kToken::ampersand, kToken::kwd_bool
                , kToken::kwd_false, kToken::kwd_if
                , kToken::kwd_int, kToken::lpar, kToken::name
                , kToken::numerical, kToken::rcbr, kToken::kwd_true
                , kToken::kwd_void, kToken::kwd_while},
                "Invalid token after if");

//    std::cout << "<-IfElse\n";
  return std::move(ifelse_synt);
}

} //end namespace RecDescent

