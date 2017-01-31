/*
 * This produces the grammar of "engineering a compiler" 2nd ed, p128 
 */

using namespace Tokenizer;

void CreateGrammar(GrammarLR1& g){
  
  using namespace GrammarAnalyzer;
  
  //Terminals
  const Symbol lpar("LPAR", "(", true);
  const Symbol rpar("RPAR", ")", true);
  
  //Non-terminals
  const Symbol prog ("PROG", "PROG", false);
  const Symbol l    ("LIST", "LIST", false);
  const Symbol p    ("PAIR", "PAIR", false);
  
  
  //Grammar
  g.AddSymbol(lpar, Tokenizer::kToken::lpar);
  g.AddSymbol(rpar, Tokenizer::kToken::rpar);
  
  g.AddStartingRule(Rule(prog,  {l}));
  
  g.AddRule(Rule(l, {l, p}));
  g.AddRule(Rule(l, {p}));
  g.AddRule(Rule(p, {lpar, p, rpar}));
  g.AddRule(Rule(p, {lpar, rpar}));
  
  //Anaylze
  g.Analyze();
  g.BuildTables();

}
