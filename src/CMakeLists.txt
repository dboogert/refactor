cmake_minimum_required (VERSION 2.6)
project (refactor)

add_executable(refactor main.cpp)
target_link_libraries( 
        refactor 
    PRIVATE
        clangTooling
        clangFrontend
        clangSerialization
        clangBasic
        clangAST
        clangASTMatchers
        
        LLVM
    )

