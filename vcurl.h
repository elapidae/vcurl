#ifndef VCURL_H
#define VCURL_H

//=======================================================================================
/*  2020-05-20      by elapidae
 *
 * vcurl -- wrapper for curl library. Only easy part, while.
 *
 *  Описание флагов:
 *  https://php.ru/manual/function.curl-setopt.html
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
    explicit vcurl( const std::string& url );
    ~vcurl();

    const std::string& result()   const;
    operator const std::string&() const;

    //  getinfo
    std::string effective_url() const;
    std::string content_type()  const;

    double total_time_seconds() const;
    std::chrono::microseconds total_time() const;

    //  setters
    void timeout( std::chrono::seconds s );         //  by default: 3 seconds
    void connect_timeout( std::chrono::seconds s ); //  by default: 3 seconds
    void url( const std::string& u );

    static std::string one_shot( const std::string& url );

    const std::string& exec( const std::string& url );
    const std::string& exec();

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
    error( const std::string& where, const std::string& err, CURLcode c );

    CURLcode code()             const noexcept;
    const std::string& text()   const noexcept;
    const std::string& where()  const noexcept;

private:
    CURLcode    _code;
    std::string _text;
    std::string _where;
};
//=======================================================================================

#endif // VCURL_H
