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
  , undeclared_name_(std::string("undeclared_name")){ BuildTokenVectors();}
//   , grammar_(grammar){}

ParserLL1RecDesc::ParserLL1RecDesc(const std::vector<char>& parse_data, CompilationUnit& unit)
//   , Grammar& grammar)
  : BaseParser(parse_data, unit)
  , undeclared_name_(std::string("undeclared_name")){}
//   , grammar_(grammar){}

void ParserLL1RecDesc::Parse(){
  Prog();
}

void ParserLL1RecDesc::BuildTokenVectors(){

  set_types_ =  std::vector<kToken>({
    kToken::kwd_void, kToken::kwd_int, kToken::kwd_bool});
}

PtrBlock ParserLL1RecDesc::ParseSubBlock(const ScopeId scope, const std::string& error){
  std::vector<PtrStatement> stmts_inht;
  PtrBlock stmts_synt = Stmts(stmts_inht, scope);
  if(stmts_synt.get() == nullptr) Error(error);
  return std::move(stmts_synt);
}

void ParserLL1RecDesc::Prog(){
  NextToken();

  const ScopeId id = unit_.NewFirstScope();
  PtrProgInit pinit = std::make_unique<AST::ProgInit>(id, CurrentLocus());
  PtrProgEnd  pend  = std::make_unique<AST::ProgEnd> (id, CurrentLocus());

  //parse all functions
  std::vector<PtrFuncDef> func_defs;
  FuncDefList(func_defs, id);

  if(func_defs.size() > 0){
    if(token_ != Tokenizer::kToken::eof) Error("More data after program.");

    std::unique_ptr<AST::ProgBody> prog =
      std::make_unique<AST::ProgBody>(id, CurrentLocus(), pinit, pend, func_defs);

    unit_.InitAst(prog);
  }else{
    Error("AST not build");
  }

}

PtrBlock ParserLL1RecDesc::Stmts(std::vector<PtrStatement>& stmts_inht, const ScopeId scope_inht){
//   std::cout << "stmts\n";
//   if(not ContinueParsing()) return nullptr;
  PtrBlock stmts_synt(nullptr);
  Locus l = CurrentLocus();

  //STMTS -> STMT STMTS  => & * bool false if int ( {nam} {num}  true while
  if( Check({ kToken::ampersand, kToken::astk, kToken::kwd_bool, kToken::kwd_if
            , kToken::kwd_int,  kToken::lpar, kToken::name, kToken::numerical
            , kToken::kwd_while})){
      PtrStatement stmt_synth = Stmt(scope_inht);

      stmts_inht.push_back(std::move(stmt_synth));
      stmts_synt = Stmts(stmts_inht, scope_inht);
      return std::move(stmts_synt);

    }

  //STMTS => }
  if(AcceptEmpty({kToken::rcbr}, "[err:3] Block not finishing in rcbr")){
//       unit.
    stmts_synt = NewBlock(stmts_inht, scope_inht, l);
  }

//   std::cout << "<-stmts\n";
  return std::move(stmts_synt);
}

} //end namespace RecDescent

