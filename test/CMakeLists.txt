project(library_test)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} library)

add_test(NAME ${PROJECT_NAME}
        COMMAND ${PROJECT_NAME})