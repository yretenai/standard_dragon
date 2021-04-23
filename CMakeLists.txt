cmake_minimum_required(VERSION 3.16)
project(standard_dragon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_library(standard_dragon INTERFACE)
target_include_directories(standard_dragon INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
target_compile_features(standard_dragon INTERFACE cxx_std_20)

if (MSVC)
    target_sources(standard_dragon INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/standard_dragon.natvis>
            $<INSTALL_INTERFACE:lib/cmake/standard_dragon/standard_dragon.natvis>)
    target_compile_definitions(standard_dragon INTERFACE _CRT_SECURE_NO_WARNINGS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(standard_dragon INTERFACE -EHs)
    endif ()
    if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_definitions(standard_dragon INTERFACE _WIN32_CLANG)
    endif ()
endif ()

if (DRAGON_TOOLS)
    add_compile_definitions(DRAGON_TOOLS)
    add_subdirectory(tool)
endif ()

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

install(EXPORT standard_dragonTargets DESTINATION lib/cmake/standard_dragon)
install(FILES standard_dragon.natvis DESTINATION lib/cmake/standard_dragon)
install(FILES
        "${PROJECT_BINARY_DIR}/standard_dragonConfigVersion.cmake"
        "${PROJECT_BINARY_DIR}/standard_dragonConfig.cmake"
        DESTINATION lib/cmake/standard_dragon)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
