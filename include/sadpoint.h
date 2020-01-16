/*! \file sadpoint.h
    
    
    Contains definitions of both 2D and 3D points
 */
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#pragma pack(push, 1)

namespace sad
{

/*! \class Point2
    This class describes a 2D Point, with it's coordinates of type _Type.
    A _Type must be numeric and castable to double type, to make computing distance possible
*/
template<typename _Type>
class Point2
{
public:
    /*! By default point initialized by zeros
     */
    inline Point2() : m_x(0), m_y(0)
    {
    }
    /*! Constructs point by two coordinates
        \param[in] x X Coordinate
        \param[in] y Y Coordinate
     */
    inline Point2(_Type x, _Type y): m_x(x), m_y(y)
    {

    }
    /*! Sums both coordinates of two points and returns result
        \param[in] o second point
        \return result
     */
    inline sad::Point2<_Type> operator+(const sad::Point2<_Type> & o) const
    {
        return sad::Point2<_Type>(m_x + o.m_x, m_y + o.m_y);
    }
    /*! Substracts coordinates of second point from first point and returns result
        \param[in] o second point
        \return result
     */
    inline sad::Point2<_Type> operator-(const sad::Point2<_Type> & o) const
    {
        return sad::Point2<_Type>(m_x - o.m_x, m_y - o.m_y);
    }
    /*! Multiplicates both coordinates of point by number and returns result
        \param[in] o number
        \return result
     */
    inline sad::Point2<_Type>  operator*(_Type o) const 
    {
        return sad::Point2<_Type>(m_x * o, m_y * o);
    }
    /*! Performs a division of both coordinates of point by number and returns result
        \param[in] o number
        \return result
     */
    inline sad::Point2<_Type>  operator/(_Type o)  const
    {
        return sad::Point2<_Type>(m_x / o, m_y / o);
    }
    /*! Adds coordinates of second point to current point
        \param[in] o second point
        \return self-reference
     */
    inline sad::Point2<_Type>& operator+=(const sad::Point2<_Type> & o)
    {
        m_x += o.m_x;
        m_y += o.m_y;
        return *this;
    }
    /*! Substracts coordinates of second point from current point
        \param[in] o second point
        \return self-reference
     */
    inline sad::Point2<_Type>& operator-=(const sad::Point2<_Type> & o)
    {
        m_x -= o.m_x;
        m_y -= o.m_y;
        return *this;
    }
    /*! Multiplicates both coordinates of current point by number
        \param[in] o number
        \return self-reference
     */
    inline sad::Point2<_Type>& operator*=(_Type o)
    {
        m_x *= o;
        m_y *= o;
        return *this;
    }
    /*! Performs division of both coordinates of current point by number
        \param[in] o number
        \return self-reference
     */
    inline sad::Point2<_Type>& operator/=(_Type o)
    {
        m_x /= o;
        m_y /= o;
        return *this;
    }
    /*! Returns X coordinate
        \return X coordinate
     */
    inline _Type x() const
    {
        return m_x;
    }
    /*! Returns Y coordinate
        \return Y coordinate
     */
    inline _Type y() const
    {
        return m_y;
    }

    /*! Sets X coordinate
        \param[in] x new X coordinate
     */
    inline void setX(_Type x) 
    {
        m_x = x;
    }
    /*! Sets Y coordinate
        \param[in] y new Y coordinate
     */
    inline void setY(_Type y)
    {
        m_y = y;
    }
    /*! Sets both coordinates of point to values
        \param[in] x X coordinate
        \param[in] y Y coordinate
     */
    inline void set(_Type x, _Type y)
    {
        m_x = x;
        m_y = y;
    }
    /*! Computes a distance between two points
        \param[in] o a second point
        \return distance between two points
     */
    inline double distance(const sad::Point2<_Type> & o) const
    {
        double dx = m_x - o.m_x;
        double dy = m_y - o.m_y;
        return sqrt(dx * dx + dy * dy);
    }

