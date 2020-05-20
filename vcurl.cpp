//=======================================================================================
/*  (Not all) related links:
 *
 * https://curl.haxx.se/libcurl/c/CURLOPT_ERRORBUFFER.html
 *
*/
//=======================================================================================

#include "vcurl.h"

#include <curl/curl.h>
#include <cassert>

#include "vcat.h"

//=======================================================================================
vcurl::error::error( const std::string& msg, CURLcode c )
    : std::runtime_error( msg )
    , _code( c )
{}
//=======================================================================================
CURLcode vcurl::error::code() const
{
    return _code;
}
//=======================================================================================

//=======================================================================================
class vcurl::_pimpl final
{
    vcurl *owner;
public:
    _pimpl( vcurl *own );
    ~_pimpl();

    std::chrono::seconds timeout {3};
    std::chrono::seconds connect_timeout {3};

    CURL *curl = nullptr;

    void init();
    void cleanup();

    void perform();

    template<typename Arg>
    void set_opt( CURLoption prm, Arg arg );

    static size_t s_write_data( void *buffer, size_t size, size_t nmemb, void *userp );
    std::string data;

    char err_buffer[CURL_ERROR_SIZE];
};
//=======================================================================================
vcurl::_pimpl::_pimpl(vcurl *own)
    : owner(own)
{}
//=======================================================================================
vcurl::_pimpl::~_pimpl()
{
    cleanup();
}
//=======================================================================================
void vcurl::_pimpl::init()
{
    cleanup();

    curl = curl_easy_init();
    if ( !curl )
        throw error( "Cannot init new connection." );

    set_opt( CURLOPT_WRITEDATA, this );
    set_opt( CURLOPT_WRITEFUNCTION, &_pimpl::s_write_data );

    set_opt( CURLOPT_ERRORBUFFER, err_buffer );
    err_buffer[0] = 0;  // clear it.

    if ( timeout.count() > 0 )
    {
        long cnt = timeout.count();         //  Перестраховка, чтобы не сломать
        set_opt( CURLOPT_TIMEOUT, cnt );    //  типизацию.
    }

    if ( connect_timeout.count() )
    {
        long cnt = connect_timeout.count();
        set_opt( CURLOPT_CONNECTTIMEOUT, cnt );
    }
}
//=======================================================================================
void vcurl::_pimpl::cleanup()
{
    if ( !curl ) return;

    data.clear();
    curl_easy_cleanup( curl );
    curl = nullptr;
}
//=======================================================================================
void vcurl::_pimpl::perform()
{
    assert( curl );
    CURLcode res = curl_easy_perform( curl );
    if ( res == CURLE_OK ) return;

    vcat msg("Perform error [",res,"]: '",err_buffer,"'.");
    throw error( msg, res );
}
//=======================================================================================
size_t vcurl::_pimpl::s_write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    auto segsize = size * nmemb;
    auto cbuffer = static_cast<const char*>( buffer );

    auto self = static_cast<_pimpl*>( userp );
    self->data.append( cbuffer, segsize );

    return segsize;
}
//=======================================================================================
template<typename Arg>
void vcurl::_pimpl::set_opt( CURLoption prm, Arg arg )
{
    assert( curl );

    auto res = curl_easy_setopt( curl, prm, arg );
    if (res != 0)
        throw error(vcat("Error during setopt: [", res, "]: ",curl_easy_strerror(res)));
}
//=======================================================================================

//=======================================================================================
vcurl::vcurl()
    : _p( new _pimpl(this) )
{}
//=======================================================================================
vcurl::~vcurl()
{}
//=======================================================================================
std::string vcurl::exec( const std::string& url )
{
    _p->init();
    _p->set_opt( CURLOPT_URL, url.c_str() );

    perform();
    return _p->data;
}
//=======================================================================================
void vcurl::perform()
{
    _p->perform();
}
//=======================================================================================
void vcurl::cleanup()
{
    _p->cleanup();
}
//=======================================================================================
