cmake_minimum_required(VERSION 3.10)
project( 21127469 )


# Enforce c++20 standard.
set (CMAKE_CXX_STANDARD 20)

# The version number.
set(Example_VERSION_MAJOR 2)
set(Example_VERSION_MINOR 3)
set(Example_VERSION_PATCH 1)

# This project consist of 3 components, each in one directory, so add each of them
add_subdirectory(Libs)


# USE_FOLDERS group cmake generated projects into one (CMakePredefinedTargets) folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Below line set Demo as startup application in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT 21127469)

# Add installer (it is called CPack) which brings all files together (.exe, .lib, .dll, and .h) and put into one file 
# include(InstallRequiredSystemLibraries)
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
# set(CPACK_PACKAGE_VERSION_MAJOR ${Example_VERSION_MAJOR})
# set(CPACK_PACKAGE_VERSION_MINOR ${Example_VERSION_MINOR})
# set(CPACK_PACKAGE_VERSION_PATCH ${Example_VERSION_PATCH})
# include(CPack)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
find_package(OpenCV REQUIRED) 

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( 21127469 main.cpp)

# Demo application require both libraries, so add them
target_link_libraries( 21127469 DLib ${OpenCV_LIBS} )

set(DLL_PATH "${CMAKE_SOURCE_DIR}/build/Libs/")
set(EXE_PATH "${CMAKE_SOURCE_DIR}/build/")

# Define the destination directory
set(DESTINATION_DIR "${CMAKE_SOURCE_DIR}/executable")
# Check if the build type is Debug or Release and set the SOURCE_DLL variable accordingly
add_custom_command(TARGET 21127469 POST_BUILD
	# Ensure the destination directory exists before copying the DLL
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${DESTINATION_DIR}
	COMMAND ${CMAKE_COMMAND} -E make_directory ${DESTINATION_DIR}
)

add_custom_command(TARGET 21127469 POST_BUILD
	# Ensure the destination directory exists before copying the DLL
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${DLL_PATH}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release> ${DESTINATION_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${EXE_PATH}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release> ${DESTINATION_DIR}
)