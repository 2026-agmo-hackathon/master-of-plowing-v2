#
# AppMainTests.cmake
#
# Optional unit-test target for the parts of the app that do not need the SeamOS
# runtime: RDDF parsing/validation and the whole tracking layer.
#
# This file is picked up automatically by the file(GLOB CMAKES ...) loop in
# src-gen/CMakeLists.txt, so it survives regeneration of the generated
# CMakeLists.txt / AppMain.cmake files.
#
# googletest is optional: without it the target is simply not created and the
# normal app build is unaffected.
#
#   cmake --build <build-dir> --target master_of_plow_app_tests
#   <build-dir>/src-gen/master_of_plow_app_tests
#
# TrackingLoop.cpp is deliberately NOT in the unit list — it talks to
# MainController, the WebSocket endpoint and the leaderboard, so it belongs to
# integration testing rather than here. Everything it drives (IPathTracker and
# its implementations) is covered.
#

find_package(GTest QUIET)

if(NOT GTest_FOUND)

  message(STATUS
    "googletest not found - skipping the master_of_plow_app_tests target")

else()

  file(GLOB APP_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../tests/*.cpp")

  set(APP_TEST_UNITS
    "${CMAKE_CURRENT_SOURCE_DIR}/AppMain/RddfParser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/AppMain/RddfValidator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/AppMain/orchestration/RunOrchestrator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/AppMain/orchestration/CatalogContract.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/AppMain/orchestration/RecordingArchive.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/AppMain/tracking/TrackerFactory.cpp"
  )
  file(GLOB APP_TEST_TRACKER_UNITS
    "${CMAKE_CURRENT_SOURCE_DIR}/AppMain/tracking/impl/*.cpp")

  add_executable(master_of_plow_app_tests
    ${APP_TEST_SOURCES}
    ${APP_TEST_UNITS}
    ${APP_TEST_TRACKER_UNITS}
  )
  # Both spellings are needed: the tracking tests include
  # <AppMain/tracking/...> while the older RDDF tests include "RddfParser.hpp".
  target_include_directories(master_of_plow_app_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/AppMain"
    ${GTEST_INCLUDE_DIRS}
  )

  if(TARGET GTest::gtest)
    target_link_libraries(master_of_plow_app_tests PRIVATE
      GTest::gtest GTest::gtest_main)
  elseif(TARGET GTest::GTest)
    # CMake 3.13 (the target-runtime-compatible Debian Buster toolchain)
    # exports the older imported-target spellings.
    target_link_libraries(master_of_plow_app_tests PRIVATE
      GTest::GTest GTest::Main)
  else()
    # Last-resort compatibility for FindGTest modules that only set variables.
    target_link_libraries(master_of_plow_app_tests PRIVATE
      ${GTEST_BOTH_LIBRARIES} Threads::Threads)
  endif()

  enable_testing()
  add_test(NAME master_of_plow_app_tests COMMAND master_of_plow_app_tests)

endif()
