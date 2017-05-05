cd ../install/tests/ll1

###################################################################
test_standalone(){
    echo "Test: $1"
    ./$1 > $1.out
    diff $1.out $1.ver
    rm $1.out
}

test_standalone test_grammar_dalang
test_standalone test_grammar
test_standalone test_grammar2
test_standalone test_parser
test_standalone test_errors_parser
test_standalone test_ir_generator
test_standalone test_backend_davm

###################################################################
cd ../davm_tests/

test_comp_davm(){
    echo "Test-comp-davm $1"
    #backend has lots of logging, redirect its stdout so it doesn't bother
    ./test_parser_compiler_davm $1.lang $1.bc > comp.out
    ./davm $1.bc > $1.vm.out
    diff $1.vm.out $1.vm.ver
    rm $1.bc $1.vm.out comp.out
}

test_comp_davm functions
