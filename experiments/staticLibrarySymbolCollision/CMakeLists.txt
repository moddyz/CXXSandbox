cpp_static_library(staticLibA
    CPPFILES
        staticLibA.cpp
)

cpp_static_library(staticLibB
    CPPFILES
        staticLibB.cpp
)

cpp_shared_library(sharedLibA
    CPPFILES
        sharedLibA.cpp
    LIBRARIES
        staticLibA
)

cpp_shared_library(sharedLibB
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

