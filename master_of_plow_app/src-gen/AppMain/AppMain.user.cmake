#
# AppMain.user.cmake
# Migrated from legacy FD AppMain.cmake by SeamOS IDE.
#
if(NOT SEAMOS_APPMAIN_USER_CMAKE_INCLUDED)
    set(SEAMOS_APPMAIN_USER_CMAKE_INCLUDED TRUE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
        target_link_libraries(${PROJECT_NAME} stdc++fs)
    endif()
    include("${CMAKE_CURRENT_SOURCE_DIR}/AppMain/AppMain.user.cmake" OPTIONAL)
endif()
