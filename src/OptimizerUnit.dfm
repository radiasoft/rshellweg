object OptimizerForm: TOptimizerForm
  Left = 0
  Top = 0
  Caption = 'Optimizer'
  ClientHeight = 666
  ClientWidth = 655
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCanResize = FormCanResize
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 120
  TextHeight = 17
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 655
    Height = 535
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alClient
    TabOrder = 0
    object BuncherPanel: TPanel
      Left = 1
      Top = 1
      Width = 366
      Height = 533
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Align = alLeft
      TabOrder = 0
      object BuncherGroup: TGroupBox
        Left = 1
        Top = 1
        Width = 364
        Height = 531
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Align = alClient
        Caption = 'Optimize Buncher'
        TabOrder = 0
        object BuncherCheck: TCheckBox
          Left = 21
          Top = 21
          Width = 127
          Height = 33
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Optimize'
          TabOrder = 0
        end
        object bInputGroup: TGroupBox
          Left = 21
          Top = 58
          Width = 305
          Height = 181
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Input Parameters'
          TabOrder = 1
          object TypeLabel: TLabel
            Left = 22
            Top = 30
            Width = 93
            Height = 17
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'Structure Type'
          end
          object ModeLabel: TLabel
            Left = 22
            Top = 65
            Width = 33
            Height = 17
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'Mode'
          end
          object FrequencyLabel: TLabel
            Left = 22
            Top = 101
            Width = 65
            Height = 17
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'Frequency'
          end
          object FrequencyDim: TLabel
            Left = 216
            Top = 101
            Width = 25
            Height = 17
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'MHz'
          end
          object PowerLabel: TLabel
            Left = 22
            Top = 136
            Width = 38
            Height = 17
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'Power'
          end
          object PowerDim: TLabel
            Left = 216
            Top = 136
            Width = 24
            Height = 17
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'MW'
          end
          object TypeCombo: TComboBox
            Left = 124
            Top = 20
            Width = 109
            Height = 25
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            TabOrder = 0
            Text = 'DLS'
            Items.Strings = (
              'DLS')
          end
          object ModeCombo: TComboBox
            Left = 124
            Top = 55
            Width = 109
            Height = 25
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            ItemIndex = 0
            TabOrder = 1
            Text = 'pi/2'
            Items.Strings = (
              'pi/2'
              '2pi/3')
          end
          object FrequencyEdit: TEdit
            Left = 124
            Top = 90
            Width = 84
            Height = 25
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            TabOrder = 2
            Text = '0.0'
          end
          object PowerEdit: TEdit
            Left = 124
            Top = 126
            Width = 84
            Height = 25
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            TabOrder = 3
            Text = '0.0'
          end
        end
        object bLimitGroup: TGroupBox
          Left = 21
          Top = 247
          Width = 305
          Height = 98
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Limits'
          TabOrder = 2
          object LimitLabel1: TLabel
            Left = 22
            Top = 61
            Width = 61
            Height = 17
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'Vary from'
          end
          object FieldTypeLabel: TLabel
            Left = 22
            Top = 26
            Width = 26
            Height = 17
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'Field'
          end
          object LimitLabel2: TLabel
            Left = 179
            Top = 61
            Width = 13
            Height = 17
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'to'
          end
          object FieldCombo: TComboBox
            Left = 69
            Top = 16
            Width = 121
            Height = 25
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            ItemIndex = 0
            TabOrder = 0
            Text = 'A []'
            Items.Strings = (
              'A []'
              'En [Ohm^1/2]'
              'E [MV/m]')
          end
          object LimitEdit1: TEdit
            Left = 92
            Top = 51
            Width = 83
            Height = 25
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            TabOrder = 1
            Text = '1.0'
          end
          object LimitEdit2: TEdit
            Left = 197
            Top = 51
            Width = 84
            Height = 25
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            TabOrder = 2
            Text = '2.0'
          end
        end
        object bObjectGroup: TGroupBox
          Left = 21
          Top = 353
          Width = 305
          Height = 130
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Object'
          TabOrder = 3
          object ParameterLabel: TLabel
            Left = 25
            Top = 61
            Width = 63
            Height = 17
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'Parameter'
          end
          object SamplesLabel: TLabel
            Left = 25
            Top = 94
            Width = 50
            Height = 17
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'Samples'
          end
          object SweepRadio: TRadioButton
            Left = 22
            Top = 21
            Width = 67
            Height = 22
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'Sweep'
            Checked = True
            TabOrder = 0
            TabStop = True
            OnClick = SweepRadioClick
          end
          object OptimumRadio: TRadioButton
            Left = 105
            Top = 21
            Width = 87
            Height = 22
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'Optimum'
            TabOrder = 1
            OnClick = OptimumRadioClick
          end
          object ParameterCombo: TComboBox
            Left = 98
            Top = 51
            Width = 144
            Height = 25
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
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
            Left = 98
            Top = 86
            Width = 77
            Height = 27
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            MaxValue = 1000
            MinValue = 2
            TabOrder = 3
            Value = 5
          end
        end
      end
    end
    object AccelPanel: TPanel
      Left = 367
      Top = 1
      Width = 287
      Height = 533
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Align = alClient
      TabOrder = 1
      object AccelGroup: TGroupBox
        Left = 1
        Top = 1
        Width = 285
        Height = 531
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Align = alClient
        Caption = 'OptimizeAccelerator'
        TabOrder = 0
        object AccelCheck: TCheckBox
          Left = 21
          Top = 27
          Width = 127
          Height = 23
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Optimize'
          Checked = True
          State = cbChecked
          TabOrder = 0
        end
        object StructureGroup: TGroupBox
          Left = 21
          Top = 58
          Width = 242
          Height = 82
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Structure Type'
          TabOrder = 1
          object GradientRadio: TRadioButton
            Left = 21
            Top = 52
            Width = 148
            Height = 23
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'Constant Gradient'
            Checked = True
            TabOrder = 0
            TabStop = True
          end
          object ImpedanceRadio: TRadioButton
            Left = 21
            Top = 22
            Width = 158
            Height = 22
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Caption = 'Constant Impedance'
            TabOrder = 1
          end
        end
      end
      object TargetGroup: TGroupBox
        Left = 21
        Top = 149
        Width = 242
        Height = 135
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Target Energy'
        TabOrder = 1
        object EnergyLabel: TLabel
          Left = 21
          Top = 26
          Width = 44
          Height = 17
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Energy'
        end
        object EnergyDim: TLabel
          Left = 166
          Top = 26
          Width = 25
          Height = 17
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'MeV'
        end
        object EnergyEdit: TEdit
          Left = 73
          Top = 20
          Width = 85
          Height = 25
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          TabOrder = 0
          Text = '20.0'
        end
        object StartCellCheck: TCheckBox
          Left = 21
          Top = 85
          Width = 116
          Height = 22
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Start from cell'
          TabOrder = 1
          OnClick = StartCellCheckClick
        end
        object StartCellSpin: TCSpinEdit
          Left = 145
          Top = 85
          Width = 66
          Height = 27
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          TabOrder = 2
        end
      end
      object ExactCheck: TCheckBox
        Left = 42
        Top = 204
        Width = 106
        Height = 22
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Exact Value'
        TabOrder = 2
      end
    end
  end
  object BottomPanel: TPanel
    Left = 0
    Top = 535
    Width = 655
    Height = 131
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alBottom
    TabOrder = 1
    object ProgressPanel: TPanel
      Left = 1
      Top = 1
      Width = 536
      Height = 129
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Align = alClient
      TabOrder = 0
      object OptimizerProgress: TProgressBar
        Left = 21
        Top = 18
        Width = 493
        Height = 23
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Max = 0
        TabOrder = 0
      end
      object SaveGraphCheck: TCheckBox
        Left = 238
        Top = 48
        Width = 127
        Height = 23
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Save Results'
        TabOrder = 1
      end
      object StartButton: TButton
        Left = 177
        Top = 78
        Width = 98
        Height = 33
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Start'
        TabOrder = 2
        OnClick = StartButtonClick
      end
      object AbortButton: TButton
        Left = 282
        Top = 78
        Width = 99
        Height = 33
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Abort'
        TabOrder = 3
        OnClick = AbortButtonClick
      end
    end
    object ButtonsPanel: TPanel
      Left = 537
      Top = 1
      Width = 117
      Height = 129
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Align = alRight
      TabOrder = 1
      object CloseButton: TButton
        Left = 8
        Top = 86
        Width = 98
        Height = 33
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Close'
        TabOrder = 0
        OnClick = CloseButtonClick
      end
      object LoadButton: TButton
        Left = 8
        Top = 46
        Width = 95
        Height = 32
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Load Results'
        TabOrder = 1
      end
      object ResultsButton: TButton
        Left = 8
        Top = 5
        Width = 98
        Height = 33
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'View Results'
        Enabled = False
        TabOrder = 2
        OnClick = ResultsButtonClick
      end
    end
  end
end
