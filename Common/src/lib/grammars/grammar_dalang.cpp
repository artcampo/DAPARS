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
  const Symbol numr = g.AddTerminal("NUM", "{num}", Tokenizer::kToken::numerical);
  const Symbol name = g.AddTerminal("NAM", "{nam}", Tokenizer::kToken::name);    
  
  const Symbol P      = g.AddNonTerminal("PROG");
  
  const Symbol STMTS  = g.AddNonTerminal("STMTS");
  const Symbol STMT   = g.AddNonTerminal("STMT");
  
  const Symbol E      = g.AddNonTerminal("E");
  const Symbol EP     = g.AddNonTerminal("E'");
  const Symbol T      = g.AddNonTerminal("T");
  const Symbol TP     = g.AddNonTerminal("T''");
  const Symbol F      = g.AddNonTerminal("F");  
  

  //Non-terminals
  
  g.AddStartingRule(Rule(P,  {STMTS}, true));
  
  //Statements
  g.AddRule(Rule(STMTS, {STMT, STMTS}));
  g.AddRule(Rule(STMTS, {Symbol::Empty()}));
  g.AddRule(Rule(STMT,  {E, semi}));
  
  //Expressions
  g.AddRule(Rule(E,  {T, EP}));
  g.AddRule(Rule(E,  {T, EP}));
  g.AddRule(Rule(EP, {plus, T, EP}));
  g.AddRule(Rule(EP, {minu, T, EP}));
  g.AddRule(Rule(EP, {Symbol::Empty()}));
  g.AddRule(Rule(T,  {F, TP}));
  g.AddRule(Rule(TP, {mult, F, TP}));
  g.AddRule(Rule(TP, {div , F, TP}));
  g.AddRule(Rule(TP, {Symbol::Empty()}));  
  g.AddRule(Rule(F,  {lpar, E, rpar}));
  g.AddRule(Rule(F,  {numr}));
  g.AddRule(Rule(F,  {name}));
  
  
  //Anaylze
  g.Analyze();

}
