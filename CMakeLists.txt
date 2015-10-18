cmake_minimum_required(VERSION 3.1)

project(MP2P)

# libcrypto and openssl
find_library(LIBCRYPTO_LIBRARY crypto REQUIRED)
find_path(OPENSSL_INCLUDE_DIR openssl/sha.h REQUIRED)
find_library(OPENSSL_LIBRARY ssl REQUIRED)

# pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# boost
find_package(Boost 1.58.0 REQUIRED COMPONENTS
  system
  iostreams
  filesystem
  program_options
)

# Set the version.
set(MP2P_VERSION_MAJOR 0)
set(MP2P_VERSION_MINOR 5)

# Set the standard to C++14
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -std=c++14")
set(CMAKE_CXX_STANDARD 14)

if(APPLE)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -pthread)
endif()

# Export the version to the header files.
configure_file(
  ${PROJECT_SOURCE_DIR}/src/config.h.in
  ${PROJECT_BINARY_DIR}/src/config.h
)
# Add config.h to the include search path
include_directories(${PROJECT_BINARY_DIR}/src)

# Set the include path
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory("src")
add_subdirectory("tests")
