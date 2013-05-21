INCLUDE(ExternalProject)

SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

ExternalProject_Add(
    googletest
    SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
    TIMEOUT 10
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

ExternalProject_Get_Property(googletest source_dir)
INCLUDE_DIRECTORIES(${source_dir}/include)

ExternalProject_Get_Property(googletest binary_dir)
set(GTEST_LIBRARIES
    ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX}
)

macro(def_test testName)
    add_executable(Test${testName} Test${testName}.cpp ${COMMON_SOURCES})
    target_link_libraries(Test${testName} ${TEST_LIBS} ${GTEST_LIBRARIES})
    if($ENV{BC_UNIT_TEST_VG})
        add_test(NAME Test${testName} COMMAND valgrind --leak-check=full --error-exitcode=1 $<TARGET_FILE:Test${testName}>)
    else()
        add_test(NAME Test${testName} COMMAND Test${testName})
    endif()

    set_tests_properties(Test${testName} PROPERTIES LABELS unit)
endmacro(def_test testName)
