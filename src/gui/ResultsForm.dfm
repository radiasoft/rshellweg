object ResForm: TResForm
  Left = 0
  Top = 0
  Caption = 'Results'
  ClientHeight = 741
  ClientWidth = 984
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  WindowState = wsMaximized
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GraphPanel: TPanel
    Left = 0
    Top = 0
    Width = 766
    Height = 639
    Align = alClient
    TabOrder = 0
    object PackChart: TChart
      Left = 1
      Top = 1
      Width = 764
      Height = 637
      AllowPanning = pmNone
      BackWall.Color = clWhite
      Legend.Alignment = laTop
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
        ColorEachLine = False
        Depth = 0
        Marks.Font.Height = -24
        SeriesColor = 8404992
        Shadow.Visible = False
        ShowInLegend = False
        Title = 'Energy'
        Brush.BackColor = clDefault
        ClickableLine = False
        Dark3D = False
        LinePen.Color = clBlue
        LinePen.EndStyle = esFlat
        LinePen.Visible = False
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        XValues.Name = 'X'
        XValues.Order = loNone
        YValues.Name = 'Y'
        YValues.Order = loAscending
        Transparency = 80
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
        SeriesColor = 4227072
        Title = 'Additional'
        Brush.BackColor = clDefault
        Dark3D = False
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
        Dark3D = False
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
      Width = 764
      Height = 637
      Legend.Visible = False
      Title.Font.Height = -32
      Title.Text.Strings = (
        'TChart')
      BottomAxis.AxisValuesFormat = '#,##0.00'
      BottomAxis.LabelsAngle = 90
      BottomAxis.LabelsFormat.Font.Height = -24
      BottomAxis.LabelsSeparation = 0
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
      Width = 764
      Height = 637
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
        SeriesColor = 33023
        Title = 'Bth Loss'
        BarWidthPercent = 100
        MultiBar = mbStacked
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
      object BarSeries7: TBarSeries
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
      Width = 764
      Height = 637
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
      Width = 764
      Height = 637
      AllowPanning = pmNone
      Legend.Visible = False
      Title.Font.Height = -32
      Title.Text.Strings = (
        'Beam Portrait')
      BottomAxis.LabelsAngle = 90
      BottomAxis.LabelsFormat.Font.Height = -24
      BottomAxis.TickOnLabelsOnly = False
      BottomAxis.Title.Font.Height = -29
      LeftAxis.ExactDateTime = False
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
        SeriesColor = clRed
        Title = 'Portrait'
        ClickableLine = False
        Pointer.InflateMargins = True
        Pointer.Style = psCircle
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
      object EnvelopeSeries: TLineSeries
        SeriesColor = 16744448
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
    Top = 639
    Width = 984
    Height = 102
    Align = alBottom
    TabOrder = 1
    object TrackBox: TGroupBox
      Left = 665
      Top = 1
      Width = 318
      Height = 100
      Align = alClient
      Caption = 'Seek Position'
      TabOrder = 0
      DesignSize = (
        318
        100)
      object PrevButton: TSpeedButton
        Left = 42
        Top = 47
        Width = 23
        Height = 22
        Caption = '<|'
        OnClick = PrevButtonClick
      end
      object FFButton: TSpeedButton
        Left = 129
        Top = 47
        Width = 23
        Height = 22
        Caption = '>>'
        OnClick = FFButtonClick
      end
      object RewButton: TSpeedButton
        Left = 13
        Top = 47
        Width = 23
        Height = 22
        Caption = '<<'
        OnClick = RewButtonClick
      end
      object PlayButton: TSpeedButton
        Left = 71
        Top = 47
        Width = 23
        Height = 22
        AllowAllUp = True
        GroupIndex = 2
        Caption = '>'
        OnClick = PlayButtonClick
      end
      object NextButton: TSpeedButton
        Left = 100
        Top = 47
        Width = 23
        Height = 22
        Caption = '|>'
        OnClick = NextButtonClick
      end
      object CellLabel: TLabel
        Left = 254
        Top = 47
        Width = 46
        Height = 13
        Anchors = [akTop, akRight]
        Caption = 'Cell #001'
        ExplicitLeft = 289
      end
      object PositionLabel: TLabel
        Left = 240
        Top = 66
        Width = 60
        Height = 13
        Anchors = [akTop, akRight]
        Caption = 'z = 0.000cm'
        ExplicitLeft = 275
      end
      object PositionTrackBar: TTrackBar
        Left = 15
        Top = 14
        Width = 300
        Height = 27
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 0
        OnChange = PositionTrackBarChange
      end
    end
    object RadiusGroup: TRadioGroup
      Left = 569
      Top = 1
      Width = 96
      Height = 100
      Align = alLeft
      Caption = 'Coordinate (r)'
      ItemIndex = 0
      Items.Strings = (
        'r: Radial '
        'x: Horizontal'
        'y: Vertical'
        '4D: Radial'
        'th: Azimuthal')
      TabOrder = 1
      OnClick = RadiusGroupClick
    end
    object TraceGroup: TGroupBox
      Left = 1
      Top = 1
      Width = 96
      Height = 100
      Align = alLeft
      Caption = 'Particle Traces'
      TabOrder = 2
      object EnergyButton: TSpeedButton
        Left = 16
        Top = 19
        Width = 65
        Height = 22
        GroupIndex = 1
        Caption = 'Energy'
        OnClick = EnergyButtonClick
      end
      object PhaseButton: TSpeedButton
        Left = 16
        Top = 47
        Width = 65
        Height = 22
        GroupIndex = 1
        Caption = 'Phase'
        OnClick = PhaseButtonClick
      end
      object RadiusButton: TSpeedButton
        Left = 16
        Top = 75
        Width = 65
        Height = 22
        GroupIndex = 1
        Caption = 'Radius'
        OnClick = RadiusButtonClick
      end
    end
    object PlotGroup: TGroupBox
      Left = 97
      Top = 1
      Width = 152
      Height = 100
      Align = alLeft
      Caption = 'Parameter Plots'
      TabOrder = 3
      object PowerButton: TSpeedButton
        Left = 77
        Top = 18
        Width = 65
        Height = 22
        GroupIndex = 1
        Caption = 'Power'
        OnClick = PowerButtonClick
      end
      object AvEnergyButton: TSpeedButton
        Left = 6
        Top = 18
        Width = 65
        Height = 22
        GroupIndex = 1
        Caption = 'Av. Energy'
        OnClick = AvEnergyButtonClick
      end
      object FieldButton: TSpeedButton
        Left = 77
        Top = 46
        Width = 65
        Height = 22
        GroupIndex = 1
        Caption = 'Field'
        OnClick = FieldButtonClick
      end
      object BettaButton: TSpeedButton
        Left = 6
        Top = 46
        Width = 65
        Height = 22
        GroupIndex = 1
        Caption = 'Velocity'
        OnClick = BettaButtonClick
      end
      object AvRadiusButton: TButton
        Left = 6
        Top = 74
        Width = 65
        Height = 22
        Caption = 'Av. Radius'
        TabOrder = 0
        OnClick = AvRadiusButtonClick
      end
      object EmittanceButton: TButton
        Left = 77
        Top = 74
        Width = 65
        Height = 22
        Caption = 'Emittance'
        TabOrder = 1
        OnClick = EmittanceButtonClick
      end
    end
    object SpectrumGroup: TGroupBox
      Left = 249
      Top = 1
      Width = 96
      Height = 100
      Align = alLeft
      Caption = 'Spectra'
      TabOrder = 4
      object WSpectrumButton: TSpeedButton
        Left = 6
        Top = 47
        Width = 74
        Height = 22
        GroupIndex = 1
        Caption = 'Energy Sp.'
        OnClick = WSpectrumButtonClick
      end
      object FSpectrumButton: TSpeedButton
        Left = 6
        Top = 19
        Width = 74
        Height = 22
        GroupIndex = 1
        Caption = 'Phase Sp.'
        OnClick = FSpectrumButtonClick
      end
      object RSpectrumButton: TButton
        Left = 6
        Top = 75
        Width = 74
        Height = 22
        Caption = 'Radial Sp.'
        TabOrder = 0
        OnClick = RSpectrumButtonClick
      end
    end
    object SpaceGroup: TGroupBox
      Left = 345
      Top = 1
      Width = 144
      Height = 100
      Align = alLeft
      Caption = 'Phase Spaces / Profiles'
      TabOrder = 5
      object TransverseButton: TSpeedButton
        Left = 6
        Top = 18
        Width = 59
        Height = 22
        GroupIndex = 1
        Caption = '(r) - (r)'#39
        OnClick = TransverseButtonClick
      end
      object LongtitudalButton: TSpeedButton
        Left = 71
        Top = 18
        Width = 59
        Height = 22
        GroupIndex = 1
        Caption = 'W - phi'
        OnClick = LongtitudalButtonClick
      end
      object LongtSectonButton: TSpeedButton
        Left = 71
        Top = 46
        Width = 59
        Height = 22
        GroupIndex = 1
        Caption = '(r) - phi'
        OnClick = LongtSectonButtonClick
      end
      object TransSectionButton: TSpeedButton
        Left = 6
        Top = 46
        Width = 59
        Height = 22
        GroupIndex = 1
        Caption = 'x - y'
        OnClick = TransSectionButtonClick
      end
      object MotionButton: TSpeedButton
        Left = 6
        Top = 74
        Width = 59
        Height = 22
        GroupIndex = 1
        Caption = 'z - (r)'
        OnClick = MotionButtonClick
      end
      object SlidingButton: TSpeedButton
        Left = 71
        Top = 74
        Width = 58
        Height = 22
        GroupIndex = 1
        Caption = '(r)- phi (s)'
        OnClick = SlidingButtonClick
      end
    end
    object OtherGroup: TGroupBox
      Left = 489
      Top = 1
      Width = 80
      Height = 100
      Align = alLeft
      Caption = 'Other'
      TabOrder = 6
      object LossButton: TSpeedButton
        Left = 6
        Top = 18
        Width = 66
        Height = 22
        GroupIndex = 1
        Caption = 'Losses'
        OnClick = LossButtonClick
      end
      object LossPieButton: TSpeedButton
        Left = 6
        Top = 46
        Width = 66
        Height = 22
        GroupIndex = 1
        Caption = 'Loss Pie'
        OnClick = LossPieButtonClick
      end
    end
  end
  object RightPanel: TPanel
    Left = 766
    Top = 0
    Width = 218
    Height = 639
    Align = alRight
    TabOrder = 2
    object OptionsBox: TGroupBox
      Left = 1
      Top = 548
      Width = 216
      Height = 90
      Align = alBottom
      Caption = 'Options'
      TabOrder = 0
      object EnvelopeCheck: TCheckBox
        Left = 5
        Top = 16
        Width = 97
        Height = 17
        Caption = 'Envelope'
        Checked = True
        State = cbChecked
        TabOrder = 0
        OnClick = EnvelopeCheckClick
      end
      object ChartCheck: TCheckBox
        Left = 5
        Top = 39
        Width = 97
        Height = 17
        Caption = 'Chart'
        Checked = True
        State = cbChecked
        TabOrder = 1
        OnClick = ChartCheckClick
      end
      object HorFitCheck: TCheckBox
        Left = 108
        Top = 39
        Width = 97
        Height = 17
        Caption = 'Fit Horizontally'
        Checked = True
        State = cbChecked
        TabOrder = 2
        OnClick = HorFitCheckClick
      end
      object VertFitCheck: TCheckBox
        Left = 108
        Top = 16
        Width = 97
        Height = 17
        Caption = 'Fit Vertically'
        Checked = True
        State = cbChecked
        TabOrder = 3
        OnClick = VertFitCheckClick
      end
      object TransparentCheck: TCheckBox
        Left = 5
        Top = 62
        Width = 97
        Height = 17
        Caption = 'Depth'
        TabOrder = 4
        OnClick = TransparentCheckClick
      end
    end
    object TableGroup: TGroupBox
      Left = 1
      Top = 1
      Width = 216
      Height = 415
      Align = alClient
      Caption = 'Parameters'
      TabOrder = 1
      object Table: TStringGrid
        Left = 2
        Top = 15
        Width = 212
        Height = 337
        Align = alClient
        ColCount = 2
        FixedCols = 0
        RowCount = 18
        FixedRows = 0
        TabOrder = 0
        OnDrawCell = TableDrawCell
        ColWidths = (
          64
          64)
        RowHeights = (
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24)
      end
      object BinsGroup: TGroupBox
        Left = 2
        Top = 352
        Width = 212
        Height = 61
        Align = alBottom
        Caption = 'Number of spectrum bins'
        TabOrder = 1
        object BinsMinLable: TLabel
          Left = 5
          Top = 45
          Width = 12
          Height = 13
          Caption = '10'
        end
        object BinsMaxLabel: TLabel
          Left = 185
          Top = 45
          Width = 18
          Height = 13
          Caption = '500'
        end
        object BinsLabel: TLabel
          Left = 93
          Top = 45
          Width = 15
          Height = 13
          Caption = '???'
        end
        object BinsTrack: TTrackBar
          Left = 2
          Top = 15
          Width = 208
          Height = 28
          Align = alTop
          Position = 8
          TabOrder = 0
          OnChange = BinsTrackChange
        end
      end
    end
    object KernelBox: TGroupBox
      Left = 1
      Top = 487
      Width = 216
      Height = 61
      Align = alBottom
      Caption = 'Number of particles in the beam core'
      TabOrder = 2
      object KernelMinLabel: TLabel
        Left = 5
        Top = 45
        Width = 17
        Height = 13
        Caption = '1%'
      end
      object KernelMaxLabel: TLabel
        Left = 188
        Top = 45
        Width = 23
        Height = 13
        Caption = '99%'
      end
      object KernelLabel: TLabel
        Left = 93
        Top = 45
        Width = 26
        Height = 13
        Caption = '???%'
      end
      object KernelTrack: TTrackBar
        Left = 2
        Top = 15
        Width = 212
        Height = 28
        Align = alTop
        Max = 20
        Position = 18
        TabOrder = 0
        OnChange = KernelTrackChange
      end
    end
    object AccuracyGroup: TGroupBox
      Left = 1
      Top = 416
      Width = 216
      Height = 71
      Align = alBottom
      Caption = 'Number of traces/points to skip'
      TabOrder = 3
      object AccuracyMinLabel: TLabel
        Left = 5
        Top = 52
        Width = 57
        Height = 13
        Caption = 'Draw 100%'
      end
      object AccuracyMaxLabel: TLabel
        Left = 162
        Top = 52
        Width = 51
        Height = 13
        Caption = 'Draw 10%'
      end
      object AccuracyLabel: TLabel
        Left = 85
        Top = 52
        Width = 54
        Height = 13
        Caption = 'Draw ???%'
      end
      object AccuracyTrack: TTrackBar
        Left = 2
        Top = 15
        Width = 212
        Height = 34
        Align = alTop
        Max = 9
        Position = 2
        TabOrder = 0
        OnChange = AccuracyTrackChange
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
