cd ../install/tests/ll1

test_standalone(){
    echo "Test: $1"
    ./$1 > $1.out
    diff $1.out $1.ver
    rm $1.out 
}

test_standalone test_grammar
test_standalone test_grammar2
test_standalone test_parser


test_with_input(){
    echo "Test: $1"
    ./$1 $2.input > $2.out
    diff $2.out $2.ver
    rm $2.out 
}

# test_with_input test_parser_compiler test_parse1
# test_with_input test_parser_compiler test_parse2
# test_with_input test_parser_compiler test_parse3
# test_with_input test_parser_compiler test_parse4

