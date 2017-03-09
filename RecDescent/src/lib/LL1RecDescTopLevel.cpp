#include "ParserLL1RecDescSourceHeaders.hpp"

/*
  Functions for non-terminals have at least a return for each derivation
  (can also have some for error recovery). While in general it's a better
  idea to have a single exit point for each function, this makes the code
  easier to read.
 */

namespace RecDescent{


void ParserLL1RecDesc::Prog(){
  NextToken();

  scope_owner_id_.push(unit_.GlobalScopeOwnerId());
  const ScopeId id = unit_.GlobalScopeId( scope_owner_id_.top() );
  PtrProgInit pinit = std::make_unique<AST::ProgInit>(id, CurrentLocus());
  PtrProgEnd  pend  = std::make_unique<AST::ProgEnd> (id, CurrentLocus());

  //parse all functions
  std::vector<PtrFuncDef>   func_defs;
  std::vector<PtrClassDef>  class_defs;
  DefList(func_defs, class_defs, id);

  if(func_defs.size() == 0 or NumFatalErrors() != 0){
    if(func_defs.size() == 0) Error("AST not build (no functions)");
    if(NumFatalErrors() != 0) Error("AST not build (fatal errors)");
    return;
  }

  if(token_ != Tokenizer::kToken::eof) Error("More data after program.");

  std::unique_ptr<AST::ProgBody> prog =
    std::make_unique<AST::ProgBody>(id, CurrentLocus(), pinit, pend, func_defs
                                  , class_defs);

  unit_.InitAst(prog);
}

void ParserLL1RecDesc::DefList(std::vector<PtrFuncDef>& fdefl_inht
    , std::vector<PtrClassDef>& cdefl_inht
    , const ScopeId scope_inht){

  if(Check(set_types_ )){
    PtrFuncDef fdef = FuncDef_(scope_inht);
    if(fdef){
      fdefl_inht.push_back( std::move(fdef));
      DefList(fdefl_inht, cdefl_inht, scope_inht);
    }
    return;
  }

  if(Check({kToken::kwd_class})){
    PtrClassDef cdef = ClassDef_(scope_inht);
    if(cdef){
      cdefl_inht.push_back( std::move(cdef));
      DefList(fdefl_inht, cdefl_inht, scope_inht);
    }
    return;
  }

  if(not Check({kToken::eof})) Error(kErr33);
}


PtrBlock ParserLL1RecDesc::Stmts(std::vector<PtrStatement>& stmts_inht, const ScopeId scope_inht){
//   std::cout << "stmts\n";
//   if(not ContinueParsing()) return nullptr;
  PtrBlock stmts_synt(nullptr);
  Locus l = CurrentLocus();

  //STMTS -> STMT STMTS  => & * bool false if int ( {nam} {num} return  true void while

  if( Check(set_stmts_)){
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

