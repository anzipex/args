cmake_minimum_required(VERSION 3.10)

project(args)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(HEADERS
    args.h
    argument_marshaller.h
    boolean_argument_marshaller.h
    double_argument_marshaller.h
    float_argument_marshaller.h
    float_array_argument_marshaller.h
    integer_argument_marshaller.h
    integer_array_argument_marshaller.h
    string_argument_marshaller.h
    string_array_argument_marshaller.h
)

set(SOURCES args.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
