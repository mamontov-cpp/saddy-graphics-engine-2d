/*! \file    hpair.hpp
    \author  HiddenSeeker

	\brief   Definition of pair class.

	This file contains a definition of pair container class.
*/
#pragma once

namespace hst
{
/*! \class  pair
    \brief  Pair container class

    Defines a pair container class
*/
template<class T1,class T2>
class pair
{
  private:
          T1 m_1;  //!< First property
          T2 m_2;  //!< Second property
  public:
          /*! Default constructor
          */
          pair();
          /*! Parametric
              \param[in] m1 first property
              \param[in] m2 second propery
          */
          pair(const T1 & m1, const T2 & m2);
          /*! Copy constructor
              \param[in] o other pair
          */
          pair(const pair & o);
          /*!  Destructor
          */
          ~pair();
          /*!  Copies a pair
               \param[in] o other pair
               \return self-reference
          */
          pair & operator=(const pair & o);

          /*! Equiualence operator
              \param[in] o other pair
              \return true if equal
          */
          bool operator==(const pair & o) const;

          /*! Equiualence operator
              \param[in] o other pair
              \return true if not equal
          */
          bool operator!=(const pair & o) const;


          /*! Returns first property
              \return m_1
          */
          const T1 & p1() const;
          /*! Returns second property
              \return m_1
          */
          const T2 & p2() const;

          /*! Sets first property
              \param[in] p first property
          */
          inline pair & set1(const T1 & p);
          /*! Sets second property
              \param[in] p second property
          */
          inline pair & set2(const T2 & p);
};

/*! \class  triplet
    \brief  Triplet container class

    Defines a triplet container class
*/
template<class T1,class T2,class T3>
class triplet
{
  private:
          T1 m_1;  //!< First property
          T2 m_2;  //!< Second property
		  T3 m_3;  //!< Third property
  public:
          /*! Default constructor
          */
          triplet();
          /*! Parametric
              \param[in] m1 first property
              \param[in] m2 second propery
			  \param[in] m3 third property
          */
          triplet(const T1 & m1, const T2 & m2,const T3 & m3);
          /*! Copy constructor
              \param[in] o other pair
          */
          triplet(const triplet & o);
          /*!  Destructor
          */
          ~triplet();
          /*!  Copies a pair
               \param[in] o other pair
               \return self-reference
          */
          triplet & operator=(const triplet & o);

          /*! Equiualence operator
              \param[in] o other pair
              \return true if equal
          */
          bool operator==(const triplet & o);

          /*! Equiualence operator
              \param[in] o other pair
              \return true if not equal
          */
          bool operator!=(const triplet & o);

          /*! Returns first property
              \return m_1
          */
          T1 & _1();
          /*! Returns second property
              \return m_1
          */
          T2 & _2();
          /*! Returns second property
              \return m_1
          */
          T3 & _3();



          /*! Returns first property
              \return m_1
          */
          const T1 & p1() const;
          /*! Returns second property
              \return m_1
          */
          const T2 & p2() const;
          /*! Returns second property
              \return m_1
          */
          const T3 & p3() const;

          /*! Sets first property
              \param[in] p first property
          */
          inline triplet & set1(const T1 & p);
          /*! Sets second property
              \param[in] p second property
          */
          inline triplet & set2(const T2 & p);
		  /*! Sets third property
              \param[in] p third property
          */
          inline triplet & set3(const T3 & p);
};

}

/*! Convenience typedef
*/
#define hpair hst::pair
/*! Convenienct typedef
*/
#define htriplet hst::triplet

#ifndef HI_PAIR
    #define HI_PAIR
    #include "hpair_src.hpp"
#endif
