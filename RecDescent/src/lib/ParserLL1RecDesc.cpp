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
  

ParserLL1RecDesc::ParserLL1RecDesc(std::string const &file_name, Block* &programBlock)
//   , Grammar& grammar) 
  : BaseParser(file_name, programBlock){}
//   , grammar_(grammar){}

ParserLL1RecDesc::ParserLL1RecDesc(const std::vector<char>& parse_data, Block* &programBlock)
//   , Grammar& grammar) 
  : BaseParser(parse_data, programBlock){}
//   , grammar_(grammar){}    

void ParserLL1RecDesc::Parse(){
  Prog();
  if(num_errors_ != 0){
    std::cout << "Program syntactically incorrect\n";
  }
}


void ParserLL1RecDesc::Prog(){
  NextToken();
  std::vector<Statement*> stmts_inht;
  Block* stmts_synt = Stmts(stmts_inht);
  
  if(stmts_synt != nullptr){
//     std::cout << "Prog" << std::endl;
    if(token_ != Tokenizer::kToken::eof){
      Error("More data after program.");
    }
  }
  
  programBlock_ = stmts_synt;
  
}

//TODO: should return Expr*
Node* ParserLL1RecDesc::Expr(){
//   std::cout << "Exp\n";
  Node* eprime_synt = nullptr;
  Node* term_synth  = Term();
  
  if(term_synth != nullptr){
    eprime_synt = ExprPrime(term_synth);
    if(eprime_synt == nullptr)
      Error("E' missing");
  }else {
    Error("Term missing.");
  }
//   std::cout << "<-Exp\n";
  return eprime_synt;
}


Node* ParserLL1RecDesc::Term(){
//   std::cout << "T\n";
  return Factor();
}



// E'    := + T E'     ** E'1.inht = new Node(+, E'.inht, T.node)
//                        E'.synt  = E'1.synt
//       |  empty      ** E'.synt  = E'1.synt
Node* ParserLL1RecDesc::ExprPrime(Node* eprime_inht){
//   std::cout << "Exp'\n";
  Node* eprime_synt = nullptr;
  
  if(TryAndAccept(kToken::plus)){
    Node* t_synt = Term();
    Node* eprime1_inht = NewBinaryOp(eprime_inht, IR_ADD, t_synt);  
    
    //A new E' will op against current op+
    eprime_synt = ExprPrime(eprime1_inht);
    if(eprime_synt == nullptr)
      Error("E' error");
    
  } else{
    //check Follow(E')
    if(AcceptEmpty({kToken::rpar, kToken::semicolon}, 
        "Expecting +")){ 
      eprime_synt = eprime_inht;
    }
  }
  
//   std::cout << "<-Exp'\n";
  return eprime_synt;
}


// F := ( E ) | numerical
Node* ParserLL1RecDesc::Factor(){
//   std::cout << "Fact\n";
  Node* f_synt;
  
  if(TryAndAccept(kToken::numerical)){
    f_synt = NewLiteral(prev_token_int_value_);
  }else //TODO: Accept returning bool
  if(TryAndAccept(kToken::lpar)){
    f_synt = Expr();
    Accept(kToken::rpar, "Expecting rpar.");
  }else{
    Error("Expecting numerical or lpar");
    NextToken();
    return Factor();
  }
  
//   std::cout << "<-Fact\n";
  return f_synt;
}



