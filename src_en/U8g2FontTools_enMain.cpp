/******************************************************************************
	U8g2 Font Tools (64-bit)

	- It is a conversion tool program that can convert Windows installed glyphs
	  into u8g2 format.
	- Supports bold fonts, italics, bold italics, underline fonts, and strikeout
	  fonts.

	Author: Wei-Chen Lin (Kaohsiung, Taiwan)
	Release date: 2023-05-06
	License: BSD-2-Clause License

	Copyright © 2023, weichenlin306@gmail.com
	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	“AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
	PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	--------------------------------------------------------------------------
	* Explanation of nouns used in the program:
	Glyph: Generally refers to the collective name of various generated words or
		patterns.
	Font face: Refers to Chinese and English text with a special style design.
		Such as Microsoft regular boldface and standard italics.
	Font: Refers to a collection of glyphs or graphics that has a file name.
		Such as jsjh.ttc, kaiu.ttf.
******************************************************************************/

#include <vcl.h>
#include <vector>
#include <algorithm>
#pragma hdrstop
using namespace std;

#include "U8g2FontTools_enMain.h"
#include "About.h"
#define Print memoArrayList->Lines->Add
#define TEST_GLYPH "龍"             // Test characters when there are no Chinese characters
#define RIGHT_SHIFT_RATIO 0.45      // Prevent j and other fonts from exceeding the left border
									//   and printing the right-shifted font width magnification
#define D_ADJUST 0       			// Character spacing fine-tuning value
#define GLYPH_COUNT_PER_BLOCK 100  	// Unicode glyphs fast jump table contains words per block
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;

struct FontHeader {
	BYTE glyphCount = 0;
	BYTE bbx_mode = 0;
	BYTE bw_m0 = 0;
	BYTE bw_m1 = 0;
	BYTE bw_bitcntW = 0;
	BYTE bw_bitcntH = 0;
	BYTE bw_bitcntX = 0;
	BYTE bw_bitcntY = 0;
	BYTE bw_bitcntD = 0;
	BYTE font_bbx_w = 0;
	BYTE font_bbx_h = 0;
	char font_bbx_x = 0;    // signed
	char font_bbx_y = 0;    // signed
	BYTE refLine;
	char ascent_of_letter_A = 0;    // signed
	char descent_of_letter_g = 0;   // signed
	char ascent_of_brachet = 0;     // signed
	char descent_of_brachet = 0;    // signed
	WORD array_offset_of_A = 0;
	WORD array_offset_of_a = 0;
	WORD glyph_0x100_jump_table = 0;
	DWORD total_byte_count;
	String encOverflow;
};
FontHeader FH;

struct JumpTable {
	WORD offset;
	WORD unicode;
};
vector<JumpTable> JT;

struct GlyphDataEnc {
	union {
		WORD value;
		struct {
			BYTE lsb;
			BYTE msb;
		};
	} unicode;
	BYTE bitcntW;
	BYTE bitcntH;
	char bitcntX;	// signed bitcntX
	char bitcntY;	// signed bitcntY
	char bitcntD;	// signed bitcntD
	vector<bool> bits;
	vector<BYTE> encoded;
};
vector<GlyphDataEnc> GDE;
bool bGlyphMapReady = false;
bool bGlyphEncodeComplete = false;

struct FontToDecode {
	String fontName;
	AnsiString octetStr = "";
	FontHeader fh;
};
vector<FontToDecode> Fonts;
vector<BYTE> FontArr;

struct GlyphDataDec {
	union {
		WORD value;
		struct {
			BYTE lsb;
			BYTE msb;
		};
	} unicode;
	BYTE bitcntW;
	BYTE bitcntH;
	char bitcntX;	// signed bitcntX
	char bitcntY;	// signed bitcntY
	char bitcntD;	// signed bitcntD
	vector<BYTE> decoded;
	vector<bool> bits;
};
vector<GlyphDataDec> GDD;

bool bArrayListed = false;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
	Caption += " (v" + GetFileVersion() + ")";

	int i;
	String str = "";

	// Default font
	for (i = 0; i < Screen->Fonts->Count; i++) {
		cbbxInstalledFonts->Items->Add(Screen->Fonts->Strings[i]);
	}
	cbbxInstalledFonts->ItemIndex = cbbxInstalledFonts->Items->Count - 1;
	cbbxInstalledFonts->Hint = cbbxInstalledFonts->Text;

	// Default font style
	cbbxFontStyle->ItemIndex = 0;
    cbbxFontStyle->Hint = cbbxFontStyle->Text;

	// Default font size
	for (i = 8; i <= 40; i += 4) {
		cbbxFontSize->Items->Add(i);
	}
	cbbxFontSize->Text = "12";

	rbtnTest1->Caption = TEST_GLYPH;
    ChangePreviewRatioListEnc();
}
//---------------------------------------------------------------------------

