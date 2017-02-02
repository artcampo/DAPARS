/*
 * This produces the grammar of "engineering a compiler" 2nd ed, p128 
 */

using namespace Tokenizer;

template <class G>
void CreateGrammar(G& g){
  
  using namespace GrammarAnalyzer;
  
  //Terminals
  const Symbol lpar("LPAR", "(", true);
  const Symbol rpar("RPAR", ")", true);
  
  //Non-terminals
  const Symbol prog ("PROG", "PROG", false);
  const Symbol l    ("LIST", "LIST", false);
  const Symbol p    ("PAIR", "PAIR", false);
  
  
  //Grammar
  g.AddTerminal(lpar, Tokenizer::kToken::lpar);
  g.AddTerminal(rpar, Tokenizer::kToken::rpar);
  
  g.AddNonTerminal(prog);
  g.AddNonTerminal(l);
  g.AddNonTerminal(p);
  
  g.AddStartingRule(Rule(prog,  {l}, true));
  
  g.AddRule(Rule(l, {l, p}));
  g.AddRule(Rule(l, {p}));
  g.AddRule(Rule(p, {lpar, p, rpar}));
  g.AddRule(Rule(p, {lpar, rpar}));
  
  //Anaylze
  g.Analyze();
  g.BuildTables();

}
