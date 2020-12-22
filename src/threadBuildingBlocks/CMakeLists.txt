cpp_executable(tbbParallelFor

    CPPFILES
        parallelFor.cpp

    LIBRARIES
        TBB::tbb
)
