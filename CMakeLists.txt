cmake_minimum_required (VERSION 2.8)
project (project)

file (GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/include/*.hpp)
file (GLOB_RECURSE sources_tests ${PROJECT_SOURCE_DIR}/tests/*.cpp)
include_directories ("${PROJECT_SOURCE_DIR}/include/")
include_directories ("${PROJECT_SOURCE_DIR}/3rd_party")

add_library (my_lib ${sources})
target_link_libraries(my_lib X11 GLEW glfw GL)
add_executable (eto main.cpp)
target_compile_options (eto PUBLIC -g -std=c++11 -Wall -Wextra)
target_link_libraries(eto my_lib X11 GLEW glfw GL)

# this defines the variables Boost_LIBRARIES that contain all library names
# that we need to link to
#find_package(Boost 1.36.0 COMPONENTS filesystem system REQUIRED)


add_executable(tests ${sources_tests})
# this allows us to use our executable as a link library
# therefore we can inherit all compiler options and library dependencies
#set_target_properties(eto PROPERTIES ENABLE_EXPORTS on)

enable_testing()
add_test(NAME tests COMMAND tests)
target_link_libraries(tests my_lib X11 GLEW glfw GL)
#target_link_libraries(tests PUBLIC
   #${GTEST_BOTH_LIBRARIES}
#   eto
#)
