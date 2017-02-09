cd ../install/tests/lalr

###################################################################
regenerate_test_standalone(){
  ./$1 > $1.ver
  cp $1.ver ../../../LALR/src/tests/verification/.
}


regenerate_test_standalone test_grammar_lalr

###################################################################
regenerate_test_with_input(){
  ./$1 $2.input > $2.ver
  cp $2.ver ../../../LALR/src/tests/verification/.
}

#regenerate_test_with_input test_parse_g1 g1


#end
echo "Tests regenerated. You need to reinstall before running tests again."
