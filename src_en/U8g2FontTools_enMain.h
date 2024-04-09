//---------------------------------------------------------------------------

#ifndef U8g2FontTools_enMainH
#define U8g2FontTools_enMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ComCtrls.hpp>
#include <System.DateUtils.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TPageControl *pgctlMain;
	TTabSheet *tshtEncode;
	TTabSheet *tshtDecode;
	TImage *imgPreviewDec;
	TLabel *lblZoomRatioDec;
	TLabel *lblFontFaceToView;
	TBitBtn *bbtnDecodePreview;
	TLabeledEdit *ledtTestGlyphToDecode;
	TComboBox *cbbxZoomRatioDec;
	TBitBtn *bbtnSelectFile;
	TLabeledEdit *ledtSelectFile;
	TComboBox *cbbxFontList;
	TCheckBox *cbxClearFonts;
	TOpenDialog *OpenDialog;
	TLabeledEdit *ledtSaveToFile;
	TBitBtn *bbtnSaveToFile;
	TSaveDialog *SaveDialog;
	TImage *imgPreviewEnc;
	TLabel *lblFontCheck;
	TLabel *FontFaceToSave;
	TEdit *edtFontFaceToSave;
	TLabel *lblFontDimDec;
	TBitBtn *bbtnEncodePreview;
	TComboBox *cbbxInstalledFonts;
	TComboBox *cbbxFontSize;
	TLabel *lblFontDimEnc;
	TRadioButton *rbtnTest1;
	TRadioButton *rbtnTest2;
	TRadioButton *rbtnTest3;
	TRadioButton *rbtnTest4;
	TPageControl *pgctlGlyphsToEncode;
	TTabSheet *tshtGlyphsToEncode;
	TTabSheet *tshtGlyphMap;
	TMemo *memoGlyphsForEncoding;
	TMemo *memoGlyphMap;
	TLabel *lblTotalCountToEncode;
	TLabel *lblZoomRatioEnc;
	TComboBox *cbbxZoomRatioEnc;
	TPageControl *pgctlGlyphsToDecode;
	TTabSheet *tshtGlyphsToDecode;
	TMemo *memoGlyphContent;
	TTabSheet *tshtGlyphArray;
	TMemo *memoArrayList;
	TLabel *lblTotalCountDecoded;
	TCheckBox *cbxIncludeASCII;
	TBitBtn *bbtnTidyUpGlyphs;
	TBitBtn *bbtnPrintGlyphArray;
	TLabeledEdit *ledtFind;
	TPanel *pnlAbout;
	TComboBox *cbbxFontStyle;
	void __fastcall bbtnTidyUpGlyphsClick(TObject *Sender);
	void __fastcall bbtnEncodePreviewClick(TObject *Sender);
	void __fastcall memoGlyphMapMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall cbbxInstalledFontsChange(TObject *Sender);
	void __fastcall cbbxFontStyleChange(TObject *Sender);
	void __fastcall cbbxFontSizeChange(TObject *Sender);
	void __fastcall bbtnSaveToFileClick(TObject *Sender);
	void __fastcall bbtnSelectFileClick(TObject *Sender);
	void __fastcall cbbxFontListChange(TObject *Sender);
	void __fastcall bbtnPrintGlyphArrayClick(TObject *Sender);
	void __fastcall ledtFindChange(TObject *Sender);
	void __fastcall memoGlyphContentMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall bbtnDecodePreviewClick(TObject *Sender);
	void __fastcall pnlAboutClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
  String __fastcall GetFileVersion(void);
	void __fastcall GlyphMap(void);
	void __fastcall EncodeAllGlyphs(void);
	bool __fastcall MakeOctetStrAndSave(String fn);
	void __fastcall ChangePreviewRatioListEnc(void);
	void __fastcall LoadFontArray(void);
	void __fastcall ListGlyphs(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
