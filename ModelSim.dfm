object SimForm: TSimForm
  Left = 504
  Top = 201
  Width = 597
  Height = 321
  Caption = 'Model Simulation'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 20
  object GroupBox1: TGroupBox
    Left = 8
    Top = 0
    Width = 201
    Height = 249
    Caption = 'Model Parameters'
    TabOrder = 0
    object leParR: TLabeledEdit
      Left = 112
      Top = 56
      Width = 73
      Height = 28
      EditLabel.Width = 97
      EditLabel.Height = 20
      EditLabel.Caption = 'Particle R  (A)'
      LabelPosition = lpLeft
      LabelSpacing = 3
      TabOrder = 0
      Text = '2.75'
    end
    object leParNum: TLabeledEdit
      Left = 112
      Top = 24
      Width = 73
      Height = 28
      EditLabel.Width = 97
      EditLabel.Height = 20
      EditLabel.Caption = 'Particle Nums'
      LabelPosition = lpLeft
      LabelSpacing = 3
      TabOrder = 1
      Text = '216'
    end
    object leVis: TLabeledEdit
      Left = 112
      Top = 120
      Width = 73
      Height = 28
      EditLabel.Width = 94
      EditLabel.Height = 20
      EditLabel.Caption = 'Viscosity (cP)'
      LabelPosition = lpLeft
      LabelSpacing = 3
      TabOrder = 2
      Text = '1'
    end
    object leTemp: TLabeledEdit
      Left = 112
      Top = 152
      Width = 73
      Height = 28
      EditLabel.Width = 93
      EditLabel.Height = 20
      EditLabel.Caption = 'T              (K)  '
      LabelPosition = lpLeft
      LabelSpacing = 3
      TabOrder = 3
      Text = '301'
    end
    object leRunStps: TLabeledEdit
      Left = 112
      Top = 184
      Width = 73
      Height = 28
      EditLabel.Width = 92
      EditLabel.Height = 20
      EditLabel.Caption = 'Run Steps    '
      LabelPosition = lpLeft
      LabelSpacing = 3
      TabOrder = 4
      Text = '5000'
    end
    object leCollectStps: TLabeledEdit
      Left = 112
      Top = 216
      Width = 73
      Height = 28
      EditLabel.Width = 90
      EditLabel.Height = 20
      EditLabel.Caption = 'CollectSteps'
      LabelPosition = lpLeft
      LabelSpacing = 3
      TabOrder = 5
      Text = '100'
    end
    object leInitBoundary: TLabeledEdit
      Left = 112
      Top = 88
      Width = 73
      Height = 28
      EditLabel.Width = 98
      EditLabel.Height = 20
      EditLabel.Caption = 'Init Boundary '
      ImeMode = imSAlpha
      LabelPosition = lpLeft
      LabelSpacing = 3
      TabOrder = 6
      Text = '50'
    end
  end
  object bRun: TButton
    Left = 8
    Top = 250
    Width = 121
    Height = 25
    Caption = 'Run'
    TabOrder = 1
    OnClick = bRunClick
  end
  object mMemo: TMemo
    Left = 216
    Top = 0
    Width = 361
    Height = 273
    TabOrder = 2
  end
end
