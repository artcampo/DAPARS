/*
 * Grammar for DAlang
 */

using namespace Tokenizer;

template <class G>
void CreateGrammarDalang(G& g){

  using GrammarAnalyzer::Symbol;
  using GrammarAnalyzer::Rule;

  //Terminals
  const Symbol semi = g.AddTerminal("SEMC", ";", Tokenizer::kToken::semicolon);
  const Symbol comma = g.AddTerminal("COMMA", ",", Tokenizer::kToken::comma);
  const Symbol lpar = g.AddTerminal("LPAR", "(", Tokenizer::kToken::lpar);
  const Symbol rpar = g.AddTerminal("RPAR", ")", Tokenizer::kToken::rpar);
  const Symbol plus = g.AddTerminal("PLUS", "+", Tokenizer::kToken::plus);
  const Symbol minu = g.AddTerminal("MINU", "-", Tokenizer::kToken::minus);
  const Symbol astk = g.AddTerminal("ASTK", "*", Tokenizer::kToken::astk);
  const Symbol div  = g.AddTerminal("DIVI", "/", Tokenizer::kToken::div);
  const Symbol numr = g.AddTerminal("NUM", "{num}",  Tokenizer::kToken::numerical);
  const Symbol name = g.AddTerminal("NAME", "{nam}", Tokenizer::kToken::name);
  const Symbol lcbr = g.AddTerminal("LCBR", "{", Tokenizer::kToken::lcbr);
  const Symbol rcbr = g.AddTerminal("RCBR", "}", Tokenizer::kToken::rcbr);
  const Symbol equl = g.AddTerminal("EQUL", "=", Tokenizer::kToken::equality);
  const Symbol amps = g.AddTerminal("AMPS", "&", Tokenizer::kToken::ampersand);
  const Symbol empty = Symbol::Empty();

  const Symbol kwd_if     = g.AddTerminalKeyword("IF", "if", Tokenizer::kToken::kwd_if);
  const Symbol kwd_else   = g.AddTerminalKeyword("ELSE", "else", Tokenizer::kToken::kwd_else);
  const Symbol kwd_while  = g.AddTerminalKeyword("While", "while", Tokenizer::kToken::kwd_while);

  const Symbol kwd_type_int  = g.AddTerminalKeyword("INT", "int", Tokenizer::kToken::kwd_int);
  const Symbol kwd_type_bool = g.AddTerminalKeyword("BOOL", "bool", Tokenizer::kToken::kwd_bool);
  const Symbol kwd_type_void = g.AddTerminalKeyword("VOID", "void", Tokenizer::kToken::kwd_void);

  const Symbol kwd_true  = g.AddTerminalKeyword("TRUE", " true",  Tokenizer::kToken::kwd_true);
  const Symbol kwd_false = g.AddTerminalKeyword("FALSE", "false", Tokenizer::kToken::kwd_false);


  //Prog
  const Symbol P      = g.AddNonTerminal("PROG");

  const Symbol FDEF   = g.AddNonTerminal("FDEF");
  const Symbol FDEFL  = g.AddNonTerminal("FDEFL");

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


  //Exprs
  const Symbol E      = g.AddNonTerminal("E");
  const Symbol EP     = g.AddNonTerminal("E'");
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

  g.AddStartingRule(Rule(P,  {FDEFL}, true));


  //Functions
  g.AddRule(Rule(FDEFL,  {FDEF, FDEFL}));
  g.AddRule(Rule(FDEFL,  {empty}));

  g.AddRule(Rule(FDEF,   {TYPE, name, lpar, PARL, rpar, lcbr, STMTS, rcbr}));

  g.AddRule(Rule(PARL,    {PAR, PARLP}));
  g.AddRule(Rule(PARL,    {empty}));
  g.AddRule(Rule(PARLP,   {PAR, PARLP}));
  g.AddRule(Rule(PARLP,   {empty}));
  g.AddRule(Rule(PAR,     {TYPE, name}));

  g.AddRule(Rule(ARGL,    {E, ARGLP}));
  g.AddRule(Rule(ARGL,    {empty}));
  g.AddRule(Rule(ARGLP,   {E, ARGLP}));
  g.AddRule(Rule(ARGLP,   {empty}));

  //Statements
  g.AddRule(Rule(STMTS,  {STMT, STMTS}));
  g.AddRule(Rule(STMTS,  {empty}));

  g.AddRule(Rule(STMT,   {E, equl, E, semi}));
  g.AddRule(Rule(STMT,   {DECL, semi}));
  g.AddRule(Rule(STMT,   {kwd_if, lpar, E, rpar, lcbr, STMTS, rcbr, IFELSE}));
  g.AddRule(Rule(STMT,   {kwd_while, lpar, E, rpar, lcbr, STMTS, rcbr}));

  g.AddRule(Rule(IFELSE, {kwd_else, lcbr, STMTS, rcbr}));
  g.AddRule(Rule(IFELSE, {empty}));

  //Types
  g.AddRule(Rule(DECL,  {TYPE,NAME_LIST}));
  g.AddRule(Rule(TYPE,  {kwd_type_int,  TYPEP}));
  g.AddRule(Rule(TYPE,  {kwd_type_bool, TYPEP}));
  g.AddRule(Rule(TYPE,  {kwd_type_void, TYPEP}));
  g.AddRule(Rule(TYPEP, {astk}));
  g.AddRule(Rule(TYPEP, {empty}));

  g.AddRule(Rule(NAME_LIST, {name, NAME_LISTP}));

  g.AddRule(Rule(NAME_LISTP, {comma, name, NAME_LISTP}));
  g.AddRule(Rule(NAME_LISTP, {empty}));

  //Exprs
  g.AddRule(Rule(E,  {T, EP}));
  g.AddRule(Rule(EP, {plus, T, EP}));
//   g.AddRule(Rule(EP, {minu, T, EP}));
  g.AddRule(Rule(EP, {empty}));
  g.AddRule(Rule(T,  {F}));

  g.AddRule(Rule(F,  {amps, FP}));
  g.AddRule(Rule(F,  {astk, FP}));
  g.AddRule(Rule(F,  {FP}));

  g.AddRule(Rule(FP,  {lpar, E, rpar}));
  g.AddRule(Rule(FP,  {numr}));
  g.AddRule(Rule(FP,  {name}));
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
  g.AddRule(Rule(ARGM,  {lpar, PARL, rpar}));

  //Anaylze
  g.Analyze();

}
