cpp_library(strong
    TYPE
        SHARED
    CPPFILES
        strong.cpp
)

# This successfully links! But will seg fault on execution, as
# GetString() does not resolve on program load.
#cpp_test(weakProgram
#    CPPFILES
#        main.cpp
#)

# This successfully links, and the program will find the GetString
# function symbol from libstrong.so
cpp_test(strongProgram
    CPPFILES
        main.cpp
    LIBRARIES
        strong
)

