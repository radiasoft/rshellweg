object GeomForm: TGeomForm
  Left = 0
  Top = 0
  Caption = 'View Geometry'
  ClientHeight = 681
  ClientWidth = 921
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 120
  TextHeight = 17
  object BottomPanel: TPanel
    Left = 0
    Top = 544
    Width = 921
    Height = 137
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alBottom
    TabOrder = 0
    object ChartGroup: TRadioGroup
      Left = 1
      Top = 1
      Width = 366
      Height = 135
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Align = alLeft
      Caption = 'Geometry Chart Data'
      Columns = 2
      ItemIndex = 0
      Items.Strings = (
        'Phase Velocity (betta)'
        'Field (A)'
        'Field (E*lambda/sqrt(P))'
        'Field (E) nominal'
        'Attenuation (alpha)'
        'Aperture (Ra)'
        'Solenoid Field (Bz)')
      TabOrder = 0
      OnClick = ChartGroupClick
    end
    object BeamGroup: TRadioGroup
      Left = 367
      Top = 1
      Width = 303
      Height = 135
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Align = alLeft
      Caption = 'Beam Chart Data'
      Columns = 2
      Enabled = False
      ItemIndex = 0
      Items.Strings = (
        'Emittance'
        'Phase Portrait'
        'Cross Section'
        'Energy Distribution'
        'Phase Distribution')
      TabOrder = 1
      OnClick = BeamGroupClick
    end
    object EnvelopeCB: TCheckBox
      Left = 677
      Top = 105
      Width = 127
      Height = 22
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Envelope'
      Checked = True
      State = cbChecked
      TabOrder = 2
    end
    object BeamBox: TCheckBox
      Left = 677
      Top = 84
      Width = 127
      Height = 22
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Particles'
      Checked = True
      State = cbChecked
      TabOrder = 3
    end
  end
  object WorkPanel: TPanel
    Left = 0
    Top = 0
    Width = 921
    Height = 544
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alClient
    TabOrder = 1
    object RightPanel: TPanel
      Left = 678
      Top = 1
      Width = 242
      Height = 542
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Align = alRight
      TabOrder = 0
      object CompBox1: TGroupBox
        Left = 1
        Top = 1
        Width = 240
        Height = 105
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Align = alTop
        Caption = 'Comparison Data 1'
        TabOrder = 0
        object LineColor1: TShape
          Left = 7
          Top = 80
          Width = 85
          Height = 15
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Brush.Color = clBlue
        end
        object OpenButton1: TButton
          Left = 166
          Top = 42
          Width = 34
          Height = 33
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = '...'
          TabOrder = 0
          OnClick = OpenButton1Click
        end
        object FileEdit1: TLabeledEdit
          Left = 69
          Top = 44
          Width = 89
          Height = 25
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          EditLabel.Width = 55
          EditLabel.Height = 17
          EditLabel.Margins.Left = 4
          EditLabel.Margins.Top = 4
          EditLabel.Margins.Right = 4
          EditLabel.Margins.Bottom = 4
          EditLabel.Caption = 'Input File'
          LabelPosition = lpLeft
          TabOrder = 1
        end
        object Compare1: TCheckBox
          Left = 7
          Top = 21
          Width = 126
          Height = 22
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Compare'
          TabOrder = 2
          OnClick = Compare1Click
        end
      end
      object ParametersGroup: TGroupBox
        Left = 1
        Top = 346
        Width = 240
        Height = 195
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Align = alBottom
        Caption = 'Parameters'
        TabOrder = 1
        object aParLabel: TLabel
          Left = 7
          Top = 21
          Width = 42
          Height = 17
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'alpha='
        end
        object bParLabel: TLabel
          Left = 7
          Top = 42
          Width = 42
          Height = 17
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'betta='
        end
        object eParLabel: TLabel
          Left = 7
          Top = 63
          Width = 51
          Height = 17
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'epsilon='
        end
        object WavParLabel: TLabel
          Left = 7
          Top = 84
          Width = 39
          Height = 17
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Wav='
        end
        object dWParLabel: TLabel
          Left = 7
          Top = 105
          Width = 32
          Height = 17
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'dW='
        end
        object FavParLabel: TLabel
          Left = 7
          Top = 126
          Width = 47
          Height = 17
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Phi av='
        end
        object dFParLabel: TLabel
          Left = 7
          Top = 146
          Width = 36
          Height = 17
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'dPhi='
        end
        object rParLabel: TLabel
          Left = 7
          Top = 171
          Width = 27
          Height = 17
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Rb='
        end
      end
    end
    object GraphPanel: TPanel
      Left = 1
      Top = 1
      Width = 677
      Height = 542
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Align = alClient
      TabOrder = 1
      object GChart: TChart
        Left = 1
        Top = 1
        Width = 675
        Height = 540
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        AllowPanning = pmNone
        Legend.Visible = False
        Title.Font.Height = -32
        Title.Text.Strings = (
          'Geometry Parameters')
        BottomAxis.AxisValuesFormat = '#0.##'
        BottomAxis.GridCentered = True
        BottomAxis.Increment = 0.100000000000000000
        BottomAxis.LabelsAngle = 90
        BottomAxis.LabelsFormat.Font.Height = -37
        BottomAxis.LabelStyle = talValue
        BottomAxis.LogarithmicBase = 2.718281828459050000
        BottomAxis.TickOnLabelsOnly = False
        BottomAxis.Title.Caption = 'U, V'
        BottomAxis.Title.Font.Height = -48
        BottomAxis.Title.Font.Style = [fsBold]
        DepthAxis.Automatic = False
        DepthAxis.AutomaticMaximum = False
        DepthAxis.AutomaticMinimum = False
        DepthAxis.Maximum = 1.040000000000000000
        DepthAxis.Minimum = 0.040000000000000240
        DepthTopAxis.Automatic = False
        DepthTopAxis.AutomaticMaximum = False
        DepthTopAxis.AutomaticMinimum = False
        DepthTopAxis.Maximum = 1.040000000000000000
        DepthTopAxis.Minimum = 0.040000000000000240
        LeftAxis.AxisValuesFormat = '#0.#'
        LeftAxis.GridCentered = True
        LeftAxis.LabelsFormat.Font.Height = -37
        LeftAxis.LabelStyle = talValue
        LeftAxis.RoundFirstLabel = False
        LeftAxis.TickOnLabelsOnly = False
        LeftAxis.Title.Caption = 'I, mA'
        LeftAxis.Title.Font.Height = -48
        LeftAxis.Title.Font.Style = [fsBold]
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        RightAxis.GridCentered = True
        TopAxis.Automatic = False
        TopAxis.AutomaticMaximum = False
        TopAxis.AutomaticMinimum = False
        View3D = False
        Zoom.Allow = False
        Align = alClient
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        PrintMargins = (
          15
          10
          15
          10)
        ColorPaletteIndex = 13
        object Series1: TLineSeries
          ColorEachPoint = True
          Depth = 0
          Brush.BackColor = clDefault
          LinePen.Width = 3
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
          Data = {
            0019000000B81E85EB11B1884085EB51B8BEE5884085EB51B80E628840666666
            6656B6884000000000009C884033333333B36E894047E17A149E0A8940F5285C
            8F328C8840A3703D0A7791884032333333A3E38740E07A14AE976C884051B81E
            850B1388403D0AD7A370F38740B81E85EB612D8840E17A14AE976C8840333333
            335367884000000000009C88401F85EB5168CB8840F6285C8F92938940A4703D
            0A27158940295C8FC28557884066666666F6AE874047E17A148E7F874047E17A
            148E7F87405C8FC2F5D8228840}
        end
        object Series2: TLineSeries
          ColorEachPoint = True
          SeriesColor = clRed
          Brush.BackColor = clDefault
          LinePen.Width = 3
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object BeamSeries1: TPointSeries
          Legend.Visible = False
          Depth = 0
          Marks.Clip = True
          Marks.DrawEvery = 100
          SeriesColor = clRed
          ShowInLegend = False
          Title = 'BeamSeries1'
          ClickableLine = False
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Pen.Visible = False
          Pointer.Style = psCircle
          XValues.Name = 'X'
          XValues.Order = loNone
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object BarSeries1: TBarSeries
          Marks.Visible = False
          Marks.Callout.Length = 8
          SeriesColor = clRed
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
        end
        object EnvelopeSeries1: TLineSeries
          Depth = 0
          Marks.Style = smsValue
          Marks.Clip = True
          Marks.DrawEvery = 50
          SeriesColor = clBlue
          Title = 'EnvelopeSeries1'
          Brush.BackColor = clDefault
          LinePen.Color = clBlue
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loNone
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
  end
  object MainMenu1: TMainMenu
    object File1: TMenuItem
      Caption = 'File'
      object Exit1: TMenuItem
        Caption = 'Colse'
        OnClick = Exit1Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 672
    Top = 32
  end
end
