#include "ParserLL1RecDescSourceHeaders.hpp"

/*
  Functions for non-terminals have at least a return for each derivation
  (can also have some for error recovery). While in general it's a better
  idea to have a single exit point for each function, this makes the code
  easier to read.
 */

namespace RecDescent{


ParserLL1RecDesc::ParserLL1RecDesc(std::string const &file_name, CompilationUnit& unit)
//   , Grammar& grammar)
  : BaseParser(file_name, unit)
  , inside_member_function_definition_(false)
  , undeclared_name_("undeclared_name")
  , defer_build_call_(false){ BuildTokenVectors();}
//   , grammar_(grammar){}

ParserLL1RecDesc::ParserLL1RecDesc(const std::vector<char>& parse_data, CompilationUnit& unit)
//   , Grammar& grammar)
  : BaseParser(parse_data, unit)
  , inside_member_function_definition_(false)
  , undeclared_name_("undeclared_name")
  , defer_build_call_(false){ BuildTokenVectors(); }
//   , grammar_(grammar){}

void ParserLL1RecDesc::Parse(){
  Prog();
}

void Insert(std::vector<kToken>& vdst, const std::vector<kToken>& vsrc){
  vdst.insert(vdst.end(), vsrc.begin(), vsrc.end());
}

void ParserLL1RecDesc::BuildTokenVectors(){

  //DECL -> TYPE NAME_LIST  => bool int void
  set_types_ =  std::vector<kToken>({
    kToken::kwd_void, kToken::kwd_int, kToken::kwd_bool, kToken::name_type});

  //E -> T E'  => & * false ( {nam} {num}  true
  set_expr_  = std::vector<kToken>({
    kToken::ampersand, kToken::astk, kToken::kwd_false, kToken::lpar
  , kToken::name, kToken::numerical, kToken::kwd_true});

  //STMTS -> STMT STMTS  => & * bool false if int ( {nam} {num} return  true void while
  set_stmts_ = std::vector<kToken>({kToken::kwd_if, kToken::kwd_return
    , kToken::kwd_while});
  Insert(set_stmts_, set_types_);
  Insert(set_stmts_, set_expr_);

  //IFELSE -> {empty}  => & * bool {empty} false if int ( {nam} {num} } return  true void while
  set_ifelse_ = std::vector<kToken>({kToken::rcbr});
  Insert(set_ifelse_, set_stmts_);

  //ARGM -> {empty}  => , {empty} = < or + ) ; 
  set_argm_ = std::vector<kToken>({kToken::comma, kToken::equality
    , kToken::lessthan, kToken::kwd_or, kToken::plus, kToken::rpar, kToken::semicolon});


  //E' -> or RE E'  => or 
  set_eprime_   = std::vector<kToken>({kToken::kwd_or});
  //E' -> {empty}  => , {empty} = ) ; 
  empty_eprime_ = std::vector<kToken>({kToken::comma, kToken::equality
    , kToken::rpar, kToken::semicolon});  
  
  //RE' -> < NE RE'  => < 
  set_releprime_ = std::vector<kToken>({kToken::lessthan});
  
  //RE' -> {empty}  => , {empty} = or ) ; 
  empty_releprime_ = std::vector<kToken>({kToken::comma, kToken::equality
    , kToken::kwd_or, kToken::rpar, kToken::semicolon});  
  

  //NE' -> + T NE'  => +   
  set_numeprime_ = std::vector<kToken>({kToken::plus});

  //NE' -> {empty}  => , {empty} = < or ) ; 
  empty_numeprime_ = std::vector<kToken>({kToken::comma, kToken::equality
    , kToken::lessthan, kToken::kwd_or, kToken::rpar, kToken::semicolon});  
}

PtrBlock ParserLL1RecDesc::ParseSubBlock(const ScopeId scope, const std::string& error){
  std::vector<PtrStatement> stmts_inht;
  PtrBlock stmts_synt = Stmts(stmts_inht, scope);
  if(stmts_synt.get() == nullptr) Error(error);
  return std::move(stmts_synt);
}


} //end namespace RecDescent

