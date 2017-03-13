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


  unit_.NewClass(name);
  unit_.EnterClassDefinition(name);


  Accept(kToken::lcbr, kErr81);
  Accept(kToken::rcbr, kErr82);

  unit_.ExitClassDefinition();

  PtrClassDef cdef = NewClassDef(name, scope_inht, l);
  RegNameType(name);

  return std::move(cdef);

}



} //end namespace RecDescent

