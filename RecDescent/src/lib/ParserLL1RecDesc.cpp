#include "ParserLL1RecDesc.hpp"
#include "Node.hpp"
#include "IRDefinition.hpp"

#include <iterator>
#include <fstream>
#include <iostream>
#include <exception>


namespace RecDescent{

using namespace Common;
using namespace IRDefinition;
using namespace SubtypesArithmetic;


ParserLL1RecDesc::ParserLL1RecDesc(std::string const &file_name, CompilationUnit& unit)
//   , Grammar& grammar)
  : BaseParser(file_name, unit){}
//   , grammar_(grammar){}

ParserLL1RecDesc::ParserLL1RecDesc(const std::vector<char>& parse_data, CompilationUnit& unit)
//   , Grammar& grammar)
  : BaseParser(parse_data, unit){}
//   , grammar_(grammar){}

void ParserLL1RecDesc::Parse(){
  Prog();
}


void ParserLL1RecDesc::Prog(){
  NextToken();
  std::vector<Statement*> stmts_inht;
  const ScopeId id = unit_.NewFirstScope();
  Block* stmts_synt = Stmts(stmts_inht, id);

  if(stmts_synt != nullptr){
//     std::cout << "Prog" << std::endl;
    if(token_ != Tokenizer::kToken::eof){
      Error("More data after program.");
    }
  }
  unit_.ast_.block_ = stmts_synt;
}

Block* ParserLL1RecDesc::Stmts(std::vector<Statement*>& stmts_inht, const ScopeId scope_inht){
//   std::cout << "stmts\n";
//   if(not ContinueParsing()) return nullptr;
  Block* stmts_synt = nullptr;
  Locus l = CurrentLocus();

  //STMTS => bool {empty} if int ( {nam} {num}
  if( Check({kToken::kwd_if, kToken::kwd_int, kToken::kwd_bool, kToken::lpar
          , kToken::numerical, kToken::name})){
      Statement* stmt_synth = Stmt(scope_inht);

      stmts_inht.push_back(stmt_synth);
      stmts_synt = Stmts(stmts_inht, scope_inht);

    }
  else{
    //check follow(Stmts)
    if(AcceptEmpty({kToken::eof, kToken::rcbr}, "[err:3] Block not finishing in eof or rcbr")){
//       unit.
      stmts_synt = NewBlock(stmts_inht, scope_inht, l);
    }
  }
//   std::cout << "<-stmts\n";
  return stmts_synt;
}

Statement* ParserLL1RecDesc::Stmt(const ScopeId scope_inht){
//   std::cout << "stmt\n";
//   if(not ContinueParsing()) return nullptr;
  Statement* stmt_synt = nullptr;

  Locus l = CurrentLocus();
  if(TryAndAccept(kToken::kwd_if)){
    //if(E){STMTS}
//     std::cout << "stmt::if\n";
    if(not Accept(kToken::lpar, "[err:6] if missing lpar.")) return nullptr;
    Node* expr_synt = Exprs(scope_inht);
    if(expr_synt == nullptr) Error("[err:7] if condition wrong.");

    if(not Accept(kToken::rpar, "[err:8] if missing rpar.")) return nullptr;

    if(not Accept(kToken::lcbr, "[err:9] if missing lcbr.")) return nullptr;
    const ScopeId nested_id = unit_.NewNestedScope();
    std::vector<Statement*> stmts_inht;
    Block* stmts_synt = Stmts(stmts_inht, nested_id);
    if(stmts_synt == nullptr){
      Error("[err:10] if missing then statement.");
      return nullptr;
    }
    if(not Accept(kToken::rcbr, "[err:11] if missing rcbr.")) return nullptr;

    unit_.RestoreScope();
    Block* ifelse_synt = IfElse(scope_inht);

    if(ifelse_synt == nullptr)
      stmt_synt = NewIfStmt(dynamic_cast<Expr*>(expr_synt), stmts_synt, scope_inht, l);
    else
      stmt_synt = NewIfStmt(dynamic_cast<Expr*>(expr_synt), stmts_synt, ifelse_synt, scope_inht, l);

  }else if(Check({kToken::kwd_int, kToken::kwd_bool})){
//     std::cout << "stmt::decl stmt\n";
    VarDeclList* decl_synt = Decl(scope_inht);
    stmt_synt = NewDeclStmt(decl_synt, scope_inht, l);
    Accept(kToken::semicolon, "[err:5] Expecting semicolon after variable declaration.");
  }
  else if(Check({kToken::lpar, kToken::name, kToken::numerical})){
//     std::cout << "stmt::assign stmt\n";
    Expr* expr_lhs  = Exprs(scope_inht);
    if(not Accept(kToken::equality, "[err:] assignment missing '='")){
      return nullptr;
    }
    Expr* expr_rhs  = Exprs(scope_inht);
    stmt_synt       = NewAssignStmt(expr_lhs, expr_rhs, scope_inht, l);

    Accept(kToken::semicolon, "[err:4] Expecting semicolon after Expr.");
  }else{
    Error("Expr wrong");
  }
//   std::cout << "<-stmt\n";
  return stmt_synt;
}

//TODO: should return Expr*
Expr* ParserLL1RecDesc::Exprs(const ScopeId scope_inht){
//   std::cout << "Exp\n";
  Expr* eprime_synt = nullptr;
  Expr* term_synth  = Term(scope_inht);

  if(term_synth != nullptr){
    eprime_synt = ExprPrime(term_synth, scope_inht);

//     if(eprime_synt == nullptr)
//       Error("E' missing");
  }else {
    Error("[err:13] Term missing.");
  }
//   std::cout << "<-Exp\n";
  return eprime_synt;
}


Expr* ParserLL1RecDesc::Term(const ScopeId scope_inht){
//   std::cout << "T\n";
  return Factor(scope_inht);
}



// E'    := + T E'     ** E'1.inht = new Node(+, E'.inht, T.node)
//                        E'.synt  = E'1.synt
//       |  empty      ** E'.synt  = E'1.synt
Expr* ParserLL1RecDesc::ExprPrime(Expr* eprime_inht, const ScopeId scope_inht){
//   std::cout << "Exp'\n";
  Expr* eprime_synt = nullptr;
  Locus l = CurrentLocus();

  if(TryAndAccept(kToken::plus)){
    Expr* t_synt = Term(scope_inht);
    Expr* eprime1_inht = NewBinaryOp(eprime_inht, IR_ADD, t_synt, scope_inht, l);

    //A new E' will op against current op+
    eprime_synt = ExprPrime(eprime1_inht, scope_inht);
    if(eprime_synt == nullptr)
      Error("[err:12] operand to + missing");

  } else{
    //check Follow(E')
    if(AcceptEmpty({kToken::rpar, kToken::semicolon, kToken::equality},
        "Expecting +")){
      eprime_synt = eprime_inht;
    }
  }

//   std::cout << "<-Exp'\n";
  return eprime_synt;
}


// F := ( E ) | numerical
Expr* ParserLL1RecDesc::Factor(const ScopeId scope_inht){
//   std::cout << "Fact\n";
  Expr* f_synt;
  Locus l = CurrentLocus();

  if(TryAndAccept(kToken::numerical)){
    f_synt = NewLiteral(prev_token_int_value_, kFirstClass::typeid_int
                      , scope_inht, l);
  }else if(TryAndAccept(kToken::name)){
    if(not unit_.Scope().IsDecl(prev_token_string_value_)){
      Error("[error:16] Var used before declaration");
      //Error recovery: insert it as int
      unit_.Scope().RegDecl(prev_token_string_value_, kFirstClass::typeid_int);
    }
    f_synt = NewVar(prev_token_string_value_
                  , unit_.Scope().GetTypeId(prev_token_string_value_)
                  , scope_inht, l);
  }else if(TryAndAccept(kToken::lpar)){
    f_synt = Exprs(scope_inht);
    Accept(kToken::rpar, "[err:14] Expecting rpar.");
  }else{
    //Error recover
    Error("Expecting numerical or lpar");
    if(not ContinueParsing()) return nullptr;
    NextToken();
    return Factor(scope_inht);
  }

//   std::cout << "<-Fact\n";
  return f_synt;
}


Block* ParserLL1RecDesc::IfElse(const ScopeId scope_inht){
//   std::cout << "IfElse\n";
  if(not ContinueParsing()) return nullptr;
  Block* ifelse_synt = nullptr;

  if(TryAndAccept(kToken::kwd_else)){

    Accept(kToken::lcbr, "else missing lcbr.");
    std::vector<Statement*> stmts_inht;
    const ScopeId nested_id = unit_.NewNestedScope();
    Block* stmts_synt = Stmts(stmts_inht, nested_id);
    if(stmts_synt == nullptr) Error("Statements within else wrong.");
    ifelse_synt = stmts_synt;
    Accept(kToken::rcbr, "else missing rcbr.");
    unit_.RestoreScope();
  }else{
    AcceptEmpty( {kToken::kwd_bool, kToken::eof, kToken::kwd_if
                , kToken::kwd_int, kToken::lpar, kToken::name
                , kToken::numerical, kToken::rcbr},
                "Invalid token after if");
  }
//   std::cout << "<-IfElse\n";
  return ifelse_synt;
}

VarDeclList*  ParserLL1RecDesc::Decl(const ScopeId scope_inht){
//   std::cout << "Decl\n";
  const TypeId type_id = Type();
  const Locus l = CurrentLocus();
  std::vector<VarDecl*> name_list_inht;
  VarDeclList* decl_synt = NameList(name_list_inht, type_id, scope_inht, l);
  return decl_synt;
}

VarDeclList*
ParserLL1RecDesc::NameList(std::vector<VarDecl*>& name_list_inht
                         , const TypeId& type_inht
                         , const ScopeId scope_inht
                         , const Locus& locus_inht){
//   std::cout << "NameList\n";
  VarDeclList* name_list_synt = nullptr;

  if(TryAndAccept(kToken::name)){
    name_list_inht.push_back(
      NewVarDecl(prev_token_string_value_, type_inht, scope_inht, locus_inht) );
//     std::cout << "new var: "<< prev_token_string_value_ << "\n";
    if(not unit_.Scope().RegDecl(prev_token_string_value_, type_inht)){
      Error("[err:15] Symbol already declared.");
    }
    name_list_synt = NameListPrime(name_list_inht, type_inht, scope_inht, locus_inht);
    if(name_list_synt != nullptr) return name_list_synt;
  }

  //Error recover
  if(not ContinueParsing()) return nullptr;
  NextToken(); //on error advance token
  return NameList(name_list_inht, type_inht, scope_inht, locus_inht);

  return name_list_synt;
}

VarDeclList*
ParserLL1RecDesc::NameListPrime(std::vector<VarDecl*>& name_list_inht
                         , const TypeId& type_inht
                         , const ScopeId scope_inht
                         , const Locus& locus_inht){
//   std::cout << "NameList\n";
  VarDeclList* name_list_synt = nullptr;

  bool has_comma = TryAndAccept(kToken::comma);
  if(not has_comma and Check({kToken::name}) and not Check({kToken::semicolon})){
    Error("[err:19] Variables must be separated with comma.");
    has_comma = true;
  }

  if(has_comma){
    if(TryAndAccept(kToken::name)){
      name_list_inht.push_back(
        NewVarDecl(prev_token_string_value_, type_inht, scope_inht, locus_inht) );

      if(not unit_.Scope().RegDecl(prev_token_string_value_, type_inht)){
        Error("[err:15] Symbol already declared.");
      }
      name_list_synt = NameListPrime(name_list_inht, type_inht, scope_inht, locus_inht);
      if(name_list_synt != nullptr) return name_list_synt;
    }
  }


  if(AcceptEmpty({kToken::semicolon}, "Name missing")){
    //empty
    name_list_synt = NewVarDeclList(name_list_inht, scope_inht, locus_inht);
    return name_list_synt;
  }
  //Error recover
  if(not ContinueParsing()) return nullptr;
  NextToken(); //on error advance token
  return NameListPrime(name_list_inht, type_inht, scope_inht, locus_inht);

}

const TypeId  ParserLL1RecDesc::Type(){
//   std::cout << "Type\n";
  TypeId t;
  if(TryAndAccept(kToken::kwd_int)){
    t = TypeId::Int();
  }
  else if(TryAndAccept(kToken::kwd_bool)){
    t = TypeId::Bool();
  }
  else{
    //Error recover
    Error("Type missing");
    t = TypeId::Int();
  }
  return t;
}


} //end namespace RecDescent

