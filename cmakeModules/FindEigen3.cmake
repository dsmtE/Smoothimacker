include(FindPackageHandleStandardArgs)

set(EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/eigen)

if(EIGEN3_INCLUDE_DIR)
  file(READ "${EIGEN3_INCLUDE_DIR}/Eigen/src/Core/util/Macros.h" eigen3_macro_header)

  string(REGEX MATCH "#define[ \t]+EIGEN_WORLD_VERSION[ \t]+([0-9]+)" _eigen3_world_version_match "${eigen3_macro_header}")
  set(EIGEN3_WORLD_VERSION "${CMAKE_MATCH_1}")
  string(REGEX MATCH "#define[ \t]+EIGEN_MAJOR_VERSION[ \t]+([0-9]+)" _eigen3_major_version_match "${eigen3_macro_header}")
  set(EIGEN3_MAJOR_VERSION "${CMAKE_MATCH_1}")
  string(REGEX MATCH "#define[ \t]+EIGEN_MINOR_VERSION[ \t]+([0-9]+)" _eigen3_minor_version_match "${eigen3_macro_header}")
  set(EIGEN3_MINOR_VERSION "${CMAKE_MATCH_1}")
  set(EIGEN3_VERSION_STRING ${EIGEN3_WORLD_VERSION}.${EIGEN3_MAJOR_VERSION}.${EIGEN3_MINOR_VERSION})
endif()

mark_as_advanced(EIGEN3_INCLUDE_DIR)
find_package_handle_standard_args(Eigen3 DEFAULT_MSG EIGEN3_INCLUDE_DIR EIGEN3_VERSION_STRING)

