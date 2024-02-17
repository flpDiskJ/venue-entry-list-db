cmake_minimum_required(VERSION 3.8)
project(entrylist)

set(SRC ${CMAKE_SOURCE_DIR}/src)

add_executable(entrylist
	${SRC}/main.c
)
target_include_directories(
	entrylist PUBLIC
)
target_link_libraries(
	entrylist PUBLIC
)

install(TARGETS entrylist
    COMPONENT linapp
    RUNTIME DESTINATION "/bin/"
    LIBRARY DESTINATION "/lib/"
    DESTINATION "/bin/"
)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "KK") #required
INCLUDE(CPack)
