cmake_minimum_required(VERSION 3.10)

# Pull CommonLibF4 dependency.

include(FetchContent)
FetchContent_Declare(
  commonlibf4
  GIT_REPOSITORY https://github.com/libxse/commonlibf4.git
  GIT_TAG frozen-1.10.63
  PATCH_COMMAND git apply "${CMAKE_CURRENT_SOURCE_DIR}/commonlibf4.patch"
  UPDATE_DISCONNECTED 1  # otherwise cmake tries (and fails) to apply the patch on build
)
cmake_policy(SET CMP0169 OLD)  # supress deprecation warning
FetchContent_Populate(commonlibf4)

# Set up VCPKG integration.

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_MANIFEST_DIR "${commonlibf4_SOURCE_DIR}/CommonLibF4")
set(VCPKG_TARGET_TRIPLET "x64-windows-static-md")

# Create project and plugin DLL.

project(ReShadeAPI_F4)

add_library(${PROJECT_NAME} SHARED main.cpp)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(
	${PROJECT_NAME}
	PRIVATE
	PLUGIN_NAME="${PROJECT_NAME}"
	PLUGIN_FLAVOR="Fallout 4"
)

# Add plugin core dependency.

add_subdirectory(../../addon ReShade_Papyrus)
target_link_libraries(${PROJECT_NAME} PRIVATE ReShadeAPI)

# Add CommonLibF4 dependency.

add_subdirectory("${commonlibf4_SOURCE_DIR}/CommonLibF4" CommonLibF4)
target_link_libraries(${PROJECT_NAME} PRIVATE CommonLibF4::CommonLibF4)
