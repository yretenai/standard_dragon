project(akpkdump)

add_executable(akpkdumpw main.cpp)
add_executable(akpkdump main.cpp)
target_compile_definitions(akpkdump PRIVATE DRAGON_AKPKANSI)
