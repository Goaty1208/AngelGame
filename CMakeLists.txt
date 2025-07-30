cmake_minimum_required(VERSION 3.12)
project(angelic)

# Set the version of Raylib, box2d and Nlohmann-Json required
set(RAYLIB_VERSION 5.5)
set(BOX2D_VERSION v3.1.0)
set(NLOHMANN_VERSION v3.11.2)

# Check if the platform is Windows or Linux
if (WIN32)
    set(OS_PLATFORM "Windows")
else()
    set(OS_PLATFORM "Linux")
endif()

message(STATUS "Compiling for ${OS_PLATFORM}")

# Allow setting the build type, default to Debug for development
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug/Release/RelWithDebInfo/MinSizeRel)" FORCE)
endif()

# Generate compile_commands.json for development tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include FetchContent for external dependencies
include(FetchContent)

# Raylib
FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG ${RAYLIB_VERSION}
)
# Prevent building Raylib examples
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# nlohmann/json
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG ${NLOHMANN_VERSION}
)
FetchContent_MakeAvailable(json)

set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)

# Box2D
FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG ${BOX2D_VERSION}
)
FetchContent_MakeAvailable(box2d)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)

# Collect source files
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set working directory (useful in Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Include src directory for headers
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link dependencies
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    raylib
    nlohmann_json::nlohmann_json
    box2d
)

# Compiler flags
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /ZI)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-missing-field-initializers -Wmissing-field-initializers -Wall -Wextra -Wpedantic -Wwrite-strings -g)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
endif()

# Define build-type macros
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE RELEASE)
endif()

# Platform-specific libraries
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm)
elseif (APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif (UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread dl m)
endif()

# Emscripten web target
if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    target_link_options(${PROJECT_NAME} PRIVATE -sUSE_GLFW=3 --preload-file resources)
endif()
