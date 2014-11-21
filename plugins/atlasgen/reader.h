/*! \file reader.h
    \author HiddenSeeker

    Defines a reader for atlases
 */
#pragma once
#include "atlas.h"

class Reader
{
public:
    /*! Whether reading was successfull
     */
    bool Successfull;
    /*! A resulting atlas
     */
    Atlas* Result;
    /*! A list of errors
     */
    QVector<QString> Errors;
    /*! An output file name for atlas config
     */
    QString OutputName;
    /*! An output file name for atlas texture
     */
    QString OutputTexture;

    /*! Constructs new reader
     */
    Reader();
    /*! Can be inherited
     */
    virtual ~Reader();
    /*! Reads a file with specified name
       \param name a reading name
     */
    virtual void read(const QString& name)  = 0;
};
