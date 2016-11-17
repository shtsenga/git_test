//---------------------------------------------------------------------------

#ifndef matrixH
#define matrixH
//---------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <cassert>
using namespace std ;
const double  SMALL = 1.0e-13 ;
template <class T> T  abs( const T& a ) {
    return  a >= static_cast<T>(0) ? a : -a ;
}

template  <class T> class   Matrix
{
  private :
    vector< vector<T> >  mat ;            // 雙重向量陣列
    int                  row , col ;      // 列數與行數
    void  swap( vector<T>& a , vector<T>& b ) const {
        vector<T>  tmp = a ;
        a = b ;
        b = tmp ;
    };
  public :
    __property int Rows= {read = row};
    __property int Cols= {read = col};
    // 預設建構函式
    Matrix() : row(0) , col(0) {};
    Matrix( int r , int c , const T& val = 0 ) : row(r) , col(c) {
        mat = vector< vector<T> >(r,vector<T>(c,val)) ;
    };
    Matrix( vector< vector<T> >& a ): mat(a), row(a.size()), col(a[0].size()) {};
    inline vector<T>&   operator [] ( int i ){return  mat[i] ;};    // 回傳矩陣的第 i 列的向量陣列參考
    inline const vector<T>&  operator [] ( int i ) const {return  mat[i] ;};
    vector<T> Vec() const;
    Matrix<T>  operator - () const ;
    T         Det() const;
    Matrix<T> Inv() const;
    Matrix<T> Trans() const;
    /* overload */
    Matrix<T>&   operator += ( const Matrix<T>& rhs );   // 重載 += 運算子
    Matrix<T>&   operator -= ( const Matrix<T>& rhs );   // 重載 -= 運算子
    Matrix<T>&   operator = ( const Matrix<T>& rhs );    // 重載 -= 運算子
    void __fastcall Resize(int NewRows,int NewCount, const T& Val=0);
};
template <class T> Matrix<T>  Matrix<T>::operator - () const {
        Matrix<T>  foo = *this ;
        int  i , j ;
        for ( i = 0 ; i < row ; ++i )
            for ( j = 0 ; j < col ; ++j ) foo.mat[i][j] = -foo.mat[i][j] ;
        return  foo ;
}

//------------------------------------------------------------------------------
template <class T>  vector<T> Matrix<T>::Vec() const
{
    int i,j;
    vector<T> v(0);
    for(i=0;i<row;i++)
        for(j=0;j<col;j++) v.push_back(mat[i][j]);
    return(v);
}
//------------------------------------------------------------------------------
template <class T>  T  Matrix<T>::Det() const
{
    assert( row == col ) ;
    vector< vector<T> >  a = mat ;
    int  i , j , k ;
    T   m ;
    for ( i = 0 ; i < row-1 ; ++i ) {
        if ( a[i][i] == static_cast<T>(0) ) {
            for ( j = i+1 ; j < row ; ++j )
                if ( a[j][i] != static_cast<T>(0) ) {swap(a[i], a[j]); break ;}
            if ( j == row ) return  static_cast<T>(0) ;
        }
        for ( j = i+1 ; j < row ; ++j ){
            m = a[j][i] / a[i][i] ;
            for ( k = i ; k < col ; ++k )   a[j][k] -= m * a[i][k] ;
        }
    }
    T  prod = static_cast<T>(1) ;
    for ( i = 0 ; i < row ; ++i ) prod *= a[i][i] ;
    return  prod ;
}
//------------------------------------------------------------------------------
template <class T> Matrix<T>  Matrix<T>::Inv() const
{
    assert( row == col ) ;
    vector< vector<T> >  a = mat ;
    vector< vector<T> >  b(row, vector<T>(col,static_cast<T>(0)))  ;
    int  i , j , k ;
    for ( i = 0 ; i < row ; ++i ) b[i][i] = static_cast<T>(1) ;
    T   m ;
    for ( i = 0 ; i < row-1 ; ++i ) {
        if ( a[i][i] == static_cast<T>(0) ) {
            for ( j = i+1 ; j < row ; ++j ) {
                if ( a[j][i] != static_cast<T>(0) ) {
                    swap( a[i] , a[j] ) ;
                    swap( b[i] , b[j] ) ;
                    break ;
                }
            }
            assert( j < row ) ;
        }
        for ( j = i+1 ; j < row ; ++j ) {
            m = a[j][i] / a[i][i] ;
            for ( k = i ; k < col ; ++k ) a[j][k] -= m * a[i][k] ;
            for ( k = 0 ; k < col ; ++k ) b[j][k] -= m * b[i][k] ;
        }
    }
    for ( i = row-1 ; i >= 0 ; --i ) {
        for ( j = i-1 ; j >= 0 ; --j ) {
            m = a[j][i] / a[i][i] ;
            for ( k = i ; k >= j ; --k )  a[j][k] -= m * a[i][k] ;
            for ( k = 0 ; k < col ; ++k ) b[j][k] -= m * b[i][k] ;
        }
    }
    for ( i = 0 ; i < row ; ++i )
        for ( j = 0 ; j < col ; ++j ) b[i][j] /= a[i][i] ;
    return  Matrix<T>(b) ;
}
//------------------------------------------------------------------------------
template <class T> Matrix<T>  Matrix<T>::Trans() const
{
    vector< vector<T> >  b(col, vector<T>(row,static_cast<T>(0)))  ;
    int i,j ;
    for ( i = 0 ; i < row ; ++i )
        for ( j = 0 ; j < col ; ++j ) b[j][i] = a[i][i] ;
    return  Matrix<T>(b) ;
}
//------------------------------------------------------------------------------
template <class T> void __fastcall Matrix<T>::Resize(int NewRows,int NewCols, const T& Val)
{
    int i,j,id;
    row = NewRows;
    col = NewCols;
    mat.clear();
    for(i=0;i<row;i++)
    {
        mat.push_back(vector<T>(0));
        for(j=0;j<col;j++) mat[i].push_back(Val);
    }
}
//------------------------------------------------------------------------------
template <class T> Matrix<T>&   Matrix<T> ::operator = ( const Matrix<T>& rhs )
{
    row = rhs.row;
    col = rhs.col;
    int  i , j ;
    mat = vector< vector<T> >(row,vector<T>(col));
    for ( i = 0 ; i < row ; ++i )
        for ( j = 0 ; j < col ; ++j ) mat[i][j] = rhs[i][j] ;
    return  *this ;
}
//------------------------------------------------------------------------------
template <class T> Matrix<T>&   Matrix<T> ::operator += ( const Matrix<T>& rhs )
{
    int  i , j ;
    for ( i = 0 ; i < row ; ++i )
        for ( j = 0 ; j < col ; ++j ) mat[i][j] += rhs[i][j] ;
    return  *this ;
}
//------------------------------------------------------------------------------
template <class T> Matrix<T>&   Matrix<T>::operator -= ( const Matrix<T>& rhs )
{
    int  i , j ;
    for ( i = 0 ; i < row ; ++i )
        for ( j = 0 ; j < col ; ++j ) mat[i][j] -= rhs[i][j] ;
    return  *this ;
}

