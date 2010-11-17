namespace hst
{
  #include <assert.h>
  #define TEMPDEF template<typename T>

  TEMPDEF static_array<T>::static_array() {}
  TEMPDEF static_array<T>::static_array(const static_array<T> & o) {m_v=o.m_v;}
  TEMPDEF static_array<T> & static_array<T>::operator=(const static_array<T> & o) {m_v=o.m_v; return *this;}
  TEMPDEF static_array<T>::~static_array() {}
  TEMPDEF inline unsigned long static_array<T>::count() {return m_v.count();}
  TEMPDEF inline const T & static_array<T>::operator[](unsigned long i) const {return m_v[i];}
  TEMPDEF inline T & static_array<T>::operator[](unsigned long i) { return m_v[i];}
  TEMPDEF void static_array<T>::fill(const T & _fill)
  {
    for (unsigned long i=0;i<count();i++) m_v[i]=fill;
  }
  TEMPDEF void static_array<T>::preserve(unsigned long imax,const T & fill)
  {
    if (imax==count()) return;
    if (imax>count())
    {
        for (unsigned long i=count();i<imax;i++)
             m_v.add(fill);
    }
    else
    {
        m_v.removeRange(imax,count()-1);
    }
  }
  TEMPDEF inline unsigned long matrix<T>::rows() const {return m_rows;}
  TEMPDEF inline unsigned long matrix<T>::cols() const {return m_cols;}

  TEMPDEF void matrix<T>::preserve(unsigned long rows, unsigned long cols,const T & fill)
  {
      static_array<T> tmpfill;
      tmpfill.preserve(cols,fill);
      for (unsigned long i=0;i<m_rows;i++)
           m_impl[i].preserve(cols,fill);
      m_impl.preserve(rows,tmpfill);
      m_rows=rows;
      m_cols=cols;
  }
  TEMPDEF void matrix<T>::fill(const T & _fill)
  {
      static_array<T> tmpfill;
      tmpfill.preserve(cols,_fill);
      m_impl.fill(tmpfill);
  }
  TEMPDEF matrix<T>::matrix()                    {m_rows=0;m_cols=0;}
  TEMPDEF matrix<T>::matrix(const matrix<T> & o) {m_rows=o.m_rows;m_cols=o.m_cols; m_impl=o.m_impl;}
  TEMPDEF matrix<T> & matrix<T>::operator=(const matrix<T> & o) {m_rows=o.m_rows;m_cols=o.m_cols; m_impl=o.m_impl; return *this;}
  TEMPDEF matrix<T>::~matrix() {}
  TEMPDEF matrix<T>::matrix(unsigned long rows, unsigned long cols, const T & fill)
  {
      m_rows=0;
      m_cols=0;
      preserve(rows,cols,fill);
  }
  TEMPDEF const static_array<T> & matrix<T>::operator[](unsigned long i) const
  {
      return m_impl[i];
  }
  TEMPDEF static_array<T> & matrix<T>::operator[](unsigned long i)
  {
      return m_impl[i];
  }
  TEMPDEF matrix<T> & matrix<T>::operator+=(const T & o)
  {
      for (unsigned long i=0;i<m_rows;i++)
          for (unsigned long j=0;j<m_cols;j++)
             (*this)[i][j]+=o;
      return *this;
  }
  TEMPDEF matrix<T> & matrix<T>::operator*=(const T & o)
  {
      for (unsigned long i=0;i<m_rows;i++)
          for (unsigned long j=0;j<m_cols;j++)
              (*this)[i][j]*=o;
      return *this;
  }
  TEMPDEF matrix<T>  matrix<T>::operator*(const T & o) const
  {
      matrix<T> res(rows(),cols());
      for (unsigned long i=0;i<m_rows;i++)
          for (unsigned long j=0;j<m_cols;j++)
              res[i][j]=m_impl[i][j]*o;
      return res;
  }
  TEMPDEF matrix<T>  matrix<T>::one(unsigned long rank)
  {
      matrix<T> res(rank,rank,static_cast<T>(0.0f));
      for (unsigned long i=0;i<rank;i++)
           res[i][i]=static_cast<T>(1.0f);
      return res;
  }
  TEMPDEF matrix<T> & matrix<T>::operator+=(const matrix<T> & o)
  {
      assert( m_rows==o.m_rows && m_cols==o.m_cols );
      for (unsigned long i=0;i<m_rows;i++)
          for (unsigned long j=0;j<m_cols;j++)
                (*this)[i][j]+=o[i][j];
      return *this;
  }
  TEMPDEF  matrix<T> matrix<T>::transpose() const
  {
      matrix<T> tmp(m_cols,m_rows);
      for (unsigned long i=0;i<m_rows;i++)
          for (unsigned long j=0;j<m_cols;j++)
             tmp[j][i]=(*this)[i][j];
      return tmp;
  }
  TEMPDEF matrix<T>  matrix<T>::minor(unsigned long ix,unsigned long jx) const
  {
      //if (!(ix<m_rows && jx<m_cols && m_rows>1 && m_cols>1))
      //    printf("Ix: %ld Jx: %ld m_rows: %ld m_cols: %ld\n",ix,jx,m_rows,m_cols);
      assert(ix<m_rows && jx<m_cols && m_rows>1 && m_cols>1);
      unsigned long curi=0,curj=0;
      matrix<T> tmp(m_rows-1,m_cols-1);
      for (unsigned long i=0; i<m_rows;i++)
      {
        curj=0;
          for (unsigned long j=0;j<m_cols;j++)
             if (jx!=j && curi<tmp.m_rows)
             {
                 tmp[curi][curj]=(*this)[i][j];
                 ++curj;
             }
        if (ix!=i) ++curi;
      }
      return tmp;
  }
  #define M m_impl
  TEMPDEF T matrix<T>::determinant() const
  {
      assert(m_rows && m_rows==m_cols);
      if (m_rows==1) return M[0][0];
      if (m_rows==2) return (M[0][0]*M[1][1])-(M[0][1]*M[1][0]);
      if (m_rows==3)
      {
          T d1=M[1][1]*M[2][2]-M[1][2]*M[2][1];
          T d2=M[1][0]*M[2][2]-M[1][2]*M[2][0];
          T d3=M[1][0]*M[2][1]-M[1][1]*M[2][0];
          return M[0][0]*d1-M[0][1]*d2+M[0][2]*d3;
      }
      T res=T();
      //Calc determinant by kramer method
      for (unsigned long i=0;i<m_cols;i++)
      {
          if (M[0][i])
          {
            T d=M[0][i]*minor(0,i).determinant();
            if (i % 2==0) res+=d; else res-=d;
          }
      }
      return res;
  }
  TEMPDEF matrix<T>  matrix<T>::inverse() const
  {
      matrix<T> res(m_rows,m_cols);
      T det=determinant();
      T c;
      assert( det && "Deteminant is 0!");
      if (m_rows==1 && m_cols ==1)
           return matrix(1,1,1/m_impl[0][0]);
      for (unsigned long i=0;i<m_rows;i++)
      {
       for (unsigned long j=0;j<m_cols;j++)
       {
          c=((i+j) % 2 ==0)?1.0f:-1.0f;
          //printf("Set: %ld %ld: %d\n",i,j,(i+j) %2 ==0);
          res[i][j]=c*minor(i,j).determinant()/det;
       }
      }
      res=res.transpose();
      return res;
  }
  TEMPDEF matrix<T> matrix<T>::operator*(const matrix<T> & o) const
  {
      assert( rows() && cols() && cols()==o.rows() );
      matrix<T> res(rows(),o.cols(), (T)(0.0f));
      for (unsigned long i=0;i<res.rows();i++)
      {
          for (unsigned long j=0;j<res.cols();j++)
          {
              for (unsigned long ic=0;ic<cols();ic++)
                 res[i][j]+=M[i][ic]*o[ic][j];
          }
      }
      return res;
  }
  #undef M
  #undef TEMPDEF
}
