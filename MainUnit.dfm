object MainForm: TMainForm
  Left = 215
  Top = 186
  Width = 676
  Height = 719
  Caption = '3D Animation'
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = pmMenu
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 18
  object iDisplay: TImage
    Left = 0
    Top = 0
    Width = 665
    Height = 657
  end
  object gbObsParam: TGroupBox
    Left = 8
    Top = 8
    Width = 313
    Height = 129
    Caption = 'Observation Site'
    TabOrder = 0
    Visible = False
    object Label2: TLabel
      Left = 18
      Top = 31
      Width = 87
      Height = 18
      Caption = 'X-Y Plan Phi'
    end
    object Label3: TLabel
      Left = 16
      Top = 64
      Width = 86
      Height = 18
      Caption = 'Z Axis Theta'
    end
    object Label4: TLabel
      Left = 120
      Top = 96
      Width = 28
      Height = 18
      Caption = 'Site'
    end
    object lSite: TLabel
      Left = 160
      Top = 96
      Width = 137
      Height = 25
      Alignment = taCenter
      AutoSize = False
      Caption = '(0,0)  deg.'
    end
    object sbPhi: TScrollBar
      Left = 120
      Top = 32
      Width = 177
      Height = 25
      Hint = '0 (deg)'
      LargeChange = 10
      Max = 360
      PageSize = 0
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnChange = sbObsChange
    end
    object sbTheta: TScrollBar
      Left = 120
      Top = 64
      Width = 177
      Height = 25
      Hint = '0 (deg)'
      LargeChange = 10
      Max = 180
      Min = -180
      PageSize = 0
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnChange = sbObsChange
    end
    object bChangeObs: TButton
      Left = 8
      Top = 96
      Width = 105
      Height = 25
      Caption = 'Action'
      TabOrder = 2
      OnClick = bChangeObsClick
    end
  end
  object sbFramePos: TScrollBar
    Left = 0
    Top = 656
    Width = 665
    Height = 25
    LargeChange = 5
    PageSize = 0
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    OnChange = sbFramePosChange
  end
  object pmMenu: TPopupMenu
    Left = 440
    Top = 608
    object miSimulate: TMenuItem
      Caption = 'Run Simulation'
      OnClick = miSimulateClick
    end
    object N4: TMenuItem
      Caption = '-'
    end
    object miLoadRaw: TMenuItem
      Caption = 'Import Raw Data'
      OnClick = miLoadRawClick
    end
    object miSaveFile: TMenuItem
      Caption = 'Save'
      OnClick = miSaveFileClick
    end
    object miOpenFile: TMenuItem
      Caption = 'Open'
      OnClick = miOpenFileClick
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object bChangeObsRange: TMenuItem
      Caption = 'Change Observation Range'
      OnClick = bChangeObsRangeClick
    end
    object miChangeObs: TMenuItem
      AutoCheck = True
      Caption = 'Change Observation Site'
      OnClick = miChangeObsClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object miAnimation: TMenuItem
      AutoCheck = True
      Caption = 'Animation'
      OnClick = miAnimationClick
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object miConvAVI: TMenuItem
      Caption = 'Convert To Vedio File'
      OnClick = miConvAVIClick
    end
  end
  object odDialog: TOpenDialog
    Filter = 'Raw Data (*.dat)|*.dat|Simulation Data(*.sim)|*.sim'
    Title = 'Open Simulation Data'
    Left = 256
    Top = 152
  end
  object sdDialog: TSaveDialog
    DefaultExt = '*.sim'
    Filter = 'Simulation Data(*.sim)|*.sim'
    FilterIndex = 0
    Title = 'Save Simulation Data'
    Left = 304
    Top = 152
  end
  object AniTimer: TTimer
    Enabled = False
    Interval = 500
    OnTimer = AniTimerTimer
    Left = 112
    Top = 144
  end
end
