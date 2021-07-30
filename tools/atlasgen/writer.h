/*! \file writer.h
    

    Defines a writer for writing output configs atlases
 */
#pragma once
#include "atlas.h"

struct OutputOptions;

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
        \param[in] atlas an atlas to write
        \param[in, out] opts options
        \return result
     */
    virtual bool write(
        const Atlas& atlas,
        OutputOptions& opts
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
