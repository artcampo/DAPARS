cd ../install/tests/lr1

###################################################################
regenerate_test_standalone(){
  ./$1 > $1.ver
  cp $1.ver ../../../LR1/src/tests/verification/.
}

regenerate_test_standalone unit_items
regenerate_test_standalone test_grammar_lr1
regenerate_test_standalone test_grammar_dragon_lr1

###################################################################
regenerate_test_with_input(){
  ./$1 $2.input > $2.ver
  cp $2.ver ../../../LR1/src/tests/verification/.
}

regenerate_test_with_input test_parse_g1 g1
regenerate_test_with_input test_parse_g1 g1_2


#end
echo "Tests regenerated. You need to reinstall before running tests again."