Statement* ParserLL1RecDesc::Stmt(){
//   std::cout << "stmt\n";
  Statement* stmt_synt = nullptr;
  
  if(TryAndAccept(kToken::kwd_if)){
    //if(E){STMTS}
//     std::cout << "stmt::if\n";
    Accept(kToken::lpar, "if missing lpar.");
    Node* expr_synt = Expr();
    if(expr_synt == nullptr) Error("if condition wrong.");
    
    Accept(kToken::rpar, "if missing rpar.");
    
    Accept(kToken::lcbr, "if missing lcbr.");
    std::vector<Statement*> stmts_inht;
    Block* stmts_synt = Stmts(stmts_inht);
    Accept(kToken::rcbr, "if missing rcbr.");
    
    Block* ifelse_synt = IfElse();
    
    if(ifelse_synt == nullptr)
      stmt_synt = NewStmtIf(dynamic_cast<Expression*>(expr_synt), stmts_synt);
    else
      stmt_synt = NewStmtIf(dynamic_cast<Expression*>(expr_synt), stmts_synt, ifelse_synt);
    
  }else if(Check({kToken::kwd_int, kToken::kwd_bool})){
//     std::cout << "stmt::decl stmt\n";
    VarDeclList* decl_synt = Decl();
    stmt_synt = NewDeclStmt(decl_synt);
    Accept(kToken::semicolon, "Expecting semicolon.");
  }
  else{
//     std::cout << "stmt::exp stmt\n";
    Node* expr_synt = Expr();
    stmt_synt       = NewExpressionStatement(expr_synt);
    
    Accept(kToken::semicolon, "Expecting semicolon.");
  }
//   std::cout << "<-stmt\n";
  return stmt_synt;
}

Block* ParserLL1RecDesc::IfElse(){
//   std::cout << "IfElse\n";
  Block* ifelse_synt = nullptr;
  if(TryAndAccept(kToken::kwd_else)){
    
    Accept(kToken::lcbr, "else missing lcbr.");
    std::vector<Statement*> stmts_inht;
    Block* stmts_synt = Stmts(stmts_inht);
    if(stmts_synt == nullptr) Error("Statements within else wrong.");
    ifelse_synt = stmts_synt;
    Accept(kToken::rcbr, "else missing rcbr.");
  }else{
    AcceptEmpty( {kToken::kwd_bool, kToken::eof, kToken::kwd_if
                , kToken::kwd_int, kToken::lpar, kToken::name
                , kToken::numerical},
                "Invalid token after if");
  }
//   std::cout << "<-IfElse\n";
  return ifelse_synt;
}

Block* ParserLL1RecDesc::Stmts(std::vector<Statement*>& stmts_inht){
//   std::cout << "stmts\n";
  Block* stmts_synt = nullptr;
  
  //STMTS => bool {empty} if int ( {nam} {num} 
  if( Check({kToken::kwd_if, kToken::kwd_int, kToken::kwd_bool, kToken::lpar
          , kToken::numerical, kToken::name})){
      Statement* stmt_synth = Stmt();
      
      stmts_inht.push_back(stmt_synth);
      stmts_synt = Stmts(stmts_inht);

    }
  else{
    //check follow(Stmts)
    if(AcceptEmpty({kToken::eof, kToken::rcbr}, "Block not finishing in eof or rcbr")){
      stmts_synt = NewBlock(stmts_inht);
    }    
  }  
//   std::cout << "<-stmts\n";
  return stmts_synt;
}
  
VarDeclList*  ParserLL1RecDesc::Decl(){
//   std::cout << "Decl\n";
  const TypeId type = Type();
  std::vector<VarDecl*> name_list_inht;
  VarDeclList* decl_synt = NameList(name_list_inht, type);
  return decl_synt;
}

VarDeclList*  
ParserLL1RecDesc::NameList(std::vector<VarDecl*>& name_list_inht
                         , const TypeId& type_inht ){
//   std::cout << "NameList\n";
  VarDeclList* name_list_synt;
  if(TryAndAccept(kToken::name)){
    name_list_inht.push_back( NewVarDecl(prev_token_string_value_, type_inht) );
    name_list_synt = NameList(name_list_inht, type_inht);
  }else{
    if(AcceptEmpty({kToken::semicolon}, "Name missing")){
      //empty
      name_list_synt = NewVarDeclList(name_list_inht);
    }else{
      NextToken(); //on error advance token 
      return NameList(name_list_inht, type_inht);
    }
  }
  return name_list_synt;
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
    Error("Type missing");
    t = TypeId::Int();
  }
  return t;
}
  
} //end namespace RecDescent
 
