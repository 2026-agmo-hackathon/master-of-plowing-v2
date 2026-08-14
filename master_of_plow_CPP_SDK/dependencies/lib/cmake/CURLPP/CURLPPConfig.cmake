include(CMakeFindDependencyMacro)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
list(REMOVE_AT CMAKE_MODULE_PATH -1)

find_package(CURL REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/CURLPPTargets.cmake")
