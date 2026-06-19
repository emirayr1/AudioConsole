cmake_minimum_required(VERSION 3.20)
project(AudioConsole VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS
        "./src/*.cpp"
)

file(GLOB_RECURSE HeaderFiles CONFIGURE_DEPENDS
        "./include/*.h"
)

add_executable(AudioConsole ${SourceFiles} ${HeaderFiles})

target_include_directories(AudioConsole PRIVATE include)