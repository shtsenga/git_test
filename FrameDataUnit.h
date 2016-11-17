//---------------------------------------------------------------------------

#ifndef FrameDataUnitH
#define FrameDataUnitH
#include <vector.h>
#include <vcl.h>
//---------------------------------------------------------------------------
#define dvec vector<double>
class TBall
{
    public:
        dvec Pos;
        double  Radius;
        TColor  Color;
        __fastcall TBall(double X, double Y, double Z, double BallRadius, TColor BallColor){
            Pos=dvec(3); Pos[0]=X; Pos[1]=Y; Pos[2]=Z; Radius= BallRadius;   Color= BallColor;
        };
        __fastcall TBall(){Pos=dvec(3); Pos[0]=Pos[1]=Pos[2]=0.0; Radius= 0;   Color= clWhite; };
        bool __fastcall SaveToStream(TStream* Stream);
        bool __fastcall LoadFromStream(TStream* Stream);
        double __fastcall FieldRadius();
};
//---------------------------------------------------------------------------
typedef vector<TBall> TCluster;
//---------------------------------------------------------------------------
class TImageFrame
{
    public:
        int RunStep;
        TCluster Cluster;
        int ActiveNum;
        __fastcall TImageFrame(){
            RunStep= 0;    ActiveNum=0; Cluster.clear();
        };
        __fastcall TImageFrame(double Step, int ActiveNumber, TCluster NewCluster){
            RunStep= Step;    ActiveNum= ActiveNumber;  Cluster= NewCluster;
        };
        bool __fastcall SaveToStream(TStream* Stream);
        bool __fastcall LoadFromStream(TStream* Stream);
        void __fastcall Assign(TImageFrame* SrcFrame);
};
//---------------------------------------------------------------------------
extern vector<TImageFrame*> SimFrameList;
extern vector<TImageFrame*> ProjFrameList;
extern double ObservRadius;
bool __fastcall SaveFrameList(String FileName, int Phi, int Theta);
bool __fastcall LoadFrameList(String FileName, int& Phi, int& Theta, int& FrameNum);
void __fastcall ResetProjFrameList();
void __fastcall UpdateProjectFrameList(double* InspectDir);
void __fastcall ChangeObSite(double Phi, double Theta);
void __fastcall GetObFrame(double Phi, double Theta, int Frame, TImageFrame& ObFrame);
void __fastcall ClearFrameList();
double __fastcall CalculateObservRadius();
void __fastcall GetAxisProj(double Theta, double Phi, dvec& Proj);
#endif
