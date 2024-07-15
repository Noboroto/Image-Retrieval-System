cmake_minimum_required(VERSION 3.10)

# Remove trailing '\' and '/' at the end of the path, so can reliably compare
STRING(REGEX REPLACE "[/\\]+$" "" CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
STRING(REGEX REPLACE "[/\\]+$" "" CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# When compiling DLib as stand alone library set to C++11,
# when library is part of other project, respect parent project settings.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	# Enforce c++17 standard.
	set (CMAKE_CXX_STANDARD 17)
endif()

# Notification when too low version of C++ is set
if(NOT DEFINED CMAKE_CXX_STANDARD OR CMAKE_CXX_STANDARD LESS 17)
	message(FATAL_ERROR "ERROR: CMAKE_CXX_STANDARD is not set or it is too low. Minimum C++ v17 is required to compile \"CommandFactory\" library.")
endif()

find_package(OpenCV REQUIRED) 
include_directories( ${OpenCV_INCLUDE_DIRS} )

file(GLOB SRC_FILES "src/*.cpp")

add_library(DLib SHARED ${SRC_FILES} )

target_link_libraries( DLib ${OpenCV_LIBS} )

# Make sure the compiler can find include files for our library
# when other libraries or executables link to code
target_include_directories(DLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_include_directories(DLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# List files that install and/or package should provide.
# Each library should know what it want to distribute,
# which files are internal or intermediate and which are public library export.
install(TARGETS DLib DESTINATION DLib-bin)
install(FILES ${HEADER_FILES} DESTINATION DLib-include)