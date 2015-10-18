#file (GLOB tests_src *.cc)
set(tests_src tests.cc mp2p.cc)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Includes Catch in the project:
include_directories("Catch/include")
enable_testing(true)
add_executable(tests ${tests_src})
target_link_libraries(tests
  mp2p
)
