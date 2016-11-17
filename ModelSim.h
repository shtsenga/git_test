//---------------------------------------------------------------------------

#ifndef ModelSimH
#define ModelSimH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TSimForm : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TLabeledEdit *leParR;
    TLabeledEdit *leParNum;
    TLabeledEdit *leVis;
    TLabeledEdit *leTemp;
    TLabeledEdit *leRunStps;
    TButton *bRun;
    TMemo *mMemo;
    TLabeledEdit *leCollectStps;
    TLabeledEdit *leInitBoundary;
    void __fastcall bRunClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TSimForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSimForm *SimForm;
//---------------------------------------------------------------------------
#endif
