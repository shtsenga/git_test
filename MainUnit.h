//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "FrameDataUnit.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
    TImage *iDisplay;
    TPopupMenu *pmMenu;
    TMenuItem *miLoadRaw;
    TOpenDialog *odDialog;
    TGroupBox *gbObsParam;
    TScrollBar *sbPhi;
    TLabel *Label2;
    TScrollBar *sbTheta;
    TLabel *Label3;
    TButton *bChangeObs;
    TMenuItem *miChangeObs;
    TLabel *Label4;
    TLabel *lSite;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *miSaveFile;
    TMenuItem *miOpenFile;
    TMenuItem *miConvAVI;
    TSaveDialog *sdDialog;
    TMenuItem *bChangeObsRange;
    TScrollBar *sbFramePos;
    TTimer *AniTimer;
    TMenuItem *N3;
    TMenuItem *miAnimation;
    TMenuItem *miSimulate;
    TMenuItem *N4;
    void __fastcall miLoadRawClick(TObject *Sender);
    void __fastcall sbObsChange(TObject *Sender);
    void __fastcall miChangeObsClick(TObject *Sender);
    void __fastcall bChangeObsClick(TObject *Sender);
    void __fastcall miSaveFileClick(TObject *Sender);
    void __fastcall miOpenFileClick(TObject *Sender);
    void __fastcall bChangeObsRangeClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall sbFramePosChange(TObject *Sender);
    void __fastcall AniTimerTimer(TObject *Sender);
    void __fastcall miAnimationClick(TObject *Sender);
    void __fastcall miConvAVIClick(TObject *Sender);
    void __fastcall miSimulateClick(TObject *Sender);
private:	// User declarations
    TImageFrame CurrentObFrame;
    void __fastcall GetCurrentObFrame();
    void __fastcall ShowFrame(TCanvas* CV, int Width, int Height, TImageFrame& ImageFrame);
public:		// User declarations
    __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
