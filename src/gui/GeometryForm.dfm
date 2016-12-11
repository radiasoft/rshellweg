object GeomForm: TGeomForm
  Left = 0
  Top = 0
  Caption = 'View Geometry'
  ClientHeight = 521
  ClientWidth = 704
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object BottomPanel: TPanel
    Left = 0
    Top = 416
    Width = 704
    Height = 105
    Align = alBottom
    TabOrder = 0
    object ChartGroup: TRadioGroup
      Left = 1
      Top = 1
      Width = 280
      Height = 103
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
      Left = 281
      Top = 1
      Width = 320
      Height = 103
      Align = alLeft
      Caption = 'Beam Chart Data'
      Columns = 3
      Enabled = False
      ItemIndex = 0
      Items.Strings = (
        'R-R'#39
        'X-X'#39
        'Y-Y'#39
        'X-Y'
        'R-Theta'
        'Theta-Theta'#39
        'W-phi'
        'Energy Spectrum'
        'Phase Spectrum'
        'R Spectrum'
        'X Spectrum'
        'Y Spectrum')
      TabOrder = 1
      OnClick = BeamGroupClick
    end
    object EnvelopeCB: TCheckBox
      Left = 607
      Top = 24
      Width = 97
      Height = 17
      Caption = 'Envelope'
      Checked = True
      State = cbChecked
      TabOrder = 2
      OnClick = EnvelopeCBClick
    end
    object BeamBox: TCheckBox
      Left = 607
      Top = 6
      Width = 97
      Height = 17
      Caption = 'Particles'
      Checked = True
      State = cbChecked
      TabOrder = 3
      OnClick = BeamBoxClick
    end
  end
  object WorkPanel: TPanel
    Left = 0
    Top = 0
    Width = 704
    Height = 416
    Align = alClient
    TabOrder = 1
    object RightPanel: TPanel
      Left = 517
      Top = 1
      Width = 186
      Height = 414
      Align = alRight
      TabOrder = 0
      object CompBox1: TGroupBox
        Left = 1
        Top = 1
        Width = 184
        Height = 80
        Align = alTop
        Caption = 'Comparison Data 1'
        TabOrder = 0
        object LineColor1: TShape
          Left = 5
          Top = 61
          Width = 65
          Height = 12
          Brush.Color = clBlue
        end
        object OpenButton1: TButton
          Left = 127
          Top = 32
          Width = 26
          Height = 25
          Caption = '...'
          TabOrder = 0
          OnClick = OpenButton1Click
        end
        object FileEdit1: TLabeledEdit
          Left = 53
          Top = 34
          Width = 68
          Height = 21
          EditLabel.Width = 45
          EditLabel.Height = 13
          EditLabel.Caption = 'Input File'
          LabelPosition = lpLeft
          TabOrder = 1
        end
        object Compare1: TCheckBox
          Left = 5
          Top = 16
          Width = 97
          Height = 17
          Caption = 'Compare'
          TabOrder = 2
          OnClick = Compare1Click
        end
      end
      object ParametersGroup: TGroupBox
        Left = 1
        Top = 232
        Width = 184
        Height = 181
        Align = alBottom
        Caption = 'Parameters'
        TabOrder = 1
        object aParLabel: TLabel
          Left = 5
          Top = 16
          Width = 34
          Height = 13
          Caption = 'alpha='
        end
        object bParLabel: TLabel
          Left = 5
          Top = 32
          Width = 34
          Height = 13
          Caption = 'betta='
        end
        object eParLabel: TLabel
          Left = 5
          Top = 48
          Width = 41
          Height = 13
          Caption = 'epsilon='
        end
        object WavParLabel: TLabel
          Left = 4
          Top = 88
          Width = 30
          Height = 13
          Caption = 'Wav='
        end
        object dWParLabel: TLabel
          Left = 5
          Top = 107
          Width = 24
          Height = 13
          Caption = 'dW='
        end
        object FavParLabel: TLabel
          Left = 5
          Top = 126
          Width = 37
          Height = 13
          Caption = 'Phi av='
        end
        object dFParLabel: TLabel
          Left = 5
          Top = 145
          Width = 28
          Height = 13
          Caption = 'dPhi='
        end
        object rParLabel: TLabel
          Left = 5
          Top = 164
          Width = 21
          Height = 13
          Caption = 'Rb='
        end
        object enParLabel: TLabel
          Left = 5
          Top = 67
          Width = 44
          Height = 13
          Caption = 'e_norm='
        end
      end
    end
    object GraphPanel: TPanel
      Left = 1
      Top = 1
      Width = 516
      Height = 414
      Align = alClient
      TabOrder = 1
      object GChart: TChart
        Left = 1
        Top = 1
        Width = 514
        Height = 412
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
        LeftAxis.ExactDateTime = False
        LeftAxis.GridCentered = True
        LeftAxis.LabelsFormat.Font.Height = -37
        LeftAxis.LabelsSeparation = 0
        LeftAxis.LabelStyle = talValue
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
          Pointer.Transparency = 70
          XValues.Name = 'X'
          XValues.Order = loNone
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object BeamSeries2: TPointSeries
          Legend.Visible = False
          Depth = 0
          Marks.Clip = True
          Marks.DrawEvery = 100
          SeriesColor = clRed
          ShowInLegend = False
          Title = 'BeamSeries2'
          ClickableLine = False
          Pointer.Brush.Color = clMaroon
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Pen.Visible = False
          Pointer.Shadow.Transparency = 49
          Pointer.Style = psCircle
          Pointer.Transparency = 70
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
        object EnvelopeSeries2: TLineSeries
          Depth = 0
          Marks.Style = smsValue
          Marks.Clip = True
          Marks.DrawEvery = 50
          SeriesColor = clNavy
          Title = 'EnvelopeSeries2'
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
        Caption = 'Close'
        OnClick = Exit1Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 672
    Top = 32
  end
end
