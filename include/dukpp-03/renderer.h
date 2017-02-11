/*! \file renderer.h
    
    A refcountable renderer implementation
 */
#pragma once
#include <renderer.h>
#include <refcountable.h>

namespace sad
{
    
namespace dukpp03
{

/*! A refcountable renderer
 */    
class Renderer: public sad::Renderer, public sad::RefCountable
{
public:
    /*! Makes new renderer
     */
    Renderer();    
    /*! Can be inherited
     */
    virtual ~Renderer();
};

}

}
