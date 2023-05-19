object frmMain: TfrmMain
  Left = 572
  Top = 166
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'U8g2 Font Tools'
  ClientHeight = 496
  ClientWidth = 650
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  TextHeight = 15
  object pgctlMain: TPageControl
    Left = 0
    Top = 8
    Width = 649
    Height = 495
    ActivePage = tshtEncode
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = #24494#36575#27491#40657#39636
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object tshtEncode: TTabSheet
      Caption = 'U8g2'#32232#30908
      object imgPreviewEnc: TImage
        Left = 313
        Top = 87
        Width = 320
        Height = 320
      end
      object lblFontCheck: TLabel
        Left = 313
        Top = 64
        Width = 65
        Height = 17
        Caption = #23383#24418#38928#35261#65306
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
      end
      object FontFaceToSave: TLabel
        Left = 6
        Top = 6
        Width = 125
        Height = 17
        Caption = 'U8g2'#23383#39636#21517#31281#35373#23450#65306
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
      end
      object lblFontDimEnc: TLabel
        Left = 384
        Top = 62
        Width = 4
        Height = 19
      end
      object lblZoomRatioEnc: TLabel
        Left = 515
        Top = 60
        Width = 29
        Height = 17
        Caption = #20493#29575':'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
      end
      object ledtSaveToFile: TLabeledEdit
        Left = 304
        Top = 26
        Width = 289
        Height = 25
        EditLabel.Width = 112
        EditLabel.Height = 17
        EditLabel.Caption = #20786#23384'U8g2'#23383#22411#27284#65306
        EditLabel.Font.Charset = ANSI_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -13
        EditLabel.Font.Name = #24494#36575#27491#40657#39636
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        Text = ''
      end
      object bbtnSaveToFile: TBitBtn
        Left = 599
        Top = 24
        Width = 34
        Height = 28
        Hint = #20786#23384'U8g2'#23383#24418#27284
        Glyph.Data = {
          360C0000424D360C000000000000360000002800000020000000200000000100
          180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEFEFEFEFEFEFEFEFE
          FEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFB
          FBFBEFEFEFD2D7D9BAC4CACDCFD0EAEAEAFBFBFBFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBEFEFEFCFD5D9A6BDCB81
          A9C166A0C261A6CE518CB74D82AC919CA6D1D1D1F3F3F3FEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFBFBFBEEEEEECCD4D9A3BBCB7EA7C0649FC25EA4CC61A9D163
          ADD463AED565A7CD4C8BB681CFF1658DABBCBCBCE5E5E5FAFAFAFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBEEEE
          EECAD3D9A1BACA7CA6C0629EC25FA5CD62ABD264AFD565AFD664AFD664AED664
          AED565AED55B95BC559AC47CCFF36AA0C3D8D8D8EEEEEEFBFBFBFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBEDEDEDC8D2D99FB9CA7AA5C0629F
          C360A8CF65AED567B3D967B3D967B2D866B1D866B1D765B0D765AFD665AFD664
          AFD66FB4D84983AD5BA4CD79CBF072ADD1D9DEE3F9F9F9FEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFFDFDFDCBD6DD9CB8CA78A4C062A1C663ACD268B3D96CB9DE6CB9
          DE6BB7DD6AB6DC69B5DB68B4DA68B3D967B2D966B2D866B1D765B0D765B0D665
          AFD66CA9CD4686B060AAD276C8ED7CBCDDC7D5E0FFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFB1CCDD64ACD26EBADF72C2E572C1E470BFE36FBDE16EBCE06DBA
          DF6DBADF6DBADF6DBADF6CBADE6CB9DE6BB7DD6AB6DB68B4DA67B2D866B0D767
          B1D75E94B84E93BC65B1D874C6EB88CBEAAAC1D3FFFFFFFFFFFFFEFEFEFFFFFF
          FFFFFFFDFEFE73B2D378C8EB76C6E975C4E873C3E673C2E574C4E776C6E877C7
          E976C7E975C5E874C4E673C2E572C1E471BFE370BEE26FBDE16EBBE06DB9DE7C
          BEDF447FA8549CC568B4DB73C4E992D8F68DACC4FFFFFFFEFEFEFEFEFEFFFFFF
          FFFFFFD5E6EF71BFE279CBED78C9EB79CAEC7BCDEE7CCFEF7CCDEE7ACCED79CB
          EC78C9EB77C8EA76C6E975C5E774C4E673C2E572C1E471BFE370BEE26FBCE177
          B1D04485AE5AA3CB6CB8DE74C3E88ED9F87FA5C0FFFFFFFEFEFEFEFEFEFFFFFF
          FFFFFFA3CEE37BCEEF7DD0F080D4F381D4F480D3F37FD1F27ED0F17CCFEF7BCD
          EE7ACCED79CBEC78C9EB77C8EA76C6E975C5E774C3E673C2E572C1E474C1E362
          95B65096BF63ADD371BCE176C4E789D8F876A2BFFFFFFFFFFFFFFEFEFEFFFFFF
          FBFCFD7EC6E583D8F785DAF884D8F783D7F682D6F581D4F480D3F37ED1F27DD0
          F07CCEEF7BCDEE7ACCED79CAEC78C9EB77C8EA76C6E875C5E774C3E68BCAE742
          7EA558A0C76DB7DB75C1E379C5E789D9F875A7C5F3F5F7FEFEFEFEFEFEFFFFFF
          D5EBF483D8F688DEFC87DDFB86DBF985DAF884D8F783D7F682D5F580D4F47FD3
          F37ED1F17DD0F07CCEEF7BCDEE7ACCED79CAEC78C9EB77C7E976C6E87FB4D04A
          8DB45DA5CB78C1E27AC4E57BC6E78EDCF885B9D5D3DDE5FEFEFEFEFEFEFFFFFF
          ADE1F48AE1FE8AE1FE89DFFD88DEFC87DCFA86DBF985DAF883D8F782D7F681D5
          F580D4F47FD2F27ED1F17DD0F07CCEEF7BCDEE7ACCED79CAEC7ECBEB6293B15E
          A9C35FA9C680C9E77EC8E77DC7E792DFF996CCE4B3C6D4FEFEFEFEFEFEF9FBFC
          8DDEFA8BE2FF8BE2FF8BE2FF8AE1FE89DFFD88DEFB87DCFA86DBF984D9F883D8
          F782D7F681D5F580D4F37FD2F27ED1F17DCFF07CCEEF7BCDEE97D3ED437FA36F
          DDA234BC5D6CBBC183CCE982CBE995E0F8AAE1F492AEC3FEFEFEFEFEFED7F1F9
          90E5FF8DE3FF8BE2FF8BE2FF8BE2FF8BE2FF8AE0FE89DFFD88DEFB86DCFA85DB
          F984D9F883D8F782D6F681D5F480D4F37FD2F27ED1F17DCFF087B9D35BAEA175
          F3964FEF7931AE596EB6C486CEEA97E1F7B0EAFB80A2BAFEFEFEFEFEFEB6EBF9
          98E8FF93E6FF8EE4FF8BE2FF8BE2FF8BE2FF8BE2FF8BE1FF8AE0FE88DFFC87DD
          FB86DCFA85DBF984D9F883D8F782D6F681D5F480D3F388D5F373ACB878EC9A72
          F2944FEF794EED78309F5973B6C998E0F5A9E9FB7BA1B9FEFEFEF7FBFBA3ECFC
          9FECFF9AE9FF93E6FF8DE3FF8BE2FF8BE2FF8BE2FF8BE2FF8BE2FF8AE1FE89E0
          FD88DFFC87DDFB86DCFA85DAF984D9F883D8F782D6F5A9DDF166D79679F3996E
          F2914FEF794FEF794CE97532925D7EBDD0A1E6F77AA2BAFBFCFCDCF5F9AAF1FF
          A5EFFF9FECFF98E9FF90E5FF8BE2FF8CE2FF9CE6FFA5E5FC9CDAF493D0ED8CE2
          FF8AE1FE89E0FD88DFFC88DDFB97E1FBA4E0F79FD4ED71CBAE6AF28D76F3966A
          F28E4FEF794FEF794FEF7949E2713A8B677FBBCE85AEC4DFE6EBC5F3F9B1F5FF
          AAF1FFACF0FFB1EDFEA4E2F998D6F297CDE98FCFEA8ED4ED9EE0F2A4E9F998E5
          FFA8E8FDA1DDF597D1EC96CBE68DC9E487CAE487D5D55BED8366F28B72F39465
          F18A4FEF794FEF794FEF794FEF7944D86B4085706D99AFBDCDD8C0ECF6AAE4F8
          9DD7F1A1D4EEA8D6EDC6E3F1E7F1F6FEFFFFBDF0F6B6F3FBB5F3FAB6F3FAA6DF
          F18CD0EA9ADDF0ABEAF6B3F1F9B3F0F9B0EFF662EA9157F07F63F1886FF29161
          F1874FEF794FEF794FEF794FEF794FEF793FC9634779749EB8C9F1F7FADFEEF6
          FBFCFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCDF4F8C0F7FCBFF7FCBCF6FCB9F6
          FCB8F5FCB8F5FCB8F5FCB9F6FCBBF7FD83ECB94EEE7853EF7C5FF0856BF18E5C
          EF834EED774DED7745DE6E2DB05025A24625984A318E5F99B7C6FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDEF7F9CDFBFECDFBFECAFBFEC6FB
          FEC1FBFFBFFBFFB9F6F8BFFBFFA7F2E24AE47448E5714BE57356E77C62E88552
          E57947E37047E3702FB552277B47799BA498B0BFB7CAD6F7F9FAFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEAF8F9DAFDFFD6FBFDB7EEDF90E1
          BC6CD59952CE7B176D2E8ECBC156C88229B34D2CB15040D8684CDC7259DE7C46
          DA6D40D96840D868219D4269A77AFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCEECD95FCF8247CE6B4CD97159E4
          7E68EF8B70F4922D8444216439B2EEEEA4E7DEACEBD83ACF6242D1684DD4713A
          CE6139CD6037C75D127E2F91BD9DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8BEBA46AF28E6FF39274F49578F5
          997CF59C7FF69E73E591125B25408360BDF8FC78D9A333C45937C55D3CC66130
          BB552DB45124A2450C7127D1E2D5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0F8E67CF49C7FF69E80F69F7FF6
          9E7BF69B77F59873F5954FC26D0B541E4884612FAE5225AD4924A646219B411E
          983E1E983E0F752A3D8B51FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFB9F4C978F69973F5956FF4
          926BF48F67F38C63F2895FF28538AF580B561F0E662510782C107C2C107E2D10
          7B2C0D6F270E6E27C3D9C9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFE9DF1B363F2895FF2
          865BF18357F17F53EF7C4FED794CE97533B4550F61240B5C200B60210B63230B
          66230F6A279BC0A4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFEFDA6F1BA51EC
          7A4CEA7549E57245E06E42DA6A3FD5663BD06334C25A20923E1677302D794160
          986FCCDDD0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEFEFEFEFEFEFEDCF5
          E386E29E4DD1703CC96153CC7372D38C93DBA6B4E4C0D5EEDCF5FAF7FEFEFEFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        OnClick = bbtnSaveToFileClick
      end
      object edtFontFaceToSave: TEdit
        Left = 6
        Top = 26
        Width = 288
        Height = 25
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ImeMode = imClose
        ParentFont = False
        TabOrder = 2
      end
      object bbtnEncodePreview: TBitBtn
        Left = 590
        Top = 419
        Width = 43
        Height = 30
        Hint = #38928#35261#27442#32232#30908#23383#23383#27171
        Caption = #38928#35261
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
        OnClick = bbtnEncodePreviewClick
      end
      object cbbxInstalledFonts: TComboBox
        Left = 313
        Top = 423
        Width = 107
        Height = 24
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 4
        OnChange = cbbxInstalledFontsChange
      end
      object cbbxFontSize: TComboBox
        Left = 480
        Top = 423
        Width = 38
        Height = 24
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        OnChange = cbbxFontSizeChange
      end
      object rbtnTest1: TRadioButton
        Left = 524
        Top = 417
        Width = 32
        Height = 17
        Checked = True
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 6
        TabStop = True
      end
      object rbtnTest2: TRadioButton
        Left = 524
        Top = 436
        Width = 32
        Height = 17
        Caption = 'A'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
      object rbtnTest3: TRadioButton
        Left = 557
        Top = 417
        Width = 27
        Height = 17
        Caption = 'g'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 8
      end
      object rbtnTest4: TRadioButton
        Left = 557
        Top = 436
        Width = 27
        Height = 17
        Caption = '('
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 9
      end
      object pgctlGlyphsToEncode: TPageControl
        Left = 6
        Top = 60
        Width = 298
        Height = 389
        ActivePage = tshtGlyphsToEncode
        TabOrder = 10
        object tshtGlyphsToEncode: TTabSheet
          Caption = #27442#32232#30908#23383
          object memoGlyphsForEncoding: TMemo
            Left = 1
            Top = 0
            Width = 289
            Height = 316
            Hint = #21487#21152#20837#20219#24847#25991#23383#65292#21487#37325#35079
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Consolas'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ScrollBars = ssVertical
            ShowHint = True
            TabOrder = 0
          end
          object cbxIncludeASCII: TCheckBox
            Left = 9
            Top = 330
            Width = 177
            Height = 17
            Caption = #21253#21547'ASCII 32-126'#23383#20803
            Checked = True
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = #24494#36575#27491#40657#39636
            Font.Style = []
            ParentFont = False
            State = cbChecked
            TabOrder = 1
          end
          object bbtnTidyUpGlyphs: TBitBtn
            Left = 212
            Top = 324
            Width = 65
            Height = 27
            Hint = #25972#29702#19981#37325#35079#24453#32232#30908#23383#20006#25918#20837#20633#22949#32232#30908#23383#21312#22602#20839
            Caption = #25972#29702
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = #24494#36575#27491#40657#39636
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 2
            OnClick = bbtnTidyUpGlyphsClick
          end
        end
        object tshtGlyphMap: TTabSheet
          Caption = #24453#32232#30908#23383
          ImageIndex = 1
          object lblTotalCountToEncode: TLabel
            Left = 3
            Top = 335
            Width = 52
            Height = 17
            Caption = #32317#23383#25976#65306
            Font.Charset = ANSI_CHARSET
            Font.Color = clMaroon
            Font.Height = -13
            Font.Name = #24494#36575#27491#40657#39636
            Font.Style = []
            ParentFont = False
          end
          object memoGlyphMap: TMemo
            Left = 1
            Top = 0
            Width = 289
            Height = 329
            Hint = #21487#20219#36984#27492#21312#20839#20219#19968#21934#23383#20006#38928#35261
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Consolas'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ReadOnly = True
            ScrollBars = ssVertical
            ShowHint = True
            TabOrder = 0
            OnMouseUp = memoGlyphMapMouseUp
          end
        end
      end
      object cbbxZoomRatioEnc: TComboBox
        Left = 551
        Top = 57
        Width = 42
        Height = 25
        DropDownCount = 10
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 11
        Items.Strings = (
          '1'
          '2'
          '3')
      end
      object cbbxFontStyle: TComboBox
        Left = 426
        Top = 423
        Width = 48
        Height = 24
        Hint = #27161#28310
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 12
        OnChange = cbbxFontStyleChange
        Items.Strings = (
          #27161#28310
          #31895#39636
          #26012#39636
          #31895#26012#39636
          #24213#32218
          #21034#38500#32218)
      end
    end
    object tshtDecode: TTabSheet
      Caption = 'U8g2'#35299#30908
      ImageIndex = 1
      object imgPreviewDec: TImage
        Left = 313
        Top = 87
        Width = 320
        Height = 320
      end
      object lblZoomRatioDec: TLabel
        Left = 560
        Top = 61
        Width = 29
        Height = 17
        Caption = #20493#29575':'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
      end
      object lblFontFaceToView: TLabel
        Left = 351
        Top = 4
        Width = 91
        Height = 17
        Caption = #30446#21069#34389#29702#23383#22411#65306
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
      end
      object lblFontDimDec: TLabel
        Left = 303
        Top = 61
        Width = 3
        Height = 17
        Font.Charset = ANSI_CHARSET
        Font.Color = clMaroon
        Font.Height = -13
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
      end
      object bbtnDecodePreview: TBitBtn
        Left = 552
        Top = 416
        Width = 80
        Height = 36
        Hint = #27298#35222#20839#21547#25991#23383#22294#27171
        Caption = #27298#35222
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        OnClick = bbtnDecodePreviewClick
      end
      object ledtTestGlyphToDecode: TLabeledEdit
        Left = 453
        Top = 422
        Width = 29
        Height = 27
        EditLabel.Width = 48
        EditLabel.Height = 27
        EditLabel.Caption = #27298#35222#23383':'
        EditLabel.Font.Charset = ANSI_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -15
        EditLabel.Font.Name = #24494#36575#27491#40657#39636
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        LabelPosition = lpLeft
        MaxLength = 1
        ParentFont = False
        ReadOnly = True
        TabOrder = 1
        Text = ''
      end
      object cbbxZoomRatioDec: TComboBox
        Left = 592
        Top = 57
        Width = 41
        Height = 25
        DropDownCount = 10
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ItemIndex = 0
        ParentFont = False
        TabOrder = 2
        Text = '1'
        Items.Strings = (
          '1'
          '2'
          '3')
      end
      object bbtnSelectFile: TBitBtn
        Left = 303
        Top = 23
        Width = 34
        Height = 28
        Hint = #36617#20837'U8g2'#23383#24418#27284
        Glyph.Data = {
          360C0000424D360C000000000000360000002800000020000000200000000100
          180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEFEFEFEFEFEFEFEFE
          FEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFB
          FBFBEFEFEFD2D7D9BAC4CACDCFD0EAEAEAFBFBFBFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBEFEFEFCFD5D9A6BDCB81
          A9C166A0C261A6CE518CB74D82AC919CA6D1D1D1F3F3F3FEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFBFBFBEEEEEECCD4D9A3BBCB7EA7C0649FC25EA4CC61A9D163
          ADD463AED565A7CD4C8BB681CFF1658DABBCBCBCE5E5E5FAFAFAFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBEEEE
          EECAD3D9A1BACA7CA6C0629EC25FA5CD62ABD264AFD565AFD664AFD664AED664
          AED565AED55B95BC559AC47CCFF36AA0C3D8D8D8EEEEEEFBFBFBFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBEDEDEDC8D2D99FB9CA7AA5C0629F
          C360A8CF65AED567B3D967B3D967B2D866B1D866B1D765B0D765AFD665AFD664
          AFD66FB4D84983AD5BA4CD79CBF072ADD1D9DEE3F9F9F9FEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFFDFDFDCBD6DD9CB8CA78A4C062A1C663ACD268B3D96CB9DE6CB9
          DE6BB7DD6AB6DC69B5DB68B4DA68B3D967B2D966B2D866B1D765B0D765B0D665
          AFD66CA9CD4686B060AAD276C8ED7CBCDDC7D5E0FFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFB1CCDD64ACD26EBADF72C2E572C1E470BFE36FBDE16EBCE06DBA
          DF6DBADF6DBADF6DBADF6CBADE6CB9DE6BB7DD6AB6DB68B4DA67B2D866B0D767
          B1D75E94B84E93BC65B1D874C6EB88CBEAAAC1D3FFFFFFFFFFFFFEFEFEFFFFFF
          FFFFFFFDFEFE73B2D378C8EB76C6E975C4E873C3E673C2E574C4E776C6E877C7
          E976C7E975C5E874C4E673C2E572C1E471BFE370BEE26FBDE16EBBE06DB9DE7C
          BEDF447FA8549CC568B4DB73C4E992D8F68DACC4FFFFFFFEFEFEFEFEFEFFFFFF
          FFFFFFD5E6EF71BFE279CBED78C9EB79CAEC7BCDEE7CCFEF7CCDEE7ACCED79CB
          EC78C9EB77C8EA76C6E975C5E774C4E673C2E572C1E471BFE370BEE26FBCE177
          B1D04485AE5AA3CB6CB8DE74C3E88ED9F87FA5C0FFFFFFFEFEFEFEFEFEFFFFFF
          FFFFFFA3CEE37BCEEF7DD0F080D4F381D4F480D3F37FD1F27ED0F17CCFEF7BCD
          EE7ACCED79CBEC78C9EB77C8EA76C6E975C5E774C3E673C2E572C1E474C1E362
          95B65096BF63ADD371BCE176C4E789D8F876A2BFFFFFFFFFFFFFFEFEFEFFFFFF
          FBFCFD7EC6E583D8F785DAF884D8F783D7F682D6F581D4F480D3F37ED1F27DD0
          F07CCEEF7BCDEE7ACCED79CAEC78C9EB77C8EA76C6E875C5E774C3E68BCAE742
          7EA558A0C76DB7DB75C1E379C5E789D9F875A7C5F3F5F7FEFEFEFEFEFEFFFFFF
          D5EBF483D8F688DEFC87DDFB86DBF985DAF884D8F783D7F682D5F580D4F47FD3
          F37ED1F17DD0F07CCEEF7BCDEE7ACCED79CAEC78C9EB77C7E976C6E87FB4D04A
          8DB45DA5CB78C1E27AC4E57BC6E78EDCF885B9D5D3DDE5FEFEFEFEFEFEFFFFFF
          ADE1F48AE1FE8AE1FE89DFFD88DEFC87DCFA86DBF985DAF883D8F782D7F681D5
          F580D4F47FD2F27ED1F17DD0F07CCEEF7BCDEE7ACCED79CAEC7ECBEB6293B160
          A7CC65ADD180C9E77EC8E77DC7E792DFF996CCE4B3C6D4FEFEFEFEFEFEF9FBFC
          8DDEFA8BE2FF8BE2FF8BE2FF8AE1FE89DFFD88DEFB87DCFA86DBF984D9F883D8
          F782D7F681D5F580D4F37FD2F27ED1F17DCFF07CCEEF7BCDEE97D3ED4380A46C
          B3D571B8D985CDE983CCE982CBE995E0F8AAE1F492AEC3FEFEFEFEFEFED7F1F9
          90E5FF8DE3FF8BE2FF8BE2FF8BE2FF8BE2FF8AE0FE89DFFD88DEFB86DCFA85DB
          F984D9F883D8F782D6F681D5F480D4F37FD2F27ED1F17DCFF087B9D3599CBE72
          B9D97CC3E08AD2EC89D1EB87CFEB97E1F7B0EAFB80A2BAFEFEFEFEFEFEB6EBF9
          98E8FF93E6FF8EE4FF8BE2FF8BE2FF8BE2FF8BE2FF8BE1FF8AE0FE88DFFC87DD
          FB86DCFA85DBF984D9F883D8F782D6F681D5F480D3F388D5F36FA0BE78BDDA7A
          C0DD89CEE890D6EE8ED5ED8DD4ED9AE2F7A9E9FB7BA1B9FEFEFEF7FBFBA3ECFC
          9FECFF9AE9FF93E6FF8DE3FF8BE2FF8BE2FF8BE2FF8BE2FF8BE2FF8AE1FE89E0
          FD88DFFC87DDFB86DCFA85DAF984D9F883D8F782D6F5A9DCF25D9EC289CDE58E
          D1E898DCF097DBF095DAEF93D8EF9DE3F6A4EAFB7AA2BAFBFCFCDCF5F9AAF1FF
          A5EFFF9FECFF98E9FF90E5FF8BE2FF8CE2FF9CE6FFA5E5FC9CDAF493D0ED8CE2
          FF8AE1FE89E0FD88DFFC88DDFB97E1FBA4E0F79FD4ED74AED194C9E781B1DF96
          D4E99BDDEE9DE0F29BDFF299DDF1A1E6F7A7ECFC8BB5CADFE6EBC5F3F9B1F5FF
          A8EDFD4D60D3678FE09DD5F398D6F297CDE98FCFEA8ED4ED9EE0F2A4E9F998E5
          FFA8E8FDA1DDF597D1EC92C2E385AFE17CA0E3899EED989EF89B9DFF2A2BB961
          89C295D3E2A4E5F4A3E5F5A5E7F6ACEFFBACEFFCA1CBDCBDCDD8C0ECF6AAE4F8
          8EB2EA3D3DD22F2FC82D30C4646ECDD9D9F0BDF0F6B6F3FBB5F3FAB6F3FAA6DF
          F18CD0EA9ADDF09DC7F19698FE9496FF9193FF8E8FFF8B8CFF8889FF2C2CBD58
          79BC99D3DEB8F6FDB7F6FEB5F5FDB3F3FDB1F2FDBAE4EF9EB8C9F1F7FADFEEF6
          DBDDF7ACAEFF7A7CEE4647D22727C02424BC4B5AC690B8E4BEF6FCBCF6FCB9F6
          FCB8F5FCB8F5FCA4CCF78385FF8082FF7D7EFF7A7BFF7778FF7475FF2324BA53
          71B794CBD6BEFAFFBCF9FEBAF8FEB8F6FEB6F5FDCBECF49AB8CAFFFFFFFFFFFF
          DCDDF9BDBEFFA6A8FF9598FF7E80F34B4BD42121B91B1BB4343CBA7E9DD9BFF2
          FAC1FBFFBFFBFFA0C6F97070FF6D6DFF6A6AFE6767FD6464FC6161FA1515AE55
          73B395C3CFC2F1F8C0E9F2C8E8F1D0E7EFD8E7EECEDDE6F7F9FAFFFFFFFFFFFF
          D6D7F9ADAFFFB2B3FF989AFF8D8FFF8B8DFF7F81F84F4FD81F1FB61313AC1E21
          AD657FCDAEE3F38FAFF65D5DF65A5AF35858F15555EE5252EB4949E10808A19E
          A8D3DFEAF0F4F7F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          CFCFF99C9EFFA2A3FFA5A6FF8B8DFF8486FF8384FF8283FF7C7DFC5253DD2020
          B50B0BA40C0DA12C32BB4141DA4848E14646DF4343DB3C3CD22424BA07079EED
          EDF6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          C7C8F98A8BFF9091FF9697FF9798FF8081FF7C7DFF7B7CFF797AFF7879FF7677
          FF5656E42323B903039C1E1EB73636CF3232C92B2BBE2727BA08089F3E3EB0FF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          C3C3F97C7DFF7D7EFF8384FF898AFF898AFF7676FF7474FF7273FF7171FF6F6F
          FE5C5CE75353E33738CE2323BB2020B21B1BAB1919AA0D0D9F000096A7A7D8FF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          C0C0F97475FF7374FF7273FF7576FF7C7CFF7A7AFF6C6DFF6B6BFF6A6AFF5252
          E106067C02027C0D0D950E0E9B0D0D9B0A0A990505940000913C3CAAFDFDFEFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          C3C3F88D8DF9A7A7F9B3B3F86B6BFD6969FE6C6CFE6B6BFC6161F95E5EF75B5B
          F44848DB0E0E8B00007B00007F00008400008700008B29299FE6E6F3FFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFCECEF86565F15757F05656ED5656EB4F4FE74B4B
          E44848E14343DB2626B30F0F940A0A892828967D7DC0F2F2F8FFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0F0FBA1A1EC6262DE4343D64949D56767
          DA8383DF9F9FE5BEBEEBDDDDF2FAFAFCFEFEFEFEFEFEFFFFFFFEFEFEFEFEFEFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
        OnClick = bbtnSelectFileClick
      end
      object ledtSelectFile: TLabeledEdit
        Left = 8
        Top = 26
        Width = 289
        Height = 25
        EditLabel.Width = 112
        EditLabel.Height = 17
        EditLabel.Caption = #36984#25799'U8g2'#23383#22411#27284#65306
        EditLabel.Font.Charset = ANSI_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -13
        EditLabel.Font.Name = #24494#36575#27491#40657#39636
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        Text = ''
      end
      object cbbxFontList: TComboBox
        Left = 351
        Top = 26
        Width = 282
        Height = 25
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        OnChange = cbbxFontListChange
      end
      object cbxClearFonts: TCheckBox
        Left = 183
        Top = 5
        Width = 119
        Height = 17
        Caption = #28165#38500#24050#36617#20837#23383#22411
        Checked = True
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        State = cbChecked
        TabOrder = 6
      end
      object pgctlGlyphsToDecode: TPageControl
        Left = 6
        Top = 60
        Width = 298
        Height = 389
        ActivePage = tshtGlyphsToDecode
        TabOrder = 7
        object tshtGlyphsToDecode: TTabSheet
          Caption = #20839#21547#25991#23383
          object lblTotalCountDecoded: TLabel
            Left = 3
            Top = 335
            Width = 52
            Height = 17
            Caption = #32317#23383#25976#65306
            Font.Charset = ANSI_CHARSET
            Font.Color = clMaroon
            Font.Height = -13
            Font.Name = #24494#36575#27491#40657#39636
            Font.Style = []
            ParentFont = False
          end
          object memoGlyphContent: TMemo
            Left = 1
            Top = 0
            Width = 289
            Height = 329
            Hint = #21487#20219#36984#27492#21312#20839#20219#19968#21934#23383#20006#27298#35222
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Consolas'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ReadOnly = True
            ScrollBars = ssVertical
            ShowHint = True
            TabOrder = 0
            OnMouseUp = memoGlyphContentMouseUp
          end
        end
        object tshtGlyphArray: TTabSheet
          Caption = #23383#22411#38499#21015
          ImageIndex = 1
          object memoArrayList: TMemo
            Left = 1
            Top = 0
            Width = 289
            Height = 314
            Hint = #21487#20219#36984#27492#21312#20839#20219#19968#21934#23383#20006#27298#35222
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Consolas'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ReadOnly = True
            ScrollBars = ssVertical
            ShowHint = True
            TabOrder = 0
            OnMouseUp = memoGlyphMapMouseUp
          end
          object bbtnPrintGlyphArray: TBitBtn
            Left = 169
            Top = 321
            Width = 90
            Height = 30
            Hint = 'Ctrl-Click'#37325#26032#21015#21360
            Caption = #21015#21360#38499#21015
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = #24494#36575#27491#40657#39636
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            OnClick = bbtnPrintGlyphArrayClick
          end
          object ledtFind: TLabeledEdit
            Left = 84
            Top = 325
            Width = 34
            Height = 25
            EditLabel.Width = 39
            EditLabel.Height = 25
            EditLabel.Caption = #23563#25214#65306
            EditLabel.Font.Charset = ANSI_CHARSET
            EditLabel.Font.Color = clWindowText
            EditLabel.Font.Height = -13
            EditLabel.Font.Name = #24494#36575#27491#40657#39636
            EditLabel.Font.Style = []
            EditLabel.ParentFont = False
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = #24494#36575#27491#40657#39636
            Font.Style = []
            LabelPosition = lpLeft
            ParentFont = False
            TabOrder = 2
            Text = ''
            OnChange = ledtFindChange
          end
        end
      end
    end
  end
  object pnlAbout: TPanel
    Left = 603
    Top = 5
    Width = 39
    Height = 24
    Caption = #38364#26044
    Font.Charset = ANSI_CHARSET
    Font.Color = clMaroon
    Font.Height = -15
    Font.Name = #27161#26999#39636
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = pnlAboutClick
  end
  object OpenDialog: TOpenDialog
    DefaultExt = 'h'
    Filter = '|*.h||*.c'
    Title = #38283#21855'U8g2'#23383#24418#27284
    Left = 488
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'h'
    Filter = '|*.h||*.c'
    Title = #20786#23384#23383#24418#27284
    Left = 560
  end
end
