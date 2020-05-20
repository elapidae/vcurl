#ifndef VCURL_H
#define VCURL_H

//=======================================================================================
/*  2020-05-20      by elapidae
 *
 * vcurl -- wrapper for curl library. Only easy part, while.
*/
//=======================================================================================

#include <chrono>
#include <memory>
#include <stdexcept>

//  Try not use curl as-is, only for CURLcode, because this enum is very big :(.
#include <curl/curl.h>

//=======================================================================================
class vcurl final
{
public:
    class error;

    vcurl();
    ~vcurl();

    void timeout( std::chrono::seconds s );         //  by default: 3 seconds
    void connect_timeout( std::chrono::seconds s ); //  by default: 3 seconds

    std::string exec( const std::string& url );

    void perform();

    //  Call automatic if need.
    void cleanup();

private:
    class _pimpl; std::unique_ptr<_pimpl> _p;
};
//=======================================================================================
class vcurl::error : public std::runtime_error
{
public:
    error( const std::string& msg, CURLcode c = CURLE_OK );
    CURLcode code() const;

private:
    CURLcode _code;
};
//=======================================================================================

#endif // VCURL_H
