object ResForm: TResForm
  Left = 0
  Top = 0
  Caption = 'Results'
  ClientHeight = 876
  ClientWidth = 1257
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 120
  TextHeight = 17
  object GraphPanel: TPanel
    Left = 0
    Top = 0
    Width = 1015
    Height = 758
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alClient
    TabOrder = 0
    object PackChart: TChart
      Left = 1
      Top = 1
      Width = 1013
      Height = 756
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      AllowPanning = pmNone
      BackWall.Color = clWhite
      Legend.CheckBoxesStyle = cbsRadio
      Legend.Font.Height = -24
      MarginTop = 5
      Title.Font.Height = -32
      Title.Text.Strings = (
        '')
      BottomAxis.LabelsAngle = 90
      BottomAxis.LabelsFormat.Font.Height = -24
      BottomAxis.Title.Font.Height = -29
      DepthAxis.Automatic = False
      DepthAxis.AutomaticMaximum = False
      DepthAxis.AutomaticMinimum = False
      DepthAxis.Maximum = 0.500000000000000200
      DepthAxis.Minimum = -0.500000000000000000
      DepthTopAxis.Automatic = False
      DepthTopAxis.AutomaticMaximum = False
      DepthTopAxis.AutomaticMinimum = False
      DepthTopAxis.Maximum = 0.500000000000000200
      DepthTopAxis.Minimum = -0.500000000000000000
      LeftAxis.LabelsFormat.Font.Height = -24
      LeftAxis.MaximumOffset = 3
      LeftAxis.Title.Font.Height = -29
      RightAxis.Automatic = False
      RightAxis.AutomaticMaximum = False
      RightAxis.AutomaticMinimum = False
      TopAxis.GridCentered = True
      View3D = False
      View3DOptions.Orthogonal = False
      View3DWalls = False
      Align = alClient
      Color = clWhite
      TabOrder = 0
      DefaultCanvas = 'TGDIPlusCanvas'
      PrintMargins = (
        15
        6
        15
        6)
      ColorPaletteIndex = 13
      object PackSeries: TLineSeries
        Legend.Visible = False
        Depth = 0
        Marks.Font.Height = -24
        ShowInLegend = False
        Title = 'Energy'
        Brush.BackColor = clDefault
        LinePen.Color = clRed
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        XValues.Name = 'X'
        XValues.Order = loNone
        YValues.Name = 'Y'
        YValues.Order = loAscending
        Data = {
          00190000000000000000F876C00000000000687A400000000000387340000000
          00001875400000000000A87B4000000000003076400000000000687540000000
          00006078400000000000207C400000000000B875400000000000207C40000000
          0000B87F4000000000001880400000000000A881400000000000108340000000
          00006C81400000000000887D400000000000907F400000000000287E40000000
          0000D07B400000000000BC814000000000006484400000000000C48340000000
          00005485400000000000688540}
      end
      object AddSeries: TLineSeries
        ColorEachLine = False
        Marks.DrawEvery = 20
        SeriesColor = 8453888
        Title = 'Additional'
        Brush.BackColor = clDefault
        LinePen.Width = 4
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
        Data = {
          01190000000000000000F876C08FC2F5285C8F204055555555553A74C00AD7A3
          703D0A1740AAAAAAAAAA7C71C03333333333331F40FFFFFFFFFF7D6DC0666666
          6666662440AAAAAAAAAA0268C014AE47E17A14294055555555558762C0EB51B8
          1E85EB2C400000000000185AC0F5285C8FC2F52B40ABAAAAAAAA424EC0B81E85
          EB51B82B40ACAAAAAAAAAA30C0703D0AD7A3702940FEFFFFFFFF2F3B407A14AE
          47E17A2E40AAAAAAAAAAC2514084EB51B81E852A405455555555B95C40B71E85
          EB51B82840FFFFFFFFFFD76340FFFFFFFFFFFF24405455555555536940E07A14
          AE47E12440A9AAAAAAAACE6E4051B81E85EB512340FFFFFFFFFF24724047E17A
          14AE472440AAAAAAAAAAE27440F5285C8FC2F522405555555555A07740D6A370
          3D0AD71B4000000000005E7A40B81E85EB51B82240ABAAAAAAAA1B7D40C2F528
          5C8FC221405655555555D97F407A14AE47E17A184000000000804B8140285C8F
          C2F52814405555555555AA8240B71E85EB51B81240AAAAAAAA2A098440989999
          9999991540FFFFFFFFFF678540E851B81E85EB0940}
      end
      object LineSeries: TLineSeries
        SeriesColor = clBlue
        Title = 'Field'
        Brush.BackColor = clDefault
        LinePen.Width = 5
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
        Data = {
          01190000000000000000F876C0295C8FC2F528044055555555553A74C0B81E85
          EB51B80240AAAAAAAAAA7C71C03D0AD7A3703D0240FFFFFFFFFF7D6DC0285C8F
          C2F528F43FAAAAAAAAAA0268C0333333333333034055555555558762C07B14AE
          47E17A08400000000000185AC00AD7A3703D0A1140ABAAAAAAAA424EC047E17A
          14AE471740ACAAAAAAAAAA30C0F5285C8FC2F51240FEFFFFFFFF2F3B40CACCCC
          CCCCCCFC3FAAAAAAAAAAC25140285C8FC2F52804405455555555B95C40656666
          6666660A40FFFFFFFFFFD76340E07A14AE47E10A405455555555536940F5285C
          8FC2F51240A9AAAAAAAACE6E40FFFFFFFFFFFF1140FFFFFFFFFF24724046E17A
          14AE470D40AAAAAAAAAAE27440B51E85EB51B8FE3F5555555555A0774013AE47
          E17A14064000000000005E7A40FFFFFFFFFFFF0740ABAAAAAAAA1B7D4002D7A3
          703D0AD73F5655555555D97F400ED7A3703D0AE73F00000000804B8140BA1E85
          EB51B8FE3F5555555555AA82400000000000003840AAAAAAAA2A098440000000
          0000003840FFFFFFFFFF6785400000000000003840}
      end
    end
    object SpectrumChart: TChart
      Left = 1
      Top = 1
      Width = 1013
      Height = 756
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Legend.Visible = False
      Title.Font.Height = -32
      Title.Text.Strings = (
        'TChart')
      BottomAxis.LabelsAngle = 90
      BottomAxis.LabelsFormat.Font.Height = -24
      BottomAxis.Title.Font.Height = -29
      DepthAxis.Automatic = False
      DepthAxis.AutomaticMaximum = False
      DepthAxis.AutomaticMinimum = False
      DepthAxis.Maximum = 0.500000000000005600
      DepthAxis.Minimum = -0.499999999999991700
      DepthTopAxis.Automatic = False
      DepthTopAxis.AutomaticMaximum = False
      DepthTopAxis.AutomaticMinimum = False
      DepthTopAxis.Maximum = 0.500000000000005600
      DepthTopAxis.Minimum = -0.499999999999991700
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.LabelsFormat.Font.Height = -24
      LeftAxis.Maximum = 1000.000000000000000000
      LeftAxis.Title.Font.Height = -29
      RightAxis.Automatic = False
      RightAxis.AutomaticMaximum = False
      RightAxis.AutomaticMinimum = False
      View3D = False
      Align = alClient
      Color = clWhite
      TabOrder = 4
      DefaultCanvas = 'TGDIPlusCanvas'
      ColorPaletteIndex = 13
      object SpectrumSeries: TBarSeries
        BarPen.Visible = False
        Marks.Visible = False
        SeriesColor = clRed
        Title = 'SpectrumSeries'
        BarWidthPercent = 100
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
      object SpecEnvSeries: TLineSeries
        SeriesColor = clBlue
        Title = 'Envelope'
        Brush.BackColor = clDefault
        LinePen.Width = 3
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
    end
    object BarsChart: TChart
      Left = 1
      Top = 1
      Width = 1013
      Height = 756
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      AllowPanning = pmNone
      Legend.Font.Height = -24
      Title.Font.Height = -32
      Title.Text.Strings = (
        'Losses Diagram')
      BottomAxis.LabelsAngle = 90
      BottomAxis.LabelsFormat.Font.Height = -24
      BottomAxis.Title.Font.Height = -29
      LeftAxis.GridCentered = True
      LeftAxis.LabelsFormat.Font.Height = -24
      LeftAxis.Title.Font.Height = -29
      View3D = False
      View3DWalls = False
      Align = alClient
      Color = clWhite
      TabOrder = 1
      DefaultCanvas = 'TGDIPlusCanvas'
      ColorPaletteIndex = 13
      object BarSeries1: TBarSeries
        BarPen.Visible = False
        Marks.Visible = False
        Marks.Arrow.Visible = False
        Marks.Callout.Arrow.Visible = False
        Title = 'Radius Loss'
        BarWidthPercent = 100
        MultiBar = mbStacked
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
      object BarSeries2: TBarSeries
        BarPen.Visible = False
        Marks.Visible = False
        Marks.Arrow.Visible = False
        Marks.Callout.Arrow.Visible = False
        SeriesColor = clBlue
        Title = 'Phase Loss'
        BarWidthPercent = 100
        MultiBar = mbStacked
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
      object BarSeries3: TBarSeries
        BarPen.Visible = False
        Marks.Visible = False
        SeriesColor = 4259584
        Title = 'Betta Loss'
        BarWidthPercent = 100
        MultiBar = mbStacked
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
      object BarSeries4: TBarSeries
        BarPen.Visible = False
        Marks.Visible = False
        SeriesColor = clYellow
        Title = 'Bz Loss'
        BarWidthPercent = 100
        MultiBar = mbStacked
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
      object BarSeries5: TBarSeries
        BarPen.Visible = False
        Marks.Visible = False
        SeriesColor = 16744703
        Title = 'Bx Loss'
        BarWidthPercent = 100
        MultiBar = mbStacked
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
      object BarSeries6: TBarSeries
        BarPen.Visible = False
        Marks.Visible = False
        SeriesColor = clSilver
        Title = 'Step Loss'
        BarWidthPercent = 100
        MultiBar = mbStacked
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
    end
    object PieChart: TChart
      Left = 1
      Top = 1
      Width = 1013
      Height = 756
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      AllowPanning = pmNone
      Legend.Font.Height = -24
      Legend.TextStyle = ltsRightPercent
      Title.Font.Height = -24
      Title.Text.Strings = (
        'Losses')
      BottomAxis.LabelsFormat.Font.Height = -24
      BottomAxis.Title.Font.Height = -24
      DepthAxis.Automatic = False
      DepthAxis.AutomaticMaximum = False
      DepthAxis.AutomaticMinimum = False
      DepthAxis.Maximum = -0.500000000000000000
      DepthAxis.Minimum = -0.500000000000000000
      DepthTopAxis.Automatic = False
      DepthTopAxis.AutomaticMaximum = False
      DepthTopAxis.AutomaticMinimum = False
      DepthTopAxis.Maximum = -0.500000000000000000
      DepthTopAxis.Minimum = -0.500000000000000000
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.LabelsFormat.Font.Height = -24
      LeftAxis.Title.Font.Height = -24
      RightAxis.Automatic = False
      RightAxis.AutomaticMaximum = False
      RightAxis.AutomaticMinimum = False
      View3DOptions.Elevation = 315
      View3DOptions.Orthogonal = False
      View3DOptions.Perspective = 0
      View3DOptions.Rotation = 360
      View3DWalls = False
      Align = alClient
      Color = clWhite
      TabOrder = 2
      DefaultCanvas = 'TGDIPlusCanvas'
      ColorPaletteIndex = 13
      object PieSeries: TPieSeries
        Marks.Font.Height = -24
        XValues.Order = loAscending
        YValues.Name = 'Pie'
        YValues.Order = loNone
        Frame.InnerBrush.BackColor = clRed
        Frame.InnerBrush.Gradient.EndColor = clGray
        Frame.InnerBrush.Gradient.MidColor = clWhite
        Frame.InnerBrush.Gradient.StartColor = 4210752
        Frame.InnerBrush.Gradient.Visible = True
        Frame.MiddleBrush.BackColor = clYellow
        Frame.MiddleBrush.Gradient.EndColor = 8553090
        Frame.MiddleBrush.Gradient.MidColor = clWhite
        Frame.MiddleBrush.Gradient.StartColor = clGray
        Frame.MiddleBrush.Gradient.Visible = True
        Frame.OuterBrush.BackColor = clGreen
        Frame.OuterBrush.Gradient.EndColor = 4210752
        Frame.OuterBrush.Gradient.MidColor = clWhite
        Frame.OuterBrush.Gradient.StartColor = clSilver
        Frame.OuterBrush.Gradient.Visible = True
        Frame.Width = 4
        OtherSlice.Legend.Visible = False
      end
    end
    object BeamChart: TChart
      Left = 1
      Top = 1
      Width = 1013
      Height = 756
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      AllowPanning = pmNone
      Legend.Visible = False
      Title.Font.Height = -32
      Title.Text.Strings = (
        'Beam Portrait')
      BottomAxis.LabelsAngle = 90
      BottomAxis.LabelsFormat.Font.Height = -24
      BottomAxis.Title.Font.Height = -29
      LeftAxis.AxisValuesFormat = '#,##0.000'
      LeftAxis.LabelsFormat.Font.Height = -24
      LeftAxis.Title.Font.Height = -29
      RightAxis.Title.Font.Height = -29
      View3D = False
      Zoom.Allow = False
      Align = alClient
      Color = clWhite
      TabOrder = 3
      DefaultCanvas = 'TGDIPlusCanvas'
      ColorPaletteIndex = 13
      object BeamSeries: TPointSeries
        Title = 'Portrait'
        ClickableLine = False
        Pointer.Brush.Color = clRed
        Pointer.InflateMargins = True
        Pointer.Style = psCircle
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
      object EnvelopeSeries: TLineSeries
        SeriesColor = clBlue
        Brush.BackColor = clDefault
        LinePen.Width = 2
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        XValues.Name = 'X'
        XValues.Order = loNone
        YValues.Name = 'Y'
        YValues.Order = loAscending
      end
    end
  end
  object BottomPanel: TPanel
    Left = 0
    Top = 758
    Width = 1257
    Height = 118
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alBottom
    TabOrder = 1
    DesignSize = (
      1257
      118)
    object EnergyButton: TSpeedButton
      Left = 21
      Top = 7
      Width = 85
      Height = 28
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Energy'
      OnClick = EnergyButtonClick
    end
    object PhaseButton: TSpeedButton
      Left = 21
      Top = 43
      Width = 85
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Phase'
      OnClick = PhaseButtonClick
    end
    object RadiusButton: TSpeedButton
      Left = 21
      Top = 80
      Width = 85
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Radius'
      OnClick = RadiusButtonClick
    end
    object FieldButton: TSpeedButton
      Left = 135
      Top = 7
      Width = 85
      Height = 28
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Field'
      OnClick = FieldButtonClick
    end
    object LossButton: TSpeedButton
      Left = 679
      Top = 44
      Width = 108
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Loss Diagram'
      OnClick = LossButtonClick
    end
    object LossPieButton: TSpeedButton
      Left = 679
      Top = 8
      Width = 108
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Loss Pie'
      OnClick = LossPieButtonClick
    end
    object FSpectrumButton: TSpeedButton
      Left = 247
      Top = 7
      Width = 119
      Height = 28
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Phase Spectrum'
      OnClick = FSpectrumButtonClick
    end
    object WSpectrumButton: TSpeedButton
      Left = 247
      Top = 43
      Width = 119
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Energy Spectrum'
      OnClick = WSpectrumButtonClick
    end
    object LongtitudalButton: TSpeedButton
      Left = 543
      Top = 7
      Width = 108
      Height = 28
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Phase Space'
      OnClick = LongtitudalButtonClick
    end
    object TransverseButton: TSpeedButton
      Left = 395
      Top = 8
      Width = 119
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Transverse Space'
      OnClick = TransverseButtonClick
    end
    object TransSectionButton: TSpeedButton
      Left = 395
      Top = 43
      Width = 119
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Transv. Section'
      OnClick = TransSectionButtonClick
    end
    object PowerButton: TSpeedButton
      Left = 135
      Top = 43
      Width = 85
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Power'
      OnClick = PowerButtonClick
    end
    object AvEnergyButton: TSpeedButton
      Left = 135
      Top = 80
      Width = 85
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Av. Energy'
      OnClick = AvEnergyButtonClick
    end
    object BettaButton: TSpeedButton
      Left = 247
      Top = 80
      Width = 119
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Phase Velocity'
      OnClick = BettaButtonClick
    end
    object LongtSectonButton: TSpeedButton
      Left = 543
      Top = 43
      Width = 108
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Longt. Section'
      OnClick = LongtSectonButtonClick
    end
    object MotionButton: TSpeedButton
      Left = 543
      Top = 80
      Width = 108
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Longt. Motion'
      OnClick = MotionButtonClick
    end
    object SlidingButton: TSpeedButton
      Left = 679
      Top = 81
      Width = 108
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Phase Sliding'
      OnClick = SlidingButtonClick
    end
    object AvRadiusButton: TSpeedButton
      Left = 395
      Top = 80
      Width = 119
      Height = 29
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      GroupIndex = 1
      Caption = 'Beam Radius'
      OnClick = AvRadiusButtonClick
    end
    object TrackBox: TGroupBox
      Left = 795
      Top = 7
      Width = 462
      Height = 109
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Anchors = [akLeft, akTop, akRight]
      Caption = 'Seek Position'
      TabOrder = 0
      DesignSize = (
        462
        109)
      object PrevButton: TSpeedButton
        Left = 55
        Top = 61
        Width = 30
        Height = 29
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = '<|'
        OnClick = PrevButtonClick
      end
      object FFButton: TSpeedButton
        Left = 169
        Top = 61
        Width = 30
        Height = 29
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = '>>'
        OnClick = FFButtonClick
      end
      object RewButton: TSpeedButton
        Left = 17
        Top = 61
        Width = 30
        Height = 29
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = '<<'
        OnClick = RewButtonClick
      end
      object PlayButton: TSpeedButton
        Left = 93
        Top = 61
        Width = 30
        Height = 29
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        AllowAllUp = True
        GroupIndex = 2
        Caption = '>'
        OnClick = PlayButtonClick
      end
      object NextButton: TSpeedButton
        Left = 131
        Top = 61
        Width = 30
        Height = 29
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = '|>'
        OnClick = NextButtonClick
      end
      object CellLabel: TLabel
        Left = 378
        Top = 61
        Width = 58
        Height = 17
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Anchors = [akTop, akRight]
        Caption = 'Cell #001'
      end
      object PositionLabel: TLabel
        Left = 360
        Top = 86
        Width = 79
        Height = 17
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Anchors = [akTop, akRight]
        Caption = 'z = 0.000cm'
      end
      object PositionTrackBar: TTrackBar
        Left = 20
        Top = 18
        Width = 438
        Height = 36
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 0
        OnChange = PositionTrackBarChange
      end
    end
  end
  object RightPanel: TPanel
    Left = 1015
    Top = 0
    Width = 242
    Height = 758
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alRight
    TabOrder = 2
    object GroupBox1: TGroupBox
      Left = 1
      Top = 620
      Width = 240
      Height = 137
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Align = alBottom
      Caption = 'Options'
      TabOrder = 0
      object EnvelopeCheck: TCheckBox
        Left = 7
        Top = 21
        Width = 126
        Height = 22
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Envelope'
        Checked = True
        State = cbChecked
        TabOrder = 0
        OnClick = EnvelopeCheckClick
      end
      object ChartCheck: TCheckBox
        Left = 7
        Top = 51
        Width = 126
        Height = 22
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Chart'
        Checked = True
        State = cbChecked
        TabOrder = 1
        OnClick = ChartCheckClick
      end
      object HorFitCheck: TCheckBox
        Left = 7
        Top = 109
        Width = 126
        Height = 22
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Fit Horizontally'
        TabOrder = 2
        OnClick = HorFitCheckClick
      end
      object VertFitCheck: TCheckBox
        Left = 7
        Top = 78
        Width = 126
        Height = 23
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Fit Vertically'
        TabOrder = 3
        OnClick = VertFitCheckClick
      end
    end
    object TableGroup: TGroupBox
      Left = 1
      Top = 1
      Width = 240
      Height = 619
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Align = alClient
      Caption = 'Parameters'
      TabOrder = 1
      object Table: TStringGrid
        Left = 2
        Top = 19
        Width = 236
        Height = 598
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Align = alClient
        ColCount = 2
        FixedCols = 0
        RowCount = 18
        FixedRows = 0
        TabOrder = 0
        OnDrawCell = TableDrawCell
      end
    end
  end
  object MainMenu1: TMainMenu
    Left = 40
    object File1: TMenuItem
      Caption = 'File'
      object OpenFile1: TMenuItem
        Caption = 'Open File'
        OnClick = OpenFile1Click
      end
      object SaveAs1: TMenuItem
        Caption = 'Save As'
        OnClick = SaveAs1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'Exit'
        OnClick = Exit1Click
      end
    end
    object About1: TMenuItem
      Caption = 'About'
    end
  end
  object OpenDialog: TOpenDialog
    Left = 72
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'res'
    Left = 104
  end
end
