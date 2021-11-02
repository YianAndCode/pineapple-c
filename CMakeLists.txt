cmake_minimum_required(VERSION 3.17)
project(pineapple-c)

set(CMAKE_C_STANDARD 99)

set(SOURCES
    src/lexer.c
    src/parser.c
    src/definitions.c
    src/main.c
    include/cHashMap/cHashMap.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)