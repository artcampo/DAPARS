cd install/tests

test_standalone(){
    echo "Test: $1"
    ./$1 > $1.out
    diff $1.out $1.ver
    rm $1.out 
}

test_standalone test_grammar
test_standalone test_grammar2

