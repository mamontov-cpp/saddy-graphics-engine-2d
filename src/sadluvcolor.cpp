#include "sadluvcolor.h"
#include <cmath>


sad::LUVColor::LUVColor(const sad::Color & c)
{
    // Implementation taken from 
    // http://software.intel.com/sites/products/documentation/hpc/ipp/ippi/ippi_ch6/ch6_color_models.html
    
    // We assume, that colors are already gamma corrected, so we just 
    // can divide them on their values, getting required R', G', B' 
    double r_apostrophe = ((double)c.r()) / 255.0;
    double g_apostrophe = ((double)c.g()) / 255.0;
    double b_apostrophe = ((double)c.b()) / 255.0;

    // Convert normalized colors of R'G'B' space to XYZ color space
    double X = 0.412453 * r_apostrophe + 0.35758  * g_apostrophe + 0.180423 * b_apostrophe;
    double Y = 0.212671 * r_apostrophe + 0.71516  * g_apostrophe + 0.072169 * b_apostrophe;
    double Z = 0.019334 * r_apostrophe + 0.119193 * g_apostrophe + 0.950227 * b_apostrophe;

    // Convert colors from XYZ space to LUV color space
    double xn = 0.312713;
    double yn = 0.329016;
    double Yn = 1.0;

    double u = 4 * X / (X + 15 * Y + 3 * Z);
    double v = 9 * Y / (X + 15 * Y + 3 * Z);
    double un = 4 * xn / ( -2 * xn + 12 * yn + 3);
    double vn = 9 * yn / ( -2 * xn + 12 * yn + 3);

    m_L = 116 * pow(Y / Yn, 1.0 / 3.0) - 16.0;
    m_u = 13 * m_L * (u - un);
    m_v = 13 * m_L * (v - vn);
}


double sad::LUVColor::distance(const sad::LUVColor & c) const
{
    double dL = m_L - c.m_L;
    double du = m_u - c.m_u;
    double dv = m_v - c.m_v;
    double squaresum =  dL * dL +  du * du + dv * dv;
    double result = sqrt(squaresum);
    return result;
}

sad::LUVColor::operator sad::Color()
{
    // Implementation taken from 
    // http://software.intel.com/sites/products/documentation/hpc/ipp/ippi/ippi_ch6/ch6_color_models.html

    // Convert from LUV color space to XYZ color space
    double xn = 0.312713;
    double yn = 0.329016;
    double Yn = 1.0;

    double un = 4 * xn / ( -2 * xn + 12 * yn + 3);
    double vn = 9 * yn / ( -2 * xn + 12 * yn + 3);

    double u = m_u / (13 * m_L) + un;
    double v = m_v / (13 * m_L) + vn;

    double Y = Yn * pow((m_L + 16) / 116, 3.0);
    double X = -9 * Y * u / ((u - 4)* v - u * v );
    double Z = (9 * Y - 15 * v * Y - v * X) / 3 * v;

    // Convert from XYZ color space to normalized RGB color space
    double r_apostrophe =  3.240479 * X - 1.53715  * Y - 0.498535 * Z;
    double g_apostrophe = -0.969256 * X + 1.875991 * Y + 0.041556 * Z;
    double b_apostrophe =  0.055648 * X - 0.204043 * Y + 1.057311 * Z;

    sad::Color result;
    
    result.setR((unsigned char)(r_apostrophe * 255.0));
    result.setG((unsigned char)(g_apostrophe * 255.0));
    result.setB((unsigned char)(b_apostrophe * 255.0));

    return result;
}
