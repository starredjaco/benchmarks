cmake_minimum_required(VERSION 3.22.1)

project("securelib")

add_library(securelib SHARED
        native-lib.cpp)

find_library(log-lib log)

target_link_libraries(securelib ${log-lib})
