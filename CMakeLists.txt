cmake_minimum_required(VERSION 3.21)
project(helloasio VERSION 0.0.0)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)  # Group CMake targets inside a folder
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)         # Generate compile_commands.json for language servers

find_package(Threads REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)

set(TARGET_DEFINITIONS "ASIO_STANDALONE")
if (NOT MSVC)
    string(JOIN " " TARGET_DEFINTIONS
        ${TARGET_DEFINTIONS}
    )
    set(TARGET_OPTIONS
        "-Wall"
        "-Wextra"
        "-Wconversion"
        "-Wpedantic"
        "-Wshadow"
        "-Werror"
    )
else()
    string(JOIN " " TARGET_DEFINTIONS
        ${TARGET_DEFINTIONS}
        "_WIN32_WINNT=0x0A00"
    )
    set(TARGET_OPTIONS
        "/W4"
        "/WX"
    )
endif()

set(TARGET_NAME echoserver)
set(TARGET_SOURCE_FILES "echoserver.cpp")
add_executable(${TARGET_NAME} ${TARGET_SOURCE_FILES})
target_include_directories(${TARGET_NAME} PRIVATE "${PROJECT_SOURCE_DIR}")
target_compile_definitions(${TARGET_NAME} PRIVATE ${TARGET_DEFINTIONS})
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)
target_link_libraries(${TARGET_NAME}
    PUBLIC
    Threads::Threads
    fmt::fmt
    asio asio::asio
)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

set(TARGET_NAME asynclient)
set(TARGET_SOURCE_FILES "asyncclient.cpp")
add_executable(${TARGET_NAME} ${TARGET_SOURCE_FILES})
target_include_directories(${TARGET_NAME} PRIVATE "${PROJECT_SOURCE_DIR}")
target_compile_definitions(${TARGET_NAME} PRIVATE ${TARGET_DEFINTIONS})
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)
target_link_libraries(${TARGET_NAME}
    PUBLIC
    Threads::Threads
    fmt::fmt
    asio asio::asio
)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})
