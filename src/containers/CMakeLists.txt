# Get the current directory name to use as the test target.
get_filename_component(TEST_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Create a single test target using all the .cpp source files in this dir.
file(GLOB CPPFILES *.cpp)
cpp_test(${TEST_NAME}
    CPPFILES
        ${CPPFILES}
)
