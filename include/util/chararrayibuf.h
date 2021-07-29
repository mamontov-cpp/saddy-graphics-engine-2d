/*! \file chararrayibuf.h

    Defines default implementation of char array stream wrapper as streambuf object, taken from 
    http://www.mr-edd.co.uk/blog/beginners_guide_streambuf .
 */
#pragma once
#include <streambuf>
#include <vector>
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdlib>
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdio>

namespace sad
{
    
namespace util
{

/*! A default implementation of char array stream wrapper a steambuf object, taken from 
    http://www.mr-edd.co.uk/blog/beginners_guide_streambuf .
 */
class CharArrayIBuf : public std::streambuf
{
public:
    /*! Constructs object
        \param[in] begin beginning of string
        \param[in] end ending of string
     */
    CharArrayIBuf(const char *begin, const char *end);

private:
    /*! Returns character on underflow
        \return character on underflow
     */
    virtual int_type underflow() override;
    /*! Returns character on underflow and advance position
        \return character on underflow
     */
    virtual int_type uflow() override;
    /*! Put character back in the case of backup underflow
        \param[in] ch character
        \return character back
     */
    virtual int_type pbackfail(int_type ch) override;
    /*! Returns number of character available
        \return number of character available
     */
    virtual std::streamsize showmanyc() override;
    /*! Seeks off in stream buf
        \param[in] off offset
        \param[in] way a way
        \param[in] which what positions should be modified
     */
    virtual std::streampos seekoff(
        std::streamoff off, 
        std::ios_base::seekdir way,
        std::ios_base::openmode which
    ) override;
    /*! Reads data from stream
        \param[in] s data
        \param[in] sz stream
        \return amount of read bytes
     */
    virtual std::streamsize xsgetn(char* s, std::streamsize sz) override;

    /*! Copying is disabled
        \param[in] o object
     */
    CharArrayIBuf(const sad::util::CharArrayIBuf& o);
    /*! Copying is disabled
        \param[in] o object
        \return self-reference
     */
    CharArrayIBuf& operator=(const CharArrayIBuf& o);

private:
    /*! A pointer to beginning of buffer
     */
    const char * const m_begin;
    /*! A pointer to end of buffer  
     */
    const char * const m_end;
    /*! A pointer to current part of buffer
     */
    const char * m_current;
};

}

}
