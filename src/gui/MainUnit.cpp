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
    ERR=Solver->LoadData();
    if (ERR!=ERR_NO){
        if (display_err)
            DisplayError();
    } else
        DisplayInputData();
    return ERR==ERR_NO;
}
//---------------------------------------------------------------------------
void TMainForm::DisplayInputData()
{
	AnsiString s,L;

	//POWER SOURCE
	Label_P0->Caption="Input Power = "+s.FormatFloat("#0.00",Solver->GetPower())+" MW";
	Label_F0->Caption="Frequency = "+s.FormatFloat("#0.00",Solver->GetFrequency())+" MHz";

	//SOLENOID
	if (true) {

	} else {
		Label_B0->Caption="Solenoid Field = "+s.FormatFloat("#0.00",Solver->GetSolenoidField())+" Tl";
		Label_Length->Caption="Solenoid Length = "+s.FormatFloat("#0.00",Solver->GetSolenoidLength())+" m";
		Label_Position->Caption="Solenoid Position = "+s.FormatFloat("#0.00",Solver->GetSolenoidPosition())+" m";
	}

	Label_I0->Caption="Input Current = "+s.FormatFloat("#0.000",Solver->GetInputCurrent())+" A";
	int Nth,Mth;
	/*Solver->GetMode(&Nth,&Mth);

	if (Nth%Mth==0)
		Label_Mode->Caption="Mode = "+s.FormatFloat("0 pi",Nth/Mth);
	else
		Label_Mode->Caption="Mode = "+s.FormatFloat("0*pi/",Nth)+s.FormatFloat("0",Mth);
	*/
	Label_Np->Caption="Number of Particles = "+s.FormatFloat("#0",Solver->GetNumberOfParticles());


	Label_W0->Caption="Average Energy = "+s.FormatFloat("#0.000",Solver->GetInputAverageEnergy())+" MeV";
    Label_Phi0->Caption="Average Phase = "+s.FormatFloat("#0.00",Solver->GetInputAveragePhase())+" deg";
    Label_dW->Caption="dW = "+s.FormatFloat("#0.000#",Solver->GetInputEnergyDeviation())+" MeV";
    Label_dPhi->Caption="dPhi = "+s.FormatFloat("#0.00",Solver->GetInputPhaseDeviation())+" deg";

   /* Label_Alpha->Caption="Alpha = "+s.FormatFloat("#0.000##",Solver->GetInputAlpha())+"";
    Label_Betta->Caption="Betta = "+s.FormatFloat("#0.000##",Solver->GetInputBetta())+" cm/rad";
	Label_Emittance->Caption="Emittance = "+s.FormatFloat("#0.000##",Solver->GetInputEpsilon())+" cm*rad";
	*/
		Label_Cells->Caption=s.FormatFloat("#0",Solver->GetNumberOfCells())+" Cells Uploaded";

	//SPACE CHARGE
	L="NONE";
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

	if (Solver->CheckMagnetization())
		L+="MAGNETIZED ";
	if (Solver->CheckReverse())
		L+="REVERSE ";

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
    LoadInputData(true);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
    delete Solver;  
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SelectFileButtonClick(TObject *Sender)
{
    InputDialog->Execute();
    InputFileName=InputDialog->FileName.c_str();
    LoadInputData(true);
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
//	int Np;
	
    bool dataReady=LoadInputData(true);
    ResultsMemo->Visible=false;
    ResultsMemo->Clear();
    ViewButton->Enabled=false;

    if (!dataReady){
        ShowMessage("Error occurred while reading input file! Impossible to start solver.");
        return;
    }
//    static int cnt=0;

    //if(cnt<1){
    try{
        Solver->CreateGeometry();
    } catch(...){
        ShowMessage("Error occurred while creating geometry. Check the values in input file!");
        return;
    }

    try{
        ERR=Solver->CreateBeam();
        if (ERR!=ERR_NO) {
		    DisplayError();
            Solver->Abort();
            Application->Terminate();
			return;
		}
    }  catch(...){
        ShowMessage("Error occurred while creating beam. Check the values in input file!");
        return;
    }
 ///    cnt++;
 // }

    try{
        Solver->Solve();
        AnsiString Fname="OUTPUT.TXT";
        Solver->Output(Fname,ResultsMemo);
        ResultsMemo->Visible=true;
        ViewButton->Enabled=true;
    } catch(...){
        ShowMessage("Error occurred while solving the task. Check the values in input file!");
        return;
    }             

    
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ViewBeamButtonClick(TObject *Sender)
{
    bool dataReady=LoadInputData(true);

    if (!dataReady){
        ShowMessage("Error occurred while reading input file! Impossible to create beam.");
        return;
    }

    try{
        Solver->CreateGeometry();
    } catch(...){
        ShowMessage("Error occurred while creating geometry. Check the values in input file!");
        return;
    }
    try{
        ShowMessage("ViewBeamButtonClick: try CreateBeam");
        Solver->CreateBeam();
    }  catch(...){
        ShowMessage("Error occurred while creating beam. Check the values in input file!");
        return;
    }

    GeomForm->MainSolver=Solver;
    GeomForm->Beam=true;
    GeomForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ViewGeometryButtonClick(TObject *Sender)
{
    bool dataReady=LoadInputData(true);

    if (!dataReady){
        ShowMessage("Error occurred while reading input file! Impossible to create geomentry.");
        return;
    }
  
    try{
        Solver->CreateGeometry();
    } catch(...){
        ShowMessage("Error occurred while creating geometry. Check the values in input file!");
        return;
    }
	try{
        ShowMessage("ViewGeometryButtonClick: try CreateBeam");
        Solver->CreateBeam();
    }  catch(...){
        ShowMessage("Error occurred while creating beam. Check the values in input file!");
        return;
	}

    GeomForm->MainSolver=Solver;
    GeomForm->Beam=false;
    GeomForm->Show();
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




