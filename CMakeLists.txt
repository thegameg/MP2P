cmake_minimum_required(VERSION 2.8)

project(MP2P)

find_library(LIBCRYPTO_LIBRARY crypto)
if (LIBCRYPTO_LIBRARY)
   set(LIBCRYPTO_FOUND TRUE)
else (LIBCRYPTO_LIBRARY)
   set(LIBCRYPTO_FOUND FALSE)
endif (LIBCRYPTO_LIBRARY)

find_path(OPENSSL_INCLUDE_DIR openssl/sha.h)
find_library(OPENSSL_LIBRARY ssl)
if (OPENSSL_INCLUDE_DIR AND OPENSSL_LIBRARY)
   set(OPENSSL_FOUND TRUE)
else (OPENSSL_INCLUDE_DIR AND OPENSSL_LIBRARY)
   set(OPENSSL_FOUND FALSE)
endif (OPENSSL_INCLUDE_DIR AND OPENSSL_LIBRARY)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(TESTS_DIR ${PROJECT_SOURCE_DIR}/test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

execute_process(COMMAND which clang++ RESULT_VARIABLE clangnotavailable OUTPUT_QUIET ERROR_QUIET)
if(${clangnotavailable} EQUAL 0)
  set(CMAKE_CXX_COMPILER "clang++")
  set(ADDITIONNAL_FLAGS "-Wno-error=zero-length-array")
else(${clangnotavailable} EQUAL 0)
  execute_process(COMMAND which clang++-3.5 RESULT_VARIABLE clang35notavailable OUTPUT_QUIET ERROR_QUIET)
  if(${clang35notavailable} EQUAL 0)
    set(CMAKE_CXX_COMPILER "clang++-3.5")
    set(ADDITIONNAL_FLAGS "-Wno-error=zero-length-array")
  else(${clang35notavailable} EQUAL 0)
    set(CMAKE_CXX_COMPILER "g++")
    set(ADDITIONNAL_FLAGS "")
  endif(${clang35notavailable} EQUAL 0)
endif(${clangnotavailable} EQUAL 0)

message(STATUS "Selected '${CMAKE_CXX_COMPILER}' as CMAKE_CXX_COMPILER.")

set(SHARED_FLAGS "-std=c++1y -pedantic -Wall -Wextra -Wno-error=deprecated-declarations -Wno-error=sign-compare ${ADDITIONNAL_FLAGS}")
set(DEBUG_FLAGS "-ggdb3 -Werror ${SHARED_FLAGS}")
set(OPTIMIZED_FLAGS "-O3 -DNDEBUG ${SHARED_FLAGS}")

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

set(BOOST_ROOT /opt/boost)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system iostreams filesystem program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

include_directories(${OPENSSL_INCLUDE_DIR})

add_custom_target(release)
add_custom_target(debug)
add_custom_target(test-suite)

file(GLOB_RECURSE LIBSOURCES ${SOURCE_DIR}/libtools/*.cc)
add_subdirectory(${SOURCE_DIR}/libtools)

file(GLOB_RECURSE CLIENTSOURCES ${SOURCE_DIR}/client/*.cc)
add_subdirectory(${SOURCE_DIR}/client)

file(GLOB_RECURSE MASTERSOURCES ${SOURCE_DIR}/master/*.cc)
add_subdirectory(${SOURCE_DIR}/master)

file(GLOB_RECURSE STORAGESOURCES ${SOURCE_DIR}/storage/*.cc)
add_subdirectory(${SOURCE_DIR}/storage)

add_subdirectory(${SOURCE_DIR}/tests)
add_subdirectory(${SOURCE_DIR}/tests/client)
add_subdirectory(${SOURCE_DIR}/tests/master)
add_subdirectory(${SOURCE_DIR}/tests/storage)
add_subdirectory(${SOURCE_DIR}/tests/libtools)
