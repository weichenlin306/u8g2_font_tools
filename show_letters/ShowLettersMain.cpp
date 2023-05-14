/*---------------------------------------------------------------------------
	ShowLetters.exe

	- 顯示選定之Windows安裝字形，以檢查是否超出預設字形邊框
	- U8g2FontTools.exe(v1.0.23.512)以預設邊框為基礎進行編碼，超出之純英文字
	  字形可能被裁切
	- 新版U8g2FontTools.exe將儘可能修正被裁切錯誤

	程式設計：林威辰(臺灣.高雄)
	完成日期：2023-05-14
	使用授權：MIT License

	Copyright © 2023 weichenlin306@gmail.com

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
---------------------------------------------------------------------------*/

#include <vcl.h>
#pragma hdrstop

#include "ShowLettersMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
TImage* pImg[96];
TLabel* pLBL;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
	int x, y, n;

	for (x = 0; x < 8; x++) {
		for (y = 0; y < 12; y++) {
			n = x * 12 + y;
			pImg[n] = new TImage(this);
			pImg[n]->Parent = frmMain;
		}
	}

	// Installed Windows Fonts
	int i;
	cbbxInstalledFonts->Clear();

	for (i = 0; i < Screen->Fonts->Count; i++) {
		cbbxInstalledFonts->Items->Add(Screen->Fonts->Strings[i]);
	}
	cbbxInstalledFonts->ItemIndex = cbbxInstalledFonts->Items->Count - 1;

	// Setup a TLabel to display the initial letter bounding box size
	pLBL = new TLabel(this);
	pLBL->Parent = frmMain;
	pLBL->Font->Color = clMaroon;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::bbtnShowClick(TObject *Sender)
{
	TBitmap* bmp = new TBitmap();
	TBitmap* bmp2 = new TBitmap();
	TSize size;
	int doubleWidth, tripleWidth;
	int x, y, n;
	char c;

	bmp->Canvas->Font->Name = cbbxInstalledFonts->Text;
	bmp->Canvas->Font->Size = 27;           // Set fixed size for demo
	bmp2->Canvas->Font->Name = bmp->Canvas->Font->Name;
	bmp2->Canvas->Font->Size = bmp->Canvas->Font->Size;
	size = bmp->Canvas->TextExtent("W");    // The widest letter
	doubleWidth = size.Width * 2;           // Expand width 2 times
	tripleWidth = size.Width * 3;           // Expand width 3 times

	bmp->SetSize(tripleWidth, size.Height);
	bmp2->SetSize(tripleWidth, size.Height);

	// 字形效果
	switch ( cbbxFontStyle->ItemIndex ) {
		case 1:     // 粗體字
			bmp->Canvas->Font->Style = TFontStyles() << fsBold;
			break;
		case 2:     // 斜體字
			bmp->Canvas->Font->Style = TFontStyles() << fsItalic;
			break;
		case 3:     // 粗斜體字
			bmp->Canvas->Font->Style = TFontStyles() << fsBold << fsItalic;
			break;
		case 4:     // 底線
			bmp->Canvas->Font->Style = TFontStyles() << fsUnderline;
			break;
		case 5:     // 刪除線
			bmp->Canvas->Font->Style = TFontStyles() << fsStrikeOut;
			break;
	}
    bmp2->Canvas->Font->Style = bmp->Canvas->Font->Style;

	// Place images with bitmaps
	for (x = 0; x < 8; x++) {
		for (y = 0; y < 12; y++) {
			n = x * 12 + y;
			c = n + 32;		// Printable ASCII starting from 32 (space)

			if ( c < 127 ) {
				// Place TImage
				pImg[n]->Width = tripleWidth;
				pImg[n]->Height = size.Height;
				pImg[n]->Left = 24 + (tripleWidth + 2) * x;
				pImg[n]->Top = 8 + (size.Height + 2) * y;

				// Clear TBitmap
				bmp->Canvas->Brush->Color = clWhite;
				bmp->Canvas->FillRect(Rect(0, 0, tripleWidth-1, size.Height-1));
				bmp2->Canvas->Brush->Color = clWhite;
				bmp2->Canvas->FillRect(Rect(0, 0, tripleWidth-1, size.Height-1));

				// Print each letter twice, with different positions, on a TBitmap
				bmp->Canvas->Font->Color = clGreen;
				bmp->Canvas->TextOut(0, 0, c);

				bmp2->Canvas->Font->Color = clBlue;
				bmp2->Canvas->TextOut(size.Width, 0, c);
				bmp2->Transparent = true;
				bmp2->TransparentColor = clWhite;

                bmp->Canvas->Draw(0, 0, bmp2);

				// Draw lines for letter bounding boxes
				bmp->Canvas->Pen->Color = clTeal;
				bmp->Canvas->PenPos = {0, 0};
				bmp->Canvas->LineTo(0, size.Height-1);
				bmp->Canvas->LineTo(tripleWidth-1, size.Height-1);
				bmp->Canvas->LineTo(tripleWidth-1, 0);
				bmp->Canvas->LineTo(0, 0);
				bmp->Canvas->PenPos = {size.Width, 0};
				bmp->Canvas->LineTo(size.Width, size.Height-1);
				bmp->Canvas->PenPos = {doubleWidth, 0};
				bmp->Canvas->LineTo(doubleWidth, size.Height-1);

				pImg[n]->Picture->Assign(bmp);
			} else {    // Last space
				pLBL->Font->Size = size.Width / 2;
				pLBL->Left = 24 + (tripleWidth + 2) * x;
				pLBL->Top = 8 + (size.Height + 2) * y + size.Height - pLBL->Height;
			}
		}
	}

	// Display letter bounding box size
	String msg;
	pLBL->Caption = msg.sprintf(L"%dw x %dh", size.Width, size.Height);

	// Adjust form size
	ClientWidth = (tripleWidth + 2) * 8 + 48;
    ClientHeight = (size.Height + 2) * 12 + 16 + pnlControlBar->Height;

    delete bmp2;
	delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	int i;

	for (i = 0; i < 96; i++) {
		delete pImg[i];
	}

    delete pLBL;
}
//---------------------------------------------------------------------------

