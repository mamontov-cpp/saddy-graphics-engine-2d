#include "util/chararrayibuf.h"

#include <functional>
#include <cassert>
// ReSharper disable once CppUnusedIncludeDirective
#include <cstring>

sad::util::CharArrayIBuf::CharArrayIBuf(const char *begin, const char *end) :
    m_begin(begin),
    m_end(end),
    m_current(begin)
{
    assert(std::less_equal<const char *>()(begin, end));
}

sad::util::CharArrayIBuf::int_type sad::util::CharArrayIBuf::underflow()
{
    if (m_current == m_end)
        return traits_type::eof();

    return traits_type::to_int_type(*m_current);
}

sad::util::CharArrayIBuf::int_type sad::util::CharArrayIBuf::uflow()
{
    if (m_current == m_end)
        return traits_type::eof();

    return traits_type::to_int_type(*m_current++);
}

sad::util::CharArrayIBuf::int_type sad::util::CharArrayIBuf::pbackfail(int_type ch)
{
    if (m_current == m_begin || (ch != traits_type::eof() && ch != m_current[-1]))
        return traits_type::eof();

    return traits_type::to_int_type(*--m_current);
}

std::streamsize sad::util::CharArrayIBuf::showmanyc()
{
    assert(std::less_equal<const char *>()(m_current, m_end));
    return m_end - m_current;
}


std::streampos sad::util::CharArrayIBuf::seekoff(
    std::streamoff off, 
    std::ios_base::seekdir way,
    std::ios_base::openmode which
)
{
    if (way == std::ios_base::beg)
    {
        m_current = m_begin + off;
    }
    else
    {
        if (way == std::ios_base::cur)
        {
           m_current += off;
        }
        else
        {
            m_current = m_end + off;
        }
    }
    return this->showmanyc();
}

std::streamsize  sad::util::CharArrayIBuf::xsgetn(char* s, std::streamsize sz)
{
    std::streamsize bytesleft = this->showmanyc();
    if (sz > bytesleft)
    {
        memcpy(s, m_current, bytesleft * sizeof(char));
        m_current += bytesleft;
        return bytesleft;
    }
    memcpy(s, m_current, sz * sizeof(char));
    m_current += sz;
    return sz;
}