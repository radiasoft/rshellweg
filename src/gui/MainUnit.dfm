object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'Hellweg 2D'
  ClientHeight = 611
  ClientWidth = 577
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnCanResize = FormCanResize
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object InputGroup: TGroupBox
    Left = 0
    Top = 0
    Width = 577
    Height = 273
    Align = alTop
    Caption = 'Input Data'
    TabOrder = 1
    object ViewGeometryButton: TButton
      Left = 21
      Top = 234
      Width = 83
      Height = 25
      Caption = 'View Geometry'
      Enabled = False
      TabOrder = 0
      OnClick = ViewGeometryButtonClick
    end
    object ViewBeamButton: TButton
      Left = 110
      Top = 234
      Width = 75
      Height = 25
      Caption = 'View Beam'
      TabOrder = 1
      OnClick = ViewBeamButtonClick
    end
    object GroupPower: TGroupBox
      Left = 21
      Top = 19
      Width = 185
      Height = 54
      Caption = 'Power Source'
      TabOrder = 2
      object Label_P0: TLabel
        Left = 16
        Top = 16
        Width = 109
        Height = 13
        Caption = 'Input Power = ??? MW'
      end
      object Label_F0: TLabel
        Left = 16
        Top = 35
        Width = 103
        Height = 13
        Caption = 'Frequency = ??? MHz'
      end
    end
    object GroupSolenoid: TGroupBox
      Left = 21
      Top = 73
      Width = 185
      Height = 81
      Caption = 'Solenoid'
      TabOrder = 3
      object Label_Length: TLabel
        Left = 16
        Top = 54
        Width = 124
        Height = 13
        Caption = 'Effective Length = ??? cm'
      end
      object Label_B0: TLabel
        Left = 16
        Top = 16
        Width = 112
        Height = 13
        Caption = 'Magnetic Field = ??? Gs'
      end
      object Label_Position: TLabel
        Left = 16
        Top = 35
        Width = 109
        Height = 13
        Caption = 'Start Position = ??? cm'
      end
    end
    object GroupBeamGeneral: TGroupBox
      Left = 215
      Top = 19
      Width = 175
      Height = 54
      Caption = 'Beam General'
      TabOrder = 4
      object Label_Np: TLabel
        Left = 16
        Top = 16
        Width = 122
        Height = 13
        Caption = 'Number of Particles = ???'
      end
      object Label_I0: TLabel
        Left = 16
        Top = 35
        Width = 105
        Height = 13
        Caption = 'Input Current = ??? A'
      end
    end
    object GroupSpaceCharge: TGroupBox
      Left = 215
      Top = 190
      Width = 175
      Height = 63
      Caption = 'Space Charge and Features'
      TabOrder = 5
      object Label_Spch: TLabel
        Left = 16
        Top = 20
        Width = 46
        Height = 13
        Caption = 'Model ???'
      end
      object Label_SpchPar: TLabel
        Left = 16
        Top = 40
        Width = 73
        Height = 13
        Caption = 'Parameters ???'
      end
    end
    object GroupZpar: TGroupBox
      Left = 215
      Top = 79
      Width = 175
      Height = 105
      Caption = 'Longitudinal Phase Space'
      TabOrder = 6
      object Label_W0: TLabel
        Left = 16
        Top = 16
        Width = 130
        Height = 13
        Caption = 'Average Energy = ??? MeV'
      end
      object Label_dW: TLabel
        Left = 16
        Top = 35
        Width = 110
        Height = 13
        Caption = 'Energy RMS = ??? MeV'
      end
      object Label_Phi0: TLabel
        Left = 16
        Top = 54
        Width = 123
        Height = 13
        Caption = 'Average Phase = ??? deg'
      end
      object Label_dPhi: TLabel
        Left = 16
        Top = 73
        Width = 103
        Height = 13
        Caption = 'Phase RMS = ??? deg'
      end
    end
    object GroupStructure: TGroupBox
      Left = 21
      Top = 160
      Width = 185
      Height = 68
      Caption = 'Structure'
      TabOrder = 7
      object Label_Cells: TLabel
        Left = 16
        Top = 20
        Width = 104
        Height = 13
        Caption = 'Number of Cells = ???'
      end
      object Label_Sections: TLabel
        Left = 16
        Top = 40
        Width = 122
        Height = 13
        Caption = 'Number of Sections = ???'
      end
    end
    object GroupTwissR: TGroupBox
      Left = 396
      Top = 19
      Width = 175
      Height = 74
      Caption = 'Twiss Parameters (R)'
      TabOrder = 8
      object Label_AlphaR: TLabel
        Left = 16
        Top = 14
        Width = 56
        Height = 13
        Caption = 'Alpha = ???'
      end
      object Label_BetaR: TLabel
        Left = 16
        Top = 33
        Width = 81
        Height = 13
        Caption = 'Beta=??? cm/rad'
      end
      object Label_EmittanceR: TLabel
        Left = 16
        Top = 52
        Width = 125
        Height = 13
        Caption = 'Emittance = ??? mm*mrad'
      end
    end
    object GroupTwissX: TGroupBox
      Left = 396
      Top = 99
      Width = 175
      Height = 74
      Caption = 'Twiss Parameters (X)'
      TabOrder = 9
      object Label_AlphaX: TLabel
        Left = 16
        Top = 14
        Width = 56
        Height = 13
        Caption = 'Alpha = ???'
      end
      object Label_BetaX: TLabel
        Left = 16
        Top = 33
        Width = 81
        Height = 13
        Caption = 'Beta=??? cm/rad'
      end
      object Label_EmittanceX: TLabel
        Left = 16
        Top = 52
        Width = 114
        Height = 13
        Caption = 'Emittance = ??? cm*rad'
      end
    end
    object GroupTwissY: TGroupBox
      Left = 396
      Top = 179
      Width = 175
      Height = 74
      Caption = 'Twiss Parameters (Y)'
      TabOrder = 10
      object Label_AlphaY: TLabel
        Left = 16
        Top = 14
        Width = 56
        Height = 13
        Caption = 'Alpha = ???'
      end
      object Label_BetaY: TLabel
        Left = 16
        Top = 33
        Width = 81
        Height = 13
        Caption = 'Beta=??? cm/rad'
      end
      object Label_EmittanceY: TLabel
        Left = 16
        Top = 52
        Width = 114
        Height = 13
        Caption = 'Emittance = ??? cm*rad'
      end
    end
  end
  object SolverGroup: TGroupBox
    Left = 0
    Top = 273
    Width = 577
    Height = 64
    Align = alTop
    Caption = 'Solver Progress'
    TabOrder = 2
  end
  object ResultsGroup: TGroupBox
    Left = 0
    Top = 385
    Width = 577
    Height = 226
    Align = alClient
    Caption = 'Results'
    TabOrder = 3
    object MemoPanel: TPanel
      Left = 2
      Top = 15
      Width = 466
      Height = 209
      Align = alClient
      TabOrder = 0
      object ResultsMemo: TMemo
        Left = 1
        Top = 1
        Width = 464
        Height = 207
        Align = alClient
        Lines.Strings = (
          'ResultsMemo')
        ScrollBars = ssVertical
        TabOrder = 0
        Visible = False
      end
    end
    object ActPanel: TPanel
      Left = 468
      Top = 15
      Width = 107
      Height = 209
      Align = alRight
      TabOrder = 1
      object OptButton: TButton
        Left = 16
        Top = 70
        Width = 75
        Height = 25
        Caption = 'Optimizer'
        TabOrder = 0
        OnClick = OptButtonClick
      end
      object ViewButton: TButton
        Left = 16
        Top = 8
        Width = 75
        Height = 25
        Caption = 'View Results'
        Enabled = False
        TabOrder = 1
        OnClick = ViewButtonClick
      end
      object LoadResultsButton: TButton
        Left = 16
        Top = 39
        Width = 75
        Height = 25
        Caption = 'Load Results'
        TabOrder = 2
        OnClick = LoadResultsButtonClick
      end
    end
  end
  object ControlGroup: TGroupBox
    Left = 0
    Top = 337
    Width = 577
    Height = 48
    Align = alTop
    Caption = 'Controls'
    TabOrder = 0
    DesignSize = (
      577
      48)
    object SelectFileButton: TButton
      Left = 3
      Top = 17
      Width = 75
      Height = 25
      Caption = 'Select File'
      TabOrder = 1
      OnClick = SelectFileButtonClick
    end
    object ExitButton: TButton
      Left = 486
      Top = 20
      Width = 73
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Exit'
      TabOrder = 2
      OnClick = ExitButtonClick
    end
    object AbortButton: TButton
      Left = 286
      Top = 17
      Width = 75
      Height = 25
      Caption = 'Abort'
      TabOrder = 3
      OnClick = AbortButtonClick
    end
    object SolveButton: TButton
      Left = 205
      Top = 17
      Width = 75
      Height = 25
      Caption = 'Solve'
      TabOrder = 0
      OnClick = SolveButtonClick
    end
  end
  object InputDialog: TOpenDialog
    FileName = 
      'D:\Storage\Laptop Backup\My Work\00 - PRG\Hellweg 2.5\Debug\INPU' +
      'T.txt'
    Filter = 'Text Files|*.txt|All Files|*.*'
    Left = 176
    Top = 24
  end
end
