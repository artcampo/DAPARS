#include "ParserLL1RecDescSourceHeaders.hpp"

namespace RecDescent{

PtrVarDeclList  ParserLL1RecDesc::Decl(const ScopeId scope_inht){
//   std::cout << "Decl\n";
  const AST::Type& type = this->Type();
  const Locus l = CurrentLocus();
  std::vector<PtrVarDecl> name_list_inht;
  PtrVarDeclList decl_synt = NameList(name_list_inht, type, scope_inht, l);
  return std::move(decl_synt);
}

PtrVarDecl ParserLL1RecDesc::NameDecl(const AST::Type& type_inht
                         , const ScopeId scope_inht
                         , const Locus& locus_inht){
  PtrVarDecl var_decl;
  if(TryAndAccept(kToken::name)){
    if(not unit_.IsDeclValid(prev_token_string_value_)){
      Error(kErr15);
      return std::move(var_decl);
    }

    var_decl = std::move(NewVarDecl(
                  prev_token_string_value_, type_inht, scope_inht, locus_inht));
    unit_.RegisterDecl(prev_token_string_value_, type_inht, *var_decl);

    return var_decl;
  }
}

PtrVarDeclList
ParserLL1RecDesc::NameList(std::vector<PtrVarDecl>& name_list_inht
                         , const AST::Type& type_inht
                         , const ScopeId scope_inht
                         , const Locus& locus_inht){
//   std::cout << "NameList\n";
  PtrVarDeclList name_list_synt(nullptr);

  //NAME_LIST := name NAME_LIST'
  if(Check({kToken::name})){

    PtrVarDecl var_decl = NameDecl(type_inht, scope_inht, locus_inht);
    if(not var_decl) return std::move(name_list_synt);

    name_list_inht.push_back(std::move(var_decl));

//     std::cout << "new var: "<< prev_token_string_value_ << "\n";
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
    Error(kErr19);
    has_comma = true;
  }
  if(has_comma){
    if(Check({kToken::name})){
      PtrVarDecl var_decl = NameDecl(type_inht, scope_inht, locus_inht);
      if(not var_decl) return std::move(name_list_synt);

      name_list_inht.push_back(std::move(var_decl));

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



} //end namespace RecDescent

