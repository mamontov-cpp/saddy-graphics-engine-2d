#include "util/fileistreambuf.h"

#include <algorithm>
// ReSharper disable once CppUnusedIncludeDirective
#include <cstring>


sad::util::FileIStreamBuf::FileIStreamBuf(FILE *fptr, size_t buff_sz, size_t put_back) :
    m_fptr(fptr),
    m_put_back(std::max(put_back, size_t(1))),
    m_buffer(std::max(buff_sz, put_back) + put_back)
{
    char *end = &m_buffer.front() + m_buffer.size();
    setg(end, end, end);
}

std::streambuf::int_type sad::util::FileIStreamBuf::underflow()
{
    if (gptr() < egptr()) // buffer not exhausted
        return traits_type::to_int_type(*gptr());

    char *base = &m_buffer.front();
    char *start = base;

    if (eback() == base) // true when this isn't the first fill
    {
        // Make arrangements for putback characters
        std::memmove(base, egptr() - m_put_back, m_put_back);
        start += m_put_back;
    }

    // start is now the start of the buffer, proper.
    // Read from fptr_ in to the provided buffer
    size_t n = std::fread(start, 1, m_buffer.size() - (start - base), m_fptr);
    if (n == 0)
        return traits_type::eof();

    // Set buffer pointers
    setg(base, start, start + n);

    return traits_type::to_int_type(*gptr());
}

std::streampos sad::util::FileIStreamBuf::seekoff (
    std::streamoff off, 
    std::ios_base::seekdir way,
    std::ios_base::openmode
)
{
    if (way == std::ios_base::beg)
    {
        fseek(m_fptr, off, SEEK_SET);
    }
    else
    {
        if (way == std::ios_base::cur)
        {
            fseek(m_fptr, off, SEEK_CUR);
        }
        else
        {
            fseek(m_fptr, off, SEEK_END);
        }
    }
    return ftell(m_fptr);
}

std::streamsize  sad::util::FileIStreamBuf::xsgetn(char* s, std::streamsize sz)
{
    return fread(s, sizeof(char), sz, m_fptr);
}

