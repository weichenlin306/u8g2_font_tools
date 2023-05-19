object frmAbout: TfrmAbout
  Left = 391
  Top = 104
  BorderStyle = bsNone
  Caption = #38364#26044
  ClientHeight = 682
  ClientWidth = 554
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  OnClick = FormClick
  OnPaint = FormPaint
  TextHeight = 15
  object memoAbout: TMemo
    Left = 19
    Top = 18
    Width = 521
    Height = 648
    BevelOuter = bvNone
    BorderStyle = bsNone
    Color = clCream
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = #24494#36575#27491#40657#39636
    Font.Style = []
    Lines.Strings = (
      #31243#24335#21517#31281': U8g2 Font Tools (64-bit) '
      
        '                   (https://github.com/weichenlin306/u8g2_font_t' +
        'ools)'
      #31243#24335#32232#35695': Embarcadero C++ Builder 11 Alexandria Community Edition'
      #31243#24335#20316#32773': '#26519#23041#36784' ('#33274#28771'.'#39640#38596')'
      #23436#25104#26085#26399': 2023-05-06'
      #26368#24460#20462#35330': '
      ''
      #8251' U8g2 Font Tools '#33879#20316#27402#23660#26044#26519#23041#36784#65292#21487#22522#26044'BSD'#38617#26781#27454#25480#27402#35215#31684#20351#29992#12290
      '    '#35443#35211' http://www.opensource.org/licenses/bsd-license.php'
      #8251' U8g2'#26684#24335#26159#20197'BDF  (Glyph Bitmap Distribution Format)'#28858#22522#30990#30332#23637#30340#23383'('#22294')'#24418#26684#24335#12290
      '    '#35443#35211' https://github.com/olikraus/u8g2/wiki/u8g2fontformat'#12290
      ''
      '========='#12298'U8g2 Font Tools '#33879#20316#27402#32882#26126'  BSD-2-Clause License'#12299'========='
      ''
      'Copyright '#169' 2023, weichenlin306@gmail.com'
      'All rights reserved.'
      ''
      
        'Redistribution and use in source and binary forms, with or witho' +
        'ut modification, are '
      'permitted provided that the following conditions are met:'
      ''
      
        '1. Redistributions of source code must retain the above copyrigh' +
        't notice, this list of '
      'conditions and the following disclaimer.'
      ''
      
        '2. Redistributions in binary form must reproduce the above copyr' +
        'ight notice, this list '
      
        'of conditions and the following disclaimer in the documentation ' +
        'and/or other '
      'materials provided with the distribution.'
      ''
      
        'THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUT' +
        'ORS '
      
        #8220'AS IS'#8221' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NO' +
        'T '
      
        'LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNES' +
        'S FOR A '
      
        'PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIG' +
        'HT '
      
        'HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCID' +
        'ENTAL, '
      
        'SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT' +
        ' '
      
        'LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF' +
        ' USE, '
      
        'DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND O' +
        'N ANY '
      
        'THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR T' +
        'ORT '
      
        '(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF TH' +
        'E USE '
      
        'OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAM' +
        'AGE.')
    ParentFont = False
    ReadOnly = True
    TabOrder = 0
    OnClick = memoAboutClick
    OnMouseEnter = memoAboutMouseEnter
  end
end
