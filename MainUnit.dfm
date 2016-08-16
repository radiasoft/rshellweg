object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'Hellweg 2D'
  ClientHeight = 724
  ClientWidth = 611
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnCanResize = FormCanResize
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 120
  TextHeight = 17
  object InputGroup: TGroupBox
    Left = 0
    Top = 0
    Width = 611
    Height = 305
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alTop
    Caption = 'Input Data'
    TabOrder = 1
    object Label_P0: TLabel
      Left = 21
      Top = 21
      Width = 142
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Input Power = ??? MW'
    end
    object Label_I0: TLabel
      Left = 21
      Top = 46
      Width = 135
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Input Current = ??? A'
    end
    object Label_F0: TLabel
      Left = 21
      Top = 71
      Width = 197
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Operating Frequency = ??? MHz'
    end
    object Label_Np: TLabel
      Left = 277
      Top = 21
      Width = 157
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Number of Particles = ???'
    end
    object Label_B0: TLabel
      Left = 21
      Top = 120
      Width = 134
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Solenoid Field = ??? Tl'
    end
    object Label_Length: TLabel
      Left = 21
      Top = 145
      Width = 153
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Solenoid Length = ??? m'
    end
    object Label_Position: TLabel
      Left = 21
      Top = 170
      Width = 157
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Solenoid Position = ??? m'
    end
    object Label_W0: TLabel
      Left = 277
      Top = 46
      Width = 166
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Average Energy = ??? MeV'
    end
    object Label_Phi0: TLabel
      Left = 277
      Top = 71
      Width = 156
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Average Phase = ??? deg'
    end
    object Label_dW: TLabel
      Left = 481
      Top = 46
      Width = 90
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'dW = ??? MeV'
    end
    object Label_dPhi: TLabel
      Left = 480
      Top = 71
      Width = 92
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'dPhi = ??? deg'
    end
    object Label1: TLabel
      Left = 277
      Top = 95
      Width = 178
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Courant-Sneider Parameters:'
    end
    object Label_Alpha: TLabel
      Left = 277
      Top = 120
      Width = 72
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Alpha = ???'
    end
    object Label_Betta: TLabel
      Left = 277
      Top = 145
      Width = 111
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Betta=??? cm/rad'
    end
    object Label_Emittance: TLabel
      Left = 277
      Top = 170
      Width = 151
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Emittance = ??? cm*rad'
    end
    object Label2: TLabel
      Left = 213
      Top = 195
      Width = 40
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Notes:'
    end
    object Label_Coulomb: TLabel
      Left = 22
      Top = 220
      Width = 120
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Coulmb Charge ???'
    end
    object Label_Wave: TLabel
      Left = 277
      Top = 220
      Width = 96
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Wave Type ???'
    end
    object Label_WDist: TLabel
      Left = 21
      Top = 242
      Width = 147
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Energy Distribution: ???'
    end
    object Label_PhiDist: TLabel
      Left = 277
      Top = 242
      Width = 139
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Phase Distribution: ???'
    end
    object Label_Cells: TLabel
      Left = 22
      Top = 277
      Width = 111
      Height = 17
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = '??? Cells Detected'
    end
    object ViewGeometryButton: TButton
      Left = 269
      Top = 267
      Width = 109
      Height = 32
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'View Geometry'
      TabOrder = 0
      OnClick = ViewGeometryButtonClick
    end
    object ViewBeamButton: TButton
      Left = 386
      Top = 267
      Width = 98
      Height = 32
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'View Beam'
      TabOrder = 1
      OnClick = ViewBeamButtonClick
    end
  end
  object SolverGroup: TGroupBox
    Left = 0
    Top = 305
    Width = 611
    Height = 83
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alTop
    Caption = 'Solver'
    TabOrder = 2
  end
  object ResultsGroup: TGroupBox
    Left = 0
    Top = 451
    Width = 611
    Height = 273
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alClient
    Caption = 'Results'
    TabOrder = 3
    object MemoPanel: TPanel
      Left = 2
      Top = 19
      Width = 468
      Height = 252
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Align = alClient
      TabOrder = 0
      object ResultsMemo: TMemo
        Left = 3
        Top = 0
        Width = 459
        Height = 251
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Lines.Strings = (
          'ResultsMemo')
        ScrollBars = ssVertical
        TabOrder = 0
        Visible = False
      end
    end
    object ActPanel: TPanel
      Left = 470
      Top = 19
      Width = 139
      Height = 252
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Align = alRight
      TabOrder = 1
    end
    object ViewButton: TButton
      Left = 500
      Top = 21
      Width = 98
      Height = 33
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'View Results'
      Enabled = False
      TabOrder = 2
      OnClick = ViewButtonClick
    end
    object LoadResultsButton: TButton
      Left = 500
      Top = 61
      Width = 98
      Height = 33
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Load Results'
      TabOrder = 3
      OnClick = LoadResultsButtonClick
    end
    object OptButton: TButton
      Left = 500
      Top = 115
      Width = 98
      Height = 33
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Optimizer'
      TabOrder = 4
      OnClick = OptButtonClick
    end
  end
  object ControlGroup: TGroupBox
    Left = 0
    Top = 388
    Width = 611
    Height = 63
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alTop
    Caption = 'Controls'
    TabOrder = 0
    object SelectFileButton: TButton
      Left = 0
      Top = 20
      Width = 98
      Height = 32
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Select File'
      TabOrder = 1
      OnClick = SelectFileButtonClick
    end
    object ExitButton: TButton
      Left = 500
      Top = 22
      Width = 98
      Height = 33
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Exit'
      TabOrder = 2
      OnClick = ExitButtonClick
    end
    object AbortButton: TButton
      Left = 301
      Top = 22
      Width = 98
      Height = 33
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Abort'
      TabOrder = 3
      OnClick = AbortButtonClick
    end
    object SolveButton: TButton
      Left = 195
      Top = 22
      Width = 98
      Height = 33
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
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
