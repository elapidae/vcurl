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
    auto addr = "http://www.example.com";
    std::string res;

    //  May use standard static method.
    res = vcurl::one_shot( addr );

    //  Using object we can save state of query and ask some things.
    vcurl url;

    //  May use exec() method.
    res = url.exec( addr );

    //  May set url direct
    url.url( addr );
    res = url.exec();

    EXPECT_EQ( res, url.result() );

    // implicit convertion for result.
    std::string res2;
    res2 = url;
    EXPECT_EQ( res, res2 );
}
//=======================================================================================
TEST_F( vcurl_test, resolv_err_example )
{
    try {
        vcurl url;
        auto res = url.exec( "http://www.any-bred.com1" );
        vdeb << res;
    } catch ( const vcurl::error &e ) {
        vdeb << e.what();
    }
}
//=======================================================================================
TEST_F( vcurl_test, info )
{
    vcurl url( "http://example.com" );
    vdeb << "eff url:" << url.effective_url();
    vdeb << "total time us:" << url.total_time();
    vdeb << "total time  s:" << url.total_time_seconds();
    vdeb << "con type: " << url.content_type();
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
