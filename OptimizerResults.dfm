object OptForm: TOptForm
  Left = 0
  Top = 0
  Caption = 'OptForm'
  ClientHeight = 807
  ClientWidth = 1149
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 120
  TextHeight = 17
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 1149
    Height = 764
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alClient
    TabOrder = 0
    object InfoPanel: TPanel
      Left = 1106
      Top = 1
      Width = 42
      Height = 761
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Align = alRight
      TabOrder = 0
    end
    object MainChart: TChart
      Left = 1
      Top = 1
      Width = 1105
      Height = 761
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Title.Font.Height = -32
      Title.Text.Strings = (
        'TChart')
      BottomAxis.LabelsAngle = 90
      BottomAxis.LabelsFormat.Font.Height = -29
      BottomAxis.Title.Font.Height = -32
      LeftAxis.LabelsFormat.Font.Height = -29
      LeftAxis.Title.Font.Height = -32
      View3D = False
      Align = alClient
      Color = clWhite
      TabOrder = 1
      DefaultCanvas = 'TGDIPlusCanvas'
      ColorPaletteIndex = 13
      object AddSeries: TLineSeries
        Legend.Visible = False
        SeriesColor = clBlue
        ShowInLegend = False
        Brush.BackColor = clDefault
        LinePen.Width = 4
        Pointer.InflateMargins = True
        Pointer.Style = psCircle
        Pointer.Visible = True
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
      object LineSeries: TLineSeries
        Legend.Visible = False
        ShowInLegend = False
        Brush.BackColor = clDefault
        LinePen.Width = 4
        Pointer.InflateMargins = True
        Pointer.Style = psCircle
        Pointer.Visible = True
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
    end
  end
  object BottomPanel: TPanel
    Left = 0
    Top = 764
    Width = 1149
    Height = 43
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alBottom
    TabOrder = 1
    DesignSize = (
      1149
      43)
    object EnergyButton: TSpeedButton
      Left = 20
      Top = 7
      Width = 96
      Height = 28
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Energy'
      OnClick = EnergyButtonClick
    end
    object CaptureButton: TSpeedButton
      Left = 124
      Top = 5
      Width = 97
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Down = True
      Caption = 'Capture'
      OnClick = CaptureButtonClick
    end
    object SpectrumButton: TSpeedButton
      Left = 229
      Top = 5
      Width = 97
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Spectrum'
      OnClick = SpectrumButtonClick
    end
    object PhaseButton: TSpeedButton
      Left = 333
      Top = 5
      Width = 97
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Phase Length'
      OnClick = PhaseButtonClick
    end
    object CloseButton: TButton
      Left = 1063
      Top = 7
      Width = 67
      Height = 32
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Anchors = [akRight, akBottom]
      Caption = 'Close'
      TabOrder = 0
      OnClick = CloseButtonClick
    end
  end
end
