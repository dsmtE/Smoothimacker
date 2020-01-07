include(FindPackageHandleStandardArgs)

set(SDL2_DIR ${CMAKE_SOURCE_DIR}/libs/SDL2-2.0.10)

set(SDL2_INCLUDE_DIR ${SDL2_DIR}/include)
set(SDL2_LIBRARIES ${SDL2_DIR}/lib/x64/SDL2main.lib;${SDL2_DIR}/lib/x64/SDL2.lib)
set(SDL2_DLLS ${SDL2_DIR}/lib/x64/SDL2.dll)

mark_as_advanced(SDL2_INCLUDE_DIR)
find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_INCLUDE_DIR SDL2_LIBRARIES SDL2_DLLS)