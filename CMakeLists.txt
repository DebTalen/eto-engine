cmake_minimum_required (VERSION 2.8)
project (eto-engine)

# lib dependecies 
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(OpenGL REQUIRED)

set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${THIRD_PARTY_DIR})
add_subdirectory(${THIRD_PARTY_DIR})

set(GLM_ROOT_DIR "${THIRD_PARTY_DIR}/glm")
set(GLFW_ROOT_DIR "${THIRD_PARTY_DIR}/glfw")
include(${THIRD_PARTY_DIR}/3rd_party.cmake)
include_directories(${GLM_INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIR})
include_directories(${CATCH_INCLUDE_DIR})

# bad way to grab files
file (GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/include/*.hpp)
file (GLOB_RECURSE sources_tests ${PROJECT_SOURCE_DIR}/tests/*.cpp)

# set (COMMON_DEPS "pthread" "X11" "glfw" "dl" "glad" "stb_image" "stb_image_write" "assimp")
# building lib
add_library (eto ${sources})
target_link_libraries(eto glad stb_image ${GLFW_LIBRARIES})
if(NOT WIN32)
	target_compile_options (eto PUBLIC -g -std=c++14 -pedantic -Wall -Wextra)
        target_link_libraries(eto dl pthread)
else()
	target_compile_options (eto PUBLIC /W4)
endif()

#building example
# set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
# configure_file(main.cxx ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_executable (example main.cpp)
if(NOT WIN32)
	target_compile_options (example PUBLIC -g -std=c++14 -Wall -Wextra)
else()
	target_compile_options (example PUBLIC /W4)
endif()
target_link_libraries(example eto)

add_executable(tests ${sources_tests})
target_compile_options (tests PUBLIC -std=c++14 -pedantic -Wall -Wextra)
target_link_libraries(tests eto)

if(WIN32)
	# It creates "CMakePredefinedTargets" folder by default and adds CMake
	# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
        # Sets default start-up project in VS
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT example)
endif()
