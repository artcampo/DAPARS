/*
 *
 *
 * www.cs.cornell.edu/courses/cs412/2007sp/lectures/lec07.pdf
 */

using namespace Tokenizer;

template <class G>
void CreateGrammarControlFlow(G& g){

  using GrammarAnalyzer::Symbol;
  using GrammarAnalyzer::Rule;

  //Terminals
  const Symbol lpar("LPAR", "(", true);
  const Symbol rpar("RPAR", ")", true);

  //Non-terminals
  const Symbol prog ("PROG", "PROG", false);



  const Symbol STMTS    ("STMTS", "STMTS", false);




  g.AddTerminal(lpar, Tokenizer::kToken::lpar);
  g.AddTerminal(rpar, Tokenizer::kToken::rpar);

  g.AddNonTerminal(prog);
  g.AddNonTerminal(STMTS);


  g.AddStartingRule(Rule(prog,  {STMTS}, true));



  g.Analyze();
  g.BuildTables();

}
