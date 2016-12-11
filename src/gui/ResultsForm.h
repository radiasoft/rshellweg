//---------------------------------------------------------------------------

#ifndef ResultsFormH
#define ResultsFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Buttons.hpp>

#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>

#include "BeamSolver.h"

const int Npar=18;
const int pZ=0;
const int pWav=1;
const int pWm=2;
const int pdW=3;
const int pI=4;
const int pkc=5;
const int pFav=6;
const int pdF=7;
const int pF=8;
const int pRa=9;
const int pE=10;
const int pPb=11;
const int pvph=12;
const int pcoord=13;
const int pr=14;
const int palpha=15;
const int pbetta=16;
const int peps=17;
const int pepsn=18;

const int colName=0;
const int colValue=1;

const int r_coord=0;
const int x_coord=1;
const int y_coord=2;
//---------------------------------------------------------------------------
class TResForm : public TForm
{
__published:    // IDE-managed Components
    TPanel *GraphPanel;
    TPanel *BottomPanel;
    TPanel *RightPanel;
    TChart *PackChart;
    TSpeedButton *EnergyButton;
    TSpeedButton *PhaseButton;
    TSpeedButton *RadiusButton;
    TSpeedButton *FieldButton;
    TSpeedButton *LossButton;
    TChart *BarsChart;
    TBarSeries *BarSeries1;
    TBarSeries *BarSeries2;
    TBarSeries *BarSeries3;
    TBarSeries *BarSeries4;
    TBarSeries *BarSeries5;
    TBarSeries *BarSeries6;
    TChart *PieChart;
    TPieSeries *PieSeries;
    TSpeedButton *LossPieButton;
    TSpeedButton *FSpectrumButton;
    TSpeedButton *WSpectrumButton;
    TSpeedButton *LongtitudalButton;
    TSpeedButton *TransverseButton;
    TSpeedButton *TransSectionButton;
    TSpeedButton *PowerButton;
    TSpeedButton *AvEnergyButton;
    TTrackBar *PositionTrackBar;
    TSpeedButton *PlayButton;
    TSpeedButton *NextButton;
    TSpeedButton *PrevButton;
    TSpeedButton *FFButton;
    TSpeedButton *RewButton;
    TSpeedButton *BettaButton;
    TLineSeries *LineSeries;
    TGroupBox *TrackBox;
    TLabel *CellLabel;
    TLabel *PositionLabel;
    TChart *BeamChart;
    TPointSeries *BeamSeries;
    TSpeedButton *LongtSectonButton;
    TSpeedButton *MotionButton;
    TLineSeries *EnvelopeSeries;
    TSpeedButton *SlidingButton;
    TLineSeries *AddSeries;
    TChart *SpectrumChart;
    TBarSeries *SpectrumSeries;
    TLineSeries *SpecEnvSeries;
	TGroupBox *OptionsBox;
    TCheckBox *EnvelopeCheck;
    TCheckBox *ChartCheck;
    TCheckBox *HorFitCheck;
    TCheckBox *VertFitCheck;
    TGroupBox *TableGroup;
    TStringGrid *Table;
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *About1;
    TMenuItem *OpenFile1;
    TMenuItem *SaveAs1;
    TMenuItem *N1;
    TMenuItem *Exit1;
    TOpenDialog *OpenDialog;
    TSaveDialog *SaveDialog;
	TRadioGroup *RadiusGroup;
	TButton *RSpectrumButton;
	TTrackBar *KernelTrack;
	TGroupBox *KernelBox;
	TLabel *KernelMinLabel;
	TLabel *KernelMaxLabel;
	TLabel *KernelLabel;
	TLineSeries *PackSeries;
	TGroupBox *AccuracyGroup;
	TTrackBar *AccuracyTrack;
	TLabel *AccuracyMinLabel;
	TLabel *AccuracyMaxLabel;
	TLabel *AccuracyLabel;
	TCheckBox *TransparentCheck;
	TGroupBox *TraceGroup;
	TGroupBox *PlotGroup;
	TButton *AvRadiusButton;
	TGroupBox *SpectrumGroup;
	TGroupBox *SpaceGroup;
	TGroupBox *OtherGroup;
	TGroupBox *BinsGroup;
	TLabel *BinsMinLable;
	TLabel *BinsMaxLabel;
	TLabel *BinsLabel;
	TTrackBar *BinsTrack;
	TButton *EmittanceButton;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall EnergyButtonClick(TObject *Sender);
    void __fastcall PhaseButtonClick(TObject *Sender);
    void __fastcall RadiusButtonClick(TObject *Sender);
    void __fastcall FieldButtonClick(TObject *Sender);
    void __fastcall LossButtonClick(TObject *Sender);
    void __fastcall LossPieButtonClick(TObject *Sender);
    void __fastcall PowerButtonClick(TObject *Sender);
    void __fastcall PositionTrackBarChange(TObject *Sender);
    void __fastcall PlayButtonClick(TObject *Sender);
    void __fastcall FFButtonClick(TObject *Sender);
    void __fastcall RewButtonClick(TObject *Sender);
    void __fastcall NextButtonClick(TObject *Sender);
    void __fastcall PrevButtonClick(TObject *Sender);
    void __fastcall BettaButtonClick(TObject *Sender);
    void __fastcall TransSectionButtonClick(TObject *Sender);
    void __fastcall LongtSectonButtonClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall MotionButtonClick(TObject *Sender);
    void __fastcall SlidingButtonClick(TObject *Sender);
    void __fastcall TransverseButtonClick(TObject *Sender);
    void __fastcall LongtitudalButtonClick(TObject *Sender);
    void __fastcall FSpectrumButtonClick(TObject *Sender);
    void __fastcall AvEnergyButtonClick(TObject *Sender);
    void __fastcall AvRadiusButtonClick(TObject *Sender);
    void __fastcall WSpectrumButtonClick(TObject *Sender);
    void __fastcall TableDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
    void __fastcall EnvelopeCheckClick(TObject *Sender);
    void __fastcall ChartCheckClick(TObject *Sender);
    void __fastcall VertFitCheckClick(TObject *Sender);
    void __fastcall HorFitCheckClick(TObject *Sender);
    void __fastcall Exit1Click(TObject *Sender);
    void __fastcall SaveAs1Click(TObject *Sender);
    void __fastcall OpenFile1Click(TObject *Sender);
	void __fastcall RadiusGroupClick(TObject *Sender);
	void __fastcall KernelTrackChange(TObject *Sender);
	void __fastcall AccuracyTrackChange(TObject *Sender);
	void __fastcall TransparentCheckClick(TObject *Sender);
	void __fastcall BinsTrackChange(TObject *Sender);
	void __fastcall RSpectrumButtonClick(TObject *Sender);
	void __fastcall EmittanceButtonClick(TObject *Sender);
private:    // User declarations
	TGraphType gType;
    int Np,Npts;//,Nb;
	double Rmax,Xmax,Wmax;
	double Kernel,Hcore;
	int SparceFactor;
	int NumBins;

