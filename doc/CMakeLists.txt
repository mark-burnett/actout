# ---------------------------------------------------------------
# Example doxygen + sphinx code from SO

macro(add_sphinx_target TARGET_NAME BUILDER COMMENT_STR)
    add_custom_target(${TARGET_NAME}
        COMMAND sphinx-build -b ${BUILDER} . sphinx/build/${BUILDER}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS doxygen
        COMMENT ${COMMENT_STR}
    )
endmacro(add_sphinx_target)
#
#add_custom_target(doxygen
#    COMMAND doxygen doxygen.conf
#    COMMENT "Build doxygen xml files used by sphinx/breathe."
#)
#


# ---------------------------------------------------------------
# Example doxygen from some blog
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in
        ${CMAKE_CURRENT_BINARY_DIR}/conf.py @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/index.rst.in
        ${CMAKE_CURRENT_BINARY_DIR}/index.rst @ONLY)

    add_custom_target(doxygen
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        #        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../src"
    )

    add_sphinx_target(doc
        html
        "Build html documentation"
    )

endif(DOXYGEN_FOUND)
