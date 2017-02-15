/*
 * This produces simple Expr grammar. LL1
 */

using namespace Tokenizer;

template <class G>
void CreateGrammar(G& g){

  using GrammarAnalyzer::Symbol;
  using GrammarAnalyzer::Rule;

  //Terminals
  const Symbol lpar("LPAR", "(", true);
  const Symbol rpar("RPAR", ")", true);
  const Symbol plus("PLUS", "+", true);
  const Symbol numr("NUM", "{num}", true);

  //Non-terminals
  const Symbol prog ("PROG", "PROG", false);
  const Symbol e    ("E", "E", false);
  const Symbol ep   ("E'", "E'", false);
  const Symbol t    ("T", "T", false);
  const Symbol f    ("F", "F", false);


  //Grammar
  g.AddTerminal(lpar, Tokenizer::kToken::lpar);
  g.AddTerminal(rpar, Tokenizer::kToken::rpar);
  g.AddTerminal(plus, Tokenizer::kToken::plus);
  g.AddTerminal(numr, Tokenizer::kToken::numerical);

  g.AddNonTerminal(prog);
  g.AddNonTerminal(e);
  g.AddNonTerminal(ep);
  g.AddNonTerminal(t);
  g.AddNonTerminal(f);

  g.AddStartingRule(Rule(prog,  {e}, true));

  g.AddRule(Rule(e,  {t, ep}));
  g.AddRule(Rule(ep, {plus,t,ep}));
  g.AddRule(Rule(ep, {Symbol::Empty()}));
  g.AddRule(Rule(t,  {f}));
  g.AddRule(Rule(f,  {lpar, e, rpar}));
  g.AddRule(Rule(f,  {numr}));

  //Anaylze
  g.Analyze();


}
