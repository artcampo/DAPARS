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
    PtrExpr expr_synt = Expr(scope_inht);
    if(expr_synt.get() == nullptr) Error("[err:7] if condition wrong.");

    if(not Accept(kToken::rpar, "[err:8] if missing rpar.")) return std::move(stmt_synt);

    if(not Accept(kToken::lcbr, "[err:9] if missing lcbr.")) return std::move(stmt_synt);
    const ScopeId nested_id = unit_.NewNestedScope();
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
    PtrExpr expr_synt = Expr(scope_inht);
    if(expr_synt.get() == nullptr) Error("[err:] while condition wrong.");
    if(not Accept(kToken::rpar, "[err:] while missing rpar.")) return std::move(stmt_synt);

    if(not Accept(kToken::lcbr, "[err:] while missing lcbr.")) return std::move(stmt_synt);
    const ScopeId nested_id = unit_.NewNestedScope();
    PtrBlock stmts_synt = ParseSubBlock(nested_id, "[err:] while missing body.");
    if(stmts_synt.get() == nullptr) return std::move(stmt_synt);
    if(not Accept(kToken::rcbr, "[err:] while missing rcbr.")) return std::move(stmt_synt);

    unit_.RestoreScope();
    stmt_synt = NewWhileStmt(expr_synt, stmts_synt, scope_inht, l);
    return std::move(stmt_synt);
  }

  //STMT -> DECL ;  => bool int void
  if(Check(set_types_)){
    stmt_synt = DeclStmt(scope_inht);
//     std::cout << "stmt::decl stmt\n";
    return std::move(stmt_synt);
  }

  //STMT -> E = E ;  => & * false ( {nam} {num}  true
  if(Check(set_expr_)){
//     std::cout << "stmt::assign stmt\n";
    PtrExpr expr_lhs  = Expr(scope_inht);
    if(not Accept(kToken::equality, "[err:] assignment missing '='")){
      return std::move(stmt_synt);
    }
    PtrExpr expr_rhs  = Expr(scope_inht);
    stmt_synt         = NewAssignStmt(expr_lhs, expr_rhs, scope_inht, l);

    Accept(kToken::semicolon, kErr4);
    return std::move(stmt_synt);
  }

  //STMT -> return E ;  => return
  if(TryAndAccept({kToken::kwd_return})){
//     std::cout << "ReturnStmt\n";
    PtrExpr exp = Expr(scope_inht);
    Accept(kToken::semicolon, kErr43);

    if(not unit_.InsideFunctionDefinition()){
      Error(kErr44);
      return std::move(nullptr);
    }

    stmt_synt   = NewReturnStmt(exp, unit_.CurrentFuncDecl(), scope_inht, l);
//     std::cout << "RetStmt created\n";

    return std::move(stmt_synt);
  }

  Error("Stmt wrong");
//   std::cout << "<-stmt\n";
  return std::move(nullptr);
}



PtrBlock ParserLL1RecDesc::IfElse(const ScopeId scope_inht){
//   std::cout << "IfElse\n";
  PtrBlock ifelse_synt(nullptr);
  if(not ContinueParsing())
    return std::move(ifelse_synt);

  if(TryAndAccept(kToken::kwd_else)){
    Accept(kToken::lcbr, "else missing lcbr.");
    std::vector<PtrStatement> stmts_inht;
    const ScopeId nested_id = unit_.NewNestedScope();
    PtrBlock stmts_synt = Stmts(stmts_inht, nested_id);
    if(stmts_synt.get() == nullptr) Error("Statements within else wrong.");
    ifelse_synt = std::move(stmts_synt);
    Accept(kToken::rcbr, "else missing rcbr.");
    unit_.RestoreScope();
    return std::move(ifelse_synt);
  }

  //IFELSE -> {empty}  => & * bool {empty} false if int ( {nam} {num} }  true void while
  AcceptEmpty( set_ifelse_, "Invalid token after if");

//    std::cout << "<-IfElse\n";
  return std::move(ifelse_synt);
}

PtrStatement ParserLL1RecDesc::DeclStmt(const ScopeId scope_inht){
  Locus l = CurrentLocus();
  PtrVarDeclList decl_synt = DeclList(scope_inht);
  PtrStatement stmt_synt = NewDeclStmt(decl_synt, scope_inht, l);
  Accept(kToken::semicolon, "[err:5] Expecting semicolon after variable declaration.");
  return std::move(stmt_synt);
}

//TODO: move to another file (?)
PtrVarDeclList ParserLL1RecDesc::DeclList(const ScopeId scope_inht){
  Locus l = CurrentLocus();
  return std::move( Decl(scope_inht) );
}



} //end namespace RecDescent