    double W,dW,F,dF;
	bool SolverLoaded;
    TSpectrumBar *WSpectrum;//, *FSpectrum;

	//void GetPlotParameters();
	void GetTransBoundaries();
	void CreateTable();

	void ResetAxis(TChartAxis *Axis,double NewMin,double NewMax,bool Auto=false);

	void DrawTrace(TBeamParameter P1);
	void DrawPlots(TStructureParameter P1,TStructureParameter P2);
	void DrawSpace(int Nknot,TBeamParameter P1,TBeamParameter P2,bool sliding=false);
	void DrawSpectrum(int Nknot,TBeamParameter P1);

    void DrawPhase();
    void DrawEnergy();
    void DrawRadius();
    void DrawField();
    void DrawPower();
    void DrawBetta();
    void DrawAvEnergy();
	void DrawAvRadius();
	void DrawEmittance();
    void DrawTransSection();
	void DrawLongtSection(bool sliding=false);
    void DrawLongtMotion();
    void DrawTransSpace();
    void DrawLongtSpace();
    void DrawPhaseSpectrum();
	void DrawEnergySpectrum();
	void DrawRadialSpectrum();

    void AllPassive();
    void ChartActive();
    void PackActive();
    void LineActive();
    void BarsActive();
    void PieActive();
    void BeamActive();
    void ParticlesActive();
    void EnvelopeActive();
    void SpectrumActive();

    void ShowParameters();
	//void DeleteTemp();

	void BinsChanged();
	void KernelChanged();
	void AccuracyChanged();
	void PositionChanged();
    void IncrementPosition();
    int NextEnd();
    int PrevEnd();
public:     // User declarations
    __fastcall TResForm(TComponent* Owner);
    TBeamSolver *Solver;
    void LoadData(); 
};
//---------------------------------------------------------------------------
extern PACKAGE TResForm *ResForm;
//---------------------------------------------------------------------------
#endif
