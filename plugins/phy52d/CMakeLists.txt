cmake_minimum_required(VERSION 2.8.12)
project(saddy-phy52d)

file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

set(SADDY_LIBRARY_NAME "saddy-phy52d")

set(SADDY_CXX_DEBUG_FLAGS "-std=c++14 -DCONFIG_DEBUG=1 -Wno-reorder -Wno-unused -Wno-sign-compare -w")
set(SADDY_CXX_RELEASE_FLAGS "-std=c++14 -O2 -Wno-reorder -Wno-unused -Wno-sign-compare -w")

if (NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, default to Release")
	set(CMAKE_BUILD_TYPE "Release")
	set(SADDY_LIBRARY_NAME "${SADDY_LIBRARY_NAME}")
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

IF (WIN32)
  add_definitions(-DWIN32)
  IF (MINGW)
	add_definitions(-DMINGW)	
	SET_GCC_FLAGS()
  ENDIF()
  IF (MSVC)
	add_definitions(-DCRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_DEPRECATE)
  ENDIF()
ELSE()
  add_definitions(-DUNIX -DLINUX -DGCC -DX11)
  SET_GCC_FLAGS()
ENDIF()


add_library(${SADDY_LIBRARY_NAME}  ${SRCS} ${HDRS})

target_link_libraries(${SADDY_LIBRARY_NAME} ${FREETYPE_LIBRARIES})

set_target_properties(${SADDY_LIBRARY_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "../../lib"
    LIBRARY_OUTPUT_DIRECTORY "../../lib"
    RUNTIME_OUTPUT_DIRECTORY "../../bin"
	DEBUG_POSTFIX "-debug"
	RELEASE_POSTFIX "-release"
)