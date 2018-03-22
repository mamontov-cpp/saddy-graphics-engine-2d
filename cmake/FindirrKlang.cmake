# - Try to find irrKlang
# Once done this will define
#
# IRRKLANG_FOUND - system has irrKlang
# IRRKLANG_INCLUDE_DIRS - the irrKlang include directory
# IRRKLANG_LIBRARIES - Link these to use irrKlang
# IRRKLANG_SO_LIBRARIES - A dynamic libraries


IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(BITSX64 ON)
ELSE()	
	set(BITSX64 OFF)
ENDIF()

MESSAGE( STATUS "BITSX64:         " ${BITSX64} )

find_path(
  IRRKLANG_INCLUDE_DIR
  irrKlang.h
  HINTS
	ENV IRRKLANG_DIR
	ENV IRRKLANG_ROOT
  PATHS
	/usr/include
  PATH_SUFFIXES
	include
	irrKlang-1.5.0/include
	irrKlang-64bit-1.5.0/include
)

IF (WIN32)
	IF (MSVC)
		IF (BITSX64)
			find_library(
			  IRRKLANG_LIBRARIES
			  irrKlang.lib
			  HINTS
				ENV IRRKLANG_DIR
				ENV IRRKLANG_ROOT
			  PATHS
				/usr/include
			  PATH_SUFFIXES
			    lib
				lib/Winx64-visualStudio
				irrKlang-64bit-1.5.0/lib/Winx64-visualStudio
				Winx64-visualStudio
			)
			
			set(IRRKLANG_SO_LIBRARIES "")
		ELSE()
			find_library(
			  IRRKLANG_LIBRARIES
			  NAMES
				irrKlang.lib
			  HINTS
				ENV IRRKLANG_DIR
				ENV IRRKLANG_ROOT
			  PATHS
				/usr/include
			  PATH_SUFFIXES
			    lib
				lib/Win32-visualStudio
				irrKlang-1.5.0/lib/Win32-visualStudio
				Win32-visualStudio
			)
			
			set(IRRKLANG_SO_LIBRARIES "")
		ENDIF()		
	ENDIF()
	# There are no such windows distribution
	IF (MINGW)
		find_library(
		  IRRKLANG_LIBRARIES
		  NAMES
			irrKlang
		  HINTS
			ENV IRRKLANG_DIR
			ENV IRRKLANG_ROOT
		  PATHS
			/usr/lib
		  PATH_SUFFIXES
			lib
			lib/Win32-gcc
			irrKlang-1.5.0/lib/Win32-gcc
			Win32-gcc
		)
			
		set(IRRKLANG_SO_LIBRARIES "")
	ENDIF()
ELSE()
	IF (BITSX64)
		set(IRRKLANG_LIBRARIES "")
		find_library(
		  IRRKLANG_SO_LIBRARIES
		  libIrrKlang.so
		  HINTS
			ENV IRRKLANG_DIR
			ENV IRRKLANG_ROOT
		  PATHS
			/bin
			/lib
			/lib64
		  PATH_SUFFIXES
			bin/linux-gcc-64
			bin
			linux-gcc-64
			irrKlang-64bit-1.5.0/bin/linux-gcc-64
		)
	ELSE()
		set(IRRKLANG_LIBRARIES "")
		find_library(
		  IRRKLANG_SO_LIBRARIES
		  libIrrKlang.so
		  HINTS
			ENV IRRKLANG_DIR
			ENV IRRKLANG_ROOT
		  PATHS
			/bin
			/lib
			/lib64
		  PATH_SUFFIXES
			bin/linux-gcc
			bin
			linux-gcc
			irrKlang-1.5.0/bin/linux-gcc
		)	
	ENDIF()
ENDIF()

IF (IRRKLANG_LIBRARIES OR IRRKLANG_SO_LIBRARIES)
	set(IRRKLANG_LIBRARIES_FOUND TRUE)
ENDIF()

IF (IRRKLANG_INCLUDE_DIR AND (IRRKLANG_LIBRARIES OR IRRKLANG_SO_LIBRARIES))
	set(IRRKLANG_FOUND TRUE)
ENDIF()


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(irrKlang  DEFAULT_MSG
                                  IRRKLANG_LIBRARIES_FOUND IRRKLANG_INCLUDE_DIR)

mark_as_advanced(IRRKLANG_INCLUDE_DIR IRRKLANG_LIBRARIES IRRKLANG_SO_LIBRARIES)