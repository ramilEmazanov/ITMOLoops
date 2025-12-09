set(CMAKE_CXX_STANDARD 20)

add_library(itmoloops_lib lib.cpp)
add_library(effect effect.cpp)
add_library(instrument instrument.cpp)
add_library(pattern pattern.cpp)
add_library(process process.cpp)
add_library(song song.cpp)

target_include_directories(itmoloops_lib PUBLIC .)
