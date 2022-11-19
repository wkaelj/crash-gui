cmake_minimum_required(VERSION 3.22.0)

project(crash-gui VERSION 0.1.0 LANGUAGES C)


if(UNIX)
  set(WINDOW_FOLDER ${PROJECT_SOURCE_DIR}/src/window_gtk)
elseif(WIN32)
  set(WINDOW_FODLER ${PROJECT_SOURCE_DIR}/src/window_win)
elseif(APPLE)
  set(WINDOW_FOLDER ${PROJECT_SOURCE_DIR}/src/window_osx)
endif()

file(GLOB SOURCE ${PROJECT_SOURCE_DIR}/src/*.c ${WINDOW_FOLDER}/*.c)

add_library(${PROJECT_NAME} ${SOURCE})

# find gtk
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

target_link_libraries(${PROJECT_NAME} PRIVATE ${GTK4_LIBRARIES})

# Add other flags to the compiler
# target_compile_definitions(${PROJECT_NAME} PRIVATE ${GTK4_CFLAGS_OTHER})
target_include_directories(${PROJECT_NAME} 
  PUBLIC ${PROJECT_SOURCE_DIR}/src 
  PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${GTK4_LIBRARY_DIRS})

