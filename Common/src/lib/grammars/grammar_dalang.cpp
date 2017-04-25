/*
 * Grammar for DAlang
 * 
 * Terminals are written in lowercase and non-terminals in uppercase.
 */

using namespace Tokenizer;

template <class G>
void CreateGrammarDalang(G& g){

  using GrammarAnalyzer::Symbol;
  using GrammarAnalyzer::Rule;

  //Terminals
  const Symbol semi  = g.AddTerminal("SEMC", ";", Tokenizer::kToken::semicolon);
  const Symbol comma = g.AddTerminal("COMMA", ",", Tokenizer::kToken::comma);
  const Symbol lpar  = g.AddTerminal("LPAR", "(", Tokenizer::kToken::lpar);
  const Symbol rpar  = g.AddTerminal("RPAR", ")", Tokenizer::kToken::rpar);
  const Symbol plus  = g.AddTerminal("PLUS", "+", Tokenizer::kToken::plus);
  const Symbol minu  = g.AddTerminal("MINU", "-", Tokenizer::kToken::minus);
  const Symbol astk  = g.AddTerminal("ASTK", "*", Tokenizer::kToken::astk);
  const Symbol div   = g.AddTerminal("DIVI", "/", Tokenizer::kToken::div);
  const Symbol numr  = g.AddTerminal("NUM", "{num}",  Tokenizer::kToken::numerical);
  const Symbol name  = g.AddTerminal("NAME", "{nam}", Tokenizer::kToken::name);
  const Symbol lcbr  = g.AddTerminal("LCBR", "{", Tokenizer::kToken::lcbr);
  const Symbol rcbr  = g.AddTerminal("RCBR", "}", Tokenizer::kToken::rcbr);
  const Symbol equl  = g.AddTerminal("EQUL", "=", Tokenizer::kToken::equality);
  const Symbol lessthan = g.AddTerminal("LEST", "<", Tokenizer::kToken::lessthan);
  
  const Symbol amps  = g.AddTerminal("AMPS", "&", Tokenizer::kToken::ampersand);
  const Symbol dot   = g.AddTerminal("DOT", ".", Tokenizer::kToken::dot);
  const Symbol colon = g.AddTerminal("COLON", ":", Tokenizer::kToken::colon);
  const Symbol empty = Symbol::Empty();

  const Symbol kwd_if     = g.AddTerminalKeyword("IF", "if", Tokenizer::kToken::kwd_if);
  const Symbol kwd_else   = g.AddTerminalKeyword("ELSE", "else", Tokenizer::kToken::kwd_else);
  const Symbol kwd_while  = g.AddTerminalKeyword("WHILE", "while", Tokenizer::kToken::kwd_while);
  const Symbol kwd_return = g.AddTerminalKeyword("RETURN", "return", Tokenizer::kToken::kwd_return);


  const Symbol kwd_class     = g.AddTerminalKeyword("CLASS", "class", Tokenizer::kToken::kwd_class);
  const Symbol kwd_type_int  = g.AddTerminalKeyword("INT", "int", Tokenizer::kToken::kwd_int);
  const Symbol kwd_type_bool = g.AddTerminalKeyword("BOOL", "bool", Tokenizer::kToken::kwd_bool);
  const Symbol kwd_type_void = g.AddTerminalKeyword("VOID", "void", Tokenizer::kToken::kwd_void);
  const Symbol name_type     = g.AddTerminalKeyword("NTYPE", "ntype", Tokenizer::kToken::name_type);


  const Symbol kwd_true  = g.AddTerminalKeyword("TRUE", " true",  Tokenizer::kToken::kwd_true);
  const Symbol kwd_false = g.AddTerminalKeyword("FALSE", "false", Tokenizer::kToken::kwd_false);

  const Symbol kwd_or    = g.AddTerminalKeyword("OR", "or", Tokenizer::kToken::kwd_or);
  


  //Prog
  const Symbol P      = g.AddNonTerminal("PROG");

  const Symbol DEFL   = g.AddNonTerminal("DEFL");
  const Symbol FDEF   = g.AddNonTerminal("FDEF");

  //Classes
  const Symbol CDEF         = g.AddNonTerminal("CDEF");
  const Symbol MEMBER_LIST  = g.AddNonTerminal("MEMBER_LIST");
  const Symbol MEMBER       = g.AddNonTerminal("MEMBER");
  const Symbol INHT_LIST    = g.AddNonTerminal("INHT_LIST");
  const Symbol INHT_LISTP   = g.AddNonTerminal("INHT_LIST'");


  //Functions
  const Symbol ARGL   = g.AddNonTerminal("ARGL");
  const Symbol ARGLP  = g.AddNonTerminal("ARGLP");

  const Symbol PARL   = g.AddNonTerminal("PARL");
  const Symbol PARLP  = g.AddNonTerminal("PARLP");
  const Symbol PAR    = g.AddNonTerminal("PAR");

  //Statements
  const Symbol STMTS  = g.AddNonTerminal("STMTS");
  const Symbol STMT   = g.AddNonTerminal("STMT");
  const Symbol IFELSE = g.AddNonTerminal("IFELSE");
  const Symbol DECL   = g.AddNonTerminal("DECL");
  const Symbol RETURN = g.AddNonTerminal("RETURN");


  //Expr
  const Symbol E      = g.AddNonTerminal("E");
  const Symbol EP     = g.AddNonTerminal("E'");
  const Symbol RE     = g.AddNonTerminal("RE");
  const Symbol REP    = g.AddNonTerminal("RE'");
  const Symbol NE     = g.AddNonTerminal("NE");
  const Symbol NEP    = g.AddNonTerminal("NE'");  
  const Symbol T      = g.AddNonTerminal("T");
  const Symbol TP     = g.AddNonTerminal("T''");
  const Symbol F      = g.AddNonTerminal("F");
  const Symbol FP     = g.AddNonTerminal("F'");

  //Extension of name
  const Symbol ARGM   = g.AddNonTerminal("ARGM");

  //Others
  const Symbol TYPE       = g.AddNonTerminal("TYPE");
  const Symbol TYPEP      = g.AddNonTerminal("TYPE'");
  const Symbol NAME_LIST  = g.AddNonTerminal("NAME_LIST");
  const Symbol NAME_LISTP = g.AddNonTerminal("NAME_LIST'");


  //Non-terminals
  g.AddStartingRule(Rule(P,  {DEFL}, true));



  //Func and class definitions
  g.AddRule(Rule(DEFL,  {FDEF, DEFL}));
  g.AddRule(Rule(DEFL,  {CDEF, DEFL}));
  g.AddRule(Rule(DEFL,  {empty}));

  //TODO: use same style for two-word non-terminals
  
  //Classes
  g.AddRule(Rule(CDEF,  {kwd_class, name, INHT_LIST, lcbr, MEMBER, rcbr}));
  g.AddRule(Rule(MEMBER_LIST, {TYPE, name, MEMBER, MEMBER_LIST}));
  g.AddRule(Rule(MEMBER_LIST, {empty}));
  g.AddRule(Rule(MEMBER,    {semi}));
  g.AddRule(Rule(MEMBER,    {lpar, PARL, rpar, lcbr, STMTS, rcbr}));

  g.AddRule(Rule(INHT_LIST,  {colon, name_type, INHT_LISTP}));
  g.AddRule(Rule(INHT_LIST,  {empty}));
  g.AddRule(Rule(INHT_LISTP, {comma, name_type, INHT_LISTP}));
  g.AddRule(Rule(INHT_LISTP, {empty}));  
  

  //Functions
  g.AddRule(Rule(FDEF,   {TYPE, name, lpar, PARL, rpar, lcbr, STMTS, rcbr}));

  g.AddRule(Rule(PARL,    {PAR, PARLP}));
  g.AddRule(Rule(PARL,    {empty}));
  g.AddRule(Rule(PARLP,   {comma, PAR, PARLP}));
  g.AddRule(Rule(PARLP,   {empty}));
  g.AddRule(Rule(PAR,     {TYPE, name}));

  g.AddRule(Rule(ARGL,    {E, ARGLP}));
  g.AddRule(Rule(ARGL,    {empty}));
  g.AddRule(Rule(ARGLP,   {comma, E, ARGLP}));
  g.AddRule(Rule(ARGLP,   {empty}));

  //Statements
  g.AddRule(Rule(STMTS,  {STMT, STMTS}));
  g.AddRule(Rule(STMTS,  {empty}));

  g.AddRule(Rule(STMT,   {E, equl, E, semi}));
  g.AddRule(Rule(STMT,   {DECL, semi}));
  g.AddRule(Rule(STMT,   {kwd_if, lpar, E, rpar, lcbr, STMTS, rcbr, IFELSE}));
  g.AddRule(Rule(STMT,   {kwd_while, lpar, E, rpar, lcbr, STMTS, rcbr}));
  g.AddRule(Rule(STMT,   {kwd_return, E, semi}));


  g.AddRule(Rule(IFELSE, {kwd_else, lcbr, STMTS, rcbr}));
  g.AddRule(Rule(IFELSE, {empty}));

  //Types
  g.AddRule(Rule(DECL,  {TYPE,NAME_LIST}));
  g.AddRule(Rule(TYPE,  {kwd_type_int,  TYPEP}));
  g.AddRule(Rule(TYPE,  {kwd_type_bool, TYPEP}));
  g.AddRule(Rule(TYPE,  {kwd_type_void, TYPEP}));
  g.AddRule(Rule(TYPE,  {name_type,     TYPEP}));
  g.AddRule(Rule(TYPEP, {astk}));
  g.AddRule(Rule(TYPEP, {empty}));

  g.AddRule(Rule(NAME_LIST, {name, NAME_LISTP}));

  g.AddRule(Rule(NAME_LISTP, {comma, name, NAME_LISTP}));
  g.AddRule(Rule(NAME_LISTP, {empty}));

  //Expr
  g.AddRule(Rule(E,   {RE, EP}));
  g.AddRule(Rule(EP,  {kwd_or, RE, EP}));
  g.AddRule(Rule(EP,  {empty}));
  g.AddRule(Rule(RE,  {NE, REP}));
  g.AddRule(Rule(REP, {lessthan, NE, REP}));    
  g.AddRule(Rule(REP, {empty}));
  g.AddRule(Rule(NE,  {T, NEP}));
  g.AddRule(Rule(NEP, {plus, T, NEP}));
  g.AddRule(Rule(NEP, {empty}));
//   g.AddRule(Rule(NEP, {minu, T, NEP}));
  
  g.AddRule(Rule(T,   {F}));

  g.AddRule(Rule(F,  {amps, FP}));
  g.AddRule(Rule(F,  {astk, FP}));
  g.AddRule(Rule(F,  {FP}));

  g.AddRule(Rule(FP,  {lpar, E, rpar}));
  g.AddRule(Rule(FP,  {numr}));
  g.AddRule(Rule(FP,  {name, ARGM}));
  g.AddRule(Rule(FP,  {kwd_true}));
  g.AddRule(Rule(FP,  {kwd_false}));
  /*
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
  */

  //Extension of name
  g.AddRule(Rule(ARGM,  {empty}));
  g.AddRule(Rule(ARGM,  {lpar, ARGL, rpar}));
  g.AddRule(Rule(ARGM,  {dot, name, ARGM}));


  //Anaylze
  g.Analyze();

}
