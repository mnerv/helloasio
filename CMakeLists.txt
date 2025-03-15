cmake_minimum_required(VERSION 3.28)
project(helloasio VERSION 0.0.0)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)  # Group CMake targets inside a folder
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)         # Generate compile_commands.json for language servers

include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        10.1.0
)
list(APPEND FETCH_CONTENTS fmt)
FetchContent_Declare(
    utf8cpp
    GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
    GIT_TAG        v3.2.4
)
list(APPEND FETCH_CONTENTS utf8cpp)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/mononerv/stb.git
    GIT_TAG        698c6fb9889c71494b49c9187d249af5fc87b211
)
list(APPEND FETCH_CONTENTS stb)
FetchContent_MakeAvailable(${FETCH_CONTENTS})

# Group dependencies in Visual Studio and Xcode
if (CMAKE_GENERATOR MATCHES "Visual Studio" OR CMAKE_GENERATOR MATCHES "Xcode")
    set_target_properties(fmt PROPERTIES FOLDER deps)
    set_target_properties(stb PROPERTIES FOLDER deps)
endif()

if (APPLE)
    set(PLATFORM_LINK_LIBRARIES "")
elseif (UNIX AND NOT APPLE)  # Linux, BSD, Solaris, Minix
    set(PLATFORM_LINK_LIBRARIES "m")
elseif (WIN32)
    set(PLATFORM_LINK_LIBRARIES "")
else()
    message(FATAL_ERROR "Unkown platform!")
endif()

# Compiler specific options
if (NOT MSVC)
    set(BASE_OPTIONS
        "-Wall"
        "-Wextra"
        "-Wconversion"
        "-Wpedantic"
        "-Wshadow"
        "-Werror"
        # fmt warnings
        "-Wno-unknown-attributes"
    )
else()
    set(BASE_OPTIONS
        "/W4"
        "/WX"
        "/utf-8"
        "/Zc:__cplusplus"
        "/wd4201"
        #"/fsanitize=address"  # Doesn't work without Visual Studio
    )
endif()

set(HEADERS "")
set(SOURCES "helloasio.cpp")
add_executable(helloasio ${HEADERS} ${SOURCES})
target_include_directories(helloasio PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_features(helloasio PRIVATE cxx_std_23)
target_compile_options(helloasio PRIVATE ${BASE_OPTIONS})
target_link_libraries(helloasio
    PRIVATE
    ${PLATFORM_LINK_LIBRARIES}
    fmt
    utf8::cpp
    stb::stb
)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${HEADERS} ${SOURCES})
