include(CMakeFindDependencyMacro)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
list(REMOVE_AT CMAKE_MODULE_PATH -1)

FIND_PACKAGE(EMF4CPP REQUIRED emf4cpp-ecore emf4cpp-ecorecpp)

include("${CMAKE_CURRENT_LIST_DIR}/FCALTargets.cmake")
