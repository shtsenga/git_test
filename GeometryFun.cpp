//---------------------------------------------------------------------------
#include <math.h>

#pragma hdrstop

#include "GeometryFun.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)
//--------------------------------------------------------------------------------------
void CorrectRect( TRect& R)
{
    if( R.Left > R.Right ) swap( R.Left, R.Right );
    if( R.Top > R.Bottom ) swap( R.Top , R.Right );
}
//------------------------------------------------------------------------------
void ValidRect( TRect& R)
{
    if( R.Left > R.Right ) swap( R.Left, R.Right );
    if( R.Top > R.Bottom ) swap( R.Top , R.Right );
}
//------------------------------------------------------------------------------
TRect operator+(TRect R, TPoint P)
{
    TRect rec = TRect( R.Left + P.x, R.Top + P.y, R.Right + P.x, R.Bottom + P.y);
    return( rec);
}
//------------------------------------------------------------------------------
TRect operator-(TRect R, TPoint P)
{
    TRect rec = TRect( R.Left - P.x, R.Top - P.y, R.Right - P.x, R.Bottom - P.y);
    return( rec);
}
//------------------------------------------------------------------------------
TRect operator*(TRect Rect, TPoint P)
{
    int cx, cy, rx,ry;
    cx = (Rect.Left + Rect.Right)/2;
    cy = (Rect.Top + Rect.Bottom)/2;
    rx = (Rect.Right - Rect.Left)* P.x/2;
    ry = (Rect.Bottom -Rect.Top)*P.y/2;
    return(TRect(cx-rx, cy-ry, cx+rx, cy+ry));
}
//------------------------------------------------------------------------------
TRect operator/(TRect Rect, TPoint P)
{
    int cx, cy, rx,ry;
    cx = (Rect.Left + Rect.Right)/2;
    cy = (Rect.Top + Rect.Bottom)/2;
    if( P.x == 0 ) P.x =1 ;
    rx = (Rect.Right - Rect.Left)/P.x/2;
    if( P.y==0) P.y = 1;
    ry = (Rect.Bottom -Rect.Top)/P.y/2;
    return(TRect(cx-rx, cy-ry, cx+rx, cy+ry));
}

//------------------------------------------------------------------------------
TPoint operator+(TPoint P1, TPoint P2)
{
    TPoint P(P1.x + P2.x, P1.y + P2.y);
    return( P );
}
//------------------------------------------------------------------------------
TPoint operator-(TPoint P1, TPoint P2)
{
    TPoint P(P1.x - P2.x, P1.y - P2.y);
    return( P );
}
//------------------------------------------------------------------------------
void MoveInside(TRect R, TPoint& P)
{
    if( P.x < R.Left ) P.x = R.Left;
    if( P.x > R.Right) P.x = R.Right;
    if( P.y < R.Top ) P.y = R.Top;
    if( P.y > R.Bottom ) P.y = R.Bottom;
}
//------------------------------------------------------------------------------
double Length(const TPoint& P1, const TPoint& P2)
{
    double r;
    TPoint p = P1-P2;
    return(sqrt(p.x*p.x + p.y*p.y));
}
//------------------------------------------------------------------------------
double Length(const double& x1, const double& y1,const double& x2,const double& y2)
{
    return( sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)) );
}
//------------------------------------------------------------------------------
// Calculate Transform matrix for coordinate transform from O to O'
// The new coordinate system rotate Angle with Axis reletive to original coordinate system
// and O'= O x RotationMatrix
Matrix<DP> __fastcall RotationMatrix(BYTE Axis, DP Angle)
{
    Matrix<DP> m(3,3);
    double c,s, theta;
    theta= PI*Angle/180.0;
    c= cos(theta);
    s= sin(theta);
    if(Axis==0){        //Rotation Axis X
        m[0][0]=1;  m[0][1]=0;  m[0][2]=0;
        m[1][0]=0;  m[1][1]=c;  m[1][2]=-s;
        m[2][0]=0;  m[2][1]=s;  m[2][2]=c;
    }else if(Axis==1){  // Rotaion Axis Y
        m[0][0]=c;  m[0][1]=0;  m[0][2]=s;
        m[1][0]=0;  m[1][1]=1;  m[1][2]=0;
        m[2][0]=-s; m[2][1]=0;  m[2][2]=c;
    }else{
        m[0][0]=c;  m[0][1]=-s; m[0][2]=0;
        m[1][0]=s;  m[1][1]=c;  m[1][2]=0;
        m[2][0]=0;  m[2][1]=0;  m[2][2]=1;
    }
    return(m);
}
//------------------------------------------------------------------------------
Matrix<DP> __fastcall EulerRotationMatrix(BYTE* Axis, DP* Angle, int N)
{
    Matrix<DP> m(3,3,0);
    if(N==0) return(RotationMatrix(0,0));
    m= RotationMatrix(Axis[0],Angle[0]);
    for(int i=1; i< N; ++i) m= m*RotationMatrix(Axis[i],Angle[i]);
    return(m);
}
//------------------------------------------------------------------------------
Matrix<DP> __fastcall EulerRotationMatrix(vector<BYTE>&  Axis, dvec& Angle)
{
    Matrix<DP> m(3,3,0);
    if(Axis.size()==0) return(RotationMatrix(0,0));
    m= RotationMatrix(Axis[0],Angle[0]);
    for(uint i=1; i< Angle.size(); ++i) m= m*RotationMatrix(Axis[i],Angle[i]);
    return(m);
}
//------------------------------------------------------------------------------