String __fastcall TfrmMain::GetFileVersion(void)
{
	String strFile = Application->ExeName;
	DWORD dwVerInfoSize = 0;
	dwVerInfoSize = GetFileVersionInfoSizeW(strFile.c_str(), &dwVerInfoSize);
	BYTE* bVerInfoBuf = new BYTE[dwVerInfoSize];
	String strVersion;

	if ( GetFileVersionInfoW(strFile.c_str(), 0, dwVerInfoSize, bVerInfoBuf) ) {
		VS_FIXEDFILEINFO* vsInfo;
		UINT vsInfoSize;
		if ( VerQueryValueW(bVerInfoBuf, L"\\", (void**)&vsInfo, &vsInfoSize) ) {
			strVersion.sprintf(L"%d.%d.%d.%d", HIWORD(vsInfo->dwFileVersionMS),
											   LOWORD(vsInfo->dwFileVersionMS),
											   HIWORD(vsInfo->dwFileVersionLS),
											   LOWORD(vsInfo->dwFileVersionLS));
		}
	}
	delete[] bVerInfoBuf;
	return strVersion;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::GlyphMap(void)
{
	GlyphDataEnc gdeTmp;
	String line;
	wchar_t wch;
	int i, j, k, cnt;

	GDE.clear();

	// Contains the ASCII to be decoded and stored in the array to be encoded
	if ( cbxIncludeASCII->Checked ) {
		for (i = 32; i < 127; i++) {    // ASCII 32-126
			gdeTmp.unicode.value = i;
			GDE.push_back(gdeTmp);
		}
	}

	for (i = 0; i < memoGlyphsForEncoding->Lines->Count; i++) {
		line = memoGlyphsForEncoding->Lines->Strings[i].Trim();
		if ( line.Length() != 0 ) {
			for (j = 1; j <= line.Length(); j++) {
				cnt++;
				wch = line[j];
				for (k = 0; k < GDE.size(); k++) {
					if ( GDE[k].unicode.value == (WORD)wch )
						break;
				}
				if ( k == GDE.size() ) {
					gdeTmp.unicode.value = (WORD)wch;
					GDE.push_back(gdeTmp);
				}
			}
		}
	}

	auto compare = [](GlyphDataEnc& a, GlyphDataEnc& b){
		return (a.unicode.value < b.unicode.value);
	};

	sort(GDE.begin(), GDE.end(), compare);
	FH.glyphCount = GDE.size();
	lblTotalCountToEncode->Caption = "Glyph Count: " + String(GDE.size());

	bGlyphMapReady = (GDE.size() > 0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::bbtnTidyUpGlyphsClick(TObject *Sender)
{
	String str = "";
	int i = 0, cnt = 0, code;

	GlyphMap();

	memoGlyphMap->Clear();

	if ( bGlyphMapReady ) {
		// Single character ASCII
		if ( GDE[0].unicode.msb == 0 ) {    // Contains ASCII characters
			while ( GDE[i].unicode.msb == 0 && i < GDE.size() ) {
				str += (char)GDE[i].unicode.lsb;
				i++;
			}
			cnt = i;

			if ( i < GDE.size() ) {	// If there are Unicode glyphs, skip column separation
				str += "\r\n";
			}
		}

		// Double-byte Unicode glyphs
		for (i = cnt; i < GDE.size(); i++) {
			code = GDE[i].unicode.value;
			str += (wchar_t)code;
		}

		memoGlyphMap->Text = str;

		// Glyph encoding calculation completion flag - set to incompleted
		bGlyphEncodeComplete = false;
	}

	pgctlGlyphsToEncode->ActivePage = tshtGlyphMap;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::memoGlyphMapMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	String str = memoGlyphMap->SelText;
	if ( str == "" ) {
		rbtnTest1->Caption = TEST_GLYPH;
	} else {
		rbtnTest1->Caption = str[1];    // Select the first char
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::EncodeAllGlyphs(void)
{
	if ( !bGlyphMapReady ) {
		Application->MessageBox(L"Click \"Tidy Up\" to collect distinct glyphs!", L"System Message", MB_ICONEXCLAMATION);
		return;
	}

	TBitmap* bmp;
	wchar_t wch;
	int i, x, y;
	TSize size;

	struct {    // Boundary calculation of the bounding box using original (computer) coordinate system
		int left;
		int right;
		int top;
		int bottom;
	} frame;

	struct {    // Temporary calculation values for expanding canvas size
		int width;
		int height;
	} maxCanvas {0, 0};
	float expandRatio = 2.0;  	// for ASCII(bigger ratio for English letters), default: 2.0
	int glyphPrintPos;        	// Glyph output position

	auto findUpperBound = [&]()-> int {		// Lambda function to find the upper boundary
		for (y = 0; y < size.Height; y++) {
			for (x = 0; x < size.Width; x++) {
				if ( bmp->Canvas->Pixels[x][y] == clBlack )
					break;
			}
			if ( x < size.Width ) {
				break;
			}
		}
		return y;
	};

	auto findLowerBound = [&]()-> int {		// Lambda function to find the lower boundary
		for (y = size.Height-1; y >= 0; y--) {
			for (x = 0; x < size.Width; x++) {
				if ( bmp->Canvas->Pixels[x][y] == clBlack )
					break;
			}
			if ( x < size.Width ) {
				break;
			}
		}
		return y;
	};

	auto encodeBits = [&](){
		/*
			Calculating 4 boundaries of the bounding box
			Original coordinate system: TBitmap defined coordinate system
			New coordinate system: U8g2 defined coordinate system
			(The bottom line "A" equals 0; negative value for descending part of "g")
		*/

		// Finding the upper boundary
		frame.top = findUpperBound();

		// Finding the lower boundary
		frame.bottom = findLowerBound();

		// Finding the left boundary
		for (x = 0; x < size.Width; x++) {
			for (y = 0; y < size.Height; y++) {
				if ( bmp->Canvas->Pixels[x][y] == clBlack )
					break;
			}
			if ( y < size.Height ) {
				break;
			}
		}
		frame.left = x;

		// Finding the right boundary
		for (x = size.Width-1; x >= 0; x--) {
			for (y = 0; y < size.Height; y++) {
				if ( bmp->Canvas->Pixels[x][y] == clBlack )
					break;
			}
			if ( y < size.Height ) {
				break;
			}
		}
		frame.right = x;

        // For blank char or no char, encode a white dot in the center of the glyph
		if ( frame.right == -1 ) {
			frame.right = bmp->Canvas->TextWidth(' ') / 2 + glyphPrintPos;
			frame.left = frame.right;
			frame.top = FH.refLine;
			frame.bottom = FH.refLine;
		}

		GDE[i].bitcntW = frame.right;	// Temporarily save the right boundary value (orig. coor. system)
		GDE[i].bitcntH = frame.top;		// Temporarily save the upper boundary value (orig. coor. system)
		GDE[i].bitcntX = frame.left;	// Temporarily save the left boundary value (orig. coor. system)
		GDE[i].bitcntY = frame.bottom;	// Temporarily save the lower boundary value (orig. coor. system)

		// Record black/white dots within a bounding box
		GDE[i].bits.clear();

		for (y = frame.top; y <= frame.bottom; y++) {
			for (x = frame.left; x <= frame.right; x++) {
				GDE[i].bits.emplace_back(bmp->Canvas->Pixels[x][y] == clBlack);
			}
		}
	};

	/*
		Create a suitable canvas size, print out the glyph to be encoded,
		and record the black/white dots.
	*/

	bmp = new TBitmap();
	bmp->Canvas->Font->Name = cbbxInstalledFonts->Text;
	bmp->Canvas->Font->Size = cbbxFontSize->Text.ToInt();
	bmp->Monochrome = true;
	bmp->PixelFormat = pf1bit;

	switch ( cbbxFontStyle->ItemIndex ) {
		case 1:     // Bold
			bmp->Canvas->Font->Style = TFontStyles() << fsBold;
			break;
		case 2:     // Italic
			bmp->Canvas->Font->Style = TFontStyles() << fsItalic;
			break;
		case 3:     // Bold italic
			bmp->Canvas->Font->Style = TFontStyles() << fsBold << fsItalic;
			break;
		case 4:     // Underline
			bmp->Canvas->Font->Style = TFontStyles() << fsUnderline;
			break;
		case 5:     // StrikeOut
			bmp->Canvas->Font->Style = TFontStyles() << fsStrikeOut;
			break;
	}

	auto max = [](int a, int b)->int {
		return (a > b)?a:b;
	};

	auto min = [](int a, int b)->int {
		return (a < b)?a:b;
	};

	// Find the max. char width, and expand the canvas size accordingly.
	for (i = 0; i < GDE.size(); i++) {
		size = bmp->Canvas->TextExtent(GDE[i].unicode.value);
		maxCanvas.width = max(size.Width, maxCanvas.width);
		maxCanvas.height = max(size.Height, maxCanvas.height);
	}

	if ( GDE[i-1].unicode.msb != 0 ) {  // Is the last glyph Unicode?
		expandRatio = 1.25;     		// If yes, default to 1.25.
	}

	// Expand canvas size and locate printout position
	size.Width = maxCanvas.width * expandRatio;
	size.Height = maxCanvas.height;
	bmp->SetSize(size.Width, size.Height);
	glyphPrintPos = maxCanvas.width * RIGHT_SHIFT_RATIO;

	auto clearBmp = [&](){  // Clear bitmap canvas: brush with white color
		bmp->SetSize(size.Width, size.Height);
		bmp->Canvas->Brush->Color = clWhite;
		bmp->Canvas->FillRect(Rect(0, 0, size.Width, size.Height));
	};

	// Calculate parameters for 'A', 'g', and '("
	//--- 'A'
	clearBmp();
	bmp->Canvas->TextOut(glyphPrintPos, 0, "A");
	FH.refLine = findLowerBound();      // Set reference line (bottom) for English letters
	FH.ascent_of_letter_A = FH.refLine - findUpperBound();
	//--- 'g'
	clearBmp();
	bmp->Canvas->TextOut(glyphPrintPos, 0, "g");
	FH.descent_of_letter_g = FH.refLine - findLowerBound();
	//--- '('
	clearBmp();
	bmp->Canvas->TextOut(glyphPrintPos, 0, "(");
	FH.ascent_of_brachet = FH.refLine - findUpperBound();
	FH.descent_of_brachet = FH.refLine - findLowerBound();

	// Print out char, cut off white boundaries, and record/encode glyphs
	for (i = 0; i < GDE.size(); i++) {
		clearBmp();
		wch = GDE[i].unicode.value;
		bmp->Canvas->TextOut(glyphPrintPos, 0, wch);
        // Cut off white boundaries and record/encode dots
		encodeBits();
	}

	/*
		Calculate boundaries of all glyphs, get the BBX value of the font to be
		recorded in "Font Header."
	*/

	// Minimize the area of each glyph and get the largest bounding box from all glyphs
	// as the font bounding box in orginal coordiate system.
	struct {
		int left;
		int right;
		int top;
		int bottom;
	} bbx {255, 0, 255, 0};

	for (i = 0; i < GDE.size(); i++) {
		bbx.right = max(GDE[i].bitcntW, bbx.right);		// Get the max. value as the right boundary
		bbx.left = min(GDE[i].bitcntX, bbx.left);		// Get the min. value as the left boundary

		bbx.top = min(GDE[i].bitcntH, bbx.top);			//  Get the min. value as the top boundary
		bbx.bottom = max(GDE[i].bitcntY, bbx.bottom);	//  Get the max. value as the bottom boundary
	}

	// Trim outer white border and calculate boundary values in new coordiate system
	FH.font_bbx_w = bbx.right - bbx.left + 1;
	FH.font_bbx_h = bbx.bottom - bbx.top + 1;
	FH.font_bbx_x = 0;
	FH.font_bbx_y = FH.refLine - bbx.bottom;

	// Record the max. value (add 1 and then get the absolute value for negatives)
	// to calculate bit width
	struct {
		BYTE bitcntW;
		BYTE bitcntH;
		BYTE bitcntX;
		BYTE bitcntY;
		BYTE bitcntD;
	} maxVal {0, 0, 0, 0, 0};

	auto absVal = [](int val){
		int num = val;
		if ( num < 0 ) {
			num++;
		}
		return abs(num);
	};

	for (i = 0; i < GDE.size(); i++) {
		wch = GDE[i].unicode.value;
		if ( wch == 0x7f ) {	// Character DEL (being excluded for possible wrong calculation)
			continue;
		} else {
			GDE[i].bitcntW = GDE[i].bitcntW - GDE[i].bitcntX + 1;					// Convert to bitcntW
			maxVal.bitcntW = max(GDE[i].bitcntW, maxVal.bitcntW);

			GDE[i].bitcntX -= bbx.left;												// X-offset after trimming left white border
			maxVal.bitcntX = max(GDE[i].bitcntX, maxVal.bitcntX);

			GDE[i].bitcntD = bmp->Canvas->TextWidth(wch) + D_ADJUST;				// Gap between this and the next letter
			maxVal.bitcntD = max(GDE[i].bitcntD, maxVal.bitcntD);

			GDE[i].bitcntH = GDE[i].bitcntY - GDE[i].bitcntH + 1;					// Convert to bitcntH
			maxVal.bitcntH = max(GDE[i].bitcntH, maxVal.bitcntH);

			GDE[i].bitcntY = FH.refLine - GDE[i].bitcntY;                           // Convert to bitcntY
			maxVal.bitcntY = max(absVal(GDE[i].bitcntY), absVal(maxVal.bitcntY));   // Manage negative values (y-Offset)
		}
	}

	delete bmp;

	// Calculate the necessary bit widths of bw_bitcntW/H/X/Y/D in Font Header
	auto CalculateBitWidth = [&](BYTE val, bool isSigned) {
		int i;
		for (i = 1; i < 7; i++) {	// The smallest value: 1 bit，the max. value not more than 54 (< 64)
			if ( val < (1U << i) )
				break;
		}

		if ( isSigned ) {			// Add 1 bit (the highest bit) for signed value: 1=positive，0=negative
			i++;                    // Different from 2's complement calculation
		}

		return i;
	};

	FH.bw_bitcntW = CalculateBitWidth(maxVal.bitcntW, false);
	FH.bw_bitcntH = CalculateBitWidth(maxVal.bitcntH, false);
	FH.bw_bitcntX = CalculateBitWidth(maxVal.bitcntX, true);
	FH.bw_bitcntY = CalculateBitWidth(maxVal.bitcntY, true);
	FH.bw_bitcntD = CalculateBitWidth(maxVal.bitcntD, true);

	// Encode with RLE
	// Please refer to the instructions of Glyph bitmaps block in
	//   https://github.com/olikraus/u8g2/wiki/u8g2fontformat for detail.
	ULONG cnt;
	int bitPos, n;
	int rle_0, rle_1, best_rle_0, best_rle_1;
	WORD tmp;
	DWORD total_bit_count = 0xffffffff;     // Set the maximum value as default

	auto signedToUnsigned = [](int val, int bw) {
		return val + ((1U<<bw)>>1);
	};

	auto writeBits = [&](int val, int bw) {
		tmp |= (val << bitPos);
		bitPos += bw;
		if ( bitPos >= 8 ) {
			GDE[i].encoded.emplace_back((BYTE)(tmp & 0xff));
			tmp >>= 8;
			bitPos %= 8;
		}
		cnt += bw;
	};

	// Encode all glyphs using m0,m1 with return value of total bit count
	auto rleCompressAllGlyphs = [&](int rle_0, int rle_1) -> ULONG {
		int a, b, last_a, last_b;
		bool bIsFirst, bCountZero, bCycleDone, bLast;

		cnt = 0;
		// Calculate all glyphs and cumulatively add the bit count
		for (i = 0; i < GDE.size(); i++) {
			GDE[i].encoded.clear();

			// Save ASCII or Unicode
			if ( GDE[i].unicode.value < 0x100 ) {
				GDE[i].encoded.emplace_back(GDE[i].unicode.lsb);
				cnt += 8;
			} else {
				GDE[i].encoded.emplace_back(GDE[i].unicode.msb);
				GDE[i].encoded.emplace_back(GDE[i].unicode.lsb);
				cnt += 16;
			}

			// Reserve 1-byte space for the bit count of each glyph after encoding,
			// and hence no bigger than 252 (255-3)
			GDE[i].encoded.emplace_back(0);
			cnt += 8;
			tmp = 0;
			bitPos = 0;

			// Encode and write to glyph header
			writeBits(GDE[i].bitcntW, FH.bw_bitcntW);
			writeBits(GDE[i].bitcntH, FH.bw_bitcntH);
			writeBits(signedToUnsigned(GDE[i].bitcntX, FH.bw_bitcntX), FH.bw_bitcntX);
			writeBits(signedToUnsigned(GDE[i].bitcntY, FH.bw_bitcntY), FH.bw_bitcntY);
			writeBits(signedToUnsigned(GDE[i].bitcntD, FH.bw_bitcntD), FH.bw_bitcntD);

			// Begin RLE encoding
			n = 0;
			a = 0; b = 0;
			last_a = -1; last_b = -1;
			bIsFirst = true;
			bCountZero = true;
			bCycleDone = false;
			for (n = 0; n < GDE[i].bits.size(); n++) {
				// Calculate serial 0's
				if ( bCountZero ) {
					if ( a < (1U << rle_0)-1 && !GDE[i].bits[n] ) {
						a++;
					} else {
						bCountZero = false;
					}
				}
				// Calculate serial 1's
				if ( !bCountZero ) {
					if ( b < (1U << rle_1)-1 && GDE[i].bits[n] ) {
						b++;
					} else {
						bCountZero = true;
						bCycleDone = true;
						n--;    // Adjust and go back 1
					}
				}

				bLast = ( n == GDE[i].bits.size() - 1);
				if ( bCycleDone || bLast ) {
					// Compare with the last 0/1 pair, designate as 1 if the same or as 0
					if ( a == last_a && b == last_b ) {
						writeBits(1, 1);
						cnt++;
					} else {
						// Record the difference if not the 1st pair
						if ( bIsFirst ) {
							bIsFirst = false;
						} else {
							writeBits(0, 1);
							cnt++;
						}

						writeBits(a, rle_0);
						writeBits(b, rle_1);
						cnt += rle_0 + rle_1;

                        // Save the current pair for comparison with the next
						last_a = a;
						last_b = b;
					}

					if ( bLast ) {
						writeBits(0, 1);
						cnt++;
					} else {
						a = 0;
						b = 0;
						bCycleDone = false;
					}
				}
			}
			// Write the last few bits into the last byte
			if ( bitPos != 0 ) {
				GDE[i].encoded.emplace_back((BYTE)tmp);
			}

			Application->ProcessMessages();
		}
		return cnt;
	};

	// Test the optimum RLE 0/1 encoding
	ULONG tcnt;
	for (rle_0 = 2; rle_0 < 9; rle_0++) {
		for (rle_1 = 2; rle_1 < 7; rle_1++) {
			tcnt = rleCompressAllGlyphs(rle_0, rle_1);
			if ( tcnt < total_bit_count ) {
				total_bit_count = tcnt;
				best_rle_0 = rle_0;
				best_rle_1 = rle_1;
			}
		}
	}

	// Record the best RLE 0/1 bit count
	FH.bw_m0 = best_rle_0;
	FH.bw_m1 = best_rle_1;

	// Encode with the best RLE 0/1 packaging method
	tcnt = rleCompressAllGlyphs(best_rle_0, best_rle_1);

	// Encode the necessary space for each Glyph and fill into individual defined field
	FH.encOverflow = "";
	for (i = 0; i < GDE.size(); i++) {
		if ( GDE[i].unicode.value < 0x100 ) {
			GDE[i].encoded[1] = GDE[i].encoded.size();
		} else {
			GDE[i].encoded[2] = GDE[i].encoded.size();
		}

		if ( GDE[i].encoded.size() > 255 ) {    // Record glyphs with overflowed bit count
			FH.encOverflow += (wchar_t)GDE[i].unicode.value;
		}
	}

	// Calculate and record the offset of 'A', 'a' in the Font Header
	WORD offset = 0;
	FH.array_offset_of_A = 0;
	FH.array_offset_of_a = 0;
	FH.glyph_0x100_jump_table = 0;
	FH.total_byte_count = 23;
	for (i = 0; i < GDE.size(); i++) {
		if ( GDE[i].unicode.value >= 0x100 ) {
			// Record Jump Table offset with glyphs codes > 0x100
			FH.glyph_0x100_jump_table = offset + 2; // Two-byte tailing zeros will be inserted before glyph > 0x100
			break;
		} else if ( GDE[i].unicode.value == 'A' ) {
			FH.array_offset_of_A = offset;
		} else if ( GDE[i].unicode.value == 'a' ) {
			FH.array_offset_of_a = offset;
		}
		offset += GDE[i].encoded.size();
	}

	FH.total_byte_count += offset + 2;      // Cumulatively add byte count (including the tailing two zeros)

	// Make a Jump Table if there is any glyph code > 0x100
	if ( i < GDE.size() ) {
		// Record the offset of the glyph code > 0x100 and the last Unicode
		JumpTable jtTmp {0, 0};
		offset = 0;
		int cnt = 0;
		JT.clear();

		while ( i < GDE.size() ) {
			cnt = ++cnt % GLYPH_COUNT_PER_BLOCK;	// Start from 1
			if ( cnt == 1 ) {   		// New jump table (block)
				jtTmp.offset = offset;
				JT.emplace_back(jtTmp);
				offset = 0;
			} else if ( cnt == 0 ) {    // The last unicode value
				JT[JT.size()-1].unicode = GDE[i].unicode.value;
			}
			offset += GDE[i].encoded.size();
			FH.total_byte_count += GDE[i].encoded.size();		// Cumulative byte count
			i++;
		}

		// Modify the offset of the 1st record of the Jump Table and the last unicode (0xffff)
		JT[0].offset = JT.size() * 4;
		JT[JT.size()-1].unicode = 0xffff;
		FH.total_byte_count += JT.size() * 4 + 2 /*Tailing two zeros*/;	// Cumulative byte count
    }

	bGlyphEncodeComplete = true;
}
//---------------------------------------------------------------------------

bool __fastcall TfrmMain::MakeOctetStrAndSave(String filePathName)
{
	int i, j, c;

	AnsiString astr = edtFontFaceToSave->Text.Trim();

	if ( astr == "" ) {
		Application->MessageBox(L"The font name is not specified!", L"System Alert", MB_ICONEXCLAMATION);
		return false;
	} else {
		for (i = 1; i <= astr.Length(); i++) {
			c = astr[i];
			if ( ( c >= '0' && c <= '9' ) || ( c >= '@' && c <= 'Z' ) ||
				( c >= 'a' && c <= 'z') || c == '_' ) {
				continue;
			} else {
				break;
            }
		}
		if ( i <= astr.Length() ) {
			Application->MessageBox(L"Only use alphanumeric characters or underscore in the font name!", L"System Alert", MB_ICONEXCLAMATION);
			return false;
		}
	}

	// Encode if glyphs are not encoded
	if ( !bGlyphEncodeComplete ) {
		EncodeAllGlyphs();
		if ( !bGlyphMapReady ) {
			return false;
		}
	}

	// Give up encoding and return if total bit count exceeds the maximum limitation
	if ( FH.encOverflow != "" ) {
        return false;
	}

	// Write as octet-encoded text file
	TStringList* pSL = new TStringList();
	String fontStyle;
	switch ( cbbxFontStyle->ItemIndex ) {
		case 0:
			fontStyle = "normal";
			break;
		case 1:
			fontStyle = "bold";
			break;
		case 2:
			fontStyle = "italic";
			break;
		case 3:
			fontStyle = "bold italic";
			break;
		case 4:
			fontStyle = "underline";
			break;
		case 5:
			fontStyle = "strikeout";
			break;
	}

	pSL->Add("/*");
	pSL->Add("  Date/Time: " + Now().FormatString("yyyy-mm-dd hh:mm:ss"));
	pSL->Add("  Fontname: " + edtFontFaceToSave->Text);
	pSL->Add("  Font face name (size|style): " + cbbxInstalledFonts->Text + "(" + cbbxFontSize->Text +
		"|" + fontStyle + ")");
	pSL->Add("  Glyph count: " + String(GDE.size()));
	pSL->Add("  BBX Mode: " + String(FH.bbx_mode));
	pSL->Add("*/");
	pSL->Add("const uint8_t " + astr + "[" + String(FH.total_byte_count) + "] U8G2_FONT_SECTION(\"" + astr + "\") =");

	// Encode the encoded bytes as octet strings
	String line = "";

	auto setOctetStr = [](BYTE b)->String {
		if ( b < ' ' || ( b >= '0' && b <= '7' ) || b == '?' || b == '\\' || b == '"' || b >= 0x7f ) {
			if ( b < 0x08 ) {			// 8
				return "\\" + IntToStr(b);
			} else if ( b < 0x40 ) {	// 64
				return "\\" + IntToStr(b >> 3) + IntToStr(b & 0x07);
			} else {
				return "\\" + IntToStr(b >> 6) + IntToStr((b >> 3) & 0x07) + IntToStr(b & 0x07);
			}
		} else {
			return (char)b;
		}
	};

	auto addLine = [&](BYTE b) {
		line += setOctetStr(b);

		if ( line.Length() > 80 ) {
			pSL->Add("  \"" + line + "\"");
			line = "";
		}
	};

	auto addLineW = [&](WORD w) {
		addLine((BYTE)(w >> 8));
		addLine((BYTE)(w & 0xff));
	};

	// Record Font Header data
	addLine(FH.glyphCount);
	addLine(FH.bbx_mode);
	addLine(FH.bw_m0);
	addLine(FH.bw_m1);
	addLine(FH.bw_bitcntW);
	addLine(FH.bw_bitcntH);
	addLine(FH.bw_bitcntX);
	addLine(FH.bw_bitcntY);
	addLine(FH.bw_bitcntD);
	addLine(FH.font_bbx_w);
	addLine(FH.font_bbx_h);
	addLine((BYTE)FH.font_bbx_x);
	addLine((BYTE)FH.font_bbx_y);
	addLine((BYTE)FH.ascent_of_letter_A);
	addLine((BYTE)FH.descent_of_letter_g);
	addLine((BYTE)FH.ascent_of_brachet);
	addLine((BYTE)FH.descent_of_brachet);

	// 'A', 'a', and > 0x100 Jump Table
	addLineW(FH.array_offset_of_A);
	addLineW(FH.array_offset_of_a);
	addLineW(FH.glyph_0x100_jump_table);

	// > 0x100 Glyph data
	bool above0x100 = false;
	for (i = 0; i < GDE.size(); i++) {
		if ( !above0x100 && GDE[i].unicode.value >= 0x100 ) {
			addLine(0);
			addLine(0);

			for (j = 0; j < JT.size(); j++) {
				addLineW(JT[j].offset);
				addLineW(JT[j].unicode);
			}
			above0x100 = true;
		}
		for (j = 0; j < GDE[i].encoded.size(); j++) {
			addLine(GDE[i].encoded[j]);
		}
	}

	// 0x100 series termiate with two zeros: add 1 zero here and the other act as
	// the last zero for the null-terminated string
	if ( line == "" ) {
		pSL->Add("  \"\\0\";");
	} else {
		addLine(0);
		pSL->Add("  \"" + line + "\";");    // Add a ";" in the end of an octet encoded string
	}

	// Write into a file
	pSL->WriteBOM = false;  // Generate a UTF-8 text file with no BOM (Byte Order Mark), supporting Chinese display
	pSL->SaveToFile(filePathName, TEncoding::UTF8);
	delete pSL;

	return true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ChangePreviewRatioListEnc(void)
{
	TBitmap* bmp = new TBitmap();
	TSize size;
	int i, s, r;

	bmp->Monochrome = true;
	bmp->PixelFormat = pf1bit;
	bmp->Canvas->Font->Name = cbbxInstalledFonts->Text;
	bmp->Canvas->Font->Size = cbbxFontSize->Text.ToInt();
	size = bmp->Canvas->TextExtent(TEST_GLYPH);
	delete bmp;

	// Calculate the max. magnification ratio
	r = (size.Width > size.Height)?size.Width:size.Height;
	r = 320 / r;    // Zoom In Ratio
	s = r / 10;     // Step
	if ( s < 1 ) {
		s = 1;
	}

	// List the usable magnification ratios
	cbbxZoomRatioEnc->Clear();
	for (i = 1; i <= r; i+=s) {
		cbbxZoomRatioEnc->Items->Add(i);
	}
	cbbxZoomRatioEnc->ItemIndex = 0;

	// Clear the string of encoding overflow
	FH.encOverflow = "";
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::cbbxInstalledFontsChange(TObject *Sender)
{
	bGlyphEncodeComplete = false;
	ChangePreviewRatioListEnc();
	cbbxInstalledFonts->Hint = cbbxInstalledFonts->Text;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::cbbxFontStyleChange(TObject *Sender)
{
	bGlyphEncodeComplete = false;
    cbbxFontStyle->Hint = cbbxFontStyle->Text;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::cbbxFontSizeChange(TObject *Sender)
{
	bGlyphEncodeComplete = false;
	ChangePreviewRatioListEnc();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::bbtnEncodePreviewClick(TObject *Sender)
{
	TBitmap* bmp = new TBitmap();
	TBitmap* bmpZ;
	String strTest;
	int i, ratio, x, xr, y, yr;
	int fsize = cbbxFontSize->Text.ToInt();
	TSize size;

	bmp->Canvas->Font->Name = cbbxInstalledFonts->Text;
	bmp->Canvas->Font->Size = fsize;
	switch ( cbbxFontStyle->ItemIndex ) {
		case 1:     // Bold
			bmp->Canvas->Font->Style = TFontStyles() << fsBold;
			break;
		case 2:     // Italic
			bmp->Canvas->Font->Style = TFontStyles() << fsItalic;
			break;
		case 3:     // Bold italic
			bmp->Canvas->Font->Style = TFontStyles() << fsBold << fsItalic;
			break;
		case 4:     // Underline
			bmp->Canvas->Font->Style = TFontStyles() << fsUnderline;
			break;
		case 5:     // StrikeOut
			bmp->Canvas->Font->Style = TFontStyles() << fsStrikeOut;
			break;
	}

	if ( rbtnTest1->Checked ) {
		strTest = rbtnTest1->Caption;
	} else if ( rbtnTest2->Checked ) {
		strTest = rbtnTest2->Caption;
	} else if ( rbtnTest3->Checked ) {
		strTest = rbtnTest3->Caption;
	} else if ( rbtnTest4->Checked ) {
		strTest = rbtnTest4->Caption;
	}

	// Output the glyph on an expanded canvas
	size = bmp->Canvas->TextExtent(TEST_GLYPH);
	bmp->SetSize(size.Width * 2.0, size.Height);     // Double the width of the canvas
	bmp->Monochrome = true;
	bmp->PixelFormat = pf1bit;
	bmp->Canvas->TextOut(size.Width * RIGHT_SHIFT_RATIO, 0, strTest);	// Right shift and output the glyph

	// Find the left and right boundaries
	for (x = 0; x < size.Width * 2; x++) {  // Left boundary
		for (y = 0; y < size.Height; y++) {
			if ( bmp->Canvas->Pixels[x][y] == clBlack ) {
				break;
			}
		}
		if ( y < size.Height ) {
			break;
		}
	}
	int left = x;

	for (x = size.Width * 2 - 1; x >= 0; x--) {  // Right boundary
		for (y = 0; y < size.Height; y++) {
			if ( bmp->Canvas->Pixels[x][y] == clBlack ) {
				break;
			}
		}
		if ( y < size.Height ) {
			break;
		}
	}
	int right = x;

	if ( right == -1 ) {    			// Blank or no glyph unicode
        size.Width = bmp->Canvas->TextWidth(' ');
	} else {
		size.Width = right - left + 1;  // The real occupied glyph width
	}
	lblFontDimEnc->Caption = String(size.Width) + "w x " + String(size.Height) + "h";

	// Trim off the left and right white borders
	bmp->Canvas->CopyRect(Rect(0, 0, size.Width, size.Height), bmp->Canvas, Rect(left, 0, right+1, size.Height));
	bmp->SetSize(size.Width, size.Height);

    // Print the glyph with the selected magnification ratio
	ratio = cbbxZoomRatioEnc->Text.ToInt();
	if ( ratio == 1 ) {
		imgPreviewEnc->Picture->Assign(bmp);
	} else {
		bmpZ = new TBitmap(size.Width * ratio, size.Height * ratio);
		bmpZ->Monochrome = true;
		bmpZ->PixelFormat = pf1bit;

		// bmp -> bmpZ Magnify as ratio specified
		for (y = 0; y < size.Height; y++) {
			for (yr = 0; yr < ratio; yr++) {
				for (x = 0; x < size.Width; x++) {
					for (xr = 0; xr < ratio; xr++) {
						bmpZ->Canvas->Pixels[x*ratio + xr][y*ratio + yr] = bmp->Canvas->Pixels[x][y];
					}
				}
			}
		}

		imgPreviewEnc->Picture->Assign(bmpZ);
		delete bmpZ;
    }

	delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::bbtnSaveToFileClick(TObject *Sender)
{
	String msg;

	if ( GDE.size() == 0 ) {
		Application->MessageBox(L"No glyph is selected for encoding", L"System Alert", MB_ICONEXCLAMATION);
	} else if ( FH.encOverflow == "" && SaveDialog->Execute() ) {	// Save U8g2 Fonts file (.h/.c)
		ledtSaveToFile->Text = SaveDialog->FileName;
        Application->ProcessMessages();     // Early display file name
		if ( MakeOctetStrAndSave(ledtSaveToFile->Text) ) {
			msg = "U8g2 encoding and file saving complete!\n\nTotal byte count: " + String(FH.total_byte_count);
			Application->MessageBox(msg.c_str(), L"System Alert", MB_ICONINFORMATION);
		}
	}

	if ( FH.encOverflow != "" ) {
		msg = "Glyphs with encoded byte count > 252:\n\n" + FH.encOverflow + "\n\nU8g2 font not saved!\r"
			  "Please use smaller font size and re-encode!";
		Application->MessageBox(msg.c_str(), L"Glyph Encoding Overflow Error", MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::bbtnSelectFileClick(TObject *Sender)
{
	TStringList* pSL = new TStringList();
	String stmp;
	AnsiString astmp;
	bool flag = false, withData = false;  // getData() data flags
	FontToDecode fontTmp;
	int i;

	auto getFontName = [&](){
		vector<int> p;  // Memorize " position
		int i;
		for (i = 1; i <= astmp.Length(); i++) {
			if ( astmp[i] == '"' )
				p.emplace_back(i);
		}
		withData = ( p.size() >= 4 );
		return astmp.SubString(p[0]+1, p[1]-p[0]-1);
	};

	auto getData = [&](){
		vector<int> p;  // Memorize " position
		int i;
		for (i = 1; i <= astmp.Length(); i++) {
			if ( astmp[i] == '"' ) {
				p.emplace_back(i);
			}
		}

		// Get the data between the right two " (double quote mark)
		i = p.size();
		if ( i >= 2 ) {
			fontTmp.octetStr += astmp.SubString(p[i-2]+1, p[i-1]-p[i-2]-1);

			// End of byte definition (right to the right " double quote mark)
			if ( astmp.Length() > p[i-1] && astmp[p[i-1]+1] == ';' ) {
				flag = false;
				Fonts.emplace_back(fontTmp);
				fontTmp.octetStr = "";
			}
		}
	};

	// Open .h file for U8g2 Fonts
	if ( OpenDialog->Execute() ) {
		if ( FileExists(OpenDialog->FileName) ) {
			ledtSelectFile->Text = OpenDialog->FileName;
		}
	} else {
		return;
	}

	// Clear the selected loaded fonts
	if ( cbxClearFonts->Checked ) {
        Fonts.clear();
	}

	// Pre-process the font file
	pSL->LoadFromFile(OpenDialog->FileName);
	for (i = 0; i < pSL->Count; i++) {
		astmp = pSL->Strings[i].Trim();	// Trim the head and tail blanks
		if ( flag ) {
			getData();
		} else if ( astmp.Pos("U8G2_FONT_SECTION") > 0 ) {
			fontTmp.fontName = getFontName();
			fontTmp.octetStr = "";
			if ( withData ) {		// True if there are data right to the equal mark
				withData = false;	// to avoid a font file failure after manual processing
                i--;
			}
            flag = true;
		}
	}

	delete pSL;

	// List the font name menu
	cbbxFontList->Items->Clear();
	for (i = 0; i < Fonts.size(); i++) {
		cbbxFontList->Items->Add(Fonts[i].fontName);
	}
	cbbxFontList->ItemIndex = cbbxFontList->Items->Count - 1;

	cbbxFontListChange(NULL);   // Process the listed fonts
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::cbbxFontListChange(TObject *Sender)
{
	LoadFontArray();
	ListGlyphs();
	bArrayListed = false;
	memoArrayList->Clear();

	// Calculate the usable magnification ratio values
	int fi = cbbxFontList->ItemIndex;
	int i, r, s;
	r = (Fonts[fi].fh.font_bbx_w > Fonts[fi].fh.font_bbx_h)?Fonts[fi].fh.font_bbx_w:Fonts[fi].fh.font_bbx_h;
	r = 320 / r;    // Zoom In Ratio
	s = r / 10;     // Step
	if ( s < 1 ) {
		s = 1;
	}

	// List the usable magnification ratio values
	cbbxZoomRatioDec->Clear();
	for (i = 1; i <= r; i+=s) {
		cbbxZoomRatioDec->Items->Add(i);
	}
	cbbxZoomRatioDec->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::LoadFontArray(void)
{
	AnsiString astmp = Fonts[cbbxFontList->ItemIndex].octetStr;
	int i = 1;

	auto getByte = [&](){
		BYTE tmp = 0;
		i++;
		while ( i <= astmp.Length() && astmp[i] >= '0' && astmp[i] < '8' ) {
			tmp <<= 3;
			tmp |= astmp[i] & 0x7;
			i++;
		}
		return tmp;
	};

	// Load font files
	FontArr.clear();
	while ( i <= astmp.Length() ) {
		if ( astmp[i] == '\\' ) {
			FontArr.emplace_back(getByte());
		} else {
			FontArr.emplace_back((BYTE)astmp[i]);
			i++;
		}
	}

    // Manually add the tailing zero at non-compile time
	FontArr.emplace_back(0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ListGlyphs(void)
{
	int fi, i;
	String str = "";
	char ch;
	wchar_t wch;

	fi = cbbxFontList->ItemIndex;

	auto signedByte = [](BYTE val)->char {
		return (val > 127)?(val - 256):val;
	};

	// Get Font Header parameters
	Fonts[fi].fh.glyphCount = FontArr[0];
	Fonts[fi].fh.bbx_mode = FontArr[1];
	Fonts[fi].fh.bw_m0 = FontArr[2];
	Fonts[fi].fh.bw_m1 = FontArr[3];
	Fonts[fi].fh.bw_bitcntW = FontArr[4];
	Fonts[fi].fh.bw_bitcntH = FontArr[5];
	Fonts[fi].fh.bw_bitcntX = signedByte(FontArr[6]);
	Fonts[fi].fh.bw_bitcntY = signedByte(FontArr[7]);
	Fonts[fi].fh.bw_bitcntD = signedByte(FontArr[8]);
	Fonts[fi].fh.font_bbx_w = FontArr[9];
	Fonts[fi].fh.font_bbx_h = FontArr[10];
	Fonts[fi].fh.font_bbx_x = signedByte(FontArr[11]);
	Fonts[fi].fh.font_bbx_y = signedByte(FontArr[12]);
	Fonts[fi].fh.ascent_of_letter_A = signedByte(FontArr[13]);
	Fonts[fi].fh.descent_of_letter_g = signedByte(FontArr[14]);
	Fonts[fi].fh.ascent_of_brachet = signedByte(FontArr[15]);
	Fonts[fi].fh.descent_of_brachet = signedByte(FontArr[16]);

	int idx, bitPos;
	WORD wdTmp;

	// Decode glyphs
	GDD.clear();

	auto readBits = [&](short bw, bool isSigned)->int {
		wdTmp = FontArr[idx];
		if ( bitPos + bw > 7 ) {
			idx++;
			wdTmp |= FontArr[idx] << 8;
		}
		wdTmp = (wdTmp >> bitPos) & ((1U << bw) - 1);
		bitPos += bw;
		bitPos %= 8;

		if ( isSigned ) {
			return wdTmp - ((1U << bw) >> 1);
		} else {
			return wdTmp;
        }
	};

	auto decode = [&](short offset){
		idx = i + offset;
		GlyphDataDec gddTmp;

		if ( offset == 2 ) {					// ASCII
			gddTmp.unicode.value = FontArr[i];
			i += FontArr[i+1];
		} else {								// Unicode glyphs
			gddTmp.unicode.msb = FontArr[i];
			gddTmp.unicode.lsb = FontArr[i+1];
			i += FontArr[i+2];
		}

		bitPos = 0;
		// Glyph header
		gddTmp.bitcntW = readBits(Fonts[fi].fh.bw_bitcntW, false);
		gddTmp.bitcntH = readBits(Fonts[fi].fh.bw_bitcntH, false);
		gddTmp.bitcntX = readBits(Fonts[fi].fh.bw_bitcntX, true);
		gddTmp.bitcntY = readBits(Fonts[fi].fh.bw_bitcntY, true);
		gddTmp.bitcntD = readBits(Fonts[fi].fh.bw_bitcntD, true);

		// Read glyph data
		int aCnt, bCnt, cnt;
		do {
			// Read serial 0 count
			aCnt = readBits(Fonts[fi].fh.bw_m0, false);
			// Read serial 1 count
			bCnt = readBits(Fonts[fi].fh.bw_m1, false);

			do {
				// Write serial 0's (false) to bits array
				for (cnt = 0; cnt < aCnt; cnt++) {
					gddTmp.bits.emplace_back(false);
				}
				// Write serial 1's (true) to bits array
				for (cnt = 0; cnt < bCnt; cnt++) {
					gddTmp.bits.emplace_back(true);
				}
			} while ( readBits(1, false) == 1 );	// Get the nect bit to see if ab series
													// is to be repeated: 1=repeat 0=terminate
		} while ( idx < i );

		GDD.emplace_back(gddTmp);
	};

	// Process ASCII ('A', 'a' Jump Table in Font Head)
	if ( FontArr[17] != 0 || FontArr[18] != 0 || FontArr[19] != 0 || FontArr[20] != 0 ) {
		i = 23;
		do {
			ch = FontArr[i];
			str += ch;
			decode(2);
		} while ( FontArr[i] != 0 );

		str += "\r\n";
	}

	// Process Unicode glyphs
	if ( FontArr[21] != 0 || FontArr[22] != 0 ) {
		// Jump to Unicode glyphs starting point via Jump Table
		i = (FontArr[21] << 8 | FontArr[22]) + 23;	// Unicode glyph jump table
		i += (FontArr[i] << 8 | FontArr[i+1]);		// Unicode glyphs start

		do {
			wch = FontArr[i] << 8 | FontArr[i+1];
			str += wch;
			decode(3);
		} while ( FontArr[i] != 0 );

		str += "\r\n";
	}

	memoGlyphContent->Clear();
	memoGlyphContent->Text = str;
	lblTotalCountDecoded->Caption = "總字數：" + String(GDD.size());

    pgctlGlyphsToDecode->ActivePage = tshtGlyphsToDecode;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::bbtnPrintGlyphArrayClick(TObject *Sender)
{
	// Font file not loaded or having been printed
	if ( cbbxFontList->Text == "" || bArrayListed ) {
        return;
	}

	int fi = cbbxFontList->ItemIndex;
	int i, idx, ptr;
	String disp;
	BYTE d;

	memoArrayList->Clear();

	// List Font Header parameters
	Print(disp.sprintf(L"【Font Header: bit width】\r\n  m0=%d m1=%d W=%d H=%d X=%d Y=%d D=%d",
		Fonts[fi].fh.bw_m0, Fonts[fi].fh.bw_m1, Fonts[fi].fh.bw_bitcntW, Fonts[fi].fh.bw_bitcntH,
		Fonts[fi].fh.bw_bitcntX, Fonts[fi].fh.bw_bitcntY, Fonts[fi].fh.bw_bitcntD)
	);
	Print(disp.sprintf(L"【Font Bounding Box】\r\n  w=%d h=%d x=%d y=%d\r\n  A↑=%d g↓=%d (↑=%d (↓=%d\r\n",
		Fonts[fi].fh.font_bbx_w, Fonts[fi].fh.font_bbx_h, Fonts[fi].fh.font_bbx_x, Fonts[fi].fh.font_bbx_y,
		Fonts[fi].fh.ascent_of_letter_A, Fonts[fi].fh.descent_of_letter_g,
		Fonts[fi].fh.ascent_of_brachet, Fonts[fi].fh.descent_of_brachet)
	);

	auto glyphParams = [&](){
		String str;
		str.sprintf(L" W=%d H=%d X=%d Y=%d D=%d", GDD[ptr].bitcntW, GDD[ptr].bitcntH,
			GDD[ptr].bitcntX, GDD[ptr].bitcntY, GDD[ptr].bitcntD);
		return str;
	};

	auto bin = [](BYTE d, bool withLeadZero){
		String str = withLeadZero?"0b":"";

		BYTE t = 0x80;
		for (int i = 0; i < 8; i++) {
			str += (d & t)?'1':'0';
			t >>= 1;
		}
		return str;
	};

	idx = 0;
	ptr = 0;    // Glyph array (GDD) pointer
	bool bJumpTable = false;
	for (i = 0; i < FontArr.size(); i++) {
		d = FontArr[i];
		if ( i == idx ) {
			if ( i == 0 ) {
				Print("【Font Header】");
				idx = 23;   // Glyph start position
			} else if ( GDD[ptr].unicode.msb == 0 && ptr < GDD.size() ) {      // ASCII
				if ( d == GDD[ptr].unicode.lsb ) {
					Print(disp.sprintf(L"【%c】0x%.2X  ", GDD[ptr].unicode.lsb, GDD[ptr].unicode.lsb) + glyphParams());
				} else {            // Use while debugging
					Print(disp.sprintf(L"【%c】0x%.2X   (mismatch)", d));
				}
				idx += FontArr[idx+1];
				ptr++;
			} else {                                // Unicode glyphs
				if ( bJumpTable ) {
					Print("【Jump Table】");
					idx += (FontArr[idx] << 8 | FontArr[idx+1]);			// Unicode glyphs start
					bJumpTable = false;
				} else {
					if ( d == 0 && FontArr[i+1] == 0 ) {
						if ( i < FontArr.size() - 2 ) {
							idx = (FontArr[21] << 8 | FontArr[22]) + 23;	// Unicode glyph jump table
							bJumpTable = true;
						}
						Print("<Null Terminator>");
					} else {
						if ( ((d << 8) | FontArr[idx+1]) == GDD[ptr].unicode.value ) {
							Print(disp.sprintf(L"【%lc】\\u%.4X", GDD[ptr].unicode.value, GDD[ptr].unicode.value) + glyphParams());
						} else {    // Use while debugging
							Print(disp.sprintf(L"【%c】\\u%.4X   (mismatch)", d));
						}
						idx += FontArr[idx+2];
						ptr++;
					}
				}
			}
		}

		if ( d >= ' ' && d < 0x7f ) {   // Printable ASCII (exclude 0x7f DEL)
			Print(disp.sprintf(L"ARR[%.5d] = %.3d  0x%.2X  %s  %c", i, d, d, bin(d, true).c_str(), d));
		} else {
			Print(disp.sprintf(L"ARR[%.5d] = %.3d  0x%.2X  %s", i, d, d, bin(d, true).c_str()));
		}

        Application->ProcessMessages();
	}

	Print("\r\nFont array size = " + String(FontArr.size()));
    bArrayListed = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ledtFindChange(TObject *Sender)
{
	if ( ledtFind->Text != "" ) {
		memoArrayList->SelStart = memoArrayList->Text.Pos("【" + ledtFind->Text + "】") -1;
		memoArrayList->SelLength = 0;
		memoArrayList->Perform(EM_SCROLLCARET, 0, +0);
		memoArrayList->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::memoGlyphContentMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	wchar_t wch;
	String str = memoGlyphContent->SelText;
	int i;

	if ( str == "" ) {
		ledtTestGlyphToDecode->Text = "";
	} else {
		wch = str[1];
		ledtTestGlyphToDecode->Text = wch;
		// Find the width and height of a glyph
		for (i = 0; i < GDD.size(); i++) {
			if ( GDD[i].unicode.value == wch ) {
				break;
			}
		}

		if ( i < GDD.size() ) {
			ledtTestGlyphToDecode->Tag = i;		// Temporarily saved in the tag property for handy use
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::bbtnDecodePreviewClick(TObject *Sender)
{
	if ( ledtTestGlyphToDecode->Text == "" || ledtTestGlyphToDecode->Text == (char)0x7f ) {
        return;
	}

	TBitmap* bmp = new TBitmap();
	TBitmap* bmpZ;
	String strTest;
	int fi = cbbxFontList->ItemIndex;
	int i = ledtTestGlyphToDecode->Tag;
	int ratio, x, xr, y, yr;
	TSize bbx = {Fonts[fi].fh.font_bbx_w, Fonts[fi].fh.font_bbx_h};
	TSize font = {GDD[i].bitcntW, GDD[i].bitcntH};

	String msg;
	lblFontDimDec->Caption = msg.sprintf(L"BBX %dw,%dh  Glyph w%d h%d x%d y%d d%d",
		bbx.Width, bbx.Height, GDD[i].bitcntW, GDD[i].bitcntH, GDD[i].bitcntX,
		GDD[i].bitcntY, GDD[i].bitcntD);
	bmp->SetSize(bbx.Width, bbx.Height);
	bmp->Monochrome = true;
	bmp->PixelFormat = pf1bit;
	bmp->Canvas->Brush->Color = clWhite;
	bmp->Canvas->FillRect(Rect(0, 0, bbx.Width, bbx.Height));

	// Adjust the x,y shift values
	int xshift = GDD[i].bitcntX - Fonts[fi].fh.font_bbx_x;
	int yshift = (Fonts[fi].fh.font_bbx_h - GDD[i].bitcntH) - (GDD[i].bitcntY - Fonts[fi].fh.font_bbx_y);

	// Draw the glyph
	for (y = 0; y < font.Height; y++) {
		for (x = 0; x < font.Width; x++) {
			if ( GDD[i].bits[y*font.Width + x] ) {
				bmp->Canvas->Pixels[x + xshift][y + yshift] = clBlack;
			} else {
				bmp->Canvas->Pixels[x + xshift][y + yshift] = clWhite;
            }
		}
	}

	ratio = cbbxZoomRatioDec->Text.ToInt();
	if ( ratio == 1 ) {
		imgPreviewDec->Picture->Assign(bmp);
	} else {
		bmpZ = new TBitmap(bbx.Width * ratio, bbx.Height * ratio);
		bmpZ->Monochrome = true;
		bmpZ->PixelFormat = pf1bit;

		// bmp -> bmpZ Magnification ratio
		for (y = 0; y < bbx.Height; y++) {
			for (yr = 0; yr < ratio; yr++) {
				for (x = 0; x < bbx.Width; x++) {
					for (xr = 0; xr < ratio; xr++) {
						bmpZ->Canvas->Pixels[x*ratio + xr][y*ratio + yr] = bmp->Canvas->Pixels[x][y];
					}
				}
			}
		}

		imgPreviewDec->Picture->Assign(bmpZ);
		delete bmpZ;
    }

	delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::pnlAboutClick(TObject *Sender)
{
    frmAbout->Position = poMainFormCenter;
	frmAbout->ShowModal();
}
//---------------------------------------------------------------------------