    /*! Converts a point to point with other type
        \param[out] o point, which should be filled with coordinates of current type
     */
    template<
        typename _OtherType
    >
    inline void toType(sad::Point2<_OtherType> & o) const
    {
        o.m_x = (_OtherType)m_x;
        o.m_y = (_OtherType)m_y;
    }
protected:
    _Type m_x;        //!< X coordinate
    _Type m_y;        //!< Y coordinate
};


template<typename _Type>
class Point3: public sad::Point2<_Type>
{
public:
    /*! By default point initialized by zeros
     */
    inline Point3() : sad::Point2<_Type>(), m_z(0)
    {
    }
    /*! Converts a 2D point to 3D point, filling last coordinate with zero
        \param[in] o other point
     */
    inline Point3(const  sad::Point2<_Type> & o) : sad::Point2<_Type>(o), m_z(0) 
    {
    }
    /*! Constructs point by two coordinates. Fills Z coordinate with zero
        \param[in] x X Coordinate
        \param[in] y Y Coordinate
     */
    inline Point3(_Type x, _Type y): sad::Point2<_Type>(x, y),  m_z(0)
    {

    }
    /*! Constructs point by three coordinates
        \param[in] x X Coordinate
        \param[in] y Y Coordinate
        \param[in] z Z Coordinate
     */
    inline Point3(_Type x, _Type y, _Type z): sad::Point2<_Type>(x, y),  m_z(z)
    {

    }
    /*! Sums both coordinates of two points and returns result
        \param[in] o second point
        \return result
     */
    inline sad::Point3<_Type> operator+(const sad::Point3<_Type> & o) const
    {
        return sad::Point3<_Type>(this->m_x + o.x(), this->m_y + o.y(), this->m_z + o.z());
    }
    /*! Substracts coordinates of second point from first point and returns result
        \param[in] o second point
        \return result
     */
    inline sad::Point3<_Type> operator-(const sad::Point3<_Type> & o) const
    {
        return sad::Point3<_Type>(this->m_x - o.x(), this->m_y - o.y(), this->m_z - o.z());
    }
    /*! Multiplicates both coordinates of point by number and returns result
        \param[in] o number
        \return result
     */
    inline sad::Point3<_Type>  operator*(_Type o) const 
    {
        return sad::Point3<_Type>(this->m_x * o, this->m_y * o, this->m_z * o);
    }
    /*! Performs a division of both coordinates of point by number and returns result
        \param[in] o number
        \return result
     */
    inline sad::Point3<_Type>  operator/(_Type o)  const
    {
        return sad::Point3<_Type>(this->m_x / o, this->m_y / o, this->m_z / o);
    }
    /*! Adds coordinates of second point to current point
        \param[in] o second point
        \return self-reference
     */
    inline sad::Point3<_Type>& operator+=(const sad::Point3<_Type> & o)
    {
        this->m_x += o.x();
        this->m_y += o.y();
        this->m_z += o.m_z;
        return *this;
    }
    /*! Substracts coordinates of second point from current point
        \param[in] o second point
        \return self-reference
     */
    inline sad::Point3<_Type>& operator-=(const sad::Point3<_Type> & o)
    {
        this->m_x -= o.x();
        this->m_y -= o.y();
        this->m_z -= o.m_z;
        return *this;
    }
    /*! Multiplicates both coordinates of current point by number
        \param[in] o number
        \return self-reference
     */
    inline sad::Point3<_Type>& operator*=(_Type o)
    {
        this->m_x *= o;
        this->m_y *= o;
        this->m_z *= o;
        return *this;
    }
    /*! Performs division of both coordinates of current point by number
        \param[in] o number
        \return self-reference
     */
    inline sad::Point3<_Type>& operator/=(_Type o)
    {
        this->m_x /= o;
        this->m_y /= o;
        this->m_z /= o;
        return *this;
    }
    /*! Returns Z coordinate
        \return z coordinate
     */
    inline _Type z() const
    {
        return m_z;
    }
    /*! Sets Z coordinate
        \param[in] z new Z coordinate
     */
    inline void setZ(_Type z)
    {
        m_z = z;
    }
    /*! Sets all components of point to value
        \param[in] x X coordinate
        \param[in] y Y coordinate
        \param[in] z Z coordinate
     */
    inline void set(_Type x, _Type y, _Type z)
    {
        this->m_x = x;
        this->m_y = y;
        this->m_z = z;
    }
    /*! Computes a distance between two points
        \param[in] o a second point
        \return distance between two points
     */
    inline double distance(const sad::Point3<_Type> & o) const
    {
        double dx = this->m_x - o.x();
        double dy = this->m_y - o.y();
        double dz = this->m_z - o.m_z;
        return sqrt(dx * dx + dy * dy + dz * dz);
    }

    /*! Converts a point to point with other type
        \param[out] o point, which should be filled with coordinates of current type
     */
    template<
        typename _OtherType
    >
    inline void toType(sad::Point3<_OtherType> & o) const
    {
        o.m_x = (_OtherType)(this->m_x);
        o.m_y = (_OtherType)(this->m_y);
        o.m_z = (_OtherType)(this->m_z);
    }
protected:
    _Type m_z;
};


/*! A default type of point, which can be used in most case
 */
typedef sad::Point2<double> Point2D;
/*! A special type, which can be used for storing window coordinates
 */
typedef sad::Point2<int>    Point2I;
/*! A special type, used to store data from mapping window coordinates 
    to OpenGL coordinates 
 */
typedef sad::Point3<double> Point3D;
/*! A type, declared for symmetry with sad::Point2I
 */
typedef sad::Point3<int>    Point3I;
/*! A vector, used for 3D operations, like glTranslatef, glRotatef
 */
typedef sad::Point3D Vector3D;
/*! A vector, used for 2D operations
 */
typedef sad::Point2D Vector2D;

}

#pragma pack(pop)
