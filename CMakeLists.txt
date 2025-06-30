cmake_minimum_required(VERSION 3.10)
project(ForeGame VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR "" CACHE PATH "Path to SFML cmake modules")

# Add the executable
add_executable(fore-game src/fore.cpp src/pch.cpp)

# Find SFML (adjust the components as needed)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# Link SFML libraries
target_link_libraries(fore-game PRIVATE sfml-graphics sfml-audio)

# Copy resource folders after build (optional, for convenience)
add_custom_command(TARGET fore-game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/graphics $<TARGET_FILE_DIR:fore-game>/graphics
)
add_custom_command(TARGET fore-game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/sound $<TARGET_FILE_DIR:fore-game>/sound
)
add_custom_command(TARGET fore-game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/fonts $<TARGET_FILE_DIR:fore-game>/fonts
)