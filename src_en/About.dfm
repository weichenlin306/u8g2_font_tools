object frmAbout: TfrmAbout
  Left = 391
  Top = 104
  BorderStyle = bsNone
  Caption = 'About'
  ClientHeight = 697
  ClientWidth = 539
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
    Left = 17
    Top = 16
    Width = 511
    Height = 665
    BevelOuter = bvNone
    BorderStyle = bsNone
    Color = clCream
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Segoe UI'
    Font.Style = []
    Lines.Strings = (
      'Program Name: U8g2 Font Tools (64-bit)'
      '    (https://github.com/weichenlin306/u8g2_font_tools)'
      
        'Compiler: Embarcadero C++ Builder 11 Alexandria Community Editio' +
        'n'
      'Author: Wei-Chen Lin (Kaohsiung, Taiwan)'
      'Release date: 2023-05-06'
      'Final revision date: '
      ''
      
        '* The copyright of U8g2 Font Tools belongs to Lin Weichen and ca' +
        'n be used based on'
      '    BSD dual-clause licensing specifications.'
      
        '    For details, see http://www.opensource.org/licenses/bsd-lice' +
        'nse.php'
      
        '* The U8g2 format is a glyph (graphic) format developed based on' +
        ' BDF (Glyph Bitmap'
      '    Distribution Format).'
      
        '    See https://github.com/olikraus/u8g2/wiki/u8g2fontformat for' +
        ' details.'
      ''
      
        '=======[[ U8g2 Font Tools Copyright Statement BSD-2-Clause Licen' +
        'se ]]======='
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
