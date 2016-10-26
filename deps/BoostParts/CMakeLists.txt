cmake_minimum_required( VERSION 2.8 )

project( BoostParts )

if(CPPBUILD)
	include(cppbuild_common)
	CPPBUILD_DEPEND()
endif()

file( GLOB_RECURSE SOURCES *.cpp )

# We need to remove all the thread cpp files and then add them on a
# per-platform
# basis
file( GLOB_RECURSE to_remove libs/thread/*.cpp libs/detail/*.cpp )

if( to_remove )
	list( REMOVE_ITEM SOURCES ${to_remove} )
endif()

set( COMMON_SOURCES
	libs/thread/src/future.cpp
	)

set( WIN_SOURCES
	libs/thread/src/win32/thread.cpp
	libs/thread/src/win32/tss_dll.cpp
	libs/thread/src/win32/tss_pe.cpp
	)

set( UNIX_SOURCES
	libs/thread/src/pthread/once.cpp
	libs/thread/src/pthread/once_atomic.cpp
	libs/thread/src/pthread/thread.cpp
	)

list( APPEND SOURCES
	${COMMON_SOURCES} )

if ( WIN32 )
	list( APPEND SOURCES
		${WIN_SOURCES} )
else()
	list( APPEND SOURCES
		${UNIX_SOURCES}
		)
endif()

#############################################################################

include_directories(
	SYSTEM
	${CMAKE_CURRENT_SOURCE_DIR}/include
	)

if(CPPBUILD)
	CPPBUILD_LIB_TARGET(BoostParts ${SOURCES})
else()
	add_library(
		BoostParts
		${SOURCES} )
endif()
