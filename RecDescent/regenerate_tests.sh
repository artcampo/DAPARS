cd ../build
# make install -j8 >/dev/null
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

cd ../davm_tests/
regenerate_test_comp_davm(){
    ./test_parser_compiler_davm $1.lang $1.bc > $1.comp.out
    ./davm $1.bc > $1.vm.out
    cp $1.vm.out   ../../../RecDescent/src/tests/verification_davm/$1.vm.ver
    cp $1.comp.out ../../../RecDescent/src/tests/verification_davm/$1.comp.ver
    rm $1.bc $1.vm.out $1.comp.out
}

regenerate_test_comp_davm functions
regenerate_test_comp_davm lang_internal_test
regenerate_test_comp_davm lang_internal_test_false
regenerate_test_comp_davm error_stack_overflow
regenerate_test_comp_davm lang_while
regenerate_test_comp_davm lang_if
regenerate_test_comp_davm lang_args
regenerate_test_comp_davm lang_pointers

###################################################################
#end
echo "Tests regenerated. You need to reinstall before running tests again."


###################################################################
#unused

###################################################################
regenerate_test_with_input(){
  ./$1 $2.input > $2.ver
  cp $2.ver ../../../RecDescent/src/tests/verification/.
}

# regenerate_test_with_input test_parser_compiler test_parse1
# regenerate_test_with_input test_parser_compiler test_parse2
# regenerate_test_with_input test_parser_compiler test_parse3
# regenerate_test_with_input test_parser_compiler test_parse4
