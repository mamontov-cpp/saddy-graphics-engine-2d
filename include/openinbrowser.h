/*! \file openinbrowser.h
 *
 *  Contains function for attempting open url in browser
 */
#pragma once

namespace sad
{
/*! Tries to open asynchronously url in browser
 *  \param[in] url an inner url
 *  \return result of underlying system() call
 */
int openInBrowser(const char* url);
}