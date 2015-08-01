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
    /*! A list of errors
     */
    QVector<QString> Errors;


    /*! Constructs new reader
     */
    Writer();
    /*! Must be inherited
     */
    virtual ~Writer();
    /*! Writes an atlas to output file
        \param[in] name a reading name
		\param[in] filename a name for file
		\param[in] outputTexture a file name for output texture
		\param[in] withindex should we write an index to file
		\return result
     */
    virtual bool write(
		const Atlas& atlas,
		const QString& filename,
		const QString& outputTexture,
		bool withindex
	)  = 0;
};
