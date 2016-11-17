//---------------------------------------------------------------------------

#include <vcl.h>
#include <vfw.h>
#include <Math.hpp>
#pragma hdrstop

#include "MainUnit.h"
#include "Matrix.h"
#include "GeometryFun.h"
#include "StringParseFun.h"
#include "FrameDataUnit.h"
#include "avi_utils.h"
#include "ModelSim.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
    CurrentObFrame.Cluster.clear();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::miLoadRawClick(TObject *Sender)
{
    int current_frame(-1),frame;
    TImageFrame *sim_frame(0), *proj_frame(0);
    TBall b;
    TStringList* lst;
    String l;
    vector<String> slst;
    odDialog->FilterIndex=1;
    if(!odDialog->Execute())return;
    lst->LoadFromFile(odDialog->FileName);
    //begin parse list data
    try{
        ClearFrameList();
        for(int i=0; i<lst->Count; ++i){
            ParseSep(l,',',slst);
            frame= slst[0].ToInt();
            if(frame!=current_frame){
                sim_frame= new TImageFrame;
                proj_frame= new TImageFrame;
                SimFrameList.push_back(sim_frame);
                ProjFrameList.push_back(proj_frame);
                sim_frame->RunStep= frame;
                proj_frame->RunStep= frame;
                current_frame= frame;
            }
            for(int j=0;j<3;++j) b.Pos[j]= slst[j+1].ToDouble();
            b.Radius= slst[4].ToDouble()/2.0;
            b.Color = (TColor)slst[5].ToInt();
            sim_frame->Cluster.push_back(b);
        }
        ChangeObSite((double)sbPhi->Position, (double)sbTheta->Position);
    }catch(...){
        ShowMessage("Invalid input raw data");
    }
    delete lst;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::sbObsChange(TObject *Sender)
{
    TScrollBar* b=(TScrollBar*)Sender;
    if(Sender!=0)   b->Hint= IntToStr(b->Position)+"(deg)";
    lSite->Caption="("+IntToStr(sbPhi->Position)+","+IntToStr(sbTheta->Position)+") deg.";

    //update display
    GetCurrentObFrame();
    ShowFrame(iDisplay->Canvas, iDisplay->Width, iDisplay->Height, CurrentObFrame);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::miChangeObsClick(TObject *Sender)
{
    gbObsParam->Visible= miChangeObs->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::bChangeObsClick(TObject *Sender)
{
    ChangeObSite((double)sbPhi->Position, (double)sbTheta->Position);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::miSaveFileClick(TObject *Sender)
{
    String fn;
    int phi, theta;
    sdDialog->Filter="Simulation Data(*.sim)|*.sim";
    sdDialog->DefaultExt=".sim";
    if(!sdDialog->Execute()) return;
    fn= sdDialog->FileName.UpperCase();
    if(fn.Pos(".sim") <=0){
        if(fn.Pos(".")>0)   fn= fn.SubString(1, fn.LastDelimiter("."))+"sim";
        else  fn+= ".sim";
    }
    if(SimFrameList.size()==0)return;
    SaveFrameList(fn, phi,theta);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::miOpenFileClick(TObject *Sender)
{
    String fn;
    int phi, theta,frames;
    odDialog->FilterIndex=1;
    if(!odDialog->Execute()) return;
    fn= odDialog->FileName.UpperCase();
    if(odDialog->FileName.Pos(".sim") <=0) return;
    if(!LoadFrameList(fn,phi,theta,frames)) return;
    ObservRadius= CalculateObservRadius();
    sbPhi->Position= phi;
    sbTheta->Position= theta;
    sbFramePos->Max=frames;
    sbFramePos->Position=0;
    sbFramePosChange(0);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::bChangeObsRangeClick(TObject *Sender)
{
    double fval;
    String s;
    s= InputBox("Input Observation Range","Visual Radius",FloatToStr(ObservRadius));
    fval = s.Trim().ToDouble();
    if(fval<=0) ObservRadius= CalculateObservRadius();
    else ObservRadius= fval;
}
//------------------------------------------------------------------------------
void __fastcall TMainForm::FormResize(TObject *Sender)
{
    int t,l;
    if(WindowState!= wsMinimized){
        t =iDisplay->Top; l=iDisplay->Left;
        if(iDisplay!=0) delete iDisplay;
        iDisplay = new TImage(this);
        iDisplay->Top=t;
        iDisplay->Left=l;
        iDisplay->Width = ClientWidth-2;
        iDisplay->Height = ClientHeight-27;
        iDisplay->Parent = this;
        iDisplay->Visible = true;
        sbFramePos->Top= iDisplay->Top+iDisplay->Height+1;
        sbFramePos->Width= ClientWidth;
    }
}
//------------------------------------------------------------------------------
void __fastcall TMainForm::GetCurrentObFrame()
{
    double theta, phi;
    int p;
    p       = (double)sbFramePos->Position;
    theta   = (double)sbTheta->Position;
    phi     = sbPhi->Position;
    GetObFrame(phi,theta,p,CurrentObFrame);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ShowFrame(TCanvas* CV, int Width, int Height, TImageFrame& ImageFrame)
{
    double xl, yl;
    double p,d;
    double theta, phi;
    int origin_x, origin_y;
    vector<double> proj;

    String s;
    TRect rect;
    d= 2.0*ObservRadius;
    if(Width > Height){
        xl= d*(DP)iDisplay->Width/(DP)iDisplay->Height;
        yl= d;
    }else{
        xl= d;
        yl= d*(DP)iDisplay->Height/(DP)iDisplay->Width;
    }
    CV->Brush->Color = clWhite;
    CV->Pen->Color = clWhite;
    CV->FillRect(TRect(0,0,iDisplay->Width, iDisplay->Height));
    CV->Pen->Color= clBlack;
    for(uint i=0;i< ImageFrame.Cluster.size();++i){
        p= ImageFrame.Cluster[i].Pos[1]-ImageFrame.Cluster[i].Radius;
        rect.Left= (int)((DP)iDisplay->Width*(0.5+p/xl)+0.5);
        p= ImageFrame.Cluster[i].Pos[1]+ImageFrame.Cluster[i].Radius;
        rect.Right= (int)((DP)iDisplay->Width*(0.5+p/xl)+0.5);
        p= ImageFrame.Cluster[i].Pos[2]-ImageFrame.Cluster[i].Radius;
        rect.Top= (int)((DP)iDisplay->Height*(0.5-p/yl)+0.5);
        p= ImageFrame.Cluster[i].Pos[2]+ImageFrame.Cluster[i].Radius;
        rect.Bottom= (int)((DP)iDisplay->Height*(0.5-p/yl)+0.5);
        CV->Brush->Color= ImageFrame.Cluster[i].Color;
        CV->Ellipse(rect);
    }
    //update current frame information
    CV->Brush->Color=clBlack;
    CV->Font->Color= clWhite;
    CV->Font->Size= 12;
    s= "Step:"+IntToStr(ImageFrame.RunStep)+" Active #:"+IntToStr(ImageFrame.ActiveNum)+
        " NonActive #:"+IntToStr(ImageFrame.Cluster.size()-ImageFrame.ActiveNum);
    CV->TextOut(1,Height-20,s);

    //draw unit axis
    theta   = (double)sbTheta->Position;
    phi     = sbPhi->Position;
    GetAxisProj(theta, phi, proj);
    CV->Pen->Color= clBlack;
    CV->Brush->Color= clWhite;
    CV->Font->Color= clBlack;
    origin_x= 40;
    origin_y= Height-60;
    for(uint i=0; i<3; ++i){
        CV->MoveTo(origin_x,origin_y);
        CV->LineTo(origin_x+proj[2*i]*30,origin_y-proj[2*i+1]*30);
        CV->TextOut(origin_x+proj[2*i]*30,origin_y-proj[2*i+1]*30,(i==0)?"X":((i==1)?"Y":"Z"));
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::sbFramePosChange(TObject *Sender)
{
    sbFramePos->Hint= IntToStr(sbFramePos->Position);
    GetCurrentObFrame();
    if((unsigned)sbFramePos->Position<ProjFrameList.size()){
        ShowFrame(iDisplay->Canvas,iDisplay->Width, iDisplay->Height, *ProjFrameList[sbFramePos->Position]);
        sbFramePos->Hint= IntToStr(sbFramePos->Position);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::AniTimerTimer(TObject *Sender)
{
    sbFramePos->Position= sbFramePos->Position+1;
    if(sbFramePos->Position>= sbFramePos->Max)
        sbFramePos->Position=0;
    if((unsigned)sbFramePos->Position<ProjFrameList.size()){
        ShowFrame(iDisplay->Canvas, iDisplay->Width, iDisplay->Height, *ProjFrameList[sbFramePos->Position]);
        sbFramePos->Hint= IntToStr(sbFramePos->Position);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::miAnimationClick(TObject *Sender)
{
    AniTimer->Enabled= miAnimation->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::miConvAVIClick(TObject *Sender)
{
    String fn("temp.avi");
    int rate(2);
    Graphics::TBitmap *bmp;
    sdDialog->Filter="VideoFile(*.avi)|*.avi";
    sdDialog->DefaultExt=".avi";
    if(!sdDialog->Execute()) return;
    fn= sdDialog->FileName;
    if(fn.LowerCase().Pos(".avi")<=0) fn+=".avi";
    bmp= new Graphics::TBitmap();
    bmp->HandleType= bmDIB;
    bmp->Canvas->Font->Height=16;
    bmp->Width = iDisplay->Width;
    bmp->Height= iDisplay->Height;
    HAVI avi = CreateAvi(fn.c_str(),1000/rate,NULL);
    for (unsigned i=0; i< ProjFrameList.size(); ++i){
        ShowFrame(bmp->Canvas,iDisplay->Width, iDisplay->Height,*(ProjFrameList[i]));
        AddAviFrame(avi,bmp->Handle);
    }
    CloseAvi(avi);
    ShowMessage( "Conversion Finished");
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::miSimulateClick(TObject *Sender)
{
    SimForm->ShowModal();
    ChangeObSite((double)sbPhi->Position, (double)sbTheta->Position);
    sbFramePos->Position= 0;
    sbFramePos->Max= SimFrameList.size();
}
//---------------------------------------------------------------------------

