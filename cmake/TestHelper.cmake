find_package(Boost COMPONENTS unit_test_framework REQUIRED)

macro(def_test testName)
    add_executable(Test${testName} Test${testName}.cpp ${TEST_MAIN})
    set_target_properties(Test${testName} PROPERTIES
        COMPILE_FLAGS
            "-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MODULE=Test${testName}")
    target_link_libraries(Test${testName}
        ${TEST_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

    if($ENV{UNIT_TEST_VALGRIND})
        add_test(NAME Test${testName}
            COMMAND valgrind --leak-check=full --error-exitcode=1 $<TARGET_FILE:Test${testName}>)
    else()
        add_test(NAME Test${testName} COMMAND Test${testName})
    endif()

    set_tests_properties(Test${testName} PROPERTIES LABELS unit)
endmacro(def_test testName)
