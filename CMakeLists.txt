cmake_minimum_required (VERSION 2.8)
project (project)

file (GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/include/*.hpp)
file (GLOB_RECURSE sources_tests ${PROJECT_SOURCE_DIR}/tests/*.cpp)
include_directories ("${PROJECT_SOURCE_DIR}/include/")
include_directories ("${PROJECT_SOURCE_DIR}/3rd_party")
add_subdirectory("3rd_party")


set (COMMON_DEPS "pthread" "X11" "glfw" "dl" "glad" "stb_image" "stb_image_write")

add_library (my_lib ${sources})
target_compile_options (my_lib PUBLIC -g -std=c++14 -Wall -Wextra)
target_link_libraries(my_lib ${COMMON_DEPS})

add_executable (eto main.cpp)
target_compile_options (eto PUBLIC -g -std=c++14 -Wall -Wextra)
target_link_libraries(eto my_lib ${COMMON_DEPS})

# this defines the variables Boost_LIBRARIES that contain all library names
# that we need to link to
#find_package(Boost 1.36.0 COMPONENTS filesystem system REQUIRED)


add_executable(tests ${sources_tests})
# this allows us to use our executable as a link library
# therefore we can inherit all compiler options and library dependencies
#set_target_properties(eto PROPERTIES ENABLE_EXPORTS on)

enable_testing()
add_test(NAME tests COMMAND tests)
target_link_libraries(tests my_lib ${COMMON_DEPS})
#target_link_libraries(tests PUBLIC
   #${GTEST_BOTH_LIBRARIES}
#   eto
#)
