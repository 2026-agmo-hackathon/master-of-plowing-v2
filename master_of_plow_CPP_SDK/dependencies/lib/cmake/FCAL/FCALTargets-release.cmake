#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "FCAL::fcal-runtime" for configuration "Release"
set_property(TARGET FCAL::fcal-runtime APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(FCAL::fcal-runtime PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libfcal-runtime.so.8.4.0"
  IMPORTED_SONAME_RELEASE "libfcal-runtime.so.8"
  )

list(APPEND _IMPORT_CHECK_TARGETS FCAL::fcal-runtime )
list(APPEND _IMPORT_CHECK_FILES_FOR_FCAL::fcal-runtime "${_IMPORT_PREFIX}/lib/libfcal-runtime.so.8.4.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
