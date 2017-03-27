include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

INSTALL(FILES test1.ver 
              test2.ver 
              test1.bc.ver 
              test2.bc.ver 
              test3.bc.ver 
              test4.bc.ver 
              test1.bc.out.ver
              test2.bc.out.ver
              test3.bc.out.ver
              test4.bc.out.ver
        DESTINATION tests)