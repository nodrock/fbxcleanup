cmake_minimum_required(VERSION 3.25)
project(fbxcleanup)

set(CMAKE_CXX_STANDARD 17)
# Set the FBX SDK root directory
set(FBX_SDK_ROOT "/Applications/Autodesk/FBX SDK/2020.3.4" CACHE PATH "Path to FBX SDK root directory")

# Find the FBX SDK libraries
set(FBX_SDK_LIB_DIR ${FBX_SDK_ROOT}/lib/clang/release)
find_library(FBX_SDK_LIB NAMES libfbxsdk.a PATHS ${FBX_SDK_LIB_DIR} REQUIRED)

# Find the FBX SDK include directory
set(FBX_SDK_INCLUDE_DIR ${FBX_SDK_ROOT}/include)

# Add your source files here
set(SOURCES
        main.cpp
        Common.cxx
        # Add your source files here
        )

# Create the executable
add_executable(fbxcleanup ${SOURCES})

# Link against the FBX SDK libraries
target_link_libraries(fbxcleanup PRIVATE ${FBX_SDK_LIB} "-framework CoreFoundation" "-lz" "-lxml2" "-liconv")

# Set the include directories
target_include_directories(fbxcleanup PRIVATE ${FBX_SDK_INCLUDE_DIR})