#!/bin/sh
cd install/tests

test_from_dacomp(){
  echo "Test from compiler: $1"
  ./davm $1.bc.ver > $1.out
  diff $1.out $1.bc.out.ver
  rm $1.out
}

test_from_dacomp test1
test_from_dacomp test2
test_from_dacomp test3
test_from_dacomp test4

test_standalone(){
    echo "Test: $1"
    ./$1 > $1.out
    diff $1.out $1.ver
    rm $1.out
}

test_standalone test1
test_standalone test2
test_standalone test3
