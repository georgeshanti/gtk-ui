cmake_minimum_required(VERSION 2.8)

project( ColourDetection )

find_package( OpenCV REQUIRED )
find_package (Threads)

# Use the package PkgConfig to detect GTK+ headers/library files
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtkmm-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( ColourDetection main.cpp GuiSettings.cpp Callibration.cpp GestureItem.cpp AddWindow.cpp List.cpp)
target_link_libraries( ColourDetection ${OpenCV_LIBS} ${GTK3_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
add_definitions(${GTK_CFLAGS} ${GTK_CFLAGS_OTHER})