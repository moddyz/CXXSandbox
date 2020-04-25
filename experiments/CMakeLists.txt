list_subdirectories(
    SUBDIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

foreach(
    subdir
    ${SUBDIRS}
)
    message( ${subdir} )
    add_subdirectory(
        ${subdir}
    )
endforeach()
