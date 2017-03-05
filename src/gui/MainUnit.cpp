//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
}
//----------------------------------------------------------------------------
/*int round(double x){
    return (x-floor(x))>(ceil(x)-x)?ceil(x):floor(x);
}       */
//---------------------------------------------------------------------------
double Trim(double x,int N)
{
    //return round(x*IntPower(10,N))/IntPower(10,N);
    return 1;
}
//---------------------------------------------------------------------------
void TMainForm::DisplayError()
{
    switch (ERR) {
	    case ERR_NO : break;
		case ERR_NOFILE:ShowMessage("ERROR: No Input File Selected");   break;
		case ERR_OPENFILE: ShowMessage("ERROR: File Read Error");   break;
		case ERR_COUPLER : ShowMessage("ERROR: Input File: COUPLER should have format Power,Frequency");    break;
		case ERR_SOLENOID : ShowMessage("ERROR: Input File: SOLENOID should have format Bz,L,Z0");  break;
		case ERR_BEAM : ShowMessage("ERROR: Input File: Check BEAM line format!");    break;
		case ERR_CURRENT : ShowMessage("ERROR: Input File: CURRENT should have format I0,Np");    break;
		case ERR_DRIFT : ShowMessage("ERROR: Input File: DRIFT should have format Length,Radius");  break;
		case ERR_CELL : ShowMessage("ERROR: Input File: CELL should have format Mode,betta,Field and optional: Attenuation,Aperture");  break;
		case ERR_CELLS : ShowMessage("ERROR: Input File: CELLS should have format N,Mode,betta,Field and optional: Attenuation,Aperture");  break;
		case ERR_OPTIONS : ShowMessage("ERROR: Input File: OPTIONS format is incorrect!");  break;
		case ERR_DUMP : ShowMessage("ERROR:  Input File: SAVE formal is incorrect!");  break;
		case ERR_IMPORT : ShowMessage("ERROR:  Particles import failed!");  break;
		case ERR_FORMAT : ShowMessage("ERROR: incorrect format of data");  break;
		case ERR_SPCHARGE : ShowMessage("Undefined option for card SPCHARGE"); break;
        default:ShowMessage("ERROR: Input File has a wrong format!");   break;
    }
}
//---------------------------------------------------------------------------
bool TMainForm::LoadInputData(bool display_err)
{
    Solver->InputFile=InputFileName;
	TError Error=Solver->LoadData();
	if (Error!=ERR_NO){
        if (display_err)
			DisplayError();
	}/* else{

		DisplayInputData();
	}    */
	return Error==ERR_NO;
}
//---------------------------------------------------------------------------
bool TMainForm::CreateInputData(bool display_err)
{
	TError Error=ERR_NO;

	if (!DataReady)
		DataReady=LoadInputData(true);

	ResultsMemo->Visible=false;
	ResultsMemo->Clear();
	ViewButton->Enabled=false;

	if (!DataReady){
		if (display_err)
			ShowMessage("Error occurred while reading input file! Impossible to start solver.");
		return false;
	}

	try{
		Solver->CreateGeometry();
	} catch(...){
		if (display_err)
			ShowMessage("Error occurred while creating geometry. Check the values in input file!");
		return false;
	}

	try{
		Error=Solver->CreateBeam();
		if (Error!=ERR_NO) {
			if (display_err)
				DisplayError();
		   /*	Solver->Abort();
			Application->Terminate();    */
			return false;
		}
	}  catch(...){
		if (display_err)
			ShowMessage("Error occurred while creating beam. Check the values in input file!");
		return false;
	}
	DisplayInputData();

	return ERR==ERR_NO;
}
//---------------------------------------------------------------------------
void TMainForm::DisplayInputData()
{
	AnsiString s,L;

	//POWER SOURCE
	Label_P0->Caption="Input Power = "+s.FormatFloat("#0.00",Solver->GetSectionPower()/1e6)+" MW";
	Label_F0->Caption="Frequency = "+s.FormatFloat("#0.00",Solver->GetSectionFrequency()/1e6)+" MHz";
	//Label_F0->Caption="Frequency = "+s.FormatFloat("#0.00",Solver->GetFrequency())+" MHz";

	//SOLENOID
	TMagnetParameters Solenoid=Solver->GetSolenoidInfo();
	Label_Position->Caption="";
	Label_Fringe->Caption="";
	switch (Solenoid.ImportType) {
		case ANALYTIC_1D:{
			Label_Length->Caption="Effective Length = "+s.FormatFloat("#0.00",100*Solenoid.Length)+" cm";
			Label_Position->Caption="Start Position = "+s.FormatFloat("#0.00",100*Solenoid.StartPos)+" cm";
			Label_Fringe->Caption="Fringe Region = "+s.FormatFloat("#0.00",100*Solenoid.Lfringe)+" cm";
		}
		case ANALYTIC_0D:{
			Label_B0->Caption="Mangetic Field = "+s.FormatFloat("#0.00",1e4*Solenoid.BField)+" Gs";
			break;
		}
		case IMPORT_2DR:{ }
		case IMPORT_2DC:{ }
		case IMPORT_3DR:{ }
		case IMPORT_1D:{
			Label_B0->Caption="IMPORTED from";
			Label_Position->Caption="File: '"+Solenoid.File+"'";
			//Label_Length->Caption="Distribution: 1D - Bz(z)";
			break;
		}
		case NO_ELEMENT:{}
		default: {
			Label_B0->Caption="NO SOLENOID";
			break;
		};
	}
	switch (Solenoid.ImportType) {
		case IMPORT_1D:{
			Label_Length->Caption="Distribution: 1D - Bz(z)";
			break;
		}
		case IMPORT_2DC:{
			Label_Length->Caption="Distribution: 2D - Br,z(r,z)";
			break;
		}
		case IMPORT_2DR:{
			Label_Length->Caption="Distribution: 2D - Bx,y,z(r,z)";
			break;
		}
		case IMPORT_3DR:{
			Label_Length->Caption="Distribution: 3D - Bx,y,z(x,y,z)";
			break;
		}
	}

	//STRUCTURE
	Label_Cells->Caption=s.FormatFloat("Number of Elements = 0",Solver->GetNumberOfCells());
	Label_Sections->Caption=s.FormatFloat("Number of Sections = 0",Solver->GetNumberOfSections());

	// BEAM
	TGauss W=Solver->GetInputEnergy();
	TGauss Phi=Solver->GetInputPhase();

	Label_W0->Caption="Average Energy = "+s.FormatFloat("#0.000",W.mean)+" MeV";
	Label_Phi0->Caption="Average Phase = "+s.FormatFloat("#0.00",RadToDegree(Phi.mean))+" deg";
	Label_dW->Caption="RMS Energy = "+s.FormatFloat("#0.000#",W.sigma)+" MeV";
	Label_dPhi->Caption="RMS Phase = "+s.FormatFloat("#0.00",RadToDegree(Phi.sigma))+" deg";

	TTwiss TR=Solver->GetInputTwiss(R_PAR);
	Label_AlphaR->Caption="Alpha = "+s.FormatFloat("#0.000##",TR.alpha)+"";
	Label_BetaR->Caption="Beta = "+s.FormatFloat("#0.000##",100*TR.beta)+" cm/rad";
	Label_EmittanceR->Caption="Emittance = "+s.FormatFloat("#0.0##",1e6*TR.epsilon)+" mm*mrad";

	TTwiss TX=Solver->GetInputTwiss(X_PAR);
	Label_AlphaX->Caption="Alpha = "+s.FormatFloat("#0.000##",TX.alpha)+"";
	Label_BetaX->Caption="Beta = "+s.FormatFloat("#0.000##",100*TX.beta)+" cm/rad";
	Label_EmittanceX->Caption="Emittance = "+s.FormatFloat("#0.0##",1e6*TX.epsilon)+" mm*mrad";

	TTwiss TY=Solver->GetInputTwiss(Y_PAR);
	Label_AlphaY->Caption="Alpha = "+s.FormatFloat("#0.000##",TY.alpha)+"";
	Label_BetaY->Caption="Beta = "+s.FormatFloat("#0.000##",100*TY.beta)+" cm/rad";
	Label_EmittanceY->Caption="Emittance = "+s.FormatFloat("#0.0##",1e6*TY.epsilon)+" mm*mrad";

	//CURRENT
	Label_I0->Caption="Input Current = "+s.FormatFloat("#0.000",Solver->GetInputCurrent())+" A";
	Label_Np->Caption="Number of Particles = "+s.FormatFloat("#0",Solver->GetNumberOfParticles());

	//SPACE CHARGE
	L="";
	TSpaceCharge Spch=Solver->GetSpaceChargeInfo();
	switch (Spch.Type) {
		case SPCH_ELL: {
			Label_Spch->Caption="Elliptical Model";
			break;
		}
		case SPCH_GW: {
			Label_Spch->Caption="Garnett-Wangler Model";
			L=s.FormatFloat("#0 slices ",Spch.NSlices);
			break;
		}
		case SPCH_NO: {}
		default: {
			Label_Spch->Caption="Not Considered";
		};
	}

  /*	if (Solver->CheckMagnetization())
		L+="MAGNETIZED ";  */
	if (Solver->CheckReverse())
		L+="REVERSE ";

	if (L=="") {
        L="NONE";
	}

	Label_SpchPar->Caption=L;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ExitButtonClick(TObject *Sender)
{
    Solver->Abort();
    Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
    AnsiString Path;
    Path=Application->ExeName;
    Path=ExtractFileDir(Path);
    Solver=new TBeamSolver(Path);
    Solver->AssignSolverPanel(SolverGroup);

	InputFileName="INPUT.txt";

	ReloadData=false;
	InputReady=false;
	DataReady=LoadInputData(false);
	if (DataReady) {
		InputReady=CreateInputData(false);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
//    delete Solver;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SelectFileButtonClick(TObject *Sender)
{
    InputDialog->Execute();
    InputFileName=InputDialog->FileName.c_str();
	DataReady=LoadInputData(true);
	if (DataReady) {
		InputReady=CreateInputData(true);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCanResize(TObject *Sender, int &NewWidth,
      int &NewHeight, bool &Resize)
{
    Resize=true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SolveButtonClick(TObject *Sender)
{
	AnsiString s,str;
	TError Err;

	if (ReloadData){
		DataReady=false;
		InputReady=false;
		ReloadData=false;
	}

	if (!InputReady)
		InputReady=CreateInputData(true);

	try{
		Err=Solver->Solve();
		if (Err==ERR_NO){
			AnsiString Fname="OUTPUT.TXT";
			Solver->Output(Fname,ResultsMemo);
			ResultsMemo->Visible=true;
			ViewButton->Enabled=true;
		}
		ReloadData=true; //reload data when repeat simulations
	} catch(...){
		ShowMessage("Error occurred while solving the task. Check the values in input file!");
		return;
	}
}
//---------------------------------------------------------------------------
void  TMainForm::ShowGeometryForm(bool BeamView)
{
	if (!DataReady)
		DataReady=LoadInputData(true);

	if (!DataReady){
		ShowMessage("Error occurred while reading input file! Impossible to create beam.");
		return;
	}

	if (!InputReady) {
		InputReady=CreateInputData(true);
	}

	if (InputReady) {
		GeomForm->MainSolver=Solver;
		GeomForm->Beam=BeamView;
		GeomForm->Show();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ViewBeamButtonClick(TObject *Sender)
{
	ShowGeometryForm(true);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ViewGeometryButtonClick(TObject *Sender)
{
    ShowGeometryForm(false);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::AbortButtonClick(TObject *Sender)
{
    Solver->Abort();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ViewButtonClick(TObject *Sender)
{
    ResForm->Solver=Solver;
    ResForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadResultsButtonClick(TObject *Sender)
{
    ResForm->LoadData();
    ResForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::OptButtonClick(TObject *Sender)
{

    if (LoadInputData(true)){
        OptimizerForm->Solver=Solver;
        OptimizerForm->Show();
    }
}
//---------------------------------------------------------------------------




