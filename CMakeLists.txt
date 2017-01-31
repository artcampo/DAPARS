cmake_minimum_required(VERSION 2.8.9)

project(DAPARS)

#repos which are dependencies
add_subdirectory(DACOMPILER)

#base code
add_subdirectory(GrammarAnalysis)
add_subdirectory(Common)

#individual parsers
add_subdirectory(RecDescent)
add_subdirectory(LR1)
add_subdirectory(LALR)
