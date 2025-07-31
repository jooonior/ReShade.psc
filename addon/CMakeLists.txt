cmake_minimum_required(VERSION 3.10)

project(ReShadeAPI VERSION 0.0.0.0)

configure_file("include/addon/config.hpp.in" "include/addon/config.hpp")

set(
	SOURCES
	src/addon.cpp
	src/api.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${PROJECT_BINARY_DIR}/include"  # for generated `config.h`
	"${CMAKE_CURRENT_SOURCE_DIR}/../data"  # for `ReShade.psc`
)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

# Fetch ReShade headers.

include(FetchContent)
FetchContent_Declare(
	reshade
	GIT_REPOSITORY https://github.com/crosire/reshade.git
	GIT_TAG v6.5.1
)
FetchContent_MakeAvailable(reshade)
target_include_directories(${PROJECT_NAME} PRIVATE "${reshade_SOURCE_DIR}/include")
