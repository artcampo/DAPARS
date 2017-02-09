cd ../install/tests/grammar

###################################################################
regenerate_test_standalone(){
  ./$1 > $1.ver
  cp $1.ver ../../../GrammarAnalysis/src/tests/verification/.
}

regenerate_test_standalone generate_grammar_strings

###################################################################
regenerate_test_with_input(){
  ./$1 $2.input > $2.ver
  cp $2.ver ../../../GrammarAnalysis/src/tests/verification/.
}


#end
echo "Tests regenerated. You need to reinstall before running tests again."
