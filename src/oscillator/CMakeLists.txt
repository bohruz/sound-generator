project(oscillator CXX)

add_library(
    oscillator
    )

target_sources(
    oscillator
    PUBLIC
        oscillator.cpp 
    PRIVATE
        oscillator.hpp
)

target_include_directories(
    oscillator
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)