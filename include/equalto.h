/*! \file equalto.h
    

    Contains nice specializations for std::equal_to for points, rects, sizes
 */
#pragma once
#include <functional>

#include "sadpoint.h"
#include "sadrect.h"
#include "sadsize.h"
#include "geometry2d.h"

namespace std
{
/*! A specialization for comparing double values
 */
template<
>
struct equal_to<double> : std::binary_function <double,double,bool>
{
  /*! Invokes comparison
      \param[in] x first argument
      \param[in] y second argument
      \return  value
   */
  inline bool operator() (const double& x, const double& y) const
  {
      return sad::is_fuzzy_equal(x, y);
  }
};

/*! A specialization for comparing float values
 */
template<
>
struct equal_to<float> : std::binary_function <float,float,bool>
{
  /*! Invokes comparison
      \param[in] x first argument
      \param[in] y second argument
      \return  value
   */
  inline bool operator() (const float& x, const float& y) const
  {
      return sad::is_fuzzy_equal(static_cast<double>(x), static_cast<double>(y));
  }
};


/*! A specialization for comparing sad::Point2D values
 */
template<
>
struct equal_to<sad::Point2D> : std::binary_function<sad::Point2D,sad::Point2D,bool>
{
  /*! Invokes comparison
      \param[in] x first argument
      \param[in] y second argument
      \return  value
   */
  inline bool operator() (const sad::Point2D& x, const sad::Point2D& y) const
  {
      return sad::equal(x, y);
  }
};

/*! A specialization for comparing sad::Point2I values
 */
template<
>
struct equal_to<sad::Point2I> : std::binary_function<sad::Point2I,sad::Point2I,bool>
{
  /*! Invokes comparison
      \param[in] x first argument
      \param[in] y second argument
      \return  value
   */
  inline bool operator() (const sad::Point2I& x, const sad::Point2I& y) const
  {
      return x.x() == y.x() && x.y() == y.y();
  }
};

/*! A specialization for comparing sad::Point3D values
 */
template<
>
struct equal_to<sad::Point3D> : std::binary_function<sad::Point3D,sad::Point3D,bool>
{
  /*! Invokes comparison
      \param[in] x first argument
      \param[in] y second argument
      \return  value
   */
  inline bool operator() (const sad::Point3D& x, const sad::Point3D& y) const
  {
      return sad::equal(x, y);
  }
};

/*! A specialization for comparing sad::Point3I values
 */
template<
>
struct equal_to<sad::Point3I> : std::binary_function<sad::Point3D,sad::Point3D,bool>
{
  /*! Invokes comparison
      \param[in] x first argument
      \param[in] y second argument
      \return  value
   */
  inline bool operator() (const sad::Point3I& x, const sad::Point3I& y) const
  {
      return x.x() == y.x() && x.y() == y.y() && x.z() == y.z();
  }
};

/*! A specialization for comparing sad::Rect2D values
 */
template<
>
struct equal_to<sad::Rect2D> : std::binary_function<sad::Rect2D,sad::Rect2D,bool>
{
  /*! Invokes comparison
      \param[in] x first argument
      \param[in] y second argument
      \return  value
   */
  inline bool operator() (const sad::Rect2D& x, const sad::Rect2D& y) const
  {
      return sad::equal(x, y);
  }
};

/*! A specialization for comparing sad::Rect2I values
 */
template<
>
struct equal_to<sad::Rect2I> : std::binary_function<sad::Rect2I,sad::Rect2I,bool>
{
  /*! Invokes comparison
      \param[in] x first argument
      \param[in] y second argument
      \return  value
   */
  inline bool operator() (const sad::Rect2I& x, const sad::Rect2I& y) const
  {
	  const std::equal_to<sad::Point2I> c;  // NOLINT(modernize-use-transparent-functors)
      return c(x[0], y[0]) && c(x[1], y[1]) && c(x[2], y[2]) && c(x[3], y[3]);
  }
};


/*! A specialization for comparing sad::Rect<sad::Point3D> values
 */
template<
>
struct equal_to<sad::Rect<sad::Point3D> > : std::binary_function<sad::Rect<sad::Point3D>,sad::Rect<sad::Point3D>,bool>
{
  /*! Invokes comparison
      \param[in] x first argument
      \param[in] y second argument
      \return  value
   */
  inline bool operator() (const sad::Rect<sad::Point3D>& x, const sad::Rect<sad::Point3D>& y) const
  {
      return sad::equal(x, y);
  }
};

/*! A specialization for comparing sad::Rect<sad::Point3I> values
 */
template<
>
struct equal_to<sad::Rect<sad::Point3I> > : std::binary_function<sad::Rect<sad::Point3I>,sad::Rect<sad::Point3I>,bool>
{
  /*! Invokes comparison
      \param[in] x first argument
      \param[in] y second argument
      \return  value
   */
  inline bool operator() (const sad::Rect<sad::Point3I>& x, const sad::Rect<sad::Point3I>& y) const
  {
	  const std::equal_to<sad::Point3I> c;  // NOLINT(modernize-use-transparent-functors)
      return c(x[0], y[0]) && c(x[1], y[1]) && c(x[2], y[2]) && c(x[3], y[3]);
  }
};

/*! A specialization for comparing sad::Size2D values
 */
template<
>
struct equal_to<sad::Size2D> : std::binary_function<sad::Size2D,sad::Size2D,bool>
{
  /*! Invokes comparison
      \param[in] x first argument
      \param[in] y second argument
      \return  value
   */
  inline bool operator() (const sad::Size2D& x, const sad::Size2D& y) const
  {
      return sad::is_fuzzy_equal(x.Width, y.Width) && sad::is_fuzzy_equal(x.Height, y.Height);
  }
};


/*! A specialization for comparing sad::Size2I values
 */
template<
>
struct equal_to<sad::Size2I> : std::binary_function<sad::Size2I,sad::Size2I,bool>
{
  /*! Invokes comparison
      \param[in] x first argument
      \param[in] y second argument
      \return  value
   */
  inline bool operator() (const sad::Size2I& x, const sad::Size2I& y) const
  {
      return x.Width == y.Width && x.Height == y.Height;
  }
};


}
