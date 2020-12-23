file(GLOB CPPFILES *.cpp)
foreach(CPPFILE ${CPPFILES})
    get_filename_component(EXECUTABLE_SUFFIX ${CPPFILE} NAME_WE)
    cpp_executable(tbb_${EXECUTABLE_SUFFIX}

        CPPFILES
            ${CPPFILE}

        LIBRARIES
            TBB::tbb
    )

endforeach()


