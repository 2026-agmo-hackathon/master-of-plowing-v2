
#
# AppMain.cmake
# Copyright (c) Robert Bosch GmbH. All rights reserved.
#

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wdouble-promotion -fshort-enums")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -s")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")

include(GNUInstallDirs)

file(GLOB_RECURSE AppMain_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/AppMain/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/AppMain/web/*.cpp)
file(GLOB_RECURSE AppMain_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/AppMain/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/AppMain/*.h ${CMAKE_CURRENT_SOURCE_DIR}/AppMain/web/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/AppMain/web/*.h)

# Produce a deterministic source identity even when the source tree is not a
# clean Git checkout (the participant skeleton is commonly distributed as an
# archive). Callers may supply -DSEAMOS_BUILD_ID=<release-id>; otherwise hash
# every app source/header and CMake input that can affect the binary.
set(SEAMOS_BUILD_ID "" CACHE STRING "Recorded source/release identity")
if(SEAMOS_BUILD_ID STREQUAL "")
    file(GLOB_RECURSE SEAMOS_PROVENANCE_FILES CONFIGURE_DEPENDS
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
        "${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt"
        "${CMAKE_CURRENT_SOURCE_DIR}/AppMain/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AppMain/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AppMain/*.hpp.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/AppMain/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cmake")
    list(SORT SEAMOS_PROVENANCE_FILES)
    set(SEAMOS_PROVENANCE_INPUT "")
    foreach(SEAMOS_PROVENANCE_FILE ${SEAMOS_PROVENANCE_FILES})
        file(SHA256
            "${CMAKE_CURRENT_SOURCE_DIR}/${SEAMOS_PROVENANCE_FILE}"
            SEAMOS_PROVENANCE_FILE_SHA256)
        string(APPEND SEAMOS_PROVENANCE_INPUT
            "${SEAMOS_PROVENANCE_FILE}:${SEAMOS_PROVENANCE_FILE_SHA256}\n")
    endforeach()
    string(SHA256 SEAMOS_SOURCE_SHA256 "${SEAMOS_PROVENANCE_INPUT}")
    set(SEAMOS_BUILD_ID "source-sha256:${SEAMOS_SOURCE_SHA256}")
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/AppMain/BuildInfo.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/AppMain/BuildInfo.hpp"
    @ONLY)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${AppMain_HEADERS} ${AppMain_SOURCES})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Participant-owned app code is warning-clean. Generated SDK libraries are
    # separate targets and retain their vendor warning policy.
    target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/generated")

message(STATUS "master_of_plow build id: ${SEAMOS_BUILD_ID}")

target_link_libraries(${PROJECT_NAME} FCAL::fcal-runtime)

target_link_libraries(${PROJECT_NAME} MASTER_OF_PLOW::master_of_plow-nevonex)

# Backend-owned simulator orchestration uses the SDK-provided HTTP client.
find_package(Poco REQUIRED COMPONENTS Foundation Net)
target_link_libraries(${PROJECT_NAME} Poco::Foundation Poco::Net)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()

if(SEAMOS_DEV_LOCAL_SIM)
    add_compile_definitions(SEAMOS_DEV_LOCAL_SIM)
    find_package(PahoMqttCpp REQUIRED)
    target_link_libraries(${PROJECT_NAME} PahoMqttCpp::paho-mqttpp3)
endif()

include("${CMAKE_CURRENT_SOURCE_DIR}/AppMain/AppMain.user.cmake" OPTIONAL)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}> COMMAND_EXPAND_LISTS)
endif (WIN32)
