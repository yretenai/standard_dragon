cmake_minimum_required(VERSION 3.16)
project(standard_dragon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_definitions(-wd"4068")
    add_definitions(-wd"4996")
    add_definitions(-wd"4251")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_definitions(-EHs)
    endif()
    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_definitions(_WIN32_CLANG)
    endif()
endif()

set(NOESIS_INCLUDE_DIR OFF CACHE PATH "Noesis header include directory")

if(NOESIS_INCLUDE_DIR)
    if(NOT EXISTS "${NOESIS_INCLUDE_DIR}/noesisplugin.cpp")
        message(FATAL_ERROR "NOESIS_INCLUDE_DIR is specified but noesis/noesisplugin.cpp is missing!")
    endif()
    if(NOT EXISTS "${NOESIS_INCLUDE_DIR}/NoeSRShared.h")
        message(FATAL_ERROR "NOESIS_INCLUDE_DIR is specified but noesis/NoeSRShared.h is missing!")
    endif()
    if(NOT EXISTS "${NOESIS_INCLUDE_DIR}/pluginbasetypes.h")
        message(FATAL_ERROR "NOESIS_INCLUDE_DIR is specified but noesis/pluginbasetypes.h is missing!")
    endif()
    if(NOT EXISTS "${NOESIS_INCLUDE_DIR}/pluginclasses.h")
        message(FATAL_ERROR "NOESIS_INCLUDE_DIR is specified but noesis/pluginclasses.h is missing!")
    endif()
    if(NOT EXISTS "${NOESIS_INCLUDE_DIR}/pluginshare.h")
        message(FATAL_ERROR "NOESIS_INCLUDE_DIR is specified but noesis/pluginshare.h is missing!")
    endif()
    if(NOT EXISTS "${NOESIS_INCLUDE_DIR}/pluginsupport.cpp")
        message(FATAL_ERROR "NOESIS_INCLUDE_DIR is specified but noesis/pluginsupport.cpp is missing!")
    endif()

    add_compile_definitions(USE_NOESIS)
    include_directories(${NOESIS_INCLUDE_DIR})
    set(NOESIS_API_SOURCES "${NOESIS_INCLUDE_DIR}/noesisplugin.cpp" "${NOESIS_INCLUDE_DIR}/pluginsupport.cpp"
        "${NOESIS_INCLUDE_DIR}/NoeSRShared.h" "${NOESIS_INCLUDE_DIR}/pluginbasetypes.h"
        "${NOESIS_INCLUDE_DIR}/pluginclasses.h" "${NOESIS_INCLUDE_DIR}/pluginshare.h")
endif()

if(DRAGON_TOOLS)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    add_compile_definitions(DRAGON_TOOLS)
    add_subdirectory(tool)
endif()

add_library(standard_dragon INTERFACE)
target_include_directories(standard_dragon INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
target_compile_features(standard_dragon INTERFACE cxx_std_20)

if (MSVC)
    set(STANDARD_DRAGON_ADD_NATVIS TRUE)
    set(STANDARD_DRAGON_NATVIS_FILE include/standard_dragon/standard_dragon.natvis)
    target_sources(standard_dragon INTERFACE $<INSTALL_INTERFACE:${STANDARD_DRAGON_NATVIS_FILE}>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${STANDARD_DRAGON_NATVIS_FILE}>)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/standard_dragonConfigVersion.cmake"
        VERSION 1.0
        COMPATIBILITY AnyNewerVersion)

install(TARGETS standard_dragon
        EXPORT standard_dragonTargets
        LIBRARY DESTINATION lib COMPONENT Runtime
        ARCHIVE DESTINATION lib COMPONENT Development
        RUNTIME DESTINATION bin COMPONENT Runtime
        PUBLIC_HEADER DESTINATION include COMPONENT Development
        BUNDLE DESTINATION bin COMPONENT Runtime)

configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/standard_dragonConfig.cmake.in"
        "${PROJECT_BINARY_DIR}/standard_dragonConfig.cmake"
        INSTALL_DESTINATION lib/cmake/standard_dragon)

if (STANDARD_DRAGON_ADD_NATVIS)
    install(FILES ${STANDARD_DRAGON_NATVIS_FILE} DESTINATION include/standard_dragon/standard_dragon.natvis)
endif()

install(EXPORT standard_dragonTargets DESTINATION lib/cmake/standard_dragon)
install(FILES
        "${PROJECT_BINARY_DIR}/standard_dragonConfigVersion.cmake"
        "${PROJECT_BINARY_DIR}/standard_dragonConfig.cmake"
        DESTINATION lib/cmake/standard_dragon)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION include)
