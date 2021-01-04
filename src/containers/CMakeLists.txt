# Get the current directory name to use as the test target.
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Create a single test target.
file(GLOB TEST_CPPFILES test*.cpp)
cpp_test(${DIR_NAME}_test
    CPPFILES
        ${TEST_CPPFILES}
)

if (BUILD_BENCHMARKING)
    # Create a single benchmark target.
    file(GLOB BENCHMARK_CPPFILES benchmark*.cpp)
    cpp_test(${DIR_NAME}_benchmark
        CPPFILES
            ${BENCHMARK_CPPFILES}
    )
endif()
