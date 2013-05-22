# --- Utility ---
macro(def_test testName)
    add_executable(Test${testName} Test${testName}.cpp ${COMMON_SOURCES})
    target_link_libraries(Test${testName} ${TEST_LIBS} ${GTEST_BOTH_LIBRARIES})
    if($ENV{UNIT_TEST_VALGRIND})
        add_test(NAME Test${testName} COMMAND valgrind --leak-check=full --error-exitcode=1 $<TARGET_FILE:Test${testName}>)
    else()
        add_test(NAME Test${testName} COMMAND Test${testName})
    endif()

    set_tests_properties(Test${testName} PROPERTIES LABELS unit)
endmacro(def_test testName)

# --- Setup Google Test ---
find_package(GTest)

if(NOT GTEST_FOUND)
    if(EXISTS "/usr/src/gtest/CMakeLists.txt")
        add_subdirectory("/usr/src/gtest" "${CMAKE_BINARY_DIR}/gtest")
        #        include_directories("/usr/include/gtest")
        set(GTEST_BOTH_LIBRARIES
            ${CMAKE_BINARY_DIR}/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}
            ${CMAKE_BINARY_DIR}/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX}
        )
    else()
        # Fallback to svn download
        include(ExternalProject)

        set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

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
        set(GTEST_BOTH_LIBRARIES
            ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}
            ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX}
        )
    endif()
endif()
