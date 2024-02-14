cmake_minimum_required(VERSION 3.8)
project(entry-list)

set(SRC ${CMAKE_SOURCE_DIR}/src)

add_executable(entry-list
	${SRC}/main.c
)
target_include_directories(
	entry-list PUBLIC
)
target_link_libraries(
	entry-list PUBLIC
)
