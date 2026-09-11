cmake_minimum_required(VERSION 3.22)

project(Logger
    VERSION 1.0.0
    DESCRIPTION "TriDrishti Logger Library"
    LANGUAGES CXX
)

# --------------------------------------------------------------------------
# C++ Configuration
# --------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "[Logger] Build type  : ${CMAKE_BUILD_TYPE}")
message(STATUS "[Logger] C++ standard: C++${CMAKE_CXX_STANDARD}")


# --------------------------------------------------------------------------
# Output directories
# --------------------------------------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


# --------------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------------

find_package(Threads REQUIRED)


# --------------------------------------------------------------------------
# Logger Library
# --------------------------------------------------------------------------

add_library(logger SHARED
    src/logger.cpp
)

target_include_directories(logger
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(logger
    PUBLIC
        Threads::Threads
)

# std::filesystem for older GCC
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND
   CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)

    target_link_libraries(logger
        PUBLIC
            stdc++fs
    )

endif()

target_compile_options(logger
    PRIVATE
        -Wall
        -Wextra
)


# --------------------------------------------------------------------------
# Installation directories
# --------------------------------------------------------------------------

include(GNUInstallDirs)

set(LOGGER_INSTALL_CMAKEDIR
    ${CMAKE_INSTALL_LIBDIR}/cmake/logger
)


# --------------------------------------------------------------------------
# Install Logger Library
# --------------------------------------------------------------------------

install(TARGETS logger
    EXPORT loggerTargets

    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}

    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


# --------------------------------------------------------------------------
# Install Headers
# --------------------------------------------------------------------------

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


# --------------------------------------------------------------------------
# Export CMake Target
# --------------------------------------------------------------------------

install(
    EXPORT loggerTargets
    FILE loggerTargets.cmake
    NAMESPACE logger::
    DESTINATION ${LOGGER_INSTALL_CMAKEDIR}
)


# --------------------------------------------------------------------------
# Generate loggerConfig.cmake
# --------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/loggerConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/loggerConfig.cmake

    INSTALL_DESTINATION
        ${LOGGER_INSTALL_CMAKEDIR}
)


# --------------------------------------------------------------------------
# Generate loggerConfigVersion.cmake
# --------------------------------------------------------------------------

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/loggerConfigVersion.cmake

    VERSION
        ${PROJECT_VERSION}

    COMPATIBILITY
        SameMajorVersion
)


# --------------------------------------------------------------------------
# Install CMake Package Configuration
# --------------------------------------------------------------------------

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/loggerConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/loggerConfigVersion.cmake

    DESTINATION
        ${LOGGER_INSTALL_CMAKEDIR}
)


# --------------------------------------------------------------------------
# Test Executable
# --------------------------------------------------------------------------

message(STATUS "[Logger] Test executable")

add_executable(test_logger
    src/main.cpp
)

target_link_libraries(test_logger
    PRIVATE
        logger
)

target_include_directories(test_logger
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# other Project cmakelists.txt

# set(LOGGER_SOURCE_DIR
#     "${CMAKE_CURRENT_SOURCE_DIR}/../../utils/logger"
# )
# if(NOT EXISTS "${LOGGER_SOURCE_DIR}/CMakeLists.txt")
#   message(FATAL_ERROR "Logger source not found at '${LOGGER_SOURCE_DIR}'")
# endif()    
# add_subdirectory("${LOGGER_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/logger")

# message(STATUS "========================================")
# message(STATUS "[Logger] Logger source dir  : ${LOGGER_SOURCE_DIR}")
# message(STATUS "========================================")