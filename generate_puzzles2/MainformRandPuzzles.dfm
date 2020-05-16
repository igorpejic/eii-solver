object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Generation of Eternity II style hard puzzle input files'
  ClientHeight = 711
  ClientWidth = 984
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  DesignSize = (
    984
    711)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 39
    Top = 17
    Width = 65
    Height = 13
    Alignment = taRightJustify
    Caption = 'Puzzle size, N'
  end
  object Label2: TLabel
    Left = 33
    Top = 45
    Width = 71
    Height = 13
    Alignment = taRightJustify
    Caption = 'Border Colours'
  end
  object Label3: TLabel
    Left = 36
    Top = 71
    Width = 68
    Height = 13
    Alignment = taRightJustify
    Caption = 'Inside Colours'
  end
  object Label6: TLabel
    Left = 400
    Top = 17
    Width = 51
    Height = 13
    Caption = 'Log notes:'
  end
  object SpinEdit1: TSpinEdit
    Left = 110
    Top = 14
    Width = 43
    Height = 22
    MaxValue = 0
    MinValue = 0
    TabOrder = 0
    Value = 4
    OnChange = SpinEdit1Change
  end
  object Button1: TButton
    Left = 39
    Top = 163
    Width = 127
    Height = 28
    Caption = 'Generate Tiles'
    Enabled = False
    TabOrder = 1
    OnClick = Button1Click
  end
  object SpinEdit2: TSpinEdit
    Left = 110
    Top = 42
    Width = 43
    Height = 22
    MaxValue = 0
    MinValue = 0
    TabOrder = 2
    Value = 3
    OnChange = SpinEdit2Change
  end
  object SpinEdit3: TSpinEdit
    Left = 110
    Top = 68
    Width = 43
    Height = 22
    MaxValue = 0
    MinValue = 0
    TabOrder = 3
    Value = 8
    OnChange = SpinEdit3Change
  end
  object Button2: TButton
    Left = 36
    Top = 197
    Width = 127
    Height = 28
    Caption = 'Check Symmetry'
    Enabled = False
    TabOrder = 4
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 36
    Top = 231
    Width = 127
    Height = 28
    Caption = 'Check Duplicates'
    Enabled = False
    TabOrder = 5
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 36
    Top = 265
    Width = 127
    Height = 28
    Caption = 'Generate good puzzle'
    TabOrder = 6
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 36
    Top = 299
    Width = 127
    Height = 28
    Caption = 'Save puzzle to file'
    TabOrder = 7
    OnClick = Button5Click
  end
  object GroupBox1: TGroupBox
    Left = 213
    Top = 156
    Width = 163
    Height = 133
    Caption = 'Automatic generation for all N'
    TabOrder = 8
    object Label4: TLabel
      Left = 12
      Top = 29
      Width = 75
      Height = 13
      Alignment = taRightJustify
      Caption = '# puzzles per N'
    end
    object Button6: TButton
      Left = 21
      Top = 62
      Width = 127
      Height = 43
      Caption = 'Generate full sequence'
      TabOrder = 0
      OnClick = Button6Click
    end
    object SpinEdit4: TSpinEdit
      Left = 93
      Top = 26
      Width = 55
      Height = 22
      MaxValue = 0
      MinValue = 0
      TabOrder = 1
      Value = 10
    end
  end
  object CheckBox2: TCheckBox
    Left = 39
    Top = 110
    Width = 126
    Height = 17
    Caption = 'Add 2x2 solutions'
    Checked = True
    State = cbChecked
    TabOrder = 9
  end
  object GroupBox2: TGroupBox
    Left = 213
    Top = 17
    Width = 163
    Height = 133
    Caption = 'Automatic generation for N'
    Color = clBtnFace
    ParentBackground = False
    ParentColor = False
    TabOrder = 10
    object Label5: TLabel
      Left = 41
      Top = 29
      Width = 46
      Height = 13
      Alignment = taRightJustify
      Caption = '# puzzles'
    end
    object Button7: TButton
      Left = 21
      Top = 70
      Width = 127
      Height = 43
      Caption = 'Generate for this N'
      TabOrder = 0
      OnClick = Button7Click
    end
    object SpinEdit5: TSpinEdit
      Left = 93
      Top = 26
      Width = 55
      Height = 22
      MaxValue = 0
      MinValue = 0
      TabOrder = 1
      Value = 100
    end
  end
  object CheckBox4: TCheckBox
    Left = 39
    Top = 133
    Width = 126
    Height = 17
    Caption = 'Randomize tiles'
    Checked = True
    State = cbChecked
    TabOrder = 11
  end
  object Memo1: TMemo
    Left = 400
    Top = 33
    Width = 576
    Height = 294
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 12
  end
  object Chart1: TChart
    Left = 8
    Top = 333
    Width = 968
    Height = 378
    Legend.Visible = False
    Title.Text.Strings = (
      'Colour distribution - frame colours first then inner colours')
    View3D = False
    TabOrder = 13
    Anchors = [akLeft, akTop, akRight, akBottom]
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object Series1: TBarSeries
      Marks.Visible = False
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Bar'
      YValues.Order = loNone
      Data = {
        00060000000000000000D066400000000000B06D400000000000587640000000
        0000E06F4000000000006870400000000000707740}
      Detail = {0000000000}
    end
  end
end
