#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "NEVONEX-FCAL-PLATFORM::nevonex-fcal-platform" for configuration "Release"
set_property(TARGET NEVONEX-FCAL-PLATFORM::nevonex-fcal-platform APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(NEVONEX-FCAL-PLATFORM::nevonex-fcal-platform PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "Boost::log_setup;CURLPP::curlpp;PahoMqttCpp::paho-mqttpp3"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libnevonex-fcal-platform.so.3.5.5"
  IMPORTED_SONAME_RELEASE "libnevonex-fcal-platform.so.3"
  )

list(APPEND _IMPORT_CHECK_TARGETS NEVONEX-FCAL-PLATFORM::nevonex-fcal-platform )
list(APPEND _IMPORT_CHECK_FILES_FOR_NEVONEX-FCAL-PLATFORM::nevonex-fcal-platform "${_IMPORT_PREFIX}/lib/libnevonex-fcal-platform.so.3.5.5" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
