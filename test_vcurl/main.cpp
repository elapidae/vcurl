/****************************************************************************************
**
**  VLIBS codebase, NIIAS
**
**  GNU Lesser General Public License Usage
**  This file may be used under the terms of the GNU Lesser General Public License
**  version 3 as published by the Free Software Foundation and appearing in the file
**  LICENSE.LGPL3 included in the packaging of this file. Please review the following
**  information to ensure the GNU Lesser General Public License version 3 requirements
**  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
****************************************************************************************/

#include "gtest/gtest.h"
#include "vcurl.h"
#include "vlog.h"

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
//    |||||||||||||||
class vcurl_test: public testing::Test
{};
#pragma GCC diagnostic pop

//=======================================================================================
TEST_F( vcurl_test, simple_example )
{
    vcurl url;
    auto res = url.exec( "https://www.example.com" );
    vdeb << res;
}
//=======================================================================================
TEST_F( vcurl_test, bad_example )
{
    try {
        vcurl url;
        auto res = url.exec( "http://www.any-bred.com1" );
        vdeb << res;
    } catch ( const vcurl::error &e ) {
        if ( e.code() == vcurl::error::COULDNT_RESOLVE_HOST ) return;
        throw;
    }
}
//=======================================================================================
//  EXPECT_TRUE
//
//  EXPECT_EQ
//  EXPECT_FLOAT_EQ
//  EXPECT_DOUBLE_EQ
//
//  EXPECT_THROW
//  EXPECT_ANY_THROW
//  EXPECT_NO_THROW
//


//=======================================================================================
//  Main, do not delete...
//=======================================================================================
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
//=======================================================================================
//  Main, do not delete...
//=======================================================================================
