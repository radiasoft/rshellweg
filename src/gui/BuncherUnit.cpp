//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BuncherUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBuncherForm *BuncherForm;
//---------------------------------------------------------------------------
__fastcall TBuncherForm::TBuncherForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TBuncherForm::GetBeamBeta()
{
	double beta0=0.15;
	AnsiString s;
	MinBetaEdit->Text=s.FormatFloat("0.000",beta0);
}
//---------------------------------------------------------------------------
bool TBuncherForm::UpdateTotalSteps()
{
	int nN=1, nM=1, nCell=1, nNCells=1, a=1, b=1;
	int cellMin=1, cellMax=1,NcellMin=1, NcellMax=1;
	double x=1,y=1;
	AnsiString s;
	bool Err=false;

	if (NCheck->Checked){
		try {
			x=NFromEdit->Text.ToDouble();
		} catch (...) {
			ShowMessage("Minimum n is not a number!");
			Err=true;
			return Err;
		}
		try {
			y=NToEdit->Text.ToDouble();
		} catch (...) {
			ShowMessage("Maximum n is not a number!");
			Err=true;
			return Err;
		}
		if (y<x) {
			NFromEdit->Text=s.FormatFloat("0.0",y);
			NToEdit->Text=s.FormatFloat("0.0",x);
		}
		if (y==x) {
			NStepEdit->Text="1";
		}
		try {
			nN=NStepEdit->Text.ToInt();
		} catch (...) {
			ShowMessage("Number of steps for n is not an integer number!");
			Err=true;
			return Err;
		}
	}
	if (MCheck->Checked){
		try {
			x=MFromEdit->Text.ToDouble();
		} catch (...) {
			ShowMessage("Minimum m is not a number!");
			Err=true;
			return Err;
		}
		try {
			y=MToEdit->Text.ToDouble();
		} catch (...) {
			ShowMessage("Maximum m is not a number!");
			Err=true;
			return Err;
		}
		if (y<x) {
			MFromEdit->Text=s.FormatFloat("0.0",y);
			MToEdit->Text=s.FormatFloat("0.0",x);
		}
		if (y==x) {
			MStepEdit->Text="1";
		}
		try {
			nM=MStepEdit->Text.ToInt();
		} catch (...) {
			ShowMessage("Number of steps for m is not an integer number!");
			Err=true;
			return Err;
		}
	}
	if (CellCheck->Checked){
		try {
			x=CellFromEdit->Text.ToInt();
		} catch (...) {
			a=round(a);

		}
		try {
			b=CellToEdit->Text.ToInt();
		} catch (...) {
			ShowMessage("Maximum skip cell # is not an intereger number!");
			Err=true;
			return Err;
		}
		if (b<1 || a<1) {
			ShowMessage("Skip cell # is not a positive number!");
			Err=true;
			return Err;
		}
		if (b<a) {
			CellFromEdit->Text=s.FormatFloat("0",b);
			CellToEdit->Text=s.FormatFloat("0",a);
		}
		cellMin=CellFromEdit->Text.ToInt();
		cellMax=CellToEdit->Text.ToInt();
		nCell=cellMax-cellMin+1;
		CellStepEdit->Text=s.FormatFloat("0",nCell);
	}
	if (NCellsCheck->Checked){
		NcellMin=NCellsFromEdit->Text.ToInt();
		NcellMax=NCellsToEdit->Text.ToInt();
		nNCells=NcellMax-NcellMin+1;
	}
	ProgressLabel->Caption="0 / "+s.FormatFloat("0",nN*nM*nCell*nNCells);

	return Err;
}
//---------------------------------------------------------------------------
void __fastcall TBuncherForm::BetaBeamCheckClick(TObject *Sender)
{
	MinBetaEdit->Enabled=!BetaBeamCheck->Checked;
	if (BetaBeamCheck->Checked)
		GetBeamBeta();
}
//---------------------------------------------------------------------------
void __fastcall TBuncherForm::RFComboChange(TObject *Sender)
{
	switch (RFCombo->ItemIndex) {
		case 0: {
			RFCommentLabel->Caption="DLS 2pi/3 or pi/2";
			break;
		}
		case 1: {
			RFCommentLabel->Caption="from rf.dat";
			break;
		}
	default:
		;
	}
}
//---------------------------------------------------------------------------
void __fastcall TBuncherForm::NCheckClick(TObject *Sender)
{
	NFromEdit->Enabled=NCheck->Checked;
	NToEdit->Enabled=NCheck->Checked;
	NStepEdit->Enabled=NCheck->Checked;
	UpdateTotalSteps();
}
//---------------------------------------------------------------------------
void __fastcall TBuncherForm::MCheckClick(TObject *Sender)
{
	MFromEdit->Enabled=MCheck->Checked;
	MToEdit->Enabled=MCheck->Checked;
	MStepEdit->Enabled=MCheck->Checked;
	UpdateTotalSteps();
}
//---------------------------------------------------------------------------
void __fastcall TBuncherForm::CellCheckClick(TObject *Sender)
{
	CellFromEdit->Enabled=CellCheck->Checked;
	CellToEdit->Enabled=CellCheck->Checked;
	if (!CellCheck->Checked) {
		NCellsCheck->Checked=false;
	}
	UpdateTotalSteps();
}
//---------------------------------------------------------------------------
void __fastcall TBuncherForm::NCellsCheckClick(TObject *Sender)
{
	NCellsFromEdit->Enabled=NCellsCheck->Checked;
	NCellsToEdit->Enabled=NCellsCheck->Checked;
	UpdateTotalSteps();
}
//---------------------------------------------------------------------------
void __fastcall TBuncherForm::CloseButtonClick(TObject *Sender)
{
	BuncherForm->Close();
}
//---------------------------------------------------------------------------
void __fastcall TBuncherForm::FormShow(TObject *Sender)
{
	UpdateTotalSteps();
}
//---------------------------------------------------------------------------
void __fastcall TBuncherForm::StartButtonClick(TObject *Sender)
{
	UpdateTotalSteps();
}
//---------------------------------------------------------------------------
