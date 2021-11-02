add_subdirectory(include)

add_executable(${PROJECT_NAME})


target_sources(${PROJECT_NAME}
                PRIVATE 
                main.cpp
                oscillator.cpp 
                wavefile.cpp
)  