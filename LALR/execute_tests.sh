cd ../install/tests/lalr

test_standalone(){
    echo "Test: $1"
    ./$1 > $1.out
    diff $1.out $1.ver
    rm $1.out 
}


test_standalone test_grammar_lalr


test_with_input(){
    echo "Test: $1"
    ./$1 $2.input > $2.out
    diff $2.out $2.ver
    rm $2.out 
}

#test_with_input test_parse_g1 g1
