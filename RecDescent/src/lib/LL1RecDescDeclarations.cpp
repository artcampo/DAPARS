#include "ParserLL1RecDescSourceHeaders.hpp"

namespace RecDescent{

PtrVarDeclList  ParserLL1RecDesc::Decl(const ScopeId scope_inht){
//   std::cout << "Decl\n";
  const AST::Type& type = Type_();
  const Locus l = CurrentLocus();
  std::vector<PtrVarDecl> name_list_inht;
  PtrVarDeclList decl_synt = NameList(name_list_inht, type, scope_inht, l);
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

    if(not unit_.IsDeclValid(prev_token_string_value_, type_inht)){
      Error("[err:15] Symbol already declared.");
      return std::move(name_list_synt);
    }

    name_list_inht.push_back(
      std::move(NewVarDecl(prev_token_string_value_, type_inht, scope_inht, locus_inht))
    );
    VarDecl& n = *name_list_inht[name_list_inht.size() - 1];
    unit_.RegisterDecl(prev_token_string_value_, type_inht, n);

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
    Error("[err:19] Variables must be separated with comma.");
    has_comma = true;
  }
  if(has_comma){
    if(TryAndAccept(kToken::name)){
      if(not unit_.IsDeclValid(prev_token_string_value_, type_inht)){
        Error("[err:15] Symbol already declared.");
        return std::move(name_list_synt);
      }

      name_list_inht.push_back(
        std::move(NewVarDecl(prev_token_string_value_, type_inht, scope_inht, locus_inht) ));

      VarDecl& n = *name_list_inht[name_list_inht.size() - 1];
      unit_.RegisterDecl(prev_token_string_value_, type_inht, n);

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

