/*
 * This produces simple Expr grammar. LL1
 */

using namespace Tokenizer;

template <class G>
void CreateGrammarExpr(G& g){

  using GrammarAnalyzer::Symbol;
  using GrammarAnalyzer::Rule;

  //Terminals
  const Symbol lpar("LPAR", "(", true);
  const Symbol rpar("RPAR", ")", true);
  const Symbol plus("PLUS", "+", true);
  const Symbol minu("MINU", "-", true);
  const Symbol mult("MULT", "*", true);
  const Symbol div ("DIVI", "/", true);
  const Symbol numr("NUM", "{num}", true);
  const Symbol name("NAM", "{nam}", true);

  //Non-terminals
  const Symbol p  ("PROG", "PROG", false);
  const Symbol e  ("E", "E", false);
  const Symbol ep ("E'", "E'", false);
  const Symbol t  ("T", "T", false);
  const Symbol tp ("T''", "T'", false);
  const Symbol f  ("F", "F", false);


  //Grammar
  g.AddTerminal(lpar, Tokenizer::kToken::lpar);
  g.AddTerminal(rpar, Tokenizer::kToken::rpar);
  g.AddTerminal(plus, Tokenizer::kToken::plus);
  g.AddTerminal(minu, Tokenizer::kToken::minus);
  g.AddTerminal(mult, Tokenizer::kToken::mult);
  g.AddTerminal(div,  Tokenizer::kToken::div);
  g.AddTerminal(numr, Tokenizer::kToken::numerical);
  g.AddTerminal(name, Tokenizer::kToken::name);

  g.AddNonTerminal(p);
  g.AddNonTerminal(e);
  g.AddNonTerminal(ep);
  g.AddNonTerminal(t);
  g.AddNonTerminal(f);

  g.AddStartingRule(Rule(p,  {e}, true));

  g.AddRule(Rule(e,  {t, ep}));
  g.AddRule(Rule(ep, {plus, t, ep}));
  g.AddRule(Rule(ep, {minu, t, ep}));
  g.AddRule(Rule(ep, {Symbol::Empty()}));

  g.AddRule(Rule(t,  {f, tp}));
  g.AddRule(Rule(tp, {mult, f, tp}));
  g.AddRule(Rule(tp, {div , f, tp}));
  g.AddRule(Rule(tp, {Symbol::Empty()}));

  g.AddRule(Rule(f,  {lpar, e, rpar}));
  g.AddRule(Rule(f,  {numr}));
  g.AddRule(Rule(f,  {name}));
  //Anaylze
  g.Analyze();

}


