#
# CMakeLists.txt
# Copyright (c) Robert Bosch GmbH. All rights reserved.
#
cmake_minimum_required(VERSION 3.12)

FIND_PACKAGE(EMF4CPP REQUIRED emf4cpp-ecore emf4cpp-ecorecpp)
if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif(NOT DEFINED CMAKE_BUILD_TYPE)


FILE(GLOB CMAKES "${CMAKE_CURRENT_SOURCE_DIR}/*.cmake")

foreach(cmake ${CMAKES})
  include(${cmake})
endforeach(cmake)


  # first we can indicate the documentation build as an option and set it to ON by default
  option(BUILD_DOCUMENTATION "Build documentation" OFF)
  # check if Doxygen is installed
if(BUILD_DOCUMENTATION)
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/../docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Document Generation Started")
    # note the option ALL which allows to build the docs together with the application
    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
    add_dependencies(doc_doxygen ${PROJECT_NAME}-nevonex)
  else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
  endif (DOXYGEN_FOUND)
endif()

add_subdirectory(cmake)
