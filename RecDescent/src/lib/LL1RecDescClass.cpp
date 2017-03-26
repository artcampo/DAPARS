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
  
  std::vector<ClassType*> parents;
  if(Check({kToken::colon})) InhtList(parents);
  
  
  const AST::Type& type = unit_.GetClassType(name);
  const ScopeId scope_id = unit_.NewClassDecl(name, parents);
  unit_.EnterClassDefinition(name);
  class_name_inht_ = name;

  Accept(kToken::lcbr, kErr81);

  std::vector<PtrVarDecl> var_decl;
  std::vector<PtrFuncDef> func_def;
  MemberList(var_decl, func_def, scope_id);

  Accept(kToken::rcbr, kErr82);

  unit_.ExitClassDefinition();

  PtrClassDef cdef = NewClassDef(name, var_decl, func_def, scope_inht, l);
  RegNameType(name);
  unit_.RegisterDecl(name, unit_.GetClassType(name), *cdef, scope_inht);
  unit_.NewClass(name, scope_id, *cdef);

  return std::move(cdef);

}

void ParserLL1RecDesc::MemberList(std::vector<PtrVarDecl>& var_decl_inht
    , std::vector<PtrFuncDef>& func_def_inht
    , const ScopeId scope_inht){

  //MEMBER_LIST := empty
  if(Check({kToken::rcbr})) return;

  if(not Check(set_types_)) { Error(kErr88); return; }

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
  if(TryAndAccept(kToken::semicolon)){
    var_decl_inht.push_back( std::move(
      BuildNameDecl(name_inht, type_inht, scope_inht, locus_inht) ));
    return;
  }

  //MEMBER -> ( PARL ) { STMTS }
  if(TryAndAccept(kToken::lpar)){
    inside_member_function_definition_ = true;
    member_scope_id_ = scope_inht;

    PtrFuncDef f = ParseFuncDef(type_inht, name_inht, locus_inht, scope_inht);

    inside_member_function_definition_ = false;
    func_def_inht.push_back(std::move(f));
    return;
  }

  Error(kErr87);
}

/*


INHT_LIST' -> , ntype INHT_LIST'  => , 
INHT_LIST' -> {empty}  => {empty} 
 
 */

void ParserLL1RecDesc::InhtList(std::vector<ClassType*>& parents_inht)
{
  //INHT_LIST -> {empty}  => {empty} {
  if(Check({kToken::lcbr})) return;
  
  //INHT_LIST -> : ntype INHT_LIST'  => : 
  if(not Accept(kToken::colon, kErr93)) return;
     
  BuildParent(parents_inht);
  InhtListPrime(parents_inht);
}

void ParserLL1RecDesc::InhtListPrime(std::vector<ClassType*>& parents_inht)
{
  //INHT_LIST' -> {empty}  => {empty} {
  if(Check({kToken::lcbr})) return;
  
  //INHT_LIST' -> , ntype INHT_LIST'  => ,
  if(not Accept(kToken::comma, kErr94)) return;
  
  BuildParent(parents_inht);
  InhtListPrime(parents_inht);     
}

void ParserLL1RecDesc::BuildParent(std::vector<ClassType*>& parents_inht){
  const AST::Type& type = this->Type();
  if(not type.IsClass()) {Error(kErr92 + type.str()); return; }
  
  parents_inht.push_back(dynamic_cast<ClassType*>(&const_cast<AST::Type&>(type)));  
}

/*
PtrVarDeclList  DeclList(const ScopeId scope_inht, PtrVarDeclList& decl_list_inht){
  if(not check(set_types_)) return std::move(decl_list_inht);

  PtrVarDeclList decl_list = DeclList(scope_inht)
}
PtrStatement  FdefList(const ScopeId scope_inht);
*/

} //end namespace RecDescent

