#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "EMF4CPP::emf4cpp-ecore" for configuration "Release"
set_property(TARGET EMF4CPP::emf4cpp-ecore APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(EMF4CPP::emf4cpp-ecore PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libemf4cpp-ecore.so.2.1.0"
  IMPORTED_SONAME_RELEASE "libemf4cpp-ecore.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS EMF4CPP::emf4cpp-ecore )
list(APPEND _IMPORT_CHECK_FILES_FOR_EMF4CPP::emf4cpp-ecore "${_IMPORT_PREFIX}/lib/libemf4cpp-ecore.so.2.1.0" )

# Import target "EMF4CPP::emf4cpp-ecorecpp" for configuration "Release"
set_property(TARGET EMF4CPP::emf4cpp-ecorecpp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(EMF4CPP::emf4cpp-ecorecpp PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libemf4cpp-ecorecpp.so.2.1.0"
  IMPORTED_SONAME_RELEASE "libemf4cpp-ecorecpp.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS EMF4CPP::emf4cpp-ecorecpp )
list(APPEND _IMPORT_CHECK_FILES_FOR_EMF4CPP::emf4cpp-ecorecpp "${_IMPORT_PREFIX}/lib/libemf4cpp-ecorecpp.so.2.1.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
