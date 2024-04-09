//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "About.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAbout *frmAbout;
//---------------------------------------------------------------------------
__fastcall TfrmAbout::TfrmAbout(TComponent* Owner)
	: TForm(Owner)
{
	Color = TColor(RGB(164, 255, 253));
	memoAbout->Color = Color;

	auto getCompileDate = [](){
		String monthName[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
			"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
		String today = __DATE__;
		String year = today.SubString(8, 4);
		String month = today.SubString(1, 3);
		String day = today.SubString(5, 2);
		int i;

		for (i = 0; i < 12; i++) {
			if ( monthName[i] == month ) {
				i++;
				break;
			}
		}

		return year + "-" + (i < 10?"0":"") + IntToStr(i) + "-" + day;
	};

	memoAbout->Lines->Strings[5] += getCompileDate();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAbout::FormClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAbout::memoAboutClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAbout::memoAboutMouseEnter(TObject *Sender)
{
	DefocusControl(memoAbout, true);
}
//---------------------------------------------------------------------------

void __fastcall TfrmAbout::FormPaint(TObject *Sender)
{
	DefocusControl(memoAbout, true);
}
//---------------------------------------------------------------------------

