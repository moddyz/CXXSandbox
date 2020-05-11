cpp_library(staticLibA
    TYPE
        STATIC
    CPPFILES
        staticLibA.cpp
)

cpp_library(staticLibB
    TYPE
        STATIC
    CPPFILES
        staticLibB.cpp
)

cpp_library(sharedLibA
    TYPE
        SHARED
    CPPFILES
        sharedLibA.cpp
    LIBRARIES
        staticLibA
)

cpp_library(sharedLibB
    TYPE
        SHARED
    CPPFILES
        sharedLibB.cpp
    LIBRARIES
        staticLibB
)

cpp_test_program(staticLibrarySymbolCollision
    CPPFILES
        main.cpp
    LIBRARIES
        sharedLibA
        sharedLibB
)

