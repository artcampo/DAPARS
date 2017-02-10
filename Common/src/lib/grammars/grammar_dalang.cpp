/*
 * Grammar for DAlang
 */

using namespace Tokenizer;

template <class G>
void CreateGrammarDalang(G& g){
  
  using namespace GrammarAnalyzer;
  
  //Terminals
  const Symbol semi("SEMC", ";", true);
  const Symbol lpar("LPAR", "(", true);
  const Symbol rpar("RPAR", ")", true);
  const Symbol plus("PLUS", "+", true);
  const Symbol minu("MINU", "-", true);
  const Symbol mult("MULT", "*", true);
  const Symbol div ("DIVI", "/", true);
  const Symbol numr("NUM", "{num}", true);
  const Symbol name("NAM", "{nam}", true);  
  
  //Non-terminals
  const Symbol P  ("PROG", "PROG", false);
  
  const Symbol STMTS ("STMTS", "STMTS", false);
  const Symbol STMT ("STMT", "STMT", false);
  
  
  
  const Symbol E  ("E", "E", false);
  const Symbol EP ("E'", "E'", false);
  const Symbol T  ("T", "T", false);
  const Symbol TP ("T''", "T'", false);
  const Symbol F  ("F", "F", false);
  
  
  //Grammar
  g.AddTerminal(semi, Tokenizer::kToken::semicolon);
  g.AddTerminal(lpar, Tokenizer::kToken::lpar);
  g.AddTerminal(rpar, Tokenizer::kToken::rpar);
  g.AddTerminal(plus, Tokenizer::kToken::plus);
  g.AddTerminal(minu, Tokenizer::kToken::minus);
  g.AddTerminal(mult, Tokenizer::kToken::mult);
  g.AddTerminal(div,  Tokenizer::kToken::div);
  g.AddTerminal(numr, Tokenizer::kToken::numerical);
  g.AddTerminal(name, Tokenizer::kToken::name);
  
  g.AddNonTerminal(P);
  g.AddNonTerminal(E);
  g.AddNonTerminal(EP);
  g.AddNonTerminal(T);
  g.AddNonTerminal(F);
  
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
