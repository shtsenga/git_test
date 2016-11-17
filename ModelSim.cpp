//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "ModelSim.h"
#include "FrameDataUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
TSimForm *SimForm;
// Generate random numbers
#define MBIG 1000000000
#define MSEED 161803398
#define MZ 0
#define FAC (1.0/MBIG)
double ran3(long *idum)
{
    static int inext,inextp;
    static long ma[56];
    static int iff=0;
    long mj,mk;
    int i,ii,k;
    if (*idum < 0 || iff == 0) {
        iff= 1;
        mj= MSEED-(*idum < 0 ? -*idum : *idum);
        mj %= MBIG;
        ma[55]=mj;
        mk=1;
        for (i=1;i<=54;i++) {
            ii=(21*i) % 55;
            ma[ii]=mk;
            mk=mj-mk;
            if (mk < MZ) mk += MBIG;
            mj=ma[ii];
        }
        for (k=1;k<=4;k++)
            for (i=1;i<=55;i++) {
                ma[i] -= ma[1+(i+30) % 55];
                if (ma[i] < MZ) ma[i] += MBIG;
            }
        inext=0;
        inextp=31;
        *idum=1;
    }
    if (++inext == 56) inext=1;
    if (++inextp == 56) inextp=1;
    mj=ma[inext]-ma[inextp];
    if (mj < MZ) mj += MBIG;
    ma[inext]=mj;
    return mj*FAC;
}
#undef MBIG
#undef MSEED
#undef MZ
#undef FAC

//---------------------------------------------------------------------------
__fastcall TSimForm::TSimForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
// definition
// particle including position, radius, color information
// frame is a set of particles with time information (here means nth/100 of simulation)
void __fastcall TSimForm::bRunClick(TObject *Sender)
{
    int i,j,k,jj,n,nn,dd, lst_ln;
    int direction,rest, nz, syn, collect_stps;
	long int jjj;
    double **pos, *r, *rr, *active;
    double interval,d,ddd;
    double dvar,*pdvar, dp[3];
	double boundary, hboundary, a, v, t, temp, log_temp, distance;
	double step, diffusion, zk;
    TImageFrame* frame;

	nz  = leParNum->Text.ToInt();
    a   = leParR->Text.ToDouble();
	v   = leVis->Text.ToDouble();
	temp= leTemp->Text.ToDouble();
    dd  = leInitBoundary->Text.ToDouble();
    log_temp= log(temp-273.0);
	syn = leRunStps->Text.ToInt();
    collect_stps= leCollectStps->Text.ToInt();
    mMemo->Lines->Clear();
    //n= 0;
    d= 1.0*dd;
    ddd= 0.0;
    mMemo->Lines->Add("Mean Particle distance= "+FloatToStr(d));
    boundary=d*(pow(nz,1.0/3.0)+1);
    hboundary= boundary/2.0;
    mMemo->Lines->Add("Boundary= " + FloatToStr(boundary));

    //create initial particle position and 1st frame
    ClearFrameList();
    frame= new TImageFrame();
    frame->Cluster.clear();
    frame->RunStep=1;
    pos= new double* [nz];
    active= new double[nz];
    r= new double[nz];
    rr=new double[nz];
    for(i=0; i< nz; ++i){
        pos[i]= new double[3];
        jjj=1000;
        for(j=0;j<3;++j){
            dvar= ran3(&jjj);
            pos[i][j]=boundary*(dvar-0.5);
        }
        r[i]= 1.74*a;
        active[i]= 1.0;
        rr[i]= 1.0;
        frame->Cluster.push_back(TBall(pos[i][0],pos[i][1],pos[i][2],r[i],clBlue));
    }
    SimFrameList.push_back(frame);
    try{
    for (jj=1; jj<syn; ++jj){//begin simulation
        if ((jj+1)%collect_stps==0){
        mMemo->Lines->Add("Step: "+IntToStr(jj+1));
        frame= new TImageFrame;
        frame->RunStep= jj+1;
        k=0;
        frame->Cluster.clear();
        for (i=0; i<nz; ++i){
            if (r[i]>=0){
                frame->Cluster.push_back(TBall(pos[i][0],pos[i][1],pos[i][2],r[i],active[i]==1.0?clRed:clBlue));
                if(active[i]==1.0) k++;
            }
        }
        frame->ActiveNum=k;
        SimFrameList.push_back(frame);
        };
        for( i = 0; i< nz; ++i){   //calculate next interaction step
            if (r[i]>0){
                diffusion = 1.38*temp/(6*M_PI*v*r[i]);
                step= sqrt(2*diffusion);
                jjj=6;
                direction= (int)(ran3(&jjj)*6);
                pdvar= &(pos[i][direction/2]);
                if((direction%2)==0){
                    *pdvar -= step;
                    if(*pdvar<-hboundary) *pdvar+= hboundary;
                }else{
                    *pdvar += step;
                    if(*pdvar>hboundary) *pdvar -= hboundary;
                }
            }
        }
        for( i= 0; i<(nz-1); ++i){
            if (r[i]<0.0) goto next_i;
            for( j= i+1; j<nz; ++j){
                if(r[j]<0.0) goto next_j;
                distance= boundary-4.0;
                for(k=0; k<3; ++k){
                    dp[k]= pos[i][k] - pos[j][k];
                    if (dp[k]>distance) dp[k]= boundary-dp[k];
                }
                interval= sqrt(dp[0]*dp[0] + dp[1]*dp[1] + dp[2]*dp[2]);
                ddd= r[i] + r[j];
                if ((interval <= ddd) && (active[i]>=0.001) && (active[j]>=0.11)){
                    dvar= r[i]*r[i]*r[i] + r[j]*r[j]*r[j];
                    r[i]= pow(dvar,1.0/3.0);
                    r[j]= -1.0;
                    active[i]= 1.0;
                    rr[i]= -1.0;
                    //n= n+1;
                } else{
                    if (rr[j]<0.0) goto next_j;
                    dvar= -0.01*((double)jj+1.0);
                    r[j]= a*(1+exp(dvar)/log_temp);
                    dvar= -1.0*((double)jj/7000.00);
                    active[j]= exp(dvar)/log_temp;
                };
next_j:     };
next_i: };
    }
    for(i=0, rest=0; i<nz; i++) if (r[i]>0) rest++;
    mMemo->Lines->Add("The rest particle = "+IntToStr(rest));
    for(i=0;i<nz;++i) delete[] pos[i];
    delete[] pos, active, r, rr;
    ObservRadius= CalculateObservRadius();
    }catch(...){
    };
}
//---------------------------------------------------------------------------
