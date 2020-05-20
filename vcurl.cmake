#########################################################################################
##
##  VLIBS codebase, NIIAS
##
##  GNU Lesser General Public License Usage
##  This file may be used under the terms of the GNU Lesser General Public License
##  version 3 as published by the Free Software Foundation and appearing in the file
##  LICENSE.LGPL3 included in the packaging of this file. Please review the following
##  information to ensure the GNU Lesser General Public License version 3 requirements
##  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#########################################################################################
# vcurl.cmake

#========================================================================================
if ( NOT  vcurl_INCLUDED )
    set ( vcurl_INCLUDED TRUE )

    message( "=== about include vcurl... ===" )

    # Need decide how to link with library...
    #pkg_search_modules( CURL REQUIRED libcurl )
    #find_package( LIBCURL )
    #message( FATAL_ERROR ${LIBCURL} )

    find_library( LIBCURL curl )
    if ( NOT LIBCURL )
        message( FATAL_ERROR "libcurl not found in system" )
    else()
        message( " >>> libcurl found in system: ${LIBCURL} " )
        set( V_LIBRARIES ${V_LIBRARIES} ${LIBCURL} )
        unset( LIBCURL )
    endif()

    include( "${VLIBS_DIR}/cmake/vcat.cmake" )    

    include_directories( "${CMAKE_CURRENT_LIST_DIR}/" )

    set( V_HEADERS ${V_HEADERS} "${CMAKE_CURRENT_LIST_DIR}/vcurl.h"   )
    set( V_SOURCES ${V_SOURCES} "${CMAKE_CURRENT_LIST_DIR}/vcurl.cpp" )

    message( "=== vcurl included ===" )
endif()
# vcurl.cmake
#========================================================================================
