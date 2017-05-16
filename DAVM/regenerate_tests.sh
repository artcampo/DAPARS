cd build
make install -j8 >/dev/null
cd ../install/tests/davm_tests

###################################################################
regenerate_test_from_dacomp(){
  cp ../../../../DACOMPILER/src/tests/verification/$1.bc.ver .
  cp ../../../../DACOMPILER/src/tests/verification/$1.bc.ver \
     ../../../src/tests/verification/.
  ./davm $1.bc.ver > $1.bc.out.ver
  cp $1.bc.out.ver ../../../src/tests/verification/.
}

regenerate_test_from_dacomp test1
# regenerate_test_from_dacomp test2
# regenerate_test_from_dacomp test3
# regenerate_test_from_dacomp test4

###################################################################
regenerate_test_standalone(){
  ./$1 > $1.ver
  cp $1.ver ../../../src/tests/verification/.
}

regenerate_test_standalone test_loadi
regenerate_test_standalone test_arith
regenerate_test_standalone test_comparisons
regenerate_test_standalone test_mem
regenerate_test_standalone test_stack
regenerate_test_standalone test_call
regenerate_test_standalone test_logic
regenerate_test_standalone test_error_mem

#end
echo "Tests regenerated. You need to reinstall before running tests again."