//------------------------------------------------------------------------------
// 定義矩陣的輸出運算子
template <class T> ostream&  operator << ( ostream& out , const Matrix<T>& m )
{

    int  i , j ;
    for ( i = 0 ; i < m.rows() ; ++i ) {
        out << '\n' ;
        for ( j = 0 ; j < m.cols() ; ++j ) {
            out << ( abs(m[i][j]) < SMALL ? static_cast<T>(0) : m[i][j] ) << "  " ;
        }
    }
    return  out << '\n' ;
}
//------------------------------------------------------------------------------
template  <class T> bool IsMulMats( const Matrix<T>& m1 ,   const Matrix<T>& m2 )
{
    return(m1.Cols==m2.Rows);
}
//------------------------------------------------------------------------------
template  <class T> bool IsDivMats( const Matrix<T>& m1 ,   const Matrix<T>& m2 )
{
    return((m1.Cols==m2.Rows)&&(m2.Cols==m2.Rows));
}
//------------------------------------------------------------------------------
// 重載矩陣乘法運算子
template  <class T> Matrix<T>  operator * ( const Matrix<T>& m1 ,   const Matrix<T>& m2 )
{

    T          sum ;
    int        i , j , k ;
    Matrix<T>  m(m1.Rows,m2.Cols) ;
    if(!IsMulMats(m1,m2))return(m);
    for ( i = 0 ; i < m1.Rows ; ++i ) {
        for ( j = 0 ; j < m2.Cols ; ++j ) {
            sum = 0 ;
            for ( k = 0 ; k < m1.Cols ; ++k )   sum += m1[i][k] * m2[k][j] ;
            m[i][j] = sum ;
        }
    }
    return(m) ;
}
//------------------------------------------------------------------------------
template <class T> vector<T> operator * (const vector<double>& vec, const Matrix<T>& m)
{
    vector<T> nv(0);
    T val;
    if(vec.size()!=(unsigned) m.Rows) return(nv);
    for(int i=0;i<m.Cols;++i){
        val=0;
        for(uint j=0;j<vec.size();++j) val += vec[j]*m[j][i];
        nv.push_back(val);
    }
    return(nv);
}
//------------------------------------------------------------------------------
template <class T> vector<T> operator * (const Matrix<T>& m, const vector<double>& vec)
{
    vector<T> nv(0);
    T val;          (signed)
    if(vec.size()!=(unsigned) m.Cols) return(nv);
    for(int i=0;i<m.Rows;++i){
        val=0;
        for(uint j=0;j<vec.size();++j) val += m[i][j]*vec[j];
        nv.push_back(val);
    }
    return(nv);
}
//------------------------------------------------------------------------------
// 重載矩陣除法運算子
template  <class T> Matrix<T>  operator / ( const Matrix<T>& m1 ,   const Matrix<T>& m2 )
{

    T          sum ;
    int        i , j , k ;
    Matrix<T>  m(m1.Rows,m2.cols()) ;
    if(IsDivMats(m1,m2))return(m);
    return(m1*m2.inverse());
}
//------------------------------------------------------------------------------
template <class T> bool   operator==( const Matrix<T>& m1 , const Matrix<T>& m2 )
{
    if ( m1.rows() != m2.rows() || m1.cols() != m2.cols() ) return false ;
    int i , j ;
    for ( i = 0 ; i < m1.rows() ; ++i )
        for ( j = 0 ; j < m1.cols() ; ++j )
            if ( m1[i][j] != m2[i][j] ) return false ;
    return true ;
}
//------------------------------------------------------------------------------
template <class T> bool   operator!=( const Matrix<T>& m1 , const Matrix<T>& m2 )
{
    return ! ( m1 == m2 ) ;
}
//------------------------------------------------------------------------------
// 重載矩陣加法運算子
template  <class T> Matrix<T>  operator + (const Matrix<T>& m1 ,   const Matrix<T>& m2 )
{
    Matrix<T>  m = m1 ;
    return  m += m2 ;
}
//------------------------------------------------------------------------------
// 重載矩陣減法運算子
template  <class T> Matrix<T>  operator - ( const Matrix<T>& m1 ,const Matrix<T>& m2 )
{
    Matrix<T>  m = m1 ;
    return  m -= m2 ;
}
//------------------------------------------------------------------------------
#endif
