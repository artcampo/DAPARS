#include "ParserLL1RecDescSourceHeaders.hpp"

namespace RecDescent{




//CDEF    := Class name{}
PtrClassDef ParserLL1RecDesc::ClassDef_(const ScopeId scope_inht){
//   std::cout << "cdef\n";
  if(not Accept(kToken::kwd_class, kErr83)) return std::move(nullptr);

  Locus       l = CurrentLocus();
  std::string name("class_name_not_parsed");
  if(Accept(kToken::name, kErr80)) name = prev_token_string_value_;

  if(not unit_.IsDeclValid(name, scope_inht)){
    Error(kErr84);
    return std::move(nullptr);
  }
  const AST::Type& type = unit_.GetClassType(name);


  const ScopeId scope_id = unit_.NewClass(name);
  unit_.EnterClassDefinition(name);


  Accept(kToken::lcbr, kErr81);

  std::vector<PtrVarDecl> var_decl_inht;
  std::vector<PtrFuncDef> func_def_inht;
  MemberList(var_decl_inht, func_def_inht, scope_id);

  Accept(kToken::rcbr, kErr82);

  unit_.ExitClassDefinition();

  PtrClassDef cdef = NewClassDef(name, scope_inht, l);
  RegNameType(name);

  return std::move(cdef);

}

void ParserLL1RecDesc::MemberList(std::vector<PtrVarDecl>& var_decl_inht
    , std::vector<PtrFuncDef>& func_def_inht
    , const ScopeId scope_inht){

  //MEMBER_LIST := empty
  if(Check({kToken::rcbr})) return;

  if(Check(set_types_)) { Error(kErr88); return; }

  //MEMBER_LIST := TYPE NAME MEMBER MEMBER_LIST
  Locus l = CurrentLocus();
  const AST::Type& type_inht  = this->Type();
  Accept(kToken::name, kErr86);
  const std::string name_inht = prev_token_string_value_;

  if(not Check({kToken::semicolon, kToken::lpar})) { Error(kErr87); return; }

  Member(type_inht, name_inht, var_decl_inht, func_def_inht, l, scope_inht);

  MemberList(var_decl_inht, func_def_inht, scope_inht);
}

void ParserLL1RecDesc::Member(const Compiler::AST::Type& type_inht
    , const std::string& name_inht
    , std::vector<PtrVarDecl>& var_decl_inht
    , std::vector<PtrFuncDef>& func_def_inht
    , const Locus& locus_inht
    , const ScopeId scope_inht ){

  //MEMBER -> ;
  if(Accept(kToken::semicolon, kErr87)){
    var_decl_inht.push_back( std::move(
      NewVarDecl(name_inht, type_inht, scope_inht, locus_inht) ));
    return;
  }

  //MEMBER -> ( PARL ) { STMTS }
  if(Accept(kToken::lpar, kErr87)){
    inside_member_function_definition_ = true;
    member_scope_id_ = scope_inht;
    var_decl_inht.push_back( std::move(
      NewVarDecl(name_inht, type_inht, scope_inht, locus_inht) ));

    inside_member_function_definition_ = false;
    return;
  }
}

/*
PtrVarDeclList  DeclList(const ScopeId scope_inht, PtrVarDeclList& decl_list_inht){
  if(not check(set_types_)) return std::move(decl_list_inht);

  PtrVarDeclList decl_list = DeclList(scope_inht)
}
PtrStatement  FdefList(const ScopeId scope_inht);
*/

} //end namespace RecDescent

