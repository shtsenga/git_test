//---------------------------------------------------------------------------
#include <vcl.h>
#include <vector.h>
#include <math.h>
#include <vector.h>
#ifndef GeometryFunH
#define GeometryFunH
#include "MyTypeDef.h"
#include "Matrix.h"
//---------------------------------------------------------------------------
void CorrectRect(TRect& R);
void ValidRect(TRect& R);

//Declaration region
TRect operator+(TRect R, TPoint P);
TRect operator-(TRect R, TPoint P);
TRect operator*(TRect R, TPoint P);
TRect operator/(TRect R, TPoint P);
TPoint operator+(TPoint P1, TPoint P2);
TPoint operator-(TPoint P1, TPoint P2);
void MoveInside(TRect R, TPoint& P);
Matrix<DP> __fastcall RotationMatrix(BYTE Axis, DP Angle);
Matrix<DP> __fastcall EulerRotationMatrix(BYTE* Axis, DP* Angle, int N);
Matrix<DP> __fastcall EulerRotationMatrix(vector<BYTE>&  Axis, dvec& Angle);
double Length(const TPoint& P1, const TPoint& P2);
double Length(const double& x1, const double& y1,const double& x2,const double& y2);
template<class T> double Length(vector<T>& X1, vector<T>& X2);
template<class T> double Length(T* X1, T* X2, uint N);
template<class T> bool __fastcall LineProject(vector<T>& Vec, vector<T>& BaseVec, vector<T>& ProjVec);
template<class T> bool __fastcall PlaneProject(vector<T>& Vec, vector<T>& Norm, vector<T>& ProjVec);
template<class T> bool InsidePolygon2D(vector<T>& PX, vector<T>& PY, T X, T Y);




// Check if point (X,Y) is inside a polygon(PX,PY) where PX[n]==PX[0]; PY[n]==PY[0]
// if inside return true
template<class T> bool InsidePolygon2D(vector<T>& PX, vector<T>& PY, T X, T Y)
{
    int i,cx(0), npol;
    double slp;
    npol = (PX.size()>PY.size())?PY.size():PX.size();
    for(i=0;i<npol-1;i++){
        if(PX[i+1]!=PX[i]){
            slp = (PY[i+1]-PY[i])/(PX[i+1]-PX[i]);
            if( ((((PY[i]-Y)*(PY[i+1]-Y))<0) || ((PY[i+1]-Y)==0.0)) && (((Y-PY[i])/slp+PX[i])>=X)) cx++;
        }else{
            if( ((((PY[i]-Y)*(PY[i+1]-Y))<0) || ((PY[i+1]-Y)==0.0)) && (PX[i]>=X)) cx++;
        }
    }
    return((cx%2)==1);
}
//------------------------------------------------------------------------------
template<class T> double Length(vector<T>& X1, vector<T>& X2)
{
    double l(0.0), val;
    if(X1.size()!=X2.size()) return(0);
    for(uint i=0;i<X1.size(); ++i){
        val= X1[i]-X2[i];
        l+= val*val;
    }
    return(sqrt(l));
}
//------------------------------------------------------------------------------
template<class T> double Length(T* X1, T* X2, uint N)
{
    double l(0.0), val;
    for(uint i=0;i<N; ++i){
        val= X1[i]-X2[i];
        l+= val*val;
    }
    return(sqrt(l));
}
//------------------------------------------------------------------------------
template<class T> bool __fastcall LineProject(vector<T>& Vec, vector<T>& BaseVec, vector<T>& ProjVec)
{
    double scale(0.0),bl(0.0);
    for(uint i=0;i<Vec.size();++i){
        scale= Vec[i]*BaseVec[i];
        bl= BaseVec[i]*BaseVec[i];
    }
    if(bl==0.0) return(false);
    scale= scale/bl;
    for(uint i=0; i<Vec.size;++i) ProjVec[i]= (T)(scale*(DP)BaseVec[i]);
    return(true);
}
//------------------------------------------------------------------------------
template<class T> bool __fastcall PlaneProject(vector<T>& Vec, vector<T>& Norm, vector<T>& ProjVec)
{
    if(!LineProject(Vec,Norm,ProjVec))return(false);
    for(uint i=0; i< Vec.size(); ++i) ProjVec[i]= Vec[i]- ProjVec[i];
    return(true);
}

#endif
