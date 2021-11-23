cmake_minimum_required(VERSION 3.0.0)
project(problem_a VERSION 0.1.0)

include(CTest)
enable_testing()
include_directories(./include)

aux_source_directory(./src/ DIR_SRCS)

add_executable(problem_a ${DIR_SRCS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

option(BUILD_TEST "Whether or not to build the tests" ON)
if (${BUILD_TEST})
    message(STATUS "Building tests")
    enable_testing()  # Enable testing only works in root scope
    add_subdirectory(test)
endif ()

include(CPack)
