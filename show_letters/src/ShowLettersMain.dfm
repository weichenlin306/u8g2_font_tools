object frmMain: TfrmMain
  Left = 470
  Top = 187
  BorderStyle = bsSingle
  Caption = 'Windows'#24050#23433#35037#23383#24418#27298#35222#24037#20855
  ClientHeight = 513
  ClientWidth = 496
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  OnClose = FormClose
  TextHeight = 15
  object pnlControlBar: TPanel
    Left = 0
    Top = 472
    Width = 496
    Height = 41
    Align = alBottom
    TabOrder = 0
    ExplicitTop = 471
    ExplicitWidth = 492
    object cbbxInstalledFonts: TComboBox
      Left = 28
      Top = 8
      Width = 237
      Height = 25
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = #24494#36575#27491#40657#39636
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object cbbxFontStyle: TComboBox
      Left = 271
      Top = 8
      Width = 61
      Height = 25
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = #24494#36575#27491#40657#39636
      Font.Style = []
      ItemIndex = 0
      ParentFont = False
      TabOrder = 1
      Text = #27161#28310#23383
      Items.Strings = (
        #27161#28310#23383
        #31895#39636#23383
        #26012#39636#23383
        #31895#26012#39636
        #24213#32218
        #21034#38500#32218)
    end
    object bbtnShow: TBitBtn
      Left = 379
      Top = 8
      Width = 84
      Height = 25
      Caption = #39023#31034#23383#24418
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = #24494#36575#27491#40657#39636
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = bbtnShowClick
    end
  end
end
