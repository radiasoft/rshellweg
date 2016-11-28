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
#include "BeamSolver.h"
#include "GeometryForm.h"
#include "ResultsForm.h"
#include "OptimizerUnit.h"

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
    TLabel *Label1;
    TLabel *Label_Alpha;
    TLabel *Label_Betta;
    TLabel *Label_Emittance;
    TLabel *Label2;
    TLabel *Label_Coulomb;
    TLabel *Label_Wave;
    TLabel *Label_WDist;
    TLabel *Label_PhiDist;
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
private:    // User declarations
//    TBeamSolver *Solver;
    void DisplayError();
    void DisplayInputData();
    bool LoadInputData(bool display_err=true);
    TError ERR;
    AnsiString InputFileName;
public:     // User declarations
    TBeamSolver *Solver;
    __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
