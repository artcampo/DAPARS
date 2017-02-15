/*
 * This produces the grammar of "engineering a compiler" 2nd ed,
 * example 4.55, pag 263
 */

using namespace Tokenizer;

template <class G>
void CreateGrammar(G& g){

  using GrammarAnalyzer::Symbol;
  using GrammarAnalyzer::Rule;

  //Terminals
  const Symbol symb_c("c", "c", true);
  const Symbol symb_d("d", "d", true);

  //Non-terminals
  const Symbol sp   ("S'", "S'", false);
  const Symbol s    ("S", "S", false);
  const Symbol c    ("C", "C", false);


  //Grammar
  g.AddTerminal(symb_c, Tokenizer::kToken::token_c);
  g.AddTerminal(symb_d, Tokenizer::kToken::token_d);

  g.AddNonTerminal(sp);
  g.AddNonTerminal(s);
  g.AddNonTerminal(c);


  g.AddStartingRule(Rule(sp,  {s}, true));

  g.AddRule(Rule(s, {c, c}));
  g.AddRule(Rule(c, {symb_c, c}));
  g.AddRule(Rule(c, {symb_d}));

  //Anaylze
  g.Analyze();
  g.BuildTables();

}
