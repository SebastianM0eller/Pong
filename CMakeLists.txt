cmake_minimum_required(VERSION 3.30)
project(Pong)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cpp)

# Settings for SFML
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SFML_STATIC_LIBRARIES ON CACHE BOOL "" FORCE)

add_subdirectory(vendor/SFML)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_sources(${PROJECT_NAME} PRIVATE
  source/Game.cpp,
  source/Players.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC SFML::Graphics)



set(ASSET_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSET_DEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "{ASSET_DEST_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${ASSET_SOURCE_DIR}"
      "${ASSET_DEST_DIR}"
)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
