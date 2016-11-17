//---------------------------------------------------------------------------
#include <Math.hpp>

#pragma hdrstop

#include "FrameDataUnit.h"
#include "GeometryFun.h"
#include "Matrix.h"
#include "IOFun.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

vector<TImageFrame*> SimFrameList;
vector<TImageFrame*> ProjFrameList;
double ObservRadius=1;

//------------------------------------------------------------------------------
bool __fastcall TBall::SaveToStream(TStream* Stream)
{
    int   site, length;
    double val;
    site = Stream->Position;
    try{
        for(int i=0;i<3; ++i){
            val= Pos[i];
            Stream->Write(&val, sizeof(double));
        }
        Stream->Write(&Radius, sizeof(double));
        Stream->Write(&Color, sizeof(TColor));
        return( true);
    } catch ( ... ){
        Stream->Position = site;
        return ( false );
    }
}
//------------------------------------------------------------------------------
double __fastcall TBall::FieldRadius()
{
    return(sqrt(Pos[0]*Pos[0]+Pos[1]*Pos[1]+Pos[2]*Pos[2])+Radius);
}
//------------------------------------------------------------------------------
bool __fastcall TBall::LoadFromStream(TStream* Stream)
{
    int   site;
    double val;
    String class_name;
    site = Stream->Position;
    try {
        for(int i=0;i<3; ++i){
            Stream->Read(&val, sizeof(double));
            Pos[i]=  val;
        }
        Stream->Read(&Radius, sizeof(double));
        Stream->Read(&Color, sizeof(TColor));
        return( true);
    } catch ( ... ){
        Stream->Position = site;
        return ( false );
    }
}
//------------------------------------------------------------------------------
bool __fastcall TImageFrame::SaveToStream(TStream* Stream)
{
    int   site[2], size[2], length, count;
    String class_name("TImageFrame");

    site[0] = Stream->Position;
    size[0] = Stream->Size;
    try{
        Stream->Write(&length, sizeof(int));
        SaveString(Stream,class_name);
        Stream->Write(&RunStep, sizeof(int));
        Stream->Write(&ActiveNum, sizeof(int));
        count= Cluster.size();
        Stream->Write(&count, sizeof(int));
        for(int i=0;i<count; ++i) Cluster[i].SaveToStream(Stream);
        site[1] = Stream->Position;
        size[1] = site[1] - site[0];
        Stream->Position = site[0];
        Stream->Write( &(size[1]), sizeof(int) );
        Stream->Position = site[1];
        return( true);
    } catch ( ... ){
        Stream->Position = site[0];
        Stream->Size = size[0];
        return ( false );
    }
}
//------------------------------------------------------------------------------
bool __fastcall TImageFrame::LoadFromStream(TStream* Stream)
{
    int   site[2], size[2], length, count;
    String class_name("TImageFrame");
    TBall b;
    site[0] = Stream->Position;
    size[0] = Stream->Size;
    try{
        Stream->Read(&(size[1]), sizeof(int));
        LoadString(Stream,class_name);
        if(class_name!="TImageFrame") throw(-1);
        Stream->Read(&RunStep, sizeof(int));
        Stream->Read(&ActiveNum, sizeof(int));
        Stream->Read(&count, sizeof(int));
        Cluster.clear();
        for(int i=0;i<count; ++i) {
            b.LoadFromStream(Stream);
            Cluster.push_back(b);
        }
        Stream->Position = site[0] + size[1];
        return( true);
    } catch ( ... ){
        Stream->Position = site[0];
        return ( false );
    }
}
//------------------------------------------------------------------------------
void __fastcall TImageFrame::Assign(TImageFrame* SrcFrame)
{
    RunStep= SrcFrame->RunStep;
    ActiveNum= SrcFrame->ActiveNum;
    Cluster= SrcFrame->Cluster;
}
//------------------------------------------------------------------------------
bool __fastcall SaveFrameList(String FileName, int Phi, int Theta)
{
    TFileStream* stream;
    int   site[2], size[2], length, count;
    String class_name="FrameList";
    TImageFrame* frm, *pfrm;
    try{
        stream= new TFileStream(FileName,fmCreate);
        SaveString(stream,class_name);
        count= SimFrameList.size();
        stream->Write(&count, sizeof(int));
        for(int i=0;i<count; ++i)
            SimFrameList[i]->SaveToStream(stream);
        stream->Write(&Phi, sizeof(int));
        stream->Write(&Theta, sizeof(int));
        delete stream;
        return( true);
    } catch ( ... ){
        return ( false );
    }
}
//------------------------------------------------------------------------------
bool __fastcall LoadFrameList(String FileName, int& Phi, int& Theta, int& FrameNum)
{
    TFileStream* stream;
    int   site[2], size[2], length;
    String class_name;
    TImageFrame* frm, *pfrm;
    try{
        stream= new TFileStream(FileName,fmOpenRead);
        LoadString(stream,class_name);
        if(class_name!="FrameList") throw(-1);
        stream->Read(&FrameNum, sizeof(int));
        ClearFrameList();
        for(int i=0;i<FrameNum; ++i) {
            frm= new TImageFrame;
            pfrm= new TImageFrame;
            frm->LoadFromStream(stream);
            pfrm->Assign(frm);
            SimFrameList.push_back(frm);
            ProjFrameList.push_back(pfrm);
        }
        stream->Read(&Phi, sizeof(int));
        stream->Read(&Theta, sizeof(int));
        ChangeObSite(Phi, Theta);
        delete stream;
        return( true);
    } catch ( ... ){
        return ( false );
    }
}
//------------------------------------------------------------------------------
void __fastcall ResetProjFrameList()
{
    TImageFrame* frame;
    for(unsigned i=0;i<ProjFrameList.size();++i) delete ProjFrameList[i];
    for(unsigned i=0;i<SimFrameList.size();++i){
        frame= new TImageFrame;
        frame->RunStep= SimFrameList[i]->RunStep;
        frame->ActiveNum= SimFrameList[i]->ActiveNum;
        frame->Cluster= SimFrameList[i]->Cluster;
        ProjFrameList.push_back(frame);
    }
}
//------------------------------------------------------------------------------
void __fastcall UpdateProjectFrameList(double* InspectDir)
{
    double a[2],l;
    BYTE axis[]={2,1};
    Matrix<DP> m(3,3);
    TCluster* lst;
    if(InspectDir[0]!=0.0) a[0]= 180.0*ArcTan2(InspectDir[1],InspectDir[0])/PI;
    else a[0]= (InspectDir[1]==0.0)?0.0:((InspectDir[1]>0)?90.0:-90.0);
    l= sqrt(InspectDir[0]*InspectDir[0]+InspectDir[1]*InspectDir[1]);
    if(l!=0) a[1]= 180.0*ArcTan2(InspectDir[2],l)/PI;
    else a[1]=(InspectDir[2]==0.0)?0.0:((InspectDir[2]>0)?90.0:-90.0);
    m=EulerRotationMatrix(axis,a,2);
    if(ProjFrameList.size()!=SimFrameList.size()) ResetProjFrameList();
    for(uint i=0; i<SimFrameList.size(); ++i){
        ProjFrameList[i]->RunStep= SimFrameList[i]->RunStep;
        ProjFrameList[i]->ActiveNum= SimFrameList[i]->ActiveNum;
        lst= &(ProjFrameList[i]->Cluster);
        for(uint j=0;j<lst->size(); ++j){
            (*lst)[j].Pos= SimFrameList[i]->Cluster[j].Pos*m;
            (*lst)[j].Radius= SimFrameList[i]->Cluster[j].Radius;
            (*lst)[j].Color= SimFrameList[i]->Cluster[j].Color;
        }
        for(uint j=0; j<lst->size()-1; ++j)
            for(uint k=j; k<lst->size(); ++k)
                if((*lst)[j].Pos[0] > (*lst)[k].Pos[0]) swap((*lst)[j], (*lst)[k]);
    }
}
//------------------------------------------------------------------------------
void __fastcall ChangeObSite(double Phi, double Theta)
{
    Matrix<double> m(3,3);
    BYTE ax[2]={2,1};
    double angle[2]={Phi,-Theta};
    TCluster* lst;
    m= EulerRotationMatrix(ax,angle,2);
    if(ProjFrameList.size()!=SimFrameList.size()) ResetProjFrameList();
    for(uint i=0; i<SimFrameList.size(); ++i){
        ProjFrameList[i]->RunStep= SimFrameList[i]->RunStep;
        ProjFrameList[i]->ActiveNum= SimFrameList[i]->ActiveNum;
        lst= &(ProjFrameList[i]->Cluster);
        for(uint j=0;j<lst->size(); ++j){
            (*lst)[j].Pos= SimFrameList[i]->Cluster[j].Pos*m;
            (*lst)[j].Radius= SimFrameList[i]->Cluster[j].Radius;
            (*lst)[j].Color= SimFrameList[i]->Cluster[j].Color;
        }
        for(uint j=0; j<lst->size()-1; ++j)
            for(uint k=j; k<lst->size(); ++k)
                if((*lst)[j].Pos[0] > (*lst)[k].Pos[0]) swap((*lst)[j], (*lst)[k]);
    }
}
//------------------------------------------------------------------------------
void __fastcall GetObFrame(double Phi, double Theta, int Frame, TImageFrame& ObFrame)
{
    Matrix<double> m(3,3);
    BYTE ax[2]={2,1};
    double angle[2]={Phi,-Theta};
    TCluster* lst;
    if(Frame>=(signed)SimFrameList.size()) return;
    m= EulerRotationMatrix(ax,angle,2);
    ObFrame.RunStep= SimFrameList[Frame]->RunStep;
    ObFrame.ActiveNum= SimFrameList[Frame]->ActiveNum;
    lst= &(ObFrame.Cluster);
    if(lst->size()!= SimFrameList[Frame]->Cluster.size())
        *lst= SimFrameList[Frame]->Cluster;
    for(uint j=0;j<lst->size(); ++j){
        (*lst)[j].Pos= SimFrameList[Frame]->Cluster[j].Pos*m;
        (*lst)[j].Radius= SimFrameList[Frame]->Cluster[j].Radius;
        (*lst)[j].Color= SimFrameList[Frame]->Cluster[j].Color;
    }
    for(uint j=0; j<lst->size()-1; ++j)
        for(uint k=j; k<lst->size(); ++k)
            if((*lst)[j].Pos[0] > (*lst)[k].Pos[0]) swap((*lst)[j], (*lst)[k]);
}
//------------------------------------------------------------------------------
void __fastcall ClearFrameList()
{
    for(unsigned i=0;i<SimFrameList.size();++i) delete SimFrameList[i];
    for(unsigned i=0;i<ProjFrameList.size();++i) delete ProjFrameList[i];
    SimFrameList.clear();
    ProjFrameList.clear();
}
//------------------------------------------------------------------------------
double __fastcall CalculateObservRadius()
{
    double r, rng,max_r(0);
    for(unsigned i=0;i<SimFrameList.size();++i){
        rng=0.0;
        for(unsigned j=0; j<SimFrameList[i]->Cluster.size();++j){
            r= SimFrameList[i]->Cluster[j].FieldRadius();
            if(rng<r) rng= r;
        }
        if(max_r<rng) max_r= rng;
    }
    return(max_r);
}
//------------------------------------------------------------------------------
void __fastcall GetAxisProj(double Theta, double Phi, dvec& Proj)
{
    Matrix<double> m(3,3);
    BYTE ax[2]={2,1};
    vector<double> null_v(3,0.0);
    vector<double> uv(3,0);
    double angle[2]={Phi,-Theta};
    m= EulerRotationMatrix(ax,angle,2);
    Proj.clear();
    for(int i=0;i<3;++i){
        uv= null_v;
        uv[i]=1.0;
        uv= uv*m;
        Proj.push_back(uv[1]);
        Proj.push_back(uv[2]);
    }
}
