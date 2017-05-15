#!/bin/sh
cd install/tests/davm_tests

test_from_dacomp(){
  echo "Test from compiler: $1"
  ./davm $1.bc.ver > $1.out
  diff $1.out $1.bc.out.ver
  rm $1.out
}

# test_from_dacomp test1
# test_from_dacomp test2
# test_from_dacomp test3
# test_from_dacomp test4

test_standalone(){
    echo "Test: $1"
    ./$1 > $1.out
    diff $1.out $1.ver
    rm $1.out
}

test_standalone test_loadi
test_standalone test_arith
test_standalone test_comparisons
test_standalone test_mem
test_standalone test_stack
test_standalone test_call
test_standalone test_logic

#unit test, should give message only on error
./unit_test
