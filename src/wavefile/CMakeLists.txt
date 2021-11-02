project(wavefile CXX)

add_library(
    wavefile
    )

target_sources(
    wavefile
    PUBLIC
        wavefile.cpp 
    PRIVATE
        wavefile.hpp
)

target_include_directories(
    wavefile
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)