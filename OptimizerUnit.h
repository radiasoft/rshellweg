//---------------------------------------------------------------------------

#ifndef OptimizerUnitH
#define OptimizerUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>

#include "BeamSolver.h"
#include "Optimizer.h"
#include "OptimizerResults.h"
#include "Spin.hpp"
#include "cspin.h"

const int sDLS=0;

const int m90=0;
const int m60=1;

const int fA=0;
const int fL=1;
const int fE=2;

const int pCapture=0;
const int pESpec=1;
const int pFSpec=2;

//---------------------------------------------------------------------------
class TOptimizerForm : public TForm
{

__published:	// IDE-managed Components
	TPanel *MainPanel;
	TPanel *BottomPanel;
	TPanel *BuncherPanel;
	TPanel *AccelPanel;
	TPanel *ProgressPanel;
	TPanel *ButtonsPanel;
	TButton *ResultsButton;
	TButton *CloseButton;
	TButton *LoadButton;
	TGroupBox *BuncherGroup;
	TCheckBox *BuncherCheck;
	TComboBox *TypeCombo;
	TLabel *TypeLabel;
	TLabel *ModeLabel;
	TComboBox *ModeCombo;
	TLabel *FrequencyLabel;
	TEdit *FrequencyEdit;
	TLabel *PowerLabel;
	TEdit *PowerEdit;
	TLabel *FrequencyDim;
	TLabel *PowerDim;
	TLabel *FieldTypeLabel;
	TComboBox *FieldCombo;
	TLabel *LimitLabel1;
	TEdit *LimitEdit1;
	TLabel *LimitLabel2;
	TEdit *LimitEdit2;
	TGroupBox *bInputGroup;
	TGroupBox *bLimitGroup;
	TGroupBox *bObjectGroup;
	TRadioButton *SweepRadio;
	TRadioButton *OptimumRadio;
	TLabel *ParameterLabel;
	TComboBox *ParameterCombo;
	TButton *StartButton;
	TButton *AbortButton;
	TLabel *SamplesLabel;
	TSpinEdit *SamplesSpin;
	TProgressBar *OptimizerProgress;
	TCheckBox *SaveGraphCheck;
	TGroupBox *AccelGroup;
	TCheckBox *AccelCheck;
	TGroupBox *StructureGroup;
	TRadioButton *ImpedanceRadio;
	TRadioButton *GradientRadio;
	TGroupBox *TargetGroup;
	TLabel *EnergyLabel;
	TEdit *EnergyEdit;
	TLabel *EnergyDim;
	TCheckBox *ExactCheck;
	TCheckBox *StartCellCheck;
	TCSpinEdit *StartCellSpin;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CloseButtonClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall SweepRadioClick(TObject *Sender);
	void __fastcall OptimumRadioClick(TObject *Sender);
	void __fastcall ParameterComboChange(TObject *Sender);
	void __fastcall FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight,
          bool &Resize);
	void __fastcall StartButtonClick(TObject *Sender);
	void __fastcall AbortButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ResultsButtonClick(TObject *Sender);
	void __fastcall StartCellCheckClick(TObject *Sender);
private:	// User declarations
	bool Abort;
	double E1,E2;
	double W;
	int counter;

	void ChangeSweep();
	void ChangeStart();
	void RefreshStart();
	bool PrepareCell(TCell& Cell);
	bool BuncherInput(TCell& Cell);

	double GetValue(TOptResult& Structure);
	double GetEnergy(TOptResult& Structure);

	TOptResult Solve(bool Save=true);
	void Sweep();
	void OptimizeBuncher();
	void OptimizeImpedance();
	void OptimizeGradient();
	void OptimizeCurrent(TOptResult& Structure);
public:		// User declarations
	TBeamSolver *Solver;
	__fastcall TOptimizerForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOptimizerForm *OptimizerForm;
//---------------------------------------------------------------------------
#endif
