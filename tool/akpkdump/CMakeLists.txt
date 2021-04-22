project(akpkdump)

add_executable(akpkdumpw main.cpp)
target_link_libraries(akpkdumpw standard_dragon)
add_executable(akpkdump main.cpp)
target_link_libraries(akpkdump standard_dragon)
target_compile_definitions(akpkdump PRIVATE DRAGON_AKPKANSI)
