cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0069 NEW) # Link time optimization support

project(orbumfront CXX)

set(COMMON_SRC_FILES
    "${CMAKE_SOURCE_DIR}/orbumfront/src/OrbumFront.cpp"
)

add_executable(orbumfront "${COMMON_SRC_FILES}")

target_link_libraries(
    orbumfront 
    PUBLIC
        "${CMAKE_THREAD_LIBS_INIT}"
        utilities
        orbum
)

# TODO: Sort out later into proper build configurations.
# Also disable MSVC non-safe copy warnings.
target_compile_definitions(
    orbumfront 
    PUBLIC 
        "_SCL_SECURE_NO_WARNINGS"
        "BUILD_DEBUG"
)

install(
    TARGETS orbumfront 
    RUNTIME DESTINATION "bin"
)
