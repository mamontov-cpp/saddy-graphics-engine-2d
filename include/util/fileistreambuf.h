/*! \file fileistreambuf.h

    Defines default implementation of FILE* stream wrapper as streambuf object, taken from 
    http://www.mr-edd.co.uk/blog/beginners_guide_streambuf .
 */
#pragma once
#include <streambuf>
#include <vector>
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdlib>
#include <cstdio>

namespace sad
{
    
namespace util
{
    
/*! A default implementation of FILE* stream buffer as streambuf object, taken from 
    http://www.mr-edd.co.uk/blog/beginners_guide_streambuf .
 */
class FileIStreamBuf : public std::streambuf
{
public:
    /*! Constructs new stream buffer
        \param[in] fptr file 
        \param[in] buff_sz size of buffer
        \param[in] put_back size data
     */
    FileIStreamBuf(FILE *fptr, std::size_t buff_sz = 256, std::size_t put_back = 8);
private:
    /*! Returns character on underflow
        \return character on underflow
     */
    virtual int_type underflow() override;
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
    FileIStreamBuf(const sad::util::FileIStreamBuf & o);
    /*! Copying is disabled
        \param[in] o object
        \return self-reference
     */
    sad::util::FileIStreamBuf& operator= (const sad::util::FileIStreamBuf & o);

private:
    FILE * m_fptr;
    const std::size_t m_put_back;
    std::vector<char> m_buffer;
};


}


}
