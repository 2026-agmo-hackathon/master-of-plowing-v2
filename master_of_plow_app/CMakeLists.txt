cmake_minimum_required(VERSION 3.12)
set(LIB_MAJOR_VERSION "8")
set(LIB_MINOR_VERSION "6")
set(LIB_PATCH_VERSION "0")
set(LIB_BUILD_NUMBER "202606260437")
set(LIB_VERSION_STRING "${LIB_MAJOR_VERSION}.${LIB_MINOR_VERSION}.${LIB_PATCH_VERSION}")
if (CMAKE_VERSION VERSION_LESS 3.0)
PROJECT(master_of_plow_app C CXX)
set(PROJECT_VERSION_MAJOR "${LIB_MAJOR_VERSION}")
set(PROJECT_VERSION_MINOR "${LIB_MINOR_VERSION}")
set(PROJECT_VERSION_PATCH "${LIB_PATCH_VERSION}")
set(PROJECT_VERSION "${LIB_VERSION_STRING}")
else()
PROJECT("master_of_plow_app" VERSION "${LIB_VERSION_STRING}" LANGUAGES C CXX)
endif()

# Developer-only MQTT bridge for a local simulator. Official FIF builds leave
# this OFF; enabling it is explicit and adds a non-production Paho dependency.
option(SEAMOS_DEV_LOCAL_SIM
  "Enable the non-production TestSimMqttBridge for local development" OFF)
if(SEAMOS_DEV_LOCAL_SIM)
  message(WARNING
    "SEAMOS_DEV_LOCAL_SIM is developer-only and must be OFF for official FIF packaging")
endif()

# Keep the generated ProcessTimer interval aligned with the authoritative model.
# The path tracker and its freshness limits are designed around a 10 Hz loop.
set(MASTER_OF_PLOW_TIMER_MS "100")
get_filename_component(MASTER_OF_PLOW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
set(MASTER_OF_PLOW_ECORE_SOURCE
    "${MASTER_OF_PLOW_ROOT}/com.bosch.fsp.master_of_plow/master_of_plow.ecore")
# The official IDE builder deliberately mounts only the app at /ws/src. Its
# staging adapter supplies an exact copy of the authoritative model inside the
# app tree; accept that copy only when its content hash matches the FSP model.
if(NOT EXISTS "${MASTER_OF_PLOW_ECORE_SOURCE}")
  set(MASTER_OF_PLOW_ECORE_SOURCE
      "${CMAKE_CURRENT_SOURCE_DIR}/.fif-build-input/master_of_plow.ecore")
endif()
if(NOT EXISTS "${MASTER_OF_PLOW_ECORE_SOURCE}")
  message(FATAL_ERROR "Authoritative master_of_plow.ecore is unavailable")
endif()
file(SHA256 "${MASTER_OF_PLOW_ECORE_SOURCE}" MASTER_OF_PLOW_ECORE_SHA256)
if(NOT MASTER_OF_PLOW_ECORE_SHA256 STREQUAL
       "69c28fc2cd2e6eccc952b69cd7e3b48f9851c30d9cb60e77ba5bb4e27c8e11ec")
  message(FATAL_ERROR
    "Staged master_of_plow.ecore does not match the authoritative FSP model")
endif()
file(READ "${MASTER_OF_PLOW_ECORE_SOURCE}" MASTER_OF_PLOW_ECORE)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/src-gen/AppMain/ApplicationMainImpl.cpp"
     MASTER_OF_PLOW_APPLICATION_MAIN)
string(REGEX MATCH "key=\"Timer\" value=\"([0-9]+)\""
       MASTER_OF_PLOW_ECORE_TIMER_MATCH "${MASTER_OF_PLOW_ECORE}")
set(MASTER_OF_PLOW_ECORE_TIMER_MS "${CMAKE_MATCH_1}")
string(REGEX MATCH "ProcessTimer\\(m_mainController, ([0-9]+)\\)"
       MASTER_OF_PLOW_GENERATED_TIMER_MATCH "${MASTER_OF_PLOW_APPLICATION_MAIN}")
set(MASTER_OF_PLOW_GENERATED_TIMER_MS "${CMAKE_MATCH_1}")
if(NOT MASTER_OF_PLOW_ECORE_TIMER_MS STREQUAL MASTER_OF_PLOW_TIMER_MS OR
   NOT MASTER_OF_PLOW_GENERATED_TIMER_MS STREQUAL MASTER_OF_PLOW_TIMER_MS)
  message(FATAL_ERROR
    "Controller timer mismatch: expected ${MASTER_OF_PLOW_TIMER_MS} ms, "
    "model=${MASTER_OF_PLOW_ECORE_TIMER_MS}, "
    "generated=${MASTER_OF_PLOW_GENERATED_TIMER_MS}")
endif()

# Set C++ standard to C++17 (required for Poco library)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



FIND_PACKAGE(MASTER_OF_PLOW REQUIRED master_of_plow-nevonex)

add_subdirectory(src-gen)
