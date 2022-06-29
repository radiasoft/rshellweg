object BuncherForm: TBuncherForm
  Left = 0
  Top = 0
  Caption = 'Buncher Synthesizer'
  ClientHeight = 438
  ClientWidth = 429
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object DescriptionGroup: TGroupBox
    Left = 0
    Top = 0
    Width = 429
    Height = 81
    Align = alTop
    Caption = 'Description'
    TabOrder = 0
    ExplicitWidth = 572
    object betaLabel: TLabel
      Left = 16
      Top = 35
      Width = 150
      Height = 13
      Caption = 'beta=(2/pi)*atan(n*ksi^2+b0)'
    end
    object DescriptionLabel: TLabel
      Left = 16
      Top = 16
      Width = 381
      Height = 13
      Caption = 
        'Buncher parameters (beta and E) will be calculated by using Lomn' +
        'ev'#39's formulas:'
    end
    object eLabel: TLabel
      Left = 16
      Top = 54
      Width = 115
      Height = 13
      Caption = 'E=Em*sin(m*ksi^2+e0)'
    end
  end
  object ConstrainsGroup: TGroupBox
    Left = 0
    Top = 81
    Width = 429
    Height = 104
    Align = alTop
    Caption = 'Constrains'
    TabOrder = 1
    object MinBetaLabel: TLabel
      Left = 16
      Top = 24
      Width = 65
      Height = 13
      Caption = 'Minimum Beta'
    end
    object MaxBetaLabel: TLabel
      Left = 16
      Top = 67
      Width = 69
      Height = 13
      Caption = 'Maximum Beta'
    end
    object MaxAklLabel: TLabel
      Left = 168
      Top = 67
      Width = 51
      Height = 13
      Caption = 'Final a/lmb'
    end
    object RFLabel: TLabel
      Left = 310
      Top = 24
      Width = 71
      Height = 13
      Caption = 'RF parameters'
    end
    object MinAklLabel: TLabel
      Left = 168
      Top = 24
      Width = 55
      Height = 13
      Caption = 'Initial a/lmb'
    end
    object RFCommentLabel: TLabel
      Left = 310
      Top = 70
      Width = 45
      Height = 13
      Caption = 'DLS 2pi/3'
    end
    object MinBetaEdit: TEdit
      Left = 91
      Top = 21
      Width = 44
      Height = 21
      TabOrder = 0
      Text = '0.3'
    end
    object BetaBeamCheck: TCheckBox
      Left = 34
      Top = 44
      Width = 97
      Height = 17
      Caption = 'Match to beam'
      TabOrder = 1
      OnClick = BetaBeamCheckClick
    end
    object MaxBetaEdit: TEdit
      Left = 91
      Top = 65
      Width = 44
      Height = 21
      TabOrder = 2
      Text = '0.95'
    end
    object MinAklEdit: TEdit
      Left = 229
      Top = 21
      Width = 44
      Height = 21
      TabOrder = 3
      Text = '0.08'
    end
    object MAxAklEdit: TEdit
      Left = 229
      Top = 65
      Width = 44
      Height = 21
      TabOrder = 4
      Text = '0.08'
    end
    object RFCombo: TComboBox
      Left = 310
      Top = 43
      Width = 87
      Height = 21
      ItemIndex = 0
      TabOrder = 5
      Text = 'Built-in '
      OnChange = RFComboChange
      Items.Strings = (
        'Built-in '
        'From files')
    end
  end
  object SweepGroup: TGroupBox
    Left = 0
    Top = 185
    Width = 429
    Height = 160
    Align = alTop
    Caption = 'Parameters sweep range'
    TabOrder = 2
    object NEdit: TLabel
      Left = 16
      Top = 43
      Width = 39
      Height = 13
      Caption = 'n (beta)'
    end
    object FromLabel: TLabel
      Left = 91
      Top = 24
      Width = 22
      Height = 13
      Caption = 'from'
    end
    object ToLabel: TLabel
      Left = 141
      Top = 24
      Width = 10
      Height = 13
      Caption = 'to'
    end
    object StepLabel: TLabel
      Left = 191
      Top = 24
      Width = 26
      Height = 13
      Caption = 'steps'
    end
    object ActiveLabel: TLabel
      Left = 244
      Top = 24
      Width = 29
      Height = 13
      Caption = 'active'
    end
    object MLabel: TLabel
      Left = 16
      Top = 70
      Width = 25
      Height = 13
      Caption = 'm (E)'
    end
    object NCellsLabel: TLabel
      Left = 16
      Top = 124
      Width = 42
      Height = 13
      Caption = 'Skip cells'
    end
    object CellLAbel: TLabel
      Left = 16
      Top = 97
      Width = 48
      Height = 13
      Caption = 'Skip cell #'
    end
    object NFromEdit: TEdit
      Left = 91
      Top = 43
      Width = 44
      Height = 21
      TabOrder = 0
      Text = '0.2'
    end
    object NToEdit: TEdit
      Left = 141
      Top = 43
      Width = 44
      Height = 21
      TabOrder = 1
      Text = '0.5'
    end
    object NStepEdit: TEdit
      Left = 191
      Top = 43
      Width = 44
      Height = 21
      TabOrder = 2
      Text = '5'
    end
    object NCheck: TCheckBox
      Left = 244
      Top = 43
      Width = 97
      Height = 17
      Caption = 'Active'
      Checked = True
      State = cbChecked
      TabOrder = 3
      OnClick = NCheckClick
    end
    object MFromEdit: TEdit
      Left = 91
      Top = 70
      Width = 44
      Height = 21
      TabOrder = 4
      Text = '0.2'
    end
    object MToEdit: TEdit
      Left = 141
      Top = 70
      Width = 44
      Height = 21
      TabOrder = 5
      Text = '0.5'
    end
    object MStepEdit: TEdit
      Left = 191
      Top = 70
      Width = 44
      Height = 21
      TabOrder = 6
      Text = '5'
    end
    object MCheck: TCheckBox
      Left = 244
      Top = 70
      Width = 97
      Height = 17
      Caption = 'Active'
      Checked = True
      State = cbChecked
      TabOrder = 7
      OnClick = MCheckClick
    end
    object CellFromEdit: TEdit
      Left = 91
      Top = 97
      Width = 44
      Height = 21
      Enabled = False
      TabOrder = 8
      Text = '4'
    end
    object CellToEdit: TEdit
      Left = 141
      Top = 97
      Width = 44
      Height = 21
      Enabled = False
      TabOrder = 9
      Text = '8'
    end
    object CellStepEdit: TEdit
      Left = 191
      Top = 97
      Width = 44
      Height = 21
      Enabled = False
      TabOrder = 10
      Text = '1'
    end
    object NCellsFromEdit: TEdit
      Left = 91
      Top = 124
      Width = 44
      Height = 21
      Enabled = False
      TabOrder = 11
      Text = '1'
    end
    object NCellsToEdit: TEdit
      Left = 141
      Top = 124
      Width = 44
      Height = 21
      Enabled = False
      TabOrder = 12
      Text = '5'
    end
    object NCellsStepEdit: TEdit
      Left = 191
      Top = 124
      Width = 44
      Height = 21
      Enabled = False
      TabOrder = 13
      Text = '1'
    end
    object CellCheck: TCheckBox
      Left = 244
      Top = 97
      Width = 97
      Height = 17
      Caption = 'Active'
      TabOrder = 14
      OnClick = CellCheckClick
    end
    object NCellsCheck: TCheckBox
      Left = 244
      Top = 124
      Width = 97
      Height = 17
      Caption = 'Active'
      TabOrder = 15
      OnClick = NCellsCheckClick
    end
  end
  object ControlGroup: TGroupBox
    Left = 0
    Top = 345
    Width = 429
    Height = 96
    Align = alTop
    Caption = 'Controls'
    TabOrder = 3
    object ProgressLabel: TLabel
      Left = 360
      Top = 24
      Width = 22
      Height = 13
      Caption = '0 / 0'
    end
    object StartButton: TButton
      Left = 38
      Top = 56
      Width = 75
      Height = 25
      Caption = 'Start'
      TabOrder = 0
      OnClick = StartButtonClick
    end
    object StopButton: TButton
      Left = 119
      Top = 56
      Width = 75
      Height = 25
      Caption = 'Stop'
      TabOrder = 1
    end
    object CloseButton: TButton
      Left = 342
      Top = 56
      Width = 75
      Height = 25
      Caption = 'Exit'
      TabOrder = 2
      OnClick = CloseButtonClick
    end
    object ProgressBar: TProgressBar
      Left = 38
      Top = 24
      Width = 303
      Height = 17
      TabOrder = 3
    end
  end
end
