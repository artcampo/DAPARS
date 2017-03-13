#include "ParserLL1RecDescSourceHeaders.hpp"

namespace RecDescent{

const AST::Type&  ParserLL1RecDesc::Type(){
//   std::cout << "Type\n";
  if(TryAndAccept(kToken::kwd_void))  return unit_.GetTypeVoid();

  if(TryAndAccept(kToken::kwd_int)){
    if(TryAndAccept(kToken::astk)) return unit_.GetTypePtrToInt();
    else                           return unit_.GetTypeInt();
  }

  if(TryAndAccept(kToken::kwd_bool)){
    if(TryAndAccept(kToken::astk))  return unit_.GetTypePtrToBool();
    else                            return unit_.GetTypeBool();
  }

  if(TryAndAccept(kToken::name_type)){
    std::string name = prev_token_string_value_;
    if(TryAndAccept(kToken::astk))
      return unit_.PtrToT( unit_.GetClassType(name) );
    else
      return unit_.GetClassType(name);
  }

  //Error recover
  Error("Type missing");
  return unit_.GetTypeInt();
}


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
    if(not unit_.IsDeclValid(prev_token_string_value_, scope_inht)){
      Error(kErr15);
      return std::move(var_decl);
    }

    var_decl = std::move(NewVarDecl(
                  prev_token_string_value_, type_inht, scope_inht, locus_inht));
    unit_.RegisterDecl(prev_token_string_value_, type_inht, *var_decl, scope_inht);

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

