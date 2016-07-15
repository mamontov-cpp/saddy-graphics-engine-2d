#include "writer.h"

Writer::Writer()
{

}

Writer::~Writer()
{

}

QVector<QString>& Writer::errors()
{
    return m_errors;
}

const QVector<QString>& Writer::errors() const
{
    return m_errors;
}