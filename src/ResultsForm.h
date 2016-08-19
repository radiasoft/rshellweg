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

const Npar=17;
const pZ=0;
const pWav=1;
const pWm=2;
const pdW=3;
const pI=4;
const pkc=5;
const pr=6;
const pFav=7;
const pdF=8;
const pF=9;
const pRa=10;
const pE=11;
const pPb=12;
const pvph=13;
const peps=14;
const palpha=15;
const pbetta=16;

const colName=0;
const colValue=1;
//---------------------------------------------------------------------------
class TResForm : public TForm
{
__published:    // IDE-managed Components
    TPanel *GraphPanel;
    TPanel *BottomPanel;
    TPanel *RightPanel;
    TChart *PackChart;
    TLineSeries *PackSeries;
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
    TSpeedButton *AvRadiusButton;
    TGroupBox *GroupBox1;
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
private:    // User declarations
    TGraphType gType;
    int Np,Npts,Nb;
    double Rmax,Xmax,Wmax;

    double W,dW,F,dF;
    bool SolverLoaded;
    TSpectrumBar *WSpectrum, *FSpectrum;

    void GetTransBoundaries();
    void CreateTable();

    void DrawPhase();
    void DrawEnergy();
    void DrawRadius();
    void DrawField();
    void DrawPower();
    void DrawBetta();
    void DrawAvEnergy();
    void DrawAvRadius();
    void DrawTransSection();
    void DrawLongtSection(bool sliding=false);
    void DrawLongtMotion();
    void DrawTransSpace();
    void DrawLongtSpace();
    void DrawPhaseSpectrum();
    void DrawEnergySpectrum();

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
    void DeleteTemp();

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
