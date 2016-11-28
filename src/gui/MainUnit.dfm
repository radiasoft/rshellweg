object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'Hellweg 2D'
  ClientHeight = 554
  ClientWidth = 467
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
    Width = 467
    Height = 233
    Align = alTop
    Caption = 'Input Data'
    TabOrder = 1
    object Label_P0: TLabel
      Left = 16
      Top = 16
      Width = 109
      Height = 13
      Caption = 'Input Power = ??? MW'
    end
    object Label_I0: TLabel
      Left = 16
      Top = 35
      Width = 105
      Height = 13
      Caption = 'Input Current = ??? A'
    end
    object Label_F0: TLabel
      Left = 16
      Top = 54
      Width = 154
      Height = 13
      Caption = 'Operating Frequency = ??? MHz'
    end
    object Label_Np: TLabel
      Left = 212
      Top = 16
      Width = 122
      Height = 13
      Caption = 'Number of Particles = ???'
    end
    object Label_B0: TLabel
      Left = 16
      Top = 92
      Width = 105
      Height = 13
      Caption = 'Solenoid Field = ??? Tl'
    end
    object Label_Length: TLabel
      Left = 16
      Top = 111
      Width = 116
      Height = 13
      Caption = 'Solenoid Length = ??? m'
    end
    object Label_Position: TLabel
      Left = 16
      Top = 130
      Width = 120
      Height = 13
      Caption = 'Solenoid Position = ??? m'
    end
    object Label_W0: TLabel
      Left = 212
      Top = 35
      Width = 130
      Height = 13
      Caption = 'Average Energy = ??? MeV'
    end
    object Label_Phi0: TLabel
      Left = 212
      Top = 54
      Width = 123
      Height = 13
      Caption = 'Average Phase = ??? deg'
    end
    object Label_dW: TLabel
      Left = 368
      Top = 35
      Width = 68
      Height = 13
      Caption = 'dW = ??? MeV'
    end
    object Label_dPhi: TLabel
      Left = 367
      Top = 54
      Width = 70
      Height = 13
      Caption = 'dPhi = ??? deg'
    end
    object Label1: TLabel
      Left = 212
      Top = 73
      Width = 141
      Height = 13
      Caption = 'Courant-Sneider Parameters:'
    end
    object Label_Alpha: TLabel
      Left = 212
      Top = 92
      Width = 56
      Height = 13
      Caption = 'Alpha = ???'
    end
    object Label_Betta: TLabel
      Left = 212
      Top = 111
      Width = 85
      Height = 13
      Caption = 'Betta=??? cm/rad'
    end
    object Label_Emittance: TLabel
      Left = 212
      Top = 130
      Width = 114
      Height = 13
      Caption = 'Emittance = ??? cm*rad'
    end
    object Label2: TLabel
      Left = 163
      Top = 149
      Width = 32
      Height = 13
      Caption = 'Notes:'
    end
    object Label_Coulomb: TLabel
      Left = 17
      Top = 168
      Width = 91
      Height = 13
      Caption = 'Coulmb Charge ???'
    end
    object Label_Wave: TLabel
      Left = 212
      Top = 168
      Width = 73
      Height = 13
      Caption = 'Wave Type ???'
    end
    object Label_WDist: TLabel
      Left = 16
      Top = 185
      Width = 113
      Height = 13
      Caption = 'Energy Distribution: ???'
    end
    object Label_PhiDist: TLabel
      Left = 212
      Top = 185
      Width = 108
      Height = 13
      Caption = 'Phase Distribution: ???'
    end
    object Label_Cells: TLabel
      Left = 17
      Top = 212
      Width = 87
      Height = 13
      Caption = '??? Cells Detected'
    end
    object ViewGeometryButton: TButton
      Left = 206
      Top = 204
      Width = 83
      Height = 25
      Caption = 'View Geometry'
      TabOrder = 0
      OnClick = ViewGeometryButtonClick
    end
    object ViewBeamButton: TButton
      Left = 295
      Top = 204
      Width = 75
      Height = 25
      Caption = 'View Beam'
      TabOrder = 1
      OnClick = ViewBeamButtonClick
    end
  end
  object SolverGroup: TGroupBox
    Left = 0
    Top = 233
    Width = 467
    Height = 64
    Align = alTop
    Caption = 'Solver'
    TabOrder = 2
  end
  object ResultsGroup: TGroupBox
    Left = 0
    Top = 345
    Width = 467
    Height = 209
    Align = alClient
    Caption = 'Results'
    TabOrder = 3
    object MemoPanel: TPanel
      Left = 2
      Top = 15
      Width = 356
      Height = 192
      Align = alClient
      TabOrder = 0
      object ResultsMemo: TMemo
        Left = 2
        Top = 0
        Width = 351
        Height = 192
        Lines.Strings = (
          'ResultsMemo')
        ScrollBars = ssVertical
        TabOrder = 0
        Visible = False
      end
    end
    object ActPanel: TPanel
      Left = 358
      Top = 15
      Width = 107
      Height = 192
      Align = alRight
      TabOrder = 1
    end
    object ViewButton: TButton
      Left = 382
      Top = 16
      Width = 75
      Height = 25
      Caption = 'View Results'
      Enabled = False
      TabOrder = 2
      OnClick = ViewButtonClick
    end
    object LoadResultsButton: TButton
      Left = 382
      Top = 47
      Width = 75
      Height = 25
      Caption = 'Load Results'
      TabOrder = 3
      OnClick = LoadResultsButtonClick
    end
    object OptButton: TButton
      Left = 382
      Top = 88
      Width = 75
      Height = 25
      Caption = 'Optimizer'
      TabOrder = 4
      OnClick = OptButtonClick
    end
  end
  object ControlGroup: TGroupBox
    Left = 0
    Top = 297
    Width = 467
    Height = 48
    Align = alTop
    Caption = 'Controls'
    TabOrder = 0
    object SelectFileButton: TButton
      Left = 0
      Top = 15
      Width = 75
      Height = 25
      Caption = 'Select File'
      TabOrder = 1
      OnClick = SelectFileButtonClick
    end
    object ExitButton: TButton
      Left = 382
      Top = 17
      Width = 75
      Height = 25
      Caption = 'Exit'
      TabOrder = 2
      OnClick = ExitButtonClick
    end
    object AbortButton: TButton
      Left = 230
      Top = 17
      Width = 75
      Height = 25
      Caption = 'Abort'
      TabOrder = 3
      OnClick = AbortButtonClick
    end
    object SolveButton: TButton
      Left = 149
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
    Left = 440
  end
end
