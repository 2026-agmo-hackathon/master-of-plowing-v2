
#
# nevonex.cmake
# Copyright (c) Robert Bosch GmbH. All rights reserved.
#

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wdouble-promotion -fshort-enums -Wconversion -Wsign-conversion -Wformat-security -Wfloat-equal -Wshadow -Wnull-dereference")
# -Wduplicated-cond is GCC-only; conditionally add for GCC
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-cond -Wlogical-not-parentheses")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -s")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")

include(GNUInstallDirs)

file(GLOB_RECURSE nevonex_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE nevonex_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME}-nevonex SHARED ${nevonex_HEADERS} ${nevonex_SOURCES})
target_include_directories(${PROJECT_NAME}-nevonex PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
    )

set_target_properties(${PROJECT_NAME}-nevonex PROPERTIES COMPILE_FLAGS "-DMAKE_NEVONEX_DLL" VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
target_link_libraries(${PROJECT_NAME}-nevonex PUBLIC EMF4CPP::emf4cpp-ecore EMF4CPP::emf4cpp-ecorecpp ${ADDITIONAL_LINK_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-nevonex PRIVATE ${ADDITIONAL_PRIVATE_LINK_LIBRARIES})

install(TARGETS ${PROJECT_NAME}-nevonex EXPORT MASTER_OF_PLOW LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION include/${PROJECT_NAME} FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")
