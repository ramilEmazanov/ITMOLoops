add_executable(itmoloops main.cpp
    ${PROJECT_SOURCE_DIR}/lib/effect.cpp
    ${PROJECT_SOURCE_DIR}/lib/instrument.cpp
    ${PROJECT_SOURCE_DIR}/lib/lib.cpp
    ${PROJECT_SOURCE_DIR}/lib/song.cpp
    ${PROJECT_SOURCE_DIR}/lib/pattern.cpp
    ${PROJECT_SOURCE_DIR}/lib/parser.cpp
    ${PROJECT_SOURCE_DIR}/lib/wav_writer.cpp
    ${PROJECT_SOURCE_DIR}/lib/process.cpp)


target_link_libraries(itmoloops PRIVATE itmoloops_lib)
target_link_libraries(itmoloops PRIVATE itmoloops_lib)




