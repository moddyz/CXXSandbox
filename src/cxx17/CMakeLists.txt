get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB CPPFILES *.cpp)
cpp_test(${DIR_NAME}
    CPPFILES
        ${CPPFILES}
)
