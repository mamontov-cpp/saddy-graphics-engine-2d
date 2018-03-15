cmake_minimum_required(VERSION 2.8.12)
project(stress-physics)


file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

set(SADDY_APPLICATION_NAME "stress-physics")
set(SADDY_LIBRARY_NAME "saddy")

set(SADDY_CXX_DEBUG_FLAGS "-std=c++14 -Wno-reorder -Wno-unused -Wno-sign-compare -w")
set(SADDY_CXX_RELEASE_FLAGS "-std=c++14 -O2 -Wno-reorder -Wno-unused -Wno-sign-compare -w")

if (NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, default to Release")
	set(CMAKE_BUILD_TYPE "Release")
	set(SADDY_APPLICATION_NAME "${SADDY_APPLICATION_NAME}-release")
	set(SADDY_LIBRARY_NAME "${SADDY_LIBRARY_NAME}-release")
else()
	string(TOLOWER ${CMAKE_BUILD_TYPE} LIBRARY_CONFIG)
	set(SADDY_LIBRARY_NAME "${SADDY_LIBRARY_NAME}-${LIBRARY_CONFIG}")
endif()

macro(SET_GCC_FLAGS)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${SADDY_CXX_DEBUG_FLAGS}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${SADDY_CXX_RELEASE_FLAGS}")
	if (NOT CMAKE_BUILD_TYPE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SADDY_CXX_RELEASE_FLAGS}")
	endif()
endmacro(SET_GCC_FLAGS)

include_directories(include)
include_directories(../../include)
link_directories("../../lib")


IF (WIN32)
  add_definitions(-DWIN32)
  IF (MINGW)
	add_definitions(-DMINGW)
	SET_GCC_FLAGS()
	set(GLOBAL_LIBS m opengl32  glu32)
  ENDIF()
  IF (MSVC)
	add_definitions(-DCRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_DEPRECATE)
	set(GLOBAL_LIBS "GLU32 OPENGL32")
  ENDIF()
ELSE()
  add_definitions(-DUNIX -DLINUX -DGCC -DX11)
  SET_GCC_FLAGS()
  link_directories("/usr/X11R6/lib")
  set(GLOBAL_LIBS m rt GL GLU pthread X11 xcb)
ENDIF()

add_executable(${SADDY_APPLICATION_NAME}  ${SRCS} ${HDRS})

target_link_libraries(${SADDY_APPLICATION_NAME} "${SADDY_LIBRARY_NAME}")
target_link_libraries(${SADDY_APPLICATION_NAME} ${GLOBAL_LIBS})

set_target_properties(${SADDY_APPLICATION_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "../../lib"
    LIBRARY_OUTPUT_DIRECTORY "../../lib"
    RUNTIME_OUTPUT_DIRECTORY "../../bin"
	DEBUG_POSTFIX "-debug"
	RELEASE_POSTFIX "-release"
)
