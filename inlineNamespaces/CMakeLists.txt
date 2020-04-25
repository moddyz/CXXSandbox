file(GLOB CPP_FILES *.cpp)
file(GLOB HEADER_FILES *.h)

cpp_library(
    cppExperiments
    inlineNamespaces
    CPPFILES
        ${CPP_FILES}
    PUBLIC_HEADERS
        ${HEADER_FILES}
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
