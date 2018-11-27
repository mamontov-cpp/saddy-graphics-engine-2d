/*! \file weight.h
    

    Describes a weight of object
 */
#pragma once

namespace sad
{

namespace p2d
{
/*! Describes a weight of object
 */
class Weight
{
public:
    /*! Creates new weight
        \param[in] value weight value
        \param[in] finite
     */
    Weight(double value = 1.0, bool finite = true);
    /*! Determines, whether weight is infinite
        \return true if infinite
     */
    bool isInfinite() const;
    /*! Returns weight value
        \return true value
     */
    double value() const;
    /*! Sets a value for a weight. Normalizes weight to 0.0001 if set to zero.
        \param[in] value a value for a weight
     */
    void setValue(double value);
    /*! Creates new constant weight
        \param[in] value weight 
        \return created weight
     */
    static Weight constant(double value);
    /*! Returns infinite weight
        \return created weight
     */
    static Weight infinite();
protected:
    bool m_finite;  //!< Is weight of object is finite
    double m_value; //!< Value of weight
};

}

}
