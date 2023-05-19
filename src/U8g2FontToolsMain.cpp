/******************************************************************************
	U8g2 Font Tools (64-bit)

	- 為可將Windows已安裝安裝字形轉碼為u8g2格式之轉換工具程式
	- 支援粗體字、斜體字、粗斜體字、底線字、刪除線字

	程式作者：林威辰 (臺灣.高雄)
	完成日期：2023-05-06
	使用授權：BSD-2-Clause License

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
	※程式中使用名詞釋義：
	字形(glyph)：泛指各種產生之文字或圖案之統稱。
	字體(font face)：指具有特殊風格設計之中英文文字。如微軟正黑體、標楷體。
	字型(font)：指集合成一個具有檔名名稱的字形或圖形集合體。如jsjh.ttc, kaiu.ttf。
******************************************************************************/

#include <vcl.h>
#include <vector>
#include <algorithm>
#pragma hdrstop
using namespace std;

#include "U8g2FontToolsMain.h"
#include "About.h"
#define Print memoArrayList->Lines->Add
#define TEST_GLYPH "龍"             // 無中文字時之測試用字
#define RIGHT_SHIFT_RATIO 0.45      // 避免j等超出左邊框字形印出右移字寬倍率
#define D_ADJUST 0       			// 字間距微調值
#define GLYPH_COUNT_PER_BLOCK 100  	// Unicode glyphs快速跳躍表每區塊包含字數
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

	// 預設字體
	for (i = 0; i < Screen->Fonts->Count; i++) {
		cbbxInstalledFonts->Items->Add(Screen->Fonts->Strings[i]);
	}
	cbbxInstalledFonts->ItemIndex = cbbxInstalledFonts->Items->Count - 1;
	cbbxInstalledFonts->Hint = cbbxInstalledFonts->Text;

	// 預設字體風格
	cbbxFontStyle->ItemIndex = 0;
    cbbxFontStyle->Hint = cbbxFontStyle->Text;

	// 預設字級
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

	// 包含要解碼的ASCII預存入待編碼陣列
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
	lblTotalCountToEncode->Caption = "總字數：" + String(GDE.size());

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
		// 單字元組ASCII
		if ( GDE[0].unicode.msb == 0 ) {    // 含ASCII字元
			while ( GDE[i].unicode.msb == 0 && i < GDE.size() ) {
				str += (char)GDE[i].unicode.lsb;
				i++;
			}
			cnt = i;

			if ( i < GDE.size() ) {     	// 有Unicode字形，則跳列區隔
				str += "\r\n";
			}
		}

		// 雙字組Unicode glyphs
		for (i = cnt; i < GDE.size(); i++) {
			code = GDE[i].unicode.value;
			str += (wchar_t)code;
		}

		memoGlyphMap->Text = str;

		// 字形編碼計算完成旗標-設為未完成
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
		rbtnTest1->Caption = str[1];    // 取所選的第一個字
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::EncodeAllGlyphs(void)
{
	if ( !bGlyphMapReady ) {
		Application->MessageBox(L"請先按「整理」收集要編碼的字！", L"系統提示", MB_ICONEXCLAMATION);
		return;
	}

	TBitmap* bmp;
	wchar_t wch;
	int i, x, y;
	TSize size;

	struct {    // 原座標系邊框範圍計算
		int left;
		int right;
		int top;
		int bottom;
	} frame;

	struct {    // 擴增畫布範圍計算用
		int width;
		int height;
	} maxCanvas {0, 0};
	float expandRatio = 2.0;  	// for ASCII(英文字母寬度變化較大), 預設2.0
	int glyphPrintPos;        	// 字形輸出位置

	auto findUpperBound = [&]()-> int {		// 獨立出尋找上邊界程序，可供其他程序使用
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

	auto findLowerBound = [&]()-> int {		// 獨立出尋找下邊界程序，可供其他程序使用
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
			計算緊鄰之字框四個邊界
			原座標系：TBitmap所定義之座標系
			新座標系：U8g2所定義之座標系(A底線為0值，g有下降部為負值)
		*/

		// 尋找上框線
		frame.top = findUpperBound();

		// 尋找下框線
		frame.bottom = findLowerBound();

		// 尋找左框線
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

		// 尋找右框線
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

        // 空白字元或無字形，只取一個白點置於圖形中央，並編碼該白點
		if ( frame.right == -1 ) {
			frame.right = bmp->Canvas->TextWidth(' ') / 2 + glyphPrintPos;
			frame.left = frame.right;
			frame.top = FH.refLine;
			frame.bottom = FH.refLine;
		}

		GDE[i].bitcntW = frame.right;	// 計算時先暫存圖形右邊界在原座標系值
		GDE[i].bitcntH = frame.top;		// 計算時先暫存圖形上邊界在原座標系值
		GDE[i].bitcntX = frame.left;	// 計算時先暫存圖形左邊界在原座標系值
		GDE[i].bitcntY = frame.bottom;	// 計算時先暫存圖形下邊界在原座標系值

		// 依緊鄰字框內字形記錄點陣圖點
		GDE[i].bits.clear();

		for (y = frame.top; y <= frame.bottom; y++) {
			for (x = frame.left; x <= frame.right; x++) {
				GDE[i].bits.emplace_back(bmp->Canvas->Pixels[x][y] == clBlack);
			}
		}
	};

	/*
		先設定適當畫布大小，印出欲編碼字形(含字形效果)，並記錄圖點
	*/

	bmp = new TBitmap();
	bmp->Canvas->Font->Name = cbbxInstalledFonts->Text;
	bmp->Canvas->Font->Size = cbbxFontSize->Text.ToInt();
	bmp->Monochrome = true;
	bmp->PixelFormat = pf1bit;

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
		case 4:     // 底線字
			bmp->Canvas->Font->Style = TFontStyles() << fsUnderline;
			break;
		case 5:     // 刪除線字
			bmp->Canvas->Font->Style = TFontStyles() << fsStrikeOut;
			break;
	}

	auto max = [](int a, int b)->int {
		return (a > b)?a:b;
	};

	auto min = [](int a, int b)->int {
		return (a < b)?a:b;
	};

	// 找最大字寬，再依所需比例擴增整體畫布寬度與高度
	for (i = 0; i < GDE.size(); i++) {
		size = bmp->Canvas->TextExtent(GDE[i].unicode.value);
		maxCanvas.width = max(size.Width, maxCanvas.width);
		maxCanvas.height = max(size.Height, maxCanvas.height);
	}

	if ( GDE[i-1].unicode.msb != 0 ) {  // 最後一個字形是否為Unicode?
		expandRatio = 1.25;     		// 若是，則預設1.25(含斜體字佔寬，傾斜角度在14.5度以內)
	}

	// 擴增並設定畫布大小、字形輸出位置
	size.Width = maxCanvas.width * expandRatio;
	size.Height = maxCanvas.height;
	bmp->SetSize(size.Width, size.Height);
	glyphPrintPos = maxCanvas.width * RIGHT_SHIFT_RATIO;

	auto clearBmp = [&](){  // 點陣圖畫布清除: 以白底遍塗畫布
		bmp->SetSize(size.Width, size.Height);
		bmp->Canvas->Brush->Color = clWhite;
		bmp->Canvas->FillRect(Rect(0, 0, size.Width, size.Height));
	};

	// 計算 'A', 'g', '(' 相關參數
	//--- 'A'
	clearBmp();
	bmp->Canvas->TextOut(glyphPrintPos, 0, "A");
	FH.refLine = findLowerBound();      // 設定英文字母基準線(零線)位置
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

	// 字形印出、切除白邊、記錄圖點
	for (i = 0; i < GDE.size(); i++) {
		clearBmp();
		wch = GDE[i].unicode.value;
		bmp->Canvas->TextOut(glyphPrintPos, 0, wch);
        // 切除點陣圖字形外多餘白邊，並進行最小字形之圖點記錄(記於GDE.bits陣列)
		encodeBits();
	}

	/*
		再計算所有字形的邊框範圍，最後獲得整體字型的BBX值(記於Font Header中)
	*/

	// 在原座標系比較所有字形把個別邊框範圍最小化，從中取出最大邊框作為字型整體邊框
	struct {
		int left;
		int right;
		int top;
		int bottom;
	} bbx {255, 0, 255, 0};

	for (i = 0; i < GDE.size(); i++) {
		bbx.right = max(GDE[i].bitcntW, bbx.right);		// 取原座標系最大值為右邊界
		bbx.left = min(GDE[i].bitcntX, bbx.left);		// 取原座標系最小值為左邊界

		bbx.top = min(GDE[i].bitcntH, bbx.top);			// 取原座標系最小值為上邊界
		bbx.bottom = max(GDE[i].bitcntY, bbx.bottom);	// 取原座標系最大值為下邊界
	}

	// 去除字形多餘白邊，並計算各邊界在新座標系之數值
	FH.font_bbx_w = bbx.right - bbx.left + 1;
	FH.font_bbx_h = bbx.bottom - bbx.top + 1;
	FH.font_bbx_x = 0;
	FH.font_bbx_y = FH.refLine - bbx.bottom;

	// 記錄圖形參數之最大數值(負值先加1再取絕對值)，用以計算所需bit寬度
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
		if ( wch == 0x7f ) {	// DEL字元(此字原則上已被剔除，避免邊框計算錯誤)
			continue;
		} else {
			GDE[i].bitcntW = GDE[i].bitcntW - GDE[i].bitcntX + 1;					// 換算為正式bitcntW值
			maxVal.bitcntW = max(GDE[i].bitcntW, maxVal.bitcntW);

			GDE[i].bitcntX -= bbx.left;												// 切除左白邊後之x-offset
			maxVal.bitcntX = max(GDE[i].bitcntX, maxVal.bitcntX);

			GDE[i].bitcntD = bmp->Canvas->TextWidth(wch) + D_ADJUST;				// 至下一字形間隔並微調
			maxVal.bitcntD = max(GDE[i].bitcntD, maxVal.bitcntD);

			GDE[i].bitcntH = GDE[i].bitcntY - GDE[i].bitcntH + 1;					// 換算為正式bitcntH值
			maxVal.bitcntH = max(GDE[i].bitcntH, maxVal.bitcntH);

			GDE[i].bitcntY = FH.refLine - GDE[i].bitcntY;                           // 換算為正式bitcntY值
			maxVal.bitcntY = max(absVal(GDE[i].bitcntY), absVal(maxVal.bitcntY));   // 須預處理負值(y-Offset)
		}
	}

	delete bmp;

	// 計算Font Header記錄bw_bitcntW/H/X/Y/D之所需位元寬度
	auto CalculateBitWidth = [&](BYTE val, bool isSigned) {
		int i;
		for (i = 1; i < 7; i++) {	// 最小值為1 bit，最大值不超過 54 (< 64)
			if ( val < (1U << i) )
				break;
		}

		if ( isSigned ) {			// 有號數增加1 bit (最高bit): 1=正數，0=負數
            i++;                    // 有別於一般有號數2補數之計算
		}

		return i;
	};

	FH.bw_bitcntW = CalculateBitWidth(maxVal.bitcntW, false);
	FH.bw_bitcntH = CalculateBitWidth(maxVal.bitcntH, false);
	FH.bw_bitcntX = CalculateBitWidth(maxVal.bitcntX, true);
	FH.bw_bitcntY = CalculateBitWidth(maxVal.bitcntY, true);
	FH.bw_bitcntD = CalculateBitWidth(maxVal.bitcntD, true);

	// 再以RLE壓縮編碼
    // 參考https://github.com/olikraus/u8g2/wiki/u8g2fontformat中Glyph bitmaps段落說明
	ULONG cnt;
	int bitPos, n;
	int rle_0, rle_1, best_rle_0, best_rle_1;
	WORD tmp;
	DWORD total_bit_count = 0xffffffff;     // 預設可能之最大值

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

	// 以m0, m1指定值進行所有glyphs編碼並回傳使用bit總數
	auto rleCompressAllGlyphs = [&](int rle_0, int rle_1) -> ULONG {
		int a, b, last_a, last_b;
		bool bIsFirst, bCountZero, bCycleDone, bLast;

		cnt = 0;
		// 計算所有glyphs並加總bit數
		for (i = 0; i < GDE.size(); i++) {
			GDE[i].encoded.clear();

			// 存入ASCII或Unicode
			if ( GDE[i].unicode.value < 0x100 ) {
				GDE[i].encoded.emplace_back(GDE[i].unicode.lsb);
				cnt += 8;
			} else {
				GDE[i].encoded.emplace_back(GDE[i].unicode.msb);
				GDE[i].encoded.emplace_back(GDE[i].unicode.lsb);
				cnt += 16;
			}

			// 預留1 byte空間，待編碼完畢再填入總佔用空間數
			// 本值代表每個glyph佔用byte數，因只使用1 byte記錄，故不得大於252(= 255-3)
			GDE[i].encoded.emplace_back(0);
			cnt += 8;
			tmp = 0;
			bitPos = 0;

			// 編碼並寫入glyph header
			writeBits(GDE[i].bitcntW, FH.bw_bitcntW);
			writeBits(GDE[i].bitcntH, FH.bw_bitcntH);
			writeBits(signedToUnsigned(GDE[i].bitcntX, FH.bw_bitcntX), FH.bw_bitcntX);
			writeBits(signedToUnsigned(GDE[i].bitcntY, FH.bw_bitcntY), FH.bw_bitcntY);
			writeBits(signedToUnsigned(GDE[i].bitcntD, FH.bw_bitcntD), FH.bw_bitcntD);

			// 開始字形RLE編碼
			n = 0;
			a = 0; b = 0;
			last_a = -1; last_b = -1;
			bIsFirst = true;
			bCountZero = true;
			bCycleDone = false;
			for (n = 0; n < GDE[i].bits.size(); n++) {
				// 計算連續0
				if ( bCountZero ) {
					if ( a < (1U << rle_0)-1 && !GDE[i].bits[n] ) {
						a++;
					} else {
						bCountZero = false;
					}
				}
				// 計算連續1
				if ( !bCountZero ) {
					if ( b < (1U << rle_1)-1 && GDE[i].bits[n] ) {
						b++;
					} else {
						bCountZero = true;
						bCycleDone = true;
						n--;    // 調整退1
					}
				}

				bLast = ( n == GDE[i].bits.size() - 1);
				if ( bCycleDone || bLast ) {
					// 比對是否與上一組相同，同則記1，異則記0
					if ( a == last_a && b == last_b ) {
						writeBits(1, 1);
						cnt++;
					} else {
						// 若非第一組a,b資料，則記錄與前組資料之異同
						if ( bIsFirst ) {
							bIsFirst = false;
						} else {
							writeBits(0, 1);
							cnt++;
						}

						writeBits(a, rle_0);
						writeBits(b, rle_1);
						cnt += rle_0 + rle_1;

                        // 記錄本組資料供下組比對
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
			// 將剩餘bits未寫入的byte寫入
			if ( bitPos != 0 ) {
				GDE[i].encoded.emplace_back((BYTE)tmp);
			}

			Application->ProcessMessages();
		}
		return cnt;
	};

	// 測試最佳RLE 0/1打包大小
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

	// 記錄最佳RLE 0/1 bit數
	FH.bw_m0 = best_rle_0;
	FH.bw_m1 = best_rle_1;

	// 以最佳RLE 0/1編碼打包
	tcnt = rleCompressAllGlyphs(best_rle_0, best_rle_1);

	// 計算每個Glyph所佔空間並填入相關編碼欄位
	FH.encOverflow = "";
	for (i = 0; i < GDE.size(); i++) {
		if ( GDE[i].unicode.value < 0x100 ) {
			GDE[i].encoded[1] = GDE[i].encoded.size();
		} else {
			GDE[i].encoded[2] = GDE[i].encoded.size();
		}

		if ( GDE[i].encoded.size() > 255 ) {    // 記錄超出編碼容量字形
			FH.encOverflow += (wchar_t)GDE[i].unicode.value;
		}
	}

	// 計算並記錄Font Header中'A', 'a'之陣列位移值
	WORD offset = 0;
	FH.array_offset_of_A = 0;
	FH.array_offset_of_a = 0;
	FH.glyph_0x100_jump_table = 0;
	FH.total_byte_count = 23;
	for (i = 0; i < GDE.size(); i++) {
		if ( GDE[i].unicode.value >= 0x100 ) {
			// 記錄大於0x100之Glyph的Jump Table位移
			FH.glyph_0x100_jump_table = offset + 2; // 小於0x100與大於0x100之Glyph間有兩個BYTE零值之結尾
			break;
		} else if ( GDE[i].unicode.value == 'A' ) {
			FH.array_offset_of_A = offset;
		} else if ( GDE[i].unicode.value == 'a' ) {
			FH.array_offset_of_a = offset;
		}
		offset += GDE[i].encoded.size();
	}

	FH.total_byte_count += offset + 2;      // 累計byte count (包含結尾雙零值)

	// 若有大於0x100之Glyph，則製作Jump Table
	if ( i < GDE.size() ) {
		// 記錄0x100以上Glyph之Jump Table位移與其最後之Unicode
		JumpTable jtTmp {0, 0};
		offset = 0;
		int cnt = 0;
		JT.clear();

		while ( i < GDE.size() ) {
			cnt = ++cnt % GLYPH_COUNT_PER_BLOCK;	// 由1開始
			if ( cnt == 1 ) {   		// 新的跳躍表區塊(block)
				jtTmp.offset = offset;
				JT.emplace_back(jtTmp);
				offset = 0;
			} else if ( cnt == 0 ) {    // 最後的unicode值
				JT[JT.size()-1].unicode = GDE[i].unicode.value;
			}
			offset += GDE[i].encoded.size();
			FH.total_byte_count += GDE[i].encoded.size();		// 累計byte count
			i++;
		}

		// 修改JumpTable第一筆記錄之offset與最後一筆記錄之最後unicode值(0xffff)
		JT[0].offset = JT.size() * 4;
		JT[JT.size()-1].unicode = 0xffff;
		FH.total_byte_count += JT.size() * 4 + 2 /*結尾雙零*/;	// 累計byte count
    }

	bGlyphEncodeComplete = true;
}
//---------------------------------------------------------------------------

bool __fastcall TfrmMain::MakeOctetStrAndSave(String filePathName)
{
	int i, j, c;

	AnsiString astr = edtFontFaceToSave->Text.Trim();

	if ( astr == "" ) {
		Application->MessageBox(L"字體名稱尚未指定！", L"系統提示", MB_ICONEXCLAMATION);
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
			Application->MessageBox(L"字體名稱只能使用文數字或底線！", L"系統提示", MB_ICONEXCLAMATION);
			return false;
		}
	}

	// 字形若未編碼則編碼
	if ( !bGlyphEncodeComplete ) {
		EncodeAllGlyphs();
		if ( !bGlyphMapReady ) {
			return false;
		}
	}

	// 若字形編碼發生超過容量限制錯誤，則返回
	if ( FH.encOverflow != "" ) {
        return false;
	}

	// 準備要寫入檔案之八進位編碼文字檔資料
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

	//  將編碼後BYTE資料再以八進位表示法記錄成字串
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

	// 記錄Font Header資料
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

	// 'A', 'a', 0x100以上 快速跳躍表
	addLineW(FH.array_offset_of_A);
	addLineW(FH.array_offset_of_a);
	addLineW(FH.glyph_0x100_jump_table);

	// 0x100 以上 Glyph 資料
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

    // 0x100系列以兩個零值終結，此處僅加一個零，另一個零由程式讀入null-terminated字串編碼時產生
	if ( line == "" ) {
		pSL->Add("  \"\\0\";");
	} else {
		addLine(0);
		pSL->Add("  \"" + line + "\";");    // 八進位編碼字串結尾時須加分號;
	}

	// 寫入檔案
	pSL->WriteBOM = false;  // 產生無BOM(Byte Order Mark)的UTF-8文字檔，支援中文顯示
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

	// 計算可放大最大倍率
	r = (size.Width > size.Height)?size.Width:size.Height;
	r = 320 / r;    // Zoom In Ratio
	s = r / 10;     // Step
	if ( s < 1 ) {
		s = 1;
	}

	// 列出適用倍率清單
	cbbxZoomRatioEnc->Clear();
	for (i = 1; i <= r; i+=s) {
		cbbxZoomRatioEnc->Items->Add(i);
	}
	cbbxZoomRatioEnc->ItemIndex = 0;

	// 清除編碼超容字串
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
		case 1:     // 粗體字
			bmp->Canvas->Font->Style = TFontStyles() << fsBold;
			break;
		case 2:     // 斜體字
			bmp->Canvas->Font->Style = TFontStyles() << fsItalic;
			break;
		case 3:     // 粗斜體字
			bmp->Canvas->Font->Style = TFontStyles() << fsBold << fsItalic;
			break;
		case 4:     // 底線字
			bmp->Canvas->Font->Style = TFontStyles() << fsUnderline;
			break;
		case 5:     // 刪除線字
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

	// 先在擴增畫布上輸出字形
	size = bmp->Canvas->TextExtent(TEST_GLYPH);
	bmp->SetSize(size.Width * 2.0, size.Height);     // 兩倍寬畫布
	bmp->Monochrome = true;
	bmp->PixelFormat = pf1bit;
	bmp->Canvas->TextOut(size.Width * RIGHT_SHIFT_RATIO, 0, strTest);	// 右移位置輸出字形

	// 尋找左右邊界
	for (x = 0; x < size.Width * 2; x++) {  // 左邊界
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

	for (x = size.Width * 2 - 1; x >= 0; x--) {  // 右邊界
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

	if ( right == -1 ) {    			// 空白字元或無字形unicode
        size.Width = bmp->Canvas->TextWidth(' ');
	} else {
		size.Width = right - left + 1;  // 確定實佔字寬
	}
	lblFontDimEnc->Caption = String(size.Width) + "w x " + String(size.Height) + "h";

	// 裁切左右白邊
	bmp->Canvas->CopyRect(Rect(0, 0, size.Width, size.Height), bmp->Canvas, Rect(left, 0, right+1, size.Height));
	bmp->SetSize(size.Width, size.Height);

    // 依所選放大倍率印出字形
	ratio = cbbxZoomRatioEnc->Text.ToInt();
	if ( ratio == 1 ) {
		imgPreviewEnc->Picture->Assign(bmp);
	} else {
		bmpZ = new TBitmap(size.Width * ratio, size.Height * ratio);
		bmpZ->Monochrome = true;
		bmpZ->PixelFormat = pf1bit;

		// bmp -> bmpZ 放大ratio倍
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
		Application->MessageBox(L"尚未收集整理任何欲編碼字形！", L"系統提示", MB_ICONEXCLAMATION);
	} else if ( FH.encOverflow == "" && SaveDialog->Execute() ) {	// Save U8g2 Fonts file (.h/.c)
		ledtSaveToFile->Text = SaveDialog->FileName;
        Application->ProcessMessages();     // 提早顯示檔名
		if ( MakeOctetStrAndSave(ledtSaveToFile->Text) ) {
			msg = "U8g2編碼與存檔完成！\n\n使用位元組總數: " + String(FH.total_byte_count);
			Application->MessageBox(msg.c_str(), L"系統提示", MB_ICONINFORMATION);
		}
	}

	if ( FH.encOverflow != "" ) {
		msg = "以下字形編碼超過252位元組(bytes):\n\n" + FH.encOverflow + "\n\nU8g2字型未存檔！\r"
			  "請選擇較小的字級大小再編碼存檔。";
		Application->MessageBox(msg.c_str(), L"字形編碼超容錯誤", MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::bbtnSelectFileClick(TObject *Sender)
{
	TStringList* pSL = new TStringList();
	String stmp;
	AnsiString astmp;
	bool flag = false, withData = false;  // getData()讀入資料旗號
	FontToDecode fontTmp;
	int i;

	auto getFontName = [&](){
		vector<int> p;  // 記錄 " (雙引號位置)
		int i;
		for (i = 1; i <= astmp.Length(); i++) {
			if ( astmp[i] == '"' )
				p.emplace_back(i);
		}
		withData = ( p.size() >= 4 );
		return astmp.SubString(p[0]+1, p[1]-p[0]-1);
	};

	auto getData = [&](){
		vector<int> p;  // 記錄 " (雙引號位置)
		int i;
		for (i = 1; i <= astmp.Length(); i++) {
			if ( astmp[i] == '"' ) {
				p.emplace_back(i);
			}
		}

		// 取右二雙引號範圍資料
		i = p.size();
		if ( i >= 2 ) {
			fontTmp.octetStr += astmp.SubString(p[i-2]+1, p[i-1]-p[i-2]-1);

			// 位元組定義結束(在最右雙引號之右)
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

	// 依選擇清除已載入字形
	if ( cbxClearFonts->Checked ) {
        Fonts.clear();
	}

	// 預處理字形檔
	pSL->LoadFromFile(OpenDialog->FileName);
	for (i = 0; i < pSL->Count; i++) {
		astmp = pSL->Strings[i].Trim();	// 去除首尾空白字元
		if ( flag ) {
			getData();
		} else if ( astmp.Pos("U8G2_FONT_SECTION") > 0 ) {
			fontTmp.fontName = getFontName();
			fontTmp.octetStr = "";
			if ( withData ) {		// 陣列宣告列等號右側若有資料則為 true
				withData = false;	// 避免經手動編輯過字型檔資料時本程式無法處理
                i--;
			}
            flag = true;
		}
	}

	delete pSL;

	// 列出編碼字清單
	cbbxFontList->Items->Clear();
	for (i = 0; i < Fonts.size(); i++) {
		cbbxFontList->Items->Add(Fonts[i].fontName);
	}
	cbbxFontList->ItemIndex = cbbxFontList->Items->Count - 1;

	cbbxFontListChange(NULL);   // 立即處理列出字形
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::cbbxFontListChange(TObject *Sender)
{
	LoadFontArray();
	ListGlyphs();
	bArrayListed = false;
	memoArrayList->Clear();

	// 計算可用放大倍率
	int fi = cbbxFontList->ItemIndex;
	int i, r, s;
	r = (Fonts[fi].fh.font_bbx_w > Fonts[fi].fh.font_bbx_h)?Fonts[fi].fh.font_bbx_w:Fonts[fi].fh.font_bbx_h;
	r = 320 / r;    // Zoom In Ratio
	s = r / 10;     // Step
	if ( s < 1 ) {
		s = 1;
	}

	// 列出適用倍率清單
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

	// 載入字形檔
	FontArr.clear();
	while ( i <= astmp.Length() ) {
		if ( astmp[i] == '\\' ) {
			FontArr.emplace_back(getByte());
		} else {
			FontArr.emplace_back((BYTE)astmp[i]);
			i++;
		}
	}

    // 因非在程式編譯時載入，須手動加入結尾0
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

	// 取出Font Header參數
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

	// 解碼指定字形各圖形
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

		// 讀取圖形資料
		int aCnt, bCnt, cnt;
		do {
			// 讀取連續0個數
			aCnt = readBits(Fonts[fi].fh.bw_m0, false);
			// 讀取連續1個數
			bCnt = readBits(Fonts[fi].fh.bw_m1, false);

			do {
				// 寫入連續0(false)到bits陣列
				for (cnt = 0; cnt < aCnt; cnt++) {
					gddTmp.bits.emplace_back(false);
				}
				// 寫入連續1(true)到bits陣列
				for (cnt = 0; cnt < bCnt; cnt++) {
					gddTmp.bits.emplace_back(true);
				}
			} while ( readBits(1, false) == 1 );	// 取下一個bit決定是否重複ab序列: 1=重複 0=終止
		} while ( idx < i );

		GDD.emplace_back(gddTmp);
	};

	// 處理ASCII字元 (Font Header中'A', 'a'快速跳躍位移記錄)
	if ( FontArr[17] != 0 || FontArr[18] != 0 || FontArr[19] != 0 || FontArr[20] != 0 ) {
		i = 23;
		do {
			ch = FontArr[i];
			str += ch;
			decode(2);
		} while ( FontArr[i] != 0 );

		str += "\r\n";
	}

	// 處理Unicode glyphs
	if ( FontArr[21] != 0 || FontArr[22] != 0 ) {
		// 經由 Jump Table 跳至 Unicode glyphs 起點
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
	// 未載入字形檔或已列印
	if ( cbbxFontList->Text == "" || bArrayListed ) {
        return;
	}

	int fi = cbbxFontList->ItemIndex;
	int i, idx, ptr;
	String disp;
	BYTE d;

	memoArrayList->Clear();

	// 列出Font Header參數
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
	ptr = 0;    // Glyph陣列(GDD)指標
	bool bJumpTable = false;
	for (i = 0; i < FontArr.size(); i++) {
		d = FontArr[i];
		if ( i == idx ) {
			if ( i == 0 ) {
				Print("【Font Header】");
				idx = 23;   // Glyph開始位置
			} else if ( GDD[ptr].unicode.msb == 0 && ptr < GDD.size() ) {      // ASCII
				if ( d == GDD[ptr].unicode.lsb ) {
					Print(disp.sprintf(L"【%c】0x%.2X  ", GDD[ptr].unicode.lsb, GDD[ptr].unicode.lsb) + glyphParams());
				} else {            // 程式偵錯時使用
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
						} else {    // 程式偵錯時使用
							Print(disp.sprintf(L"【%c】\\u%.4X   (mismatch)", d));
						}
						idx += FontArr[idx+2];
						ptr++;
					}
				}
			}
		}

		if ( d >= ' ' && d < 0x7f ) {   // Printable ASCII (排除 0x7f DEL字元)
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
		// 尋找該字寬、高
		for (i = 0; i < GDD.size(); i++) {
			if ( GDD[i].unicode.value == wch ) {
				break;
			}
		}

		if ( i < GDD.size() ) {
			ledtTestGlyphToDecode->Tag = i;		// 暫存於檢視字Tag中，供立即檢視取用
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

	// 調整圖形的x,y平移位移值
	int xshift = GDD[i].bitcntX - Fonts[fi].fh.font_bbx_x;
	int yshift = (Fonts[fi].fh.font_bbx_h - GDD[i].bitcntH) - (GDD[i].bitcntY - Fonts[fi].fh.font_bbx_y);

	// 畫出字形
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

		// bmp -> bmpZ 放大ratio倍
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

