# /////////////////////////////////////////////////////////////////////////////
# ///////////////////////////////// SETTINGS //////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////
# 
cmake_minimum_required(VERSION 3.8)
project(Smoothimacker C CXX)
set(CMAKE_CXX_STANDARD 17)
# set(default_build_type "Release")

# used for find SDL & others libs on windows
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmakeModules")

#set output build directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# /////////////////////////////////////////////////////////////////////////////
# ////////////////////////////// PROJECT FILES ////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

set(MAIN_FILES ${PROJECT_SOURCE_DIR}/src/Smoothimacker.cpp ${PROJECT_SOURCE_DIR}/src/Test.cpp)

file(GLOB_RECURSE SRC_FILES  ${PROJECT_SOURCE_DIR}/src/*.cpp )

# remove main & test file from sources
LIST(REMOVE_ITEM SRC_FILES ${MAIN_FILES})

file(GLOB_RECURSE HEADERS_HPP_FILES "src/*.hpp")
file(GLOB_RECURSE HEADERS_H_FILES "src/*.h")
set(HEADERS_FILES ${HEADERS_HPP_FILES} ${HEADERS_H_FILES})

# /////////////////////////////////////////////////////////////////////////////
# /////////////////////////////// DEPENDENCIES ////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

# ////////// OpenGL ////////// #

find_package(OpenGL REQUIRED)

# ////////// EIGEN ////////// #

find_package(Eigen3 REQUIRED)

if(EIGEN3_FOUND)
    message(STATUS "Eigen found (include: ${EIGEN3_INCLUDE_DIR}, version: ${EIGEN3_VERSION_STRING})")
else()
    message(FATAL_ERROR "lib EIGEN3 not found")
endif()

# ////////// SDL2 ////////// #

find_package(SDL2 REQUIRED)

# ////////// BUILD OTHERS DEPENDENCIES IF NEEDED ////////// #

# ////////// GLAD ////////// #
add_library(GLAD STATIC "libs/glad/src/glad.c")

# ////////// STB_IMAGE ////////// #
add_library(STB_IMAGE STATIC "libs/stb_image/stb_image.cpp")

# ////////// STB_IIMGUIMAGE ////////// #
file(GLOB_RECURSE IMGUI_SOURCES libs/imgui/*)
add_library(IMGUI STATIC ${IMGUI_SOURCES})

# /////////////////////////////////////////////////////////////////////////////
# ///////////////////////////////// INCLUDES //////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

include_directories(
    ${OPENGL_INCLUDE_DIR} 
    ${SDL2_INCLUDE_DIR}
    libs/glad/include # for glad
	libs #for glm & imgui
    ${EIGEN3_INCLUDE_DIR}
)

# /////////////////////////////////////////////////////////////////////////////
# /////////////////////////////////// BUILD ///////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

set(LIBRARIES ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} GLAD STB_IMAGE IMGUI)

foreach(FILE ${MAIN_FILES})
    get_filename_component(FILE_NAME ${FILE} NAME_WE)
    add_executable(${FILE_NAME} ${FILE} ${HEADERS_FILES} ${SRC_FILES})
    target_link_libraries(${FILE_NAME} ${LIBRARIES})

    # ---------- COPY SDL DLLS ---------- #
    if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET ${FILE_NAME} POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:${FILE_NAME}>)
    endforeach()
    endif()

    # ---------- COPY ASSETS FOLDER ---------- #
    add_custom_command(
        TARGET ${FILE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets/
        $<TARGET_FILE_DIR:${FILE_NAME}>/assets/
    )

endforeach()
