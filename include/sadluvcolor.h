/*! \file   sadluvcolor.h
    

    Describes color in CIE LUV color model.
    See http://dcssrv1.oit.uci.edu/~wiedeman/cspace/me/infoluv.html for detail
 */
#pragma once
#include "sadcolor.h"

namespace sad
{

class LUVColor
{
public:
    /*! Creates new color with ligtness set to 0 and u and v are set to their 
        minimal values - -134 for u, -140 for v. This values were taken from
        http://software.intel.com/sites/products/documentation/hpc/ipp/ippi/ippi_ch6/ch6_color_models.html
        (see bottom of page)
     */
    inline LUVColor() : m_L(0), m_u(-134.0), m_v(-140.0)
    {

    }
    /*! Converts typical color to LUV color model
        \param[in] c color
     */ 
    LUVColor(const sad::Color & c);
    /*! Returns Euclidean distance between to colors
        \param[in] c color
        \return color distance
     */
    double distance(const sad::LUVColor & c) const;
    /*! Converts to common color to color space
        \return new color, equal to taken
     */
    operator sad::Color();
    /*! Returns lightness component
        \return lightness component
     */
    inline double L() const
    {
        return m_L;
    }
    /*! Returns chrominance component, which defines shifts from green to red
        \return chrominance component
     */
    inline double u() const
    {
        return m_u;
    }
    /*! Returns chrominance component, which defines shifts from blue to purple
        \return chrominance component
     */
    inline double v() const
    {
        return m_v;
    }

    /*! Sets ligtness component
        \param[in] L lightness component
     */
    inline void setL(double L)
    {
        m_L = L;
    }
    /*! Sets chrominance component, which defines shifts from green to red
        \param[in] u chrominance component
     */
    inline void setU(double u)
    {
        m_u = u;
    }
    /*! Sets chrominance component, which defines shifts from blue to purple
        \param[in] v chrominance component
     */
    inline void setV(double v)
    {
        m_v = v;
    }
protected:
    double m_L; //!< A lightness component 
    double m_u; //!< Chrominance component, which defines shifts from green to red
    double m_v; //!< Chrominance component, which defines shifts from blue to purple
};

}
