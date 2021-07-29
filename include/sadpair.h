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
              \param[in] m2 second property
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

          /*! Equivalence operator
              \param[in] o other Pair
              \return true if equal
          */
          bool operator==(const Pair & o) const;

          /*! Equivalence operator
              \param[in] o other Pair
              \return true if not equal
          */
          bool operator!=(const Pair & o) const;


          /*! Returns first property
              \return m_1
          */
          const T1 & p1() const;
          /*! Returns second property
              \return m_2
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

    Defines a triplet container class
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
              \param[in] m2 second property
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

          /*! Equivalence operator
              \param[in] o other Pair
              \return true if equal
          */
          bool operator==(const Triplet & o);

          /*! Equivalence operator
              \param[in] o other Pair
              \return true if not equal
          */
          bool operator!=(const Triplet & o);

          /*! Returns first property
              \return m_1
          */
          T1 & _1();
          /*! Returns second property
              \return m_2
          */
          T2 & _2();
          /*! Returns third property
              \return m_3
          */
          T3 & _3();



          /*! Returns first property
              \return m_1
          */
          const T1 & p1() const;
          /*! Returns second property
              \return m_2
          */
          const T2 & p2() const;
          /*! Returns third property
              \return m_3
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

/*! \class  Quadruplet
    \brief  A container class, which contains four linked elements

    Defines a container  for having a quadruplets
*/
template<class T1,class T2,class T3, class T4>
class Quadruplet
{
  private:
          T1 m_1;  //!< First property
          T2 m_2;  //!< Second property
          T3 m_3;  //!< Third property
          T4 m_4;  //!< Fourth property
  public:
          /*! Default constructor
          */
          Quadruplet();
          /*! Parametric
              \param[in] m1 first property
              \param[in] m2 second property
              \param[in] m3 third property
              \param[in] m4 fourth property
          */
          Quadruplet(const T1 & m1, const T2 & m2,const T3 & m3, const T4 & m4);
          /*! Copy constructor
              \param[in] o other Pair
          */
          Quadruplet(const Quadruplet & o);
          /*!  Destructor
          */
          ~Quadruplet();
          /*!  Copies a Pair
               \param[in] o other Pair
               \return self-reference
          */
          Quadruplet & operator=(const Quadruplet & o);

          /*! Equivalence operator
              \param[in] o other Pair
              \return true if equal
          */
          bool operator==(const Quadruplet & o);

          /*! Equivalence operator
              \param[in] o other Pair
              \return true if not equal
          */
          bool operator!=(const Quadruplet & o);

          /*! Returns first property
              \return m_1
          */
          T1 & _1();
          /*! Returns second property
              \return m_2
          */
          T2 & _2();
          /*! Returns third property
              \return m_3
          */
          T3 & _3();
          /*! Returns fourth property
              \return m_4
          */
          T4 & _4();



          /*! Returns first property
              \return m_1
          */
          const T1 & p1() const;
          /*! Returns second property
              \return m_2
          */
          const T2 & p2() const;
          /*! Returns third property
              \return m_3
          */
          const T3 & p3() const;
          /*! Returns fourth property
              \return m_4
          */
          const T4 & p4() const;

          /*! Sets first property
              \param[in] p first property
          */
          inline Quadruplet & set1(const T1 & p);
          /*! Sets second property
              \param[in] p second property
          */
          inline Quadruplet & set2(const T2 & p);
          /*! Sets third property
              \param[in] p third property
          */
          inline Quadruplet & set3(const T3 & p);
          /*! Sets fourth property
              \param[in] p fourth property
          */
          inline Quadruplet & set4(const T4 & p);
          
};

}

#include "sadpair_src.h"
