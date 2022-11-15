cmake_minimum_required(VERSION 3.22.0)

project(crash-gui C)


file(GLOB_RECURSE SOURCE ${PROJECT_SOURCE_DIR}/src/*.c)

# find gtk
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

add_library(${PROJECT_NAME} STATIC ${SOURCE})
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 23)
target_link_libraries(${PROJECT_NAME} PRIVATE ${GTK_LIBRARIES})
# Add other flags to the compiler
target_compile_definitions(${PROJECT_NAME} PRIVATE ${GTK_CFLAGS_OTHER})

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${GTK_INCLUDE_DIRS})