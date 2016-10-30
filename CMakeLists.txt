###
### cmake
###
cmake_minimum_required(VERSION 3.0)

project(dbger CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test/lib)

### compiler option
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG(-std=c++11 CXX_STD_CXX11)
if(CXX_STD_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

### library
set(CMAKE_FIND_LIBRARY_PREFIXES lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES .a)

### c++ suffix
set(CXX_SUFFIX cpp)

###
### build core library
###
set(LIB_NAME lib)
set(LIB_SOURCE_DIR ${PROJECT_SOURCE_DIR}/${LIB_NAME})
set(LIB_HEADER_DIR ${LIB_SOURCE_DIR})

set(CORE_NAME core)
set(LIB_CORE_SOURCE_DIR ${LIB_SOURCE_DIR}/${CORE_NAME})
set(LIB_CORE_HEADER_DIR ${LIB_CORE_SOURCE_DIR})

include_directories(${LIB_HEADER_DIR})
file(GLOB LIB_CORE_FILES ${LIB_CORE_SOURCE_DIR}/*.${CXX_SUFFIX})
set(LIB_CORE_NAME ${CORE_NAME})
add_library(${LIB_CORE_NAME} SHARED
	${LIB_CORE_FILES}
	)

###
### test
###
enable_testing()

### function
### build_test
function(build_test test_name test_main_file test_source_list link_lib_list)
	add_executable(${test_name}
		${test_main_file}
		${test_source_list}
		)
	target_link_libraries(${test_name}
		${link_lib_list}
		)

	add_test(
		NAME ${test_name}
		COMMAND ${test_name} -v
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		)
	set_tests_properties(${test_name}
		PROPERTIES
		PASS_REGULAR_EXPRESSION OK.*
		)
endfunction()

### CppUTest library
find_path(
	CPPUTEST_HEADER_DIR
	CppUTest
	)
find_path(
	CPPUTESTEXT_INCLUDE
	CppUTestExt
	)

find_library(
	CPPUTEST_LIBS
	CppUTest
	)
find_library(
	CPPUTESTEXT_LIBS
	CppUTestExt
	)

set(TEST_FRAMEWORK_HEADER_DIR ${CPPUTEST_HEADER_DIR} ${CPPUTESTEXT_INCLUDE})
set(TEST_FRAMEWORK_LIBS ${CPPUTEST_LIBS} ${CPPUTESTEXT_LIBS})

### test path
set(TEST_NAME test)
set(TEST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/${TEST_NAME})
set(TEST_HEADER_DIR ${PROJECT_SOURCE_DIR})

### mock name
set(TEST_CASE_NAME cases)
set(MOCK_NAME mock)

### test core path
set(TEST_CORE_SOURCE_DIR ${TEST_SOURCE_DIR}/${CORE_NAME})

#
### build core mock library
#
set(TEST_CORE_MOCK_SOURCE_DIR ${TEST_CORE_SOURCE_DIR}/${MOCK_NAME})
file(GLOB_RECURSE
	TEST_CORE_MOCK_FILES ${TEST_CORE_MOCK_SOURCE_DIR}/*.${CXX_SUFFIX}
	)

include_directories(
	${TEST_FRAMEWORK_HEADER_DIR}
	${CORE_HEADER_DIR}
	${TEST_HEADER_DIR}
	)
set(LIB_TEST_CORE_MOCK_NAME ${TEST_NAME}_${CORE_NAME}_${MOCK_NAME})
add_library(${LIB_TEST_CORE_MOCK_NAME} SHARED
	${TEST_CORE_MOCK_FILES}
	)

#
### build test program of core/cmsis_dap
#
set(TEST_MAIN_FILE ${TEST_SOURCE_DIR}/test.${CXX_SUFFIX})

file(GLOB_RECURSE
	TEST_CORE_FILES
	${TEST_CORE_SOURCE_DIR}/${TEST_CASE_NAME}/*.${CXX_SUFFIX}
	)
build_test(${TEST_NAME}_${CORE_NAME}
	${TEST_MAIN_FILE}
	"${TEST_CORE_FILES}"
	"${TEST_FRAMEWORK_LIBS};${LIB_CORE_NAME};${LIB_TEST_CORE_MOCK_NAME}"
	)

### platform
set(LIB_PLATFORM_SOURCE_DIR ${LIB_SOURCE_DIR}/platforms)
set(TEST_PLATFORM_DIR ${TEST_SOURCE_DIR}/platforms)

### mbed
set(PLATFORM_MBED_NAME mbed)
set(LIB_PLATFORM_MBED_DIR ${LIB_PLATFORM_SOURCE_DIR}/${PLATFORM_MBED_NAME})

### build mbed library
file(GLOB
	LIB_PLATFORM_MBED_FILES ${LIB_PLATFORM_MBED_DIR}/*.${CXX_SUFFIX}
	)

### test mbed file/directory
set(TEST_PLATFORM_MBED_DIR ${TEST_PLATFORM_DIR}/${PLATFORM_MBED_NAME})
set(TEST_PLATFORM_MBED_MOCK_DIR ${TEST_PLATFORM_MBED_DIR}/${MOCK_NAME})

include_directories(
	${TEST_FRAMEWORK_HEADER_DIR}
	${PROJECT_HEADER_PATH}
	${TEST_HEADER_DIR}
	${TEST_PLATFORM_MBED_MOCK_DIR}
	)
set(LIB_PLATFORM_MBED_NAME ${PLATFORM_MBED_NAME})
add_library(${LIB_PLATFORM_MBED_NAME} SHARED
	${LIB_PLATFORM_MBED_FILES}
	)


### build mbed/mock library
file(GLOB_RECURSE
	TEST_PLATFORM_MBED_MOCK_FILES ${TEST_PLATFORM_MBED_MOCK_DIR}/*.${CXX_SUFFIX}
	)
set(LIB_TEST_PLATFORM_MBED_MOCK_NAME ${TEST_NAME}_${PLATFORM_MBED_NAME}_${MOCK_NAME})
add_library(${LIB_TEST_PLATFORM_MBED_MOCK_NAME} SHARED
	${TEST_PLATFORM_MBED_MOCK_FILES}
	)

### build test program of mbed
unset(SUBMODULE_NAME)
set(SUBMODULE_NAME cmsis_dap)
file(GLOB_RECURSE
	TEST_PLATFORM_MBED_FILES ${TEST_PLATFORM_MBED_DIR}/${TEST_CASE_NAME}/*.${CXX_SUFFIX}
	)

build_test(${TEST_NAME}_${PLATFORM_MBED_NAME}
	${TEST_MAIN_FILE}
	"${TEST_PLATFORM_MBED_FILES}"
	"${TEST_FRAMEWORK_LIBS};${LIB_CORE_NAME};${LIB_PLATFORM_MBED_NAME};${LIB_TEST_PLATFORM_MBED_MOCK_NAME}"
	)
