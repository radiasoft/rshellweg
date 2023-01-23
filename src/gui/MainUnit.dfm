object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'Hellweg (rshellweg) v. 01/23/23'
  ClientHeight = 741
  ClientWidth = 593
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  Visible = True
  OnActivate = FormActivate
  OnCanResize = FormCanResize
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object InputGroup: TGroupBox
    Left = 0
    Top = 0
    Width = 593
    Height = 319
    Align = alTop
    Caption = 'Input Data'
    TabOrder = 1
    object ViewGeometryButton: TButton
      Left = 58
      Top = 242
      Width = 83
      Height = 25
      Caption = 'View Structure'
      TabOrder = 0
      OnClick = ViewGeometryButtonClick
    end
    object ViewBeamButton: TButton
      Left = 58
      Top = 273
      Width = 82
      Height = 25
      Caption = 'View Beam'
      TabOrder = 1
      OnClick = ViewBeamButtonClick
    end
    object GroupPower: TGroupBox
      Left = 21
      Top = 19
      Width = 172
      Height = 54
      Caption = 'Power Source (First)'
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
      Width = 172
      Height = 91
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
      object Label_Fringe: TLabel
        Left = 16
        Top = 73
        Width = 108
        Height = 13
        Caption = 'Fringe Region= ??? cm'
      end
    end
    object GroupBeamGeneral: TGroupBox
      Left = 205
      Top = 19
      Width = 201
      Height = 74
      Caption = 'Beam General'
      TabOrder = 4
      object Label_Np: TLabel
        Left = 16
        Top = 33
        Width = 122
        Height = 13
        Caption = 'Number of Particles = ???'
      end
      object Label_I0: TLabel
        Left = 16
        Top = 52
        Width = 105
        Height = 13
        Caption = 'Input Current = ??? A'
      end
      object Label_Particles: TLabel
        Left = 15
        Top = 14
        Width = 69
        Height = 13
        Caption = 'Particles = ???'
      end
    end
    object GroupSpaceCharge: TGroupBox
      Left = 205
      Top = 246
      Width = 201
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
      Left = 205
      Top = 172
      Width = 201
      Height = 68
      Caption = 'Phase'
      TabOrder = 6
      object Label_Phi0: TLabel
        Left = 16
        Top = 22
        Width = 123
        Height = 13
        Caption = 'Average Phase = ??? deg'
      end
      object Label_dPhi: TLabel
        Left = 16
        Top = 41
        Width = 103
        Height = 13
        Caption = 'Phase RMS = ??? deg'
      end
    end
    object GroupStructure: TGroupBox
      Left = 21
      Top = 170
      Width = 172
      Height = 66
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
      Left = 412
      Top = 19
      Width = 175
      Height = 74
      Caption = 'Twiss Parameters (R)'
      TabOrder = 8
      OnExit = GroupTwissRExit
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
      Left = 412
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
      Left = 412
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
    object GroupEnergy: TGroupBox
      Left = 205
      Top = 92
      Width = 201
      Height = 74
      Caption = 'Energy'
      TabOrder = 11
      object Label_dW: TLabel
        Left = 16
        Top = 54
        Width = 110
        Height = 13
        Caption = 'Energy RMS = ??? MeV'
      end
      object Label_W0: TLabel
        Left = 16
        Top = 16
        Width = 130
        Height = 13
        Caption = 'Average Energy = ??? MeV'
      end
      object Label_beta: TLabel
        Left = 16
        Top = 35
        Width = 51
        Height = 13
        Caption = 'Beta = ???'
      end
      object Label_W0_full: TLabel
        Left = 108
        Top = 35
        Width = 38
        Height = 13
        Caption = '??? MeV'
      end
    end
  end
  object SolverGroup: TGroupBox
    Left = 0
    Top = 431
    Width = 593
    Height = 63
    Align = alTop
    Caption = 'Solver Progress'
    TabOrder = 2
  end
  object ResultsGroup: TGroupBox
    Left = 0
    Top = 494
    Width = 593
    Height = 247
    Align = alClient
    Caption = 'Results'
    TabOrder = 3
    object MemoPanel: TPanel
      Left = 2
      Top = 15
      Width = 482
      Height = 230
      Align = alClient
      TabOrder = 0
      object ResultsMemo: TMemo
        Left = 1
        Top = 1
        Width = 480
        Height = 228
        Align = alClient
        Lines.Strings = (
          'ResultsMemo')
        ReadOnly = True
        ScrollBars = ssVertical
        TabOrder = 0
        Visible = False
      end
    end
    object ActPanel: TPanel
      Left = 484
      Top = 15
      Width = 107
      Height = 230
      Align = alRight
      TabOrder = 1
      object OptButton: TButton
        Left = 16
        Top = 70
        Width = 75
        Height = 25
        Caption = 'Optimizer'
        Enabled = False
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
        Enabled = False
        TabOrder = 2
        OnClick = LoadResultsButtonClick
      end
      object cBuncherButton: TButton
        Left = 16
        Top = 101
        Width = 75
        Height = 25
        Caption = 'Buncher'
        Enabled = False
        TabOrder = 3
        OnClick = cBuncherButtonClick
      end
    end
  end
  object ControlGroup: TGroupBox
    Left = 0
    Top = 319
    Width = 593
    Height = 56
    Align = alTop
    Caption = 'Controls'
    TabOrder = 0
    DesignSize = (
      593
      56)
    object ExitButton: TButton
      Left = 502
      Top = 17
      Width = 73
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Exit'
      TabOrder = 1
      OnClick = ExitButtonClick
    end
    object AbortButton: TButton
      Left = 286
      Top = 17
      Width = 75
      Height = 25
      Caption = 'Abort'
      TabOrder = 2
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
  object InterfaceGroup: TGroupBox
    Left = 0
    Top = 375
    Width = 593
    Height = 56
    Align = alTop
    Caption = 'Data Interface '
    TabOrder = 4
    object Label_Input: TLabel
      Left = 21
      Top = 27
      Width = 45
      Height = 13
      Caption = 'Input File'
    end
    object Label_Output: TLabel
      Left = 215
      Top = 27
      Width = 53
      Height = 13
      Caption = 'Output File'
    end
    object SelectFileButton: TButton
      Left = 72
      Top = 25
      Width = 32
      Height = 25
      Caption = '...'
      TabOrder = 0
      OnClick = SelectFileButtonClick
    end
    object InputFileEdit: TEdit
      Left = 110
      Top = 24
      Width = 83
      Height = 21
      ReadOnly = True
      TabOrder = 1
      Text = 'INPUT.TXT'
    end
    object OutputFileEdit: TEdit
      Left = 297
      Top = 24
      Width = 109
      Height = 21
      TabOrder = 2
      Text = 'OUTPUT'
    end
    object BinaryCheck: TCheckBox
      Left = 428
      Top = 16
      Width = 97
      Height = 17
      Caption = 'Binary Format'
      Enabled = False
      TabOrder = 3
    end
    object TrajectoryCheck: TCheckBox
      Left = 428
      Top = 33
      Width = 109
      Height = 17
      Caption = 'Save Trajectories'
      Enabled = False
      TabOrder = 4
    end
    object OutputCheck: TCheckBox
      Left = 274
      Top = 24
      Width = 17
      Height = 17
      Checked = True
      State = cbChecked
      TabOrder = 5
      OnClick = OutputCheckClick
    end
  end
  object InputDialog: TOpenDialog
    FileName = 
      'D:\Storage\Laptop Backup\My Work\00 - PRG\Hellweg 2.5\Debug\INPU' +
      'T.txt'
    Filter = 'Text Files|*.txt|All Files|*.*'
    Left = 464
    Top = 264
  end
end
