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
    enum Code
    {
        PHONY,  // Reason in text.
        COULDNT_RESOLVE_HOST
    };
    error( const std::string& msg, Code c = PHONY );
    Code code() const;

private:
    Code _code;
};
//=======================================================================================

#endif // VCURL_H
