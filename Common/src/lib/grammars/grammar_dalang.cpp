/*
 * Grammar for DAlang
 */

using namespace Tokenizer;

template <class G>
void CreateGrammarDalang(G& g){
  
  using namespace GrammarAnalyzer;
  
  //Terminals
  const Symbol semi = g.AddTerminal("SEMC", ";", Tokenizer::kToken::semicolon);
  const Symbol lpar = g.AddTerminal("LPAR", "(", Tokenizer::kToken::lpar);
  const Symbol rpar = g.AddTerminal("RPAR", ")", Tokenizer::kToken::rpar);
  const Symbol plus = g.AddTerminal("PLUS", "+", Tokenizer::kToken::plus);
  const Symbol minu = g.AddTerminal("MINU", "-", Tokenizer::kToken::minus);
  const Symbol mult = g.AddTerminal("MULT", "*", Tokenizer::kToken::mult);
  const Symbol div  = g.AddTerminal("DIVI", "/", Tokenizer::kToken::div);
  const Symbol numr = g.AddTerminal("NUM", "{num}",  Tokenizer::kToken::numerical);
  const Symbol name = g.AddTerminal("NAME", "{nam}", Tokenizer::kToken::name);    
  const Symbol lcbr = g.AddTerminal("LCBR", "{", Tokenizer::kToken::lcbr);
  const Symbol rcbr = g.AddTerminal("RCBR", "}", Tokenizer::kToken::rcbr);
  const Symbol equl = g.AddTerminal("EQUL", "=", Tokenizer::kToken::equality);
  const Symbol empty = Symbol::Empty();
  
  const Symbol kwd_if   = g.AddTerminalKeyword("IF", "if", Tokenizer::kToken::kwd_if);    
  const Symbol kwd_else = g.AddTerminalKeyword("ELSE", "else", Tokenizer::kToken::kwd_else);    
  const Symbol kwd_type_int  = g.AddTerminalKeyword("INT", "int", Tokenizer::kToken::kwd_int);
  const Symbol kwd_type_bool = g.AddTerminalKeyword("BOOL", "bool", Tokenizer::kToken::kwd_bool);

  //Prog
  const Symbol P      = g.AddNonTerminal("PROG");
  
  //Statements
  const Symbol STMTS  = g.AddNonTerminal("STMTS");
  const Symbol STMT   = g.AddNonTerminal("STMT");
  const Symbol IFELSE = g.AddNonTerminal("IFELSE");
  const Symbol DECL   = g.AddNonTerminal("DECL");
  
  //Expressions
  const Symbol E      = g.AddNonTerminal("E");
  const Symbol EP     = g.AddNonTerminal("E'");
  const Symbol T      = g.AddNonTerminal("T");
  const Symbol TP     = g.AddNonTerminal("T''");
  const Symbol F      = g.AddNonTerminal("F");  
  
  //Others
  const Symbol TYPE       = g.AddNonTerminal("TYPE");  
  const Symbol NAME_LIST  = g.AddNonTerminal("NAME_LIST");  
  

  //Non-terminals
  
  g.AddStartingRule(Rule(P,  {STMTS}, true));
  
  //Statements
  g.AddRule(Rule(STMTS,  {STMT, STMTS}));
  g.AddRule(Rule(STMTS,  {empty}));
  
  g.AddRule(Rule(STMT,   {E, equl, E}));          
  g.AddRule(Rule(STMT,   {DECL, semi}));
  g.AddRule(Rule(STMT,   {kwd_if, lpar, E, rpar, lcbr, STMTS, rcbr, IFELSE}));
  
  g.AddRule(Rule(IFELSE, {kwd_else, lcbr, STMTS, rcbr}));
  g.AddRule(Rule(IFELSE, {empty}));
  

  //Types
  g.AddRule(Rule(DECL, {TYPE,NAME_LIST}));
  g.AddRule(Rule(TYPE, {kwd_type_int}));
  g.AddRule(Rule(TYPE, {kwd_type_bool}));
  g.AddRule(Rule(NAME_LIST, {name, NAME_LIST}));
  g.AddRule(Rule(NAME_LIST, {empty}));
  
  //Expressions
  g.AddRule(Rule(E,  {T, EP}));
  g.AddRule(Rule(EP, {plus, T, EP}));
  g.AddRule(Rule(EP, {minu, T, EP}));
  g.AddRule(Rule(EP, {empty}));
  g.AddRule(Rule(T,  {F, TP}));
  g.AddRule(Rule(TP, {mult, F, TP}));
  g.AddRule(Rule(TP, {div , F, TP}));
  g.AddRule(Rule(TP, {empty}));  
  g.AddRule(Rule(F,  {lpar, E, rpar}));
  g.AddRule(Rule(F,  {numr}));
  g.AddRule(Rule(F,  {name}));
  
  
  //Anaylze
  g.Analyze();

}
