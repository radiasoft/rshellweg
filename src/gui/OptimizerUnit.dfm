object OptimizerForm: TOptimizerForm
  Left = 0
  Top = 0
  Caption = 'Optimizer'
  ClientHeight = 509
  ClientWidth = 501
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCanResize = FormCanResize
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 501
    Height = 409
    Align = alClient
    TabOrder = 0
    object BuncherPanel: TPanel
      Left = 1
      Top = 1
      Width = 280
      Height = 407
      Align = alLeft
      TabOrder = 0
      object BuncherGroup: TGroupBox
        Left = 1
        Top = 1
        Width = 278
        Height = 405
        Align = alClient
        Caption = 'Optimize Buncher'
        TabOrder = 0
        object BuncherCheck: TCheckBox
          Left = 16
          Top = 16
          Width = 97
          Height = 25
          Caption = 'Optimize'
          TabOrder = 0
        end
        object bInputGroup: TGroupBox
          Left = 16
          Top = 44
          Width = 233
          Height = 139
          Caption = 'Input Parameters'
          TabOrder = 1
          object TypeLabel: TLabel
            Left = 17
            Top = 23
            Width = 72
            Height = 13
            Caption = 'Structure Type'
          end
          object ModeLabel: TLabel
            Left = 17
            Top = 50
            Width = 26
            Height = 13
            Caption = 'Mode'
          end
          object FrequencyLabel: TLabel
            Left = 17
            Top = 77
            Width = 51
            Height = 13
            Caption = 'Frequency'
          end
          object FrequencyDim: TLabel
            Left = 165
            Top = 77
            Width = 20
            Height = 13
            Caption = 'MHz'
          end
          object PowerLabel: TLabel
            Left = 17
            Top = 104
            Width = 30
            Height = 13
            Caption = 'Power'
          end
          object PowerDim: TLabel
            Left = 165
            Top = 104
            Width = 18
            Height = 13
            Caption = 'MW'
          end
          object TypeCombo: TComboBox
            Left = 95
            Top = 15
            Width = 83
            Height = 21
            TabOrder = 0
            Text = 'DLS'
            Items.Strings = (
              'DLS')
          end
          object ModeCombo: TComboBox
            Left = 95
            Top = 42
            Width = 83
            Height = 21
            ItemIndex = 0
            TabOrder = 1
            Text = 'pi/2'
            Items.Strings = (
              'pi/2'
              '2pi/3')
          end
          object FrequencyEdit: TEdit
            Left = 95
            Top = 69
            Width = 64
            Height = 21
            TabOrder = 2
            Text = '0.0'
          end
          object PowerEdit: TEdit
            Left = 95
            Top = 96
            Width = 64
            Height = 21
            TabOrder = 3
            Text = '0.0'
          end
        end
        object bLimitGroup: TGroupBox
          Left = 16
          Top = 189
          Width = 233
          Height = 75
          Caption = 'Limits'
          TabOrder = 2
          object LimitLabel1: TLabel
            Left = 17
            Top = 47
            Width = 47
            Height = 13
            Caption = 'Vary from'
          end
          object FieldTypeLabel: TLabel
            Left = 17
            Top = 20
            Width = 22
            Height = 13
            Caption = 'Field'
          end
          object LimitLabel2: TLabel
            Left = 137
            Top = 47
            Width = 10
            Height = 13
            Caption = 'to'
          end
          object FieldCombo: TComboBox
            Left = 53
            Top = 12
            Width = 92
            Height = 21
            ItemIndex = 0
            TabOrder = 0
            Text = 'A []'
            Items.Strings = (
              'A []'
              'En [Ohm^1/2]'
              'E [MV/m]')
          end
          object LimitEdit1: TEdit
            Left = 70
            Top = 39
            Width = 64
            Height = 21
            TabOrder = 1
            Text = '1.0'
          end
          object LimitEdit2: TEdit
            Left = 151
            Top = 39
            Width = 64
            Height = 21
            TabOrder = 2
            Text = '2.0'
          end
        end
        object bObjectGroup: TGroupBox
          Left = 16
          Top = 270
          Width = 233
          Height = 99
          Caption = 'Object'
          TabOrder = 3
          object ParameterLabel: TLabel
            Left = 19
            Top = 47
            Width = 50
            Height = 13
            Caption = 'Parameter'
          end
          object SamplesLabel: TLabel
            Left = 19
            Top = 72
            Width = 39
            Height = 13
            Caption = 'Samples'
          end
          object SweepRadio: TRadioButton
            Left = 17
            Top = 16
            Width = 51
            Height = 17
            Caption = 'Sweep'
            Checked = True
            TabOrder = 0
            TabStop = True
            OnClick = SweepRadioClick
          end
          object OptimumRadio: TRadioButton
            Left = 80
            Top = 16
            Width = 67
            Height = 17
            Caption = 'Optimum'
            TabOrder = 1
            OnClick = OptimumRadioClick
          end
          object ParameterCombo: TComboBox
            Left = 75
            Top = 39
            Width = 110
            Height = 21
            ItemIndex = 0
            TabOrder = 2
            Text = 'Capture'
            OnChange = ParameterComboChange
            Items.Strings = (
              'Capture'
              'Energy Spectrum'
              'Phase Spectrum')
          end
          object SamplesSpin: TSpinEdit
            Left = 75
            Top = 66
            Width = 59
            Height = 22
            MaxValue = 1000
            MinValue = 2
            TabOrder = 3
            Value = 5
          end
        end
      end
    end
    object AccelPanel: TPanel
      Left = 281
      Top = 1
      Width = 219
      Height = 407
      Align = alClient
      TabOrder = 1
      object AccelGroup: TGroupBox
        Left = 1
        Top = 1
        Width = 217
        Height = 405
        Align = alClient
        Caption = 'OptimizeAccelerator'
        TabOrder = 0
        object AccelCheck: TCheckBox
          Left = 16
          Top = 21
          Width = 97
          Height = 17
          Caption = 'Optimize'
          Checked = True
          State = cbChecked
          TabOrder = 0
        end
        object StructureGroup: TGroupBox
          Left = 16
          Top = 44
          Width = 185
          Height = 63
          Caption = 'Structure Type'
          TabOrder = 1
          object GradientRadio: TRadioButton
            Left = 16
            Top = 40
            Width = 113
            Height = 17
            Caption = 'Constant Gradient'
            Checked = True
            TabOrder = 0
            TabStop = True
          end
          object ImpedanceRadio: TRadioButton
            Left = 16
            Top = 17
            Width = 121
            Height = 17
            Caption = 'Constant Impedance'
            TabOrder = 1
          end
        end
      end
      object TargetGroup: TGroupBox
        Left = 16
        Top = 114
        Width = 185
        Height = 103
        Caption = 'Target Energy'
        TabOrder = 1
        object EnergyLabel: TLabel
          Left = 16
          Top = 20
          Width = 34
          Height = 13
          Caption = 'Energy'
        end
        object EnergyDim: TLabel
          Left = 127
          Top = 20
          Width = 20
          Height = 13
          Caption = 'MeV'
        end
        object EnergyEdit: TEdit
          Left = 56
          Top = 15
          Width = 65
          Height = 21
          TabOrder = 0
          Text = '20.0'
        end
        object StartCellCheck: TCheckBox
          Left = 16
          Top = 65
          Width = 89
          Height = 17
          Caption = 'Start from cell'
          TabOrder = 1
          OnClick = StartCellCheckClick
        end
        object StartCellSpin: TCSpinEdit
          Left = 111
          Top = 65
          Width = 50
          Height = 22
          TabOrder = 2
        end
      end
      object ExactCheck: TCheckBox
        Left = 32
        Top = 156
        Width = 81
        Height = 17
        Caption = 'Exact Value'
        TabOrder = 2
      end
    end
  end
  object BottomPanel: TPanel
    Left = 0
    Top = 409
    Width = 501
    Height = 100
    Align = alBottom
    TabOrder = 1
    object ProgressPanel: TPanel
      Left = 1
      Top = 1
      Width = 410
      Height = 98
      Align = alClient
      TabOrder = 0
      object OptimizerProgress: TProgressBar
        Left = 16
        Top = 14
        Width = 377
        Height = 17
        Max = 0
        TabOrder = 0
      end
      object SaveGraphCheck: TCheckBox
        Left = 182
        Top = 37
        Width = 97
        Height = 17
        Caption = 'Save Results'
        TabOrder = 1
      end
      object StartButton: TButton
        Left = 135
        Top = 60
        Width = 75
        Height = 25
        Caption = 'Start'
        TabOrder = 2
        OnClick = StartButtonClick
      end
      object AbortButton: TButton
        Left = 216
        Top = 60
        Width = 75
        Height = 25
        Caption = 'Abort'
        TabOrder = 3
        OnClick = AbortButtonClick
      end
    end
    object ButtonsPanel: TPanel
      Left = 411
      Top = 1
      Width = 89
      Height = 98
      Align = alRight
      TabOrder = 1
      object CloseButton: TButton
        Left = 6
        Top = 66
        Width = 75
        Height = 25
        Caption = 'Close'
        TabOrder = 0
        OnClick = CloseButtonClick
      end
      object LoadButton: TButton
        Left = 6
        Top = 35
        Width = 73
        Height = 25
        Caption = 'Load Results'
        TabOrder = 1
      end
      object ResultsButton: TButton
        Left = 6
        Top = 4
        Width = 75
        Height = 25
        Caption = 'View Results'
        Enabled = False
        TabOrder = 2
        OnClick = ResultsButtonClick
      end
    end
  end
end
