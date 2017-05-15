test_folder(){
    cd $1
    printf "                                          "
    printf $1
    printf "\n"
    ./execute_tests.sh
    cd ..
}

test_folder LALR
test_folder LR1
test_folder GrammarAnalysis
test_folder RecDescent
test_folder DAVM

