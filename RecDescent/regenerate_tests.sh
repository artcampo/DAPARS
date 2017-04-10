cd ../install/tests/ll1

###################################################################
regenerate_test_standalone(){
  ./$1 > $1.ver
  cp $1.ver ../../../RecDescent/src/tests/verification/.
}

regenerate_test_standalone test_grammar_dalang
regenerate_test_standalone test_grammar
regenerate_test_standalone test_grammar2
regenerate_test_standalone test_parser
regenerate_test_standalone test_errors_parser
regenerate_test_standalone test_ir_generator
regenerate_test_standalone test_backend_davm

###################################################################
regenerate_test_with_input(){
  ./$1 $2.input > $2.ver
  cp $2.ver ../../../RecDescent/src/tests/verification/.
}

# regenerate_test_with_input test_parser_compiler test_parse1
# regenerate_test_with_input test_parser_compiler test_parse2
# regenerate_test_with_input test_parser_compiler test_parse3
# regenerate_test_with_input test_parser_compiler test_parse4


#end
echo "Tests regenerated. You need to reinstall before running tests again."
