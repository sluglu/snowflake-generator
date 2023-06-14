cmake_minimum_required(VERSION 3.5)
project(snowflake_gen)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)


# Add all source files in the src folder
file(GLOB SOURCE_FILES *.cpp)

# Add an executable target
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(barebone-graphics-engine)

target_link_libraries(${PROJECT_NAME} barebone-engine)