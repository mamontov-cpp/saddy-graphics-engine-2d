/*! \file    sadpair.h
    \author  mamontov-cpp

	\brief   Definition of Pair class.

	This file contains a definition of Pair container class.
*/
#pragma once

namespace sad
{
/*! \class  Pair
    \brief  Pair container class

    Defines a Pair container class
*/
template<class T1,class T2>
class Pair
{
  private:
          T1 m_1;  //!< First property
          T2 m_2;  //!< Second property
  public:
          /*! Default constructor
          */
          Pair();
          /*! Parametric constructor
              \param[in] m1 first property
              \param[in] m2 second propery
          */
          Pair(const T1 & m1, const T2 & m2);
          /*! Copy constructor
              \param[in] o other Pair
          */
          Pair(const Pair & o);
          /*!  Destructor
          */
          ~Pair();
          /*!  Copies a Pair
               \param[in] o other Pair
               \return self-reference
          */
          Pair & operator=(const Pair & o);

          /*! Equiualence operator
              \param[in] o other Pair
              \return true if equal
          */
          bool operator==(const Pair & o) const;

          /*! Equiualence operator
              \param[in] o other Pair
              \return true if not equal
          */
          bool operator!=(const Pair & o) const;


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
          inline Pair & set1(const T1 & p);
          /*! Sets second property
              \param[in] p second property
          */
          inline Pair & set2(const T2 & p);
		  /*! Returns a reference to property
			  \return first property
		  */
		  inline T1 & _1() { return m_1; }
		  /*! Returns a reference to property
			  \return second property
		  */
		  inline T2 & _2() { return m_2; }
};

/*! \class  Triplet
    \brief  Triplet container class

    Defines a Triplet container class
*/
template<class T1,class T2,class T3>
class Triplet
{
  private:
          T1 m_1;  //!< First property
          T2 m_2;  //!< Second property
		  T3 m_3;  //!< Third property
  public:
          /*! Default constructor
          */
          Triplet();
          /*! Parametric
              \param[in] m1 first property
              \param[in] m2 second propery
			  \param[in] m3 third property
          */
          Triplet(const T1 & m1, const T2 & m2,const T3 & m3);
          /*! Copy constructor
              \param[in] o other Pair
          */
          Triplet(const Triplet & o);
          /*!  Destructor
          */
          ~Triplet();
          /*!  Copies a Pair
               \param[in] o other Pair
               \return self-reference
          */
          Triplet & operator=(const Triplet & o);

          /*! Equiualence operator
              \param[in] o other Pair
              \return true if equal
          */
          bool operator==(const Triplet & o);

          /*! Equiualence operator
              \param[in] o other Pair
              \return true if not equal
          */
          bool operator!=(const Triplet & o);

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
          inline Triplet & set1(const T1 & p);
          /*! Sets second property
              \param[in] p second property
          */
          inline Triplet & set2(const T2 & p);
		  /*! Sets third property
              \param[in] p third property
          */
          inline Triplet & set3(const T3 & p);
};

}

#include "sadpair_src.h"
