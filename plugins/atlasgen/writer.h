/*! \file writer.h
    

    Defines a writer for writing output configs atlases
 */
#pragma once
#include "atlas.h"

/*! Implement this to perform writing output configs for atlases
 */
class Writer
{
public:
    /*! Constructs new reader
     */
    Writer();
    /*! Must be inherited
     */
    virtual ~Writer();
    /*! Writes an atlas to output file
        \param[in] name a reading name
        \param[in] withindex should we write an index to file
        \return result
     */
    virtual bool write(
        const Atlas& atlas,
        bool withindex
    )  = 0;
    /*! Returns errors for reader
        \return errors
     */
    QVector<QString>& errors();
    /*! Returns errors for reader 
        \return errors
     */
    const QVector<QString>& errors() const;
protected:
    /*! A list of errors
     */
    QVector<QString> m_errors;
};
