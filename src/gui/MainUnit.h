//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>

#include "Types.h"
#include "Functions.h"
#include "BeamSolver.h"
#include "GeometryForm.h"
#include "ResultsForm.h"
#include "OptimizerUnit.h"
#include "BuncherUnit.h"

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:    // IDE-managed Components
    TButton *SelectFileButton;
    TButton *SolveButton;
    TButton *ExitButton;
    TOpenDialog *InputDialog;
    TGroupBox *InputGroup;
    TLabel *Label_P0;
    TLabel *Label_I0;
    TLabel *Label_F0;
    TLabel *Label_Np;
    TLabel *Label_B0;
    TLabel *Label_Length;
    TLabel *Label_Position;
    TLabel *Label_W0;
    TLabel *Label_Phi0;
    TLabel *Label_dW;
    TLabel *Label_dPhi;
	TLabel *Label_AlphaR;
	TLabel *Label_BetaR;
	TLabel *Label_EmittanceR;
	TLabel *Label_Spch;
	TLabel *Label_Sections;
    TLabel *Label_Cells;
    TButton *ViewGeometryButton;
    TButton *ViewBeamButton;
    TGroupBox *SolverGroup;
    TGroupBox *ResultsGroup;
    TButton *AbortButton;
    TGroupBox *ControlGroup;
    TMemo *ResultsMemo;
    TPanel *MemoPanel;
    TPanel *ActPanel;
    TButton *ViewButton;
    TButton *LoadResultsButton;
    TButton *OptButton;
	TGroupBox *GroupPower;
	TGroupBox *GroupSolenoid;
	TGroupBox *GroupBeamGeneral;
	TGroupBox *GroupSpaceCharge;
	TLabel *Label_SpchPar;
	TGroupBox *GroupZpar;
	TGroupBox *GroupStructure;
	TGroupBox *GroupTwissR;
	TGroupBox *GroupTwissX;
	TLabel *Label_AlphaX;
	TLabel *Label_BetaX;
	TLabel *Label_EmittanceX;
	TGroupBox *GroupTwissY;
	TLabel *Label_AlphaY;
	TLabel *Label_BetaY;
	TLabel *Label_EmittanceY;
	TLabel *Label_Fringe;
	TButton *cBuncherButton;
	TLabel *Label_Input;
	TGroupBox *InterfaceGroup;
	TEdit *InputFileEdit;
	TLabel *Label_Output;
	TEdit *OutputFileEdit;
	TCheckBox *BinaryCheck;
	TCheckBox *TrajectoryCheck;
	TCheckBox *OutputCheck;
	TLabel *Label_Particles;
	TGroupBox *GroupEnergy;
	TLabel *Label_beta;
	TLabel *Label_W0_full;
    void __fastcall ExitButtonClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall SelectFileButtonClick(TObject *Sender);
    void __fastcall FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight,
          bool &Resize);
    void __fastcall SolveButtonClick(TObject *Sender);
    void __fastcall ViewBeamButtonClick(TObject *Sender);
    void __fastcall ViewGeometryButtonClick(TObject *Sender);
    void __fastcall AbortButtonClick(TObject *Sender);
    void __fastcall ViewButtonClick(TObject *Sender);
    void __fastcall LoadResultsButtonClick(TObject *Sender);
	void __fastcall OptButtonClick(TObject *Sender);
	void __fastcall cBuncherButtonClick(TObject *Sender);
	void __fastcall OutputCheckClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
private:    // User declarations
//    TBeamSolver *Solver;
	bool GUIDisabled, SaveOutput, SaveTemp, SaveTraj, OutputBinary;
	AnsiString Path,UserIniPath, InputFileName, OutputName, TempFileName;

	void InterfaceVisibility();
    void GUIVisibility();

	void DisplayError();
	void DisplayInputData();
	void Initialize();
    bool LoadIniFile();
	bool LoadInputData(bool display_err=true);
	bool CreateInputData(bool display_err=true);
	void ShowGeometryForm(bool BeamView);
	TError ERR; //remove
	//AnsiString InputFileName;
	bool DataReady;
	bool InputReady;
	bool ReloadData;
public:     // User declarations
    TBeamSolver *Solver;
    __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
