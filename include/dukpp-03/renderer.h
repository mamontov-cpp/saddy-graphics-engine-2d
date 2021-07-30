/*! \file dukpp-03\renderer.h
    
    A ref countable renderer implementation
 */
#pragma once
#include <renderer.h>
#include <refcountable.h>

namespace sad
{
    
namespace dukpp03
{

/*! A ref countable renderer
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
