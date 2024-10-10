cmake_minimum_required(VERSION 3.10)

project(args)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(HEADERS
    Args.h
    ArgumentMarshaler.h
    BooleanArgumentMarshaler.h
    DoubleArgumentMarshaler.h
    FloatArgumentMarshaler.h
    FloatArrayArgumentMarshaler.h
    IntegerArgumentMarshaler.h
    IntegerArrayArgumentMarshaler.h
    StringArgumentMarshaler.h
    StringArrayArgumentMarshaler.h
)

set(SOURCES Args.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
