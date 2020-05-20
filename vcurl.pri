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
# vcurl.pri

#========================================================================================
isEmpty(qi_vcurl) {
    qi_vcurl = 1;
    isEmpty(qi_not_print_pri_messages): message("=== vcurl appended ===")

    isEmpty(VLIBS_DIR): error("vcurl: Need VLIBS_DIR correct path.")
    include( $$VLIBS_DIR/qmake/vcat.pri )

    INCLUDEPATH += $$PWD

    CONFIG      *= link_pkgconfig
    PKGCONFIG   *= libcurl

    #------------------------------------------------------------------------------------
    #   CORE
    HEADERS     += $$PWD/vcurl.h
    SOURCES     += $$PWD/vcurl.cpp

    #------------------------------------------------------------------------------------
    OTHER_FILES += $$PWD/vcurl.cmake
    OTHER_FILES += $$PWD/README
}
# vcurl.pri
#========================================================================================

