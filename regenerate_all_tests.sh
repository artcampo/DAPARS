test_folder(){
    cd $1
    printf $1
    ./regenerate_tests.sh
    cd .. 
}

# test_folder LALR
test_folder LR1
test_folder RecDescent

