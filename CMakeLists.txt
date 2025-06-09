cmake_minimum_required(VERSION 3.15)
project(KeyPer)

# download CPM.cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.8/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=78ba32abdf798bc616bab7c73aac32a17bbd7b06ad9e26a6add69de8f3ae4791
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

# Download cryptopp
CPMAddPackage(
	NAME          	cryptopp-cmake
	GIT_REPOSITORY	"https://github.com/abdes/cryptopp-cmake.git"
	GIT_TAG       	"CRYPTOPP_8_9_0"
	OPTIONS       	"CRYPTOPP_BUILD_TESTING OFF" "CRYPTOPP_INSTALL OFF"
)

# Download sqlite3 via cpm because I am *TIRED* of these package management issues 
CPMAddPackage("gh:sjinks/sqlite3-cmake@3.49.1")

# Gather all .cpp files in the directory
file(GLOB ALL_CPP_FILES "*.cpp")

# Remove KeyPer.cpp from the list of other files
list(REMOVE_ITEM ALL_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/KeyPer.cpp")
list(REMOVE_ITEM ALL_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp")  # Don't include tests.cpp in main executable

# Main executable
add_executable(KeyPer KeyPer.cpp ${ALL_CPP_FILES})
target_link_libraries(KeyPer PRIVATE cryptopp SQLite3)

# Tests executable
add_executable(run_tests tests.cpp ${ALL_CPP_FILES})
target_link_libraries(run_tests PRIVATE cryptopp SQLite3)
