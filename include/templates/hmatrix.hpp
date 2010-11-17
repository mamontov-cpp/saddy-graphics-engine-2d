/*! \file   hmatrix.hpp
    \author HiddenSeeker

    \brief contains static_array and matrix, that provides linear
    algebra operations on matrices.

    Contains hst::static_array and hst::matrix definitions
*/
#include "hlvector.hpp"
#pragma once

namespace hst
{
  /*! \class static_array
      \brief Class of static_array, that provides simple operations over the vector

     Class of static array
  */
  template<typename T>
  class static_array
  {
    private:
            hlvector<T>  m_v; //!< Wrapped vector
    public:
            /*! Default constructor
            */
            static_array();
            /*! Copy constructor
                \param[in] o other array
            */
            static_array(const static_array & o);
            /*! Assignment operator overload
                \param[in] o other array
                \return reference
            */
            static_array & operator=(const static_array & o);
            /*! Default destructor
            */
            ~static_array();

            /*! Returns amount of elements
                \return count
            */
            inline unsigned long count();
            /*! Returns constant reference to object
               \param[in] i
               \return constant reference
            */
            inline const T & operator[](unsigned long i) const;
            /*! Returns constant reference to object
               \param[in] i
               \return constant reference
            */
            inline T & operator[](unsigned long i) ;
            /*! Reserves following amount of elements, without cleaning it
                \param[in] imax new amount of elements
                \param[in] fill fill element
            */
            void preserve(unsigned long imax,const T & fill=T());
            /*! Fill with elements
                \param[in] _fill fill element
            */
            void fill(const T & _fill);
  };
  /*! \class matrix
      \brief class of matrix, providing mathematic operations

      This class provides operation, known of linear algebra, such as
      transposition, inversion (if can ), multiplication and determinant
      calculations
  */
  template<typename T>
  class matrix
  {
    private:
           hst::static_array< hst::static_array<T> > m_impl; //!< Implementaion of it
           unsigned long m_rows;
           unsigned long m_cols;
    public:
           /*! Default constructor
           */
           matrix();
           /*! Copy constructor
               \param[in] o other matrix
           */
           matrix(const matrix & o);
           /*! Assignment operator overload
              \param[in] o other matrix
              \return self-reference
           */
           matrix & operator=(const matrix & o);
           /*! Default destructor
           */
           ~matrix();
           /*! Parametric constructor
               \param[in] rows rows
               \param[in] cols cols
               \param[in] fill filling element
           */
           matrix(unsigned long rows, unsigned long cols, const T & fill=T());
           /*! Returns a constant reference to row
               \param[in] row row index
               \return reference
           */
           const static_array<T> & operator[](unsigned long i) const;
           /*! Returns a reference to row
               \param[in] row row index
               \return reference
           */
           static_array<T> & operator[](unsigned long i) ;
           /*! Sums all elements with value
               \param[in] o value
               \return self-refernece
           */
           matrix & operator+=(const T & o);
           /*! Sums all elements with other matrix elements
               \param[in] other matrix
               \return self-refernece
           */
           matrix & operator+=(const matrix & o);
           /*! Returns transposed matrix
               \return transposed matrix
           */
           matrix transpose() const;
           /*! Returns ix,jx minor of matrix
               \param[in] ix row position
               \param[in] jx col position
           */
           matrix minor(unsigned long ix,unsigned long jx) const;
           /*! Calculates determinant of matrix
               \return determinant
           */
           T determinant() const;
           /*! Inverses matrix
           */
           matrix inverse() const;
           /*! Provides non-commutative multiplication
               \param[in] o other matrix
               \return result
           */
           matrix operator*(const matrix & o) const;
           /*! Provides  multiplication
               \param[in] o value
               \return result
           */
           matrix operator*(const T & o) const;
           /*! Returns one matrix, specified rank
               \param[in] rank
               \return matrix
           */
           static  matrix one(unsigned long rank);
           /*! Multiplies all elements with value
               \param[in] o value
               \return self-refernece
           */
           matrix & operator*=(const T & o);
           /*! Fill with elements
               \param[in] _fill fill element
           */
           void fill(const T & _fill= T() );
           /*! preserves amount of rows and cols in matrix
               \param[in] rows new rows count
               \param[in] cols new cols count
               \param[in] fill filler element
           */
           void preserve(unsigned long rows, unsigned long cols,const T & fill=T());
           /*! Return amount of rows in matrix
               \return amount of rows
           */
           inline unsigned long rows() const;
           /*! Return amount of columns in matrix
               \return amount of columns
           */
           inline unsigned long cols() const;
  };

  /*! Defines a common used matrix
  */
  typedef matrix<float> matrixf;
}

#ifndef HI_MATRIX_H
     #define HI_MATRIX_H
     #include "hmatrix_src.hpp"
#endif
