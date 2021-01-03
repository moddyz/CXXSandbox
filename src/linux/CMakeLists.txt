# Get the current directory name.
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Create an executable for each .cpp file.
file(GLOB CPPFILES *.cpp)
foreach(CPPFILE ${CPPFILES})
    get_filename_component(EXECUTABLE_SUFFIX ${CPPFILE} NAME_WE)
    cpp_executable(${DIR_NAME}_${EXECUTABLE_SUFFIX}
        CPPFILES
            ${CPPFILE}
        LIBRARIES
            rt
    )
endforeach()
