test_folder(){
    cd $1
    printf $1
    printf " >>> "
    ./execute_tests.sh
    cd .. 
}

test_folder LALR
test_folder LR1
test_folder GrammarAnalysis
test_folder RecDescent

