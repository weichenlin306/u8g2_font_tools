//---------------------------------------------------------------------------

#ifndef AboutH
#define AboutH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Mask.hpp>
//---------------------------------------------------------------------------
class TfrmAbout : public TForm
{
__published:	// IDE-managed Components
	TMemo *memoAbout;
	void __fastcall FormClick(TObject *Sender);
	void __fastcall memoAboutClick(TObject *Sender);
	void __fastcall memoAboutMouseEnter(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAbout *frmAbout;
//---------------------------------------------------------------------------
#endif
