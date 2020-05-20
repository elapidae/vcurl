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

using namespace std;

//=======================================================================================
//  https://curl.haxx.se/libcurl/c/curl_global_init.html
//  Contract for call below functions:
struct Init_All final
{
    Init_All()  { curl_global_init(CURL_GLOBAL_ALL); }
    ~Init_All() { curl_global_cleanup();             }
} _init_all_;
//=======================================================================================


//=======================================================================================
static std::string _make_err_msg( const string& where, string err, CURLcode c )
{
    if ( err.empty() )
        err = curl_easy_strerror( c );

    return vcat("vcurl error from:", where, ", code:", c, ", text:'", err, "'");
}
vcurl::error::error( const string &where, const std::string& err, CURLcode c )
    : std::runtime_error( _make_err_msg(where,err,c) )
    , _code( c )
    , _text( err )
    , _where( where )
{}
//=======================================================================================
CURLcode vcurl::error::code() const noexcept
{
    return _code;
}
//=======================================================================================
const string &vcurl::error::text() const noexcept
{
    return _text;
}
//=======================================================================================
const string &vcurl::error::where() const noexcept
{
    return _where;
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
    std::string url;

    CURL *curl = nullptr;

    void init();
    void cleanup();

    void perform();

    template<typename Arg>
    void get_opt( CURLINFO opt, Arg arg );

    std::string get_opt_str( CURLINFO opt );

    template<typename Res>
    Res get_opt_any( CURLINFO opt );

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
        throw error( "init", "curl_easy_init()==nullptr", CURLE_OK ); // not real error.

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

    set_opt( CURLOPT_URL, url.c_str() );
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

    throw error( "perform", err_buffer, res );
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
void vcurl::_pimpl::get_opt( CURLINFO opt, Arg arg )
{
    assert( curl );

    auto res = curl_easy_getinfo( curl, opt, arg );
    if ( res != CURLE_OK )
        throw error( "get_opt",err_buffer,res );
}
//=======================================================================================
template<typename Res>
Res vcurl::_pimpl::get_opt_any( CURLINFO opt )
{
    Res res;
    get_opt( opt, &res );
    return res;
}
//=======================================================================================
std::string vcurl::_pimpl::get_opt_str( CURLINFO opt )
{
    char * ptr = nullptr;
    get_opt( opt, &ptr );

    assert( ptr );
    return ptr;
}
//=======================================================================================
template<typename Arg>
void vcurl::_pimpl::set_opt( CURLoption prm, Arg arg )
{
    assert( curl );

    auto res = curl_easy_setopt( curl, prm, arg );
    if ( res != CURLE_OK )
        throw error( "set_opt", err_buffer, res );
}
//=======================================================================================

//=======================================================================================
vcurl::vcurl()
    : _p( new _pimpl(this) )
{}
//=======================================================================================
vcurl::vcurl( const std::string& url )
    : _p( new _pimpl(this) )
{
    exec( url );
}
//=======================================================================================
vcurl::~vcurl()
{}
//=======================================================================================
const std::string &vcurl::result() const
{
    return _p->data;
}
//=======================================================================================
vcurl::operator const std::string& () const
{
    return result();
}
//=======================================================================================
std::string vcurl::effective_url() const
{
    return _p->get_opt_str( CURLINFO_EFFECTIVE_URL );
}
//=======================================================================================
string vcurl::content_type() const
{
    return _p->get_opt_str( CURLINFO_CONTENT_TYPE );
}
//=======================================================================================
double vcurl::total_time_seconds() const
{
    return _p->get_opt_any<double>( CURLINFO_TOTAL_TIME );
}
//=======================================================================================
std::chrono::microseconds vcurl::total_time() const
{
    return chrono::microseconds( int64_t(total_time_seconds() * 1e6) );
}
//=======================================================================================
std::string vcurl::one_shot( const std::string& url )
{
    return vcurl( url );
}
//=======================================================================================
const std::string& vcurl::exec( const std::string& u )
{
    url( u );
    return exec();
}
//=======================================================================================
const std::string& vcurl::exec()
{
    _p->init();
    perform();
    return result();
}
//=======================================================================================
void vcurl::url( const string& u )
{
    _p->url = u;
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

//=======================================================================================
ostream &operator <<(ostream &os, const vcurl &url)
{
    os << url.result();
    return os;
}
//=======================================================================================
