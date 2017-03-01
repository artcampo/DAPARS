#include "ParserLL1RecDesc.hpp"
#include "Node.hpp"
#include "IRDefinition.hpp"
// #include "CompilationUnit.hpp"
#include "ErrorLog/Messages.hpp"
#include "Types.hpp"
#include <iterator>
#include <fstream>
#include <iostream>
#include <exception>
#include <memory>

/*
  Functions for non-terminals have at least a return for each derivation
  (can also have some for error recovery). While in general it's a better
  idea to have a single exit point for each function, this makes the code
  easier to read.
 */

namespace RecDescent{

using namespace Common;
using namespace IRDefinition;
using namespace SubtypesArithmetic;
using namespace Compiler;

ParserLL1RecDesc::ParserLL1RecDesc(std::string const &file_name, CompilationUnit& unit)
//   , Grammar& grammar)
  : BaseParser(file_name, unit), undeclared_name_(std::string("undeclared_name")){}
//   , grammar_(grammar){}

ParserLL1RecDesc::ParserLL1RecDesc(const std::vector<char>& parse_data, CompilationUnit& unit)
//   , Grammar& grammar)
  : BaseParser(parse_data, unit), undeclared_name_(std::string("undeclared_name")){}
//   , grammar_(grammar){}

void ParserLL1RecDesc::Parse(){
  Prog();
}


void ParserLL1RecDesc::Prog(){
  NextToken();

  const ScopeId id = unit_.NewFirstScope();
  PtrProgInit pinit = std::make_unique<AST::ProgInit>(id, CurrentLocus());
  PtrProgEnd  pend  = std::make_unique<AST::ProgEnd> (id, CurrentLocus());

  //parse main function

  PtrFuncDecl pfunc = FuncDecl_(id);
  if(pfunc){
//     std::cout << "Prog" << std::endl;
    if(token_ != Tokenizer::kToken::eof) Error("More data after program.");

    std::unique_ptr<AST::ProgBody> prog =
      std::make_unique<AST::ProgBody>(id, CurrentLocus(), pinit, pend, pfunc);

    unit_.InitAst(prog);
  }else{
    Error("AST not build");
  }

}

//FDECL :=  name (){ STMTS }
PtrFuncDecl ParserLL1RecDesc::FuncDecl_(const ScopeId scope_inht){
  PtrFuncDecl func_decl_synth(nullptr);
  std::string name("func_name");
  Locus       l = CurrentLocus();

  if(Accept(kToken::name, kErr26)) name = prev_token_string_value_;


  Accept(kToken::lpar, kErr27);
  Accept(kToken::rpar, kErr28);


  Accept(kToken::lcbr, kErr29);
  const ScopeId id = unit_.NewFunction(name);

  //STMTS
  std::vector<PtrStatement> stmts_inht;
  PtrBlock stmts_synt = Stmts(stmts_inht, id);
  if(not stmts_synt) Error(kErr31);

  Accept(kToken::rcbr, kErr30);

  func_decl_synth = NewFuncDecl(name, stmts_synt, scope_inht, l);

  unit_.GetFunc(name).SetOriginNode(*func_decl_synth);

  unit_.ExitFunctionDefinition();


  return std::move(func_decl_synth);
}

PtrBlock ParserLL1RecDesc::Stmts(std::vector<PtrStatement>& stmts_inht, const ScopeId scope_inht){
//   std::cout << "stmts\n";
//   if(not ContinueParsing()) return nullptr;
  PtrBlock stmts_synt(nullptr);
  Locus l = CurrentLocus();

  //STMTS => bool {empty} if int ( {nam} {num}
  if( Check({kToken::kwd_if, kToken::kwd_int, kToken::kwd_bool, kToken::lpar
          , kToken::numerical, kToken::name, kToken::kwd_while})){
      PtrStatement stmt_synth = Stmt(scope_inht);

      stmts_inht.push_back(std::move(stmt_synth));
      stmts_synt = Stmts(stmts_inht, scope_inht);
      return std::move(stmts_synt);

    }

  //check follow(Stmts)
  if(AcceptEmpty({kToken::eof, kToken::rcbr}, "[err:3] Block not finishing in eof or rcbr")){
//       unit.
    stmts_synt = NewBlock(stmts_inht, scope_inht, l);
  }

//   std::cout << "<-stmts\n";
  return std::move(stmts_synt);
}

PtrStatement ParserLL1RecDesc::Stmt(const ScopeId scope_inht){
//   std::cout << "stmt\n";
//   if(not ContinueParsing()) return nullptr;
  PtrStatement stmt_synt(nullptr);
  Locus l = CurrentLocus();

  //if(E){STMTS}
  if(TryAndAccept(kToken::kwd_if)){

//      std::cout << "stmt::if\n";
    if(not Accept(kToken::lpar, "[err:6] if missing lpar.")) return std::move(stmt_synt);
    PtrExpr expr_synt = Exprs(scope_inht);
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

  //while(E){STMTS}
  if(TryAndAccept(kToken::kwd_while)){
    if(not Accept(kToken::lpar, "[err:] while missing lpar.")) return std::move(stmt_synt);
    PtrExpr expr_synt = Exprs(scope_inht);
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

  //VarDecl
  if(Check({kToken::kwd_int, kToken::kwd_bool})){
//     std::cout << "stmt::decl stmt\n";
    PtrVarDeclList decl_synt = Decl(scope_inht);
    stmt_synt = NewDeclStmt(decl_synt, scope_inht, l);
    Accept(kToken::semicolon, "[err:5] Expecting semicolon after variable declaration.");
    return std::move(stmt_synt);
  }

  if(Check({kToken::lpar, kToken::name, kToken::numerical})){
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

  //IFELSE -> {empty}  => & * bool {empty} false if int ( {nam} {num} }  true while
  AcceptEmpty( { kToken::astk, kToken::ampersand, kToken::kwd_bool
                , kToken::kwd_false, kToken::kwd_if
                , kToken::kwd_int, kToken::lpar, kToken::name
                , kToken::numerical, kToken::rcbr, kToken::kwd_true
                , kToken::kwd_while},
                "Invalid token after if");

//    std::cout << "<-IfElse\n";
  return std::move(ifelse_synt);
}

PtrVarDeclList  ParserLL1RecDesc::Decl(const ScopeId scope_inht){
//   std::cout << "Decl\n";
  const AST::Type& type_id = Type_();
  const Locus l = CurrentLocus();
  std::vector<PtrVarDecl> name_list_inht;
  PtrVarDeclList decl_synt = NameList(name_list_inht, type_id, scope_inht, l);
  return std::move(decl_synt);
}


PtrVarDeclList
ParserLL1RecDesc::NameList(std::vector<PtrVarDecl>& name_list_inht
                         , const AST::Type& type_inht
                         , const ScopeId scope_inht
                         , const Locus& locus_inht){
//   std::cout << "NameList\n";
  PtrVarDeclList name_list_synt(nullptr);

  //NAME_LIST := name NAME_LIST'
  if(TryAndAccept(kToken::name)){

    name_list_inht.push_back(
      std::move(NewVarDecl(prev_token_string_value_, type_inht, scope_inht, locus_inht))
    );
    VarDecl& n = *name_list_inht[name_list_inht.size() - 1];

//     std::cout << "new var: "<< prev_token_string_value_ << "\n";
    if(not unit_.RegisterDecl(prev_token_string_value_, type_inht, n)){
      Error("[err:15] Symbol already declared.");
    }
    name_list_synt = NameListPrime(name_list_inht, type_inht, scope_inht, locus_inht);
    if(name_list_synt.get() != nullptr) return std::move(name_list_synt);
  }

  //Error recover
  if(not ContinueParsing()) return std::move(name_list_synt);
  NextToken(); //on error advance token
  return NameList(name_list_inht, type_inht, scope_inht, locus_inht);
}

PtrVarDeclList
ParserLL1RecDesc::NameListPrime(std::vector<PtrVarDecl>& name_list_inht
                         , const AST::Type& type_inht
                         , const ScopeId scope_inht
                         , const Locus& locus_inht){
  PtrVarDeclList name_list_synt(nullptr);
//     std::cout << "NameList'\n";

  //NAME_LIST':= , name NAME_LIST'
  bool has_comma = TryAndAccept(kToken::comma);
  if(not has_comma and Check({kToken::name}) and not Check({kToken::semicolon})){
    Error("[err:19] Variables must be separated with comma.");
    has_comma = true;
  }
  if(has_comma){
    if(TryAndAccept(kToken::name)){
      name_list_inht.push_back(
        std::move(NewVarDecl(prev_token_string_value_, type_inht, scope_inht, locus_inht) ));

      VarDecl& n = *name_list_inht[name_list_inht.size() - 1];

      if(not unit_.RegisterDecl(prev_token_string_value_, type_inht, n)){
        Error("[err:15] Symbol already declared.");
      }
      name_list_synt = NameListPrime(name_list_inht, type_inht, scope_inht, locus_inht);
      if(name_list_synt.get() != nullptr) return std::move(name_list_synt);
    }
  }

  //NAME_LIST' -> {empty}  => {empty} ;
  if(AcceptEmpty({kToken::semicolon}, "Name missing")){
    //empty
    name_list_synt = NewVarDeclList(name_list_inht, scope_inht, locus_inht);
    return std::move(name_list_synt);
  }

  //Error recover
  if(not ContinueParsing())
    return std::move(name_list_synt);
  NextToken(); //on error advance token
  return NameListPrime(name_list_inht, type_inht, scope_inht, locus_inht);

}

const AST::Type&  ParserLL1RecDesc::Type_(){
//   std::cout << "Type\n";

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

PtrBlock ParserLL1RecDesc::ParseSubBlock(const ScopeId scope, const std::string& error){
  std::vector<PtrStatement> stmts_inht;
  PtrBlock stmts_synt = Stmts(stmts_inht, scope);
  if(stmts_synt.get() == nullptr) Error(error);
  return std::move(stmts_synt);
}

} //end namespace RecDescent

