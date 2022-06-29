//---------------------------------------------------------------------------

#ifndef BuncherUnitH
#define BuncherUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include "Functions.h"
//---------------------------------------------------------------------------
class TBuncherForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *DescriptionLabel;
	TLabel *betaLabel;
	TLabel *eLabel;
	TGroupBox *DescriptionGroup;
	TGroupBox *ConstrainsGroup;
	TLabel *MinBetaLabel;
	TLabel *MaxBetaLabel;
	TEdit *MinBetaEdit;
	TCheckBox *BetaBeamCheck;
	TEdit *MaxBetaEdit;
	TLabel *MinAklLabel;
	TLabel *MaxAklLabel;
	TEdit *MinAklEdit;
	TEdit *MAxAklEdit;
	TLabel *RFLabel;
	TComboBox *RFCombo;
	TGroupBox *SweepGroup;
	TLabel *NEdit;
	TLabel *FromLabel;
	TLabel *ToLabel;
	TLabel *StepLabel;
	TLabel *ActiveLabel;
	TEdit *NFromEdit;
	TEdit *NToEdit;
	TEdit *NStepEdit;
	TCheckBox *NCheck;
	TEdit *MFromEdit;
	TEdit *MToEdit;
	TEdit *MStepEdit;
	TCheckBox *MCheck;
	TEdit *CellFromEdit;
	TEdit *CellToEdit;
	TLabel *MLabel;
	TLabel *NCellsLabel;
	TLabel *CellLAbel;
	TEdit *CellStepEdit;
	TEdit *NCellsFromEdit;
	TEdit *NCellsToEdit;
	TEdit *NCellsStepEdit;
	TCheckBox *CellCheck;
	TCheckBox *NCellsCheck;
	TGroupBox *ControlGroup;
	TButton *StartButton;
	TButton *StopButton;
	TButton *CloseButton;
	TProgressBar *ProgressBar;
	TLabel *ProgressLabel;
	TLabel *RFCommentLabel;
	void __fastcall BetaBeamCheckClick(TObject *Sender);
	void __fastcall RFComboChange(TObject *Sender);
	void __fastcall NCheckClick(TObject *Sender);
	void __fastcall MCheckClick(TObject *Sender);
	void __fastcall CellCheckClick(TObject *Sender);
	void __fastcall NCellsCheckClick(TObject *Sender);
	void __fastcall CloseButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall StartButtonClick(TObject *Sender);
private:	// User declarations
	void GetBeamBeta();
	bool UpdateTotalSteps();
public:		// User declarations
	__fastcall TBuncherForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBuncherForm *BuncherForm;
//---------------------------------------------------------------------------
#endif
