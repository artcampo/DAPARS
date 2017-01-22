cd install/tests

###################################################################
regenerate_test_standalone(){
  ./$1 > $1.ver
  cp $1.ver ../../src/tests/verification/.
}


regenerate_test_standalone test_grammar
regenerate_test_standalone test_grammar2


#end
echo "Tests regenerated. You need to reinstall before running tests again."
