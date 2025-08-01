cmake_minimum_required(VERSION 3.10)

project(ReShadeAPI_F4NG)

# Download xrepo.cmake if not exists in build directory.
if(NOT EXISTS "${CMAKE_BINARY_DIR}/xrepo.cmake")
    message(STATUS "Downloading xrepo.cmake from https://github.com/xmake-io/xrepo-cmake/")
    file(
        DOWNLOAD "https://raw.githubusercontent.com/xmake-io/xrepo-cmake/main/xrepo.cmake"
		"${CMAKE_BINARY_DIR}/xrepo.cmake"
		TLS_VERIFY ON
	)
endif()

set(XMAKE_RELEASE_LATEST 3.0.1)
# Include xrepo.cmake so we can use xrepo_package function.
include(${CMAKE_BINARY_DIR}/xrepo.cmake)

set(XREPO_XMAKEFILE "${CMAKE_SOURCE_DIR}/xmake.lua")

xrepo_package(
    commonlibf4
	MODE "${CMAKE_BUILD_TYPE}"
    DEPS
)

set(
	SOURCES
	main.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED True
)

target_compile_definitions(
	${PROJECT_NAME}
	PRIVATE
	PLUGIN_NAME="${PROJECT_NAME}"
	PLUGIN_FLAVOR="Fallout 4 NG"
)

add_subdirectory(../../addon ReShade_Papyrus)
target_link_libraries(${PROJECT_NAME} PRIVATE ReShadeAPI)

xrepo_target_packages(${PROJECT_NAME} commonlibf4)
