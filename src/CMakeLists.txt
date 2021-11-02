project(osc CXX)

add_subdirectory(wavefile)
add_subdirectory(oscillator)


add_executable(
    osc
    )


target_sources(
    osc
    PRIVATE 
        main.cpp
)  

target_link_libraries(
    osc
    PRIVATE 
        wavefile
        oscillator
)