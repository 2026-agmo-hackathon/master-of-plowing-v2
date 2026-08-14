#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CURLPP::curlpp" for configuration "Release"
set_property(TARGET CURLPP::curlpp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(CURLPP::curlpp PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libcurlpp.so.1.0.0"
  IMPORTED_SONAME_RELEASE "libcurlpp.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS CURLPP::curlpp )
list(APPEND _IMPORT_CHECK_FILES_FOR_CURLPP::curlpp "${_IMPORT_PREFIX}/lib/libcurlpp.so.1.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
