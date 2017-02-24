object OptForm: TOptForm
  Left = 0
  Top = 0
  Caption = 'OptForm'
  ClientHeight = 617
  ClientWidth = 879
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
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 879
    Height = 584
    Align = alClient
    TabOrder = 0
    object InfoPanel: TPanel
      Left = 846
      Top = 1
      Width = 32
      Height = 582
      Align = alRight
      TabOrder = 0
    end
    object MainChart: TChart
      Left = 1
      Top = 1
      Width = 845
      Height = 582
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
    Top = 584
    Width = 879
    Height = 33
    Align = alBottom
    TabOrder = 1
    DesignSize = (
      879
      33)
    object EnergyButton: TSpeedButton
      Left = 15
      Top = 5
      Width = 74
      Height = 22
      GroupIndex = 1
      Caption = 'Energy'
      OnClick = EnergyButtonClick
    end
    object CaptureButton: TSpeedButton
      Left = 95
      Top = 4
      Width = 74
      Height = 22
      GroupIndex = 1
      Down = True
      Caption = 'Capture'
      OnClick = CaptureButtonClick
    end
    object SpectrumButton: TSpeedButton
      Left = 175
      Top = 4
      Width = 74
      Height = 22
      GroupIndex = 1
      Caption = 'Spectrum'
      OnClick = SpectrumButtonClick
    end
    object PhaseButton: TSpeedButton
      Left = 255
      Top = 4
      Width = 74
      Height = 22
      GroupIndex = 1
      Caption = 'Phase Length'
      OnClick = PhaseButtonClick
    end
    object CloseButton: TButton
      Left = 813
      Top = 5
      Width = 51
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = 'Close'
      TabOrder = 0
      OnClick = CloseButtonClick
    end
  end
end
