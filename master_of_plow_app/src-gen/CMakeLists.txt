#
# CMakeLists.txt
# Copyright (c) Robert Bosch GmbH. All rights reserved.
#
cmake_minimum_required(VERSION 3.7)

FIND_PACKAGE(EMF4CPP REQUIRED emf4cpp-ecore emf4cpp-ecorecpp)
# add_definitions(-DQT_NO_VERSION_TAGGING)
if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif(NOT DEFINED CMAKE_BUILD_TYPE)


FILE(GLOB CMAKES "${CMAKE_CURRENT_SOURCE_DIR}/*.cmake")

foreach(cmake ${CMAKES})
  include(${cmake})
endforeach(cmake)


