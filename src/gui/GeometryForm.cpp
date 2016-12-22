//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GeometryForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGeomForm *GeomForm;

//---------------------------------------------------------------------------
__fastcall TGeomForm::TGeomForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TGeomForm::SignChart(TChartType ChartType)
{
    switch (ChartType) {
        case CH_BETTA:{
            GChart->Title->Caption="Wave Phase Velocity";
            GChart->BottomAxis->Title->Caption="z,cm";
            GChart->LeftAxis->Title->Caption="betta phase";
            break;
        };
        case CH_A:{
            GChart->Title->Caption="Normalized Electric Field";
            GChart->BottomAxis->Title->Caption="z,cm";
            GChart->LeftAxis->Title->Caption="A";
            break;
        };
        case CH_ELP:{
            GChart->Title->Caption="Deduced Electric Field";
            GChart->BottomAxis->Title->Caption="z,cm";
            GChart->LeftAxis->Title->Caption="E*lmb/sqrt(P), Ohm^-1/2";
            break;
        };
        case CH_B:{
            GChart->Title->Caption="Field Strength (no beam load or lossses)";
            GChart->BottomAxis->Title->Caption="z,cm";
            GChart->LeftAxis->Title->Caption="E, MV/m";
            break;
        };
        case CH_ATT:{
            GChart->Title->Caption="Attenuation";
            GChart->BottomAxis->Title->Caption="z,cm";
            GChart->LeftAxis->Title->Caption="att, 1/m";
            break;
        };
        case CH_APP:{
            GChart->Title->Caption="Aperture";
            GChart->BottomAxis->Title->Caption="z,cm";
            GChart->LeftAxis->Title->Caption="Ra, cm";
            break;
        };
        case CH_BEXT:{
            GChart->Title->Caption="External Magnetic Field";
            GChart->BottomAxis->Title->Caption="z,cm";
            GChart->LeftAxis->Title->Caption="Bz, Gs";
            break;
        };
        case CH_CLEAR:{}
        default:{
            GChart->Title->Caption="";
            GChart->LeftAxis->Title->Caption="";
            GChart->BottomAxis->Title->Caption="";
            break;
        }
        
    }
}
//---------------------------------------------------------------------------
void TGeomForm::ClearParameters()
{
    aParLabel->Visible=false;
    bParLabel->Visible=false;
    eParLabel->Visible=false;
    WavParLabel->Visible=false;
    dWParLabel->Visible=false;
    FavParLabel->Visible=false;
    dFParLabel->Visible=false;
}
//---------------------------------------------------------------------------
void TGeomForm::SetParameters()
{
	AnsiString s,A;
	TBeamParameter P;

    aParLabel->Visible=true;
    bParLabel->Visible=true;
    eParLabel->Visible=true;
    WavParLabel->Visible=true;
    dWParLabel->Visible=true;
    FavParLabel->Visible=true;
	dFParLabel->Visible=true;

	switch (BeamGroup->ItemIndex) {
		case (xpx_chart):{
			A="_x";
			P=X_PAR;
			break;
		}
		case (ypy_chart):{
			A="_y";
			P=Y_PAR;
			break;
		}
		case (rpr_chart):{ }
		default: {
        	A="_r";
			P=R_PAR;
			break;
		}
	}

	TTwiss T=MainSolver->GetInputTwiss(P);
	TGauss Gw=MainSolver->GetEnergyStats(0);
	TGauss Gphi=MainSolver->GetPhaseStats(0);
	double beta_gamma=MeVToVelocity(Gw.mean)*MeVToGamma(Gw.mean);
	double Rb=MainSolver->GetBeamRadius(0);

	aParLabel->Caption="alpha"+A+" = "+s.FormatFloat("#0.000##",T.alpha);
	bParLabel->Caption="betta"+A+" = "+s.FormatFloat("#0.000##",100*T.beta)+" cm/rad";
	eParLabel->Caption="epsilon"+A+" = "+s.FormatFloat("#0.000##",1e6*T.epsilon)+" um";
	enParLabel->Caption="e_norm"+A+" = "+s.FormatFloat("#0.000##",1e6*T.epsilon*beta_gamma)+" um";
	WavParLabel->Caption="Wav = "+s.FormatFloat("#0.000##",Gw.mean)+" MeV";
	dWParLabel->Caption="dW = "+s.FormatFloat("#0.000##",Gw.sigma)+" MeV";
	FavParLabel->Caption="Phi av = "+s.FormatFloat("#0.000##",RadToDegree(Gphi.mean))+" deg";
	dFParLabel->Caption="dPhi = "+s.FormatFloat("#0.000##",RadToDegree(Gphi.sigma))+" deg";
	rParLabel->Caption="Rbeam (rms) = "+s.FormatFloat("#0.000##",1000*Rb)+" mm";
}
//---------------------------------------------------------------------------
bool TGeomForm::IsSpace(int index)
{
	bool R=false;

	switch (index) {
		case (rpr_chart):{ }
		case (xpx_chart):{ }
		case (ypy_chart):{ }
		case (xy_chart) :{ }
		case (rth_chart) :{ }
		case (thpth_chart) :{}
		case (wphi_chart):{
			R=true;
		}
	}

	return R;
}
//---------------------------------------------------------------------------
bool TGeomForm::IsSpectrum(int index)
{
	bool R=false;

	switch (index) {
		case (w_chart):{ }
		case (phi_chart):{ }
		case (r_chart):{ }
		case (x_chart) :{ }
		case (y_chart) :{
			R=true;
		}
	}

	return R;
}
//---------------------------------------------------------------------------
void TGeomForm::DrawBeamEnvelope(TLineSeries *Series0,TBeamSolver *Solver,TColor Col1)
{
	double x,y,x1,y1,k;
	double s,An;
   // int Nb=Solver->GetNumberOfBars();
    TSpectrumBar *Spectrum=NULL;
	double X,dX;
	bool ellipse_env=false;
	TEllipse E;
	TBeamParameter P;
    #define Np 500

    Series0->Clear();

	if(IsSpace(BeamGroup->ItemIndex)){
		switch (BeamGroup->ItemIndex) {
			case (rpr_chart):{
				P=R_PAR;
				ellipse_env=true;
				break;
			};
			case (xpx_chart):{
				P=X_PAR;
				ellipse_env=true;
				break;
			};
			case (ypy_chart):{
				P=Y_PAR;
				ellipse_env=true;
				break;
			};
		}
		E=Solver->GetEllipse(0,P);
		//Solver->GetCourantSneider(0,alpha,betta,eps);
		E.x0*=1e3;
		E.y0*=1e3;
        h=2.27;
		E.ax*=1e3*h;
		E.by*=1e3*h;

		if (ellipse_env){
            for (int i=0;i<Np;i++){
				k=2.0*E.ax/(Np/2-1);
				if (i<Np/2){
					x=-E.ax+k*i;//+E.x0;
					s=1;
				}else{
					x=3*E.ax-k*i;//+E.x0;
					s=-1;
				}
				An=1-sqr((x/*-E.x0*/)/E.ax);
				if (An>=0)
					y=s*E.by*sqrt(An);//+E.y0;
				else
					y=0;//E.y0;

				x1=x*cos(E.phi)-y*sin(E.phi);
                y1=x*sin(E.phi)+y*cos(E.phi);
				Series0->AddXY(x1+E.x0,y1+E.y0,"",Col1);
            }
        }
	} /*else if (IsSpectrum(BeamGroup->ItemIndex)) {
		bool Smooth=true;
		switch (BeamGroup->ItemIndex) {
			case w_chart:
			{
				Spectrum=Solver->GetEnergySpectrum(0,Smooth);
				break;
			}
			case phi_chart:
			{
				Spectrum=Solver->GetPhaseSpectrum(0,Smooth);
				break;
			}
			case r_chart:
			{
				Spectrum=Solver->GetRSpectrum(0,Smooth);
				break;
			}
			case x_chart:
			{
				Spectrum=Solver->GetXSpectrum(0,Smooth);
				break;
			}
			case y_chart:
			{
				Spectrum=Solver->GetYSpectrum(0,Smooth);
				break;
			}
		}
		if (Spectrum!=NULL) {
			for (int i=0;i<Nb;i++){
				x=Spectrum[i].x;
				y=Spectrum[i].N;
				switch (BeamGroup->ItemIndex){
					case (phi_chart):{
						x=RadToDegree(x);
					break;
				}
				case (r_chart):{}
				case (x_chart):{}
				case (y_chart):{
					x=1000*x;
					break;
				}
			}
			Series0->AddXY(Spectrum[i].x,Spectrum[i].y,"",Col1);
			}
			delete[] Spectrum;
		}
	}         */
    #undef Np

}
//---------------------------------------------------------------------------
void TGeomForm::DrawBeam(TPointSeries *Series0,TBeamSolver *Solver,TColor Col1)
{
    using namespace HellwegTypes ;
    Series0->Clear();

    int Np=Solver->GetNumberOfParticles();
	double *X0=NULL,*Y0=NULL;
	double x=0,y=0,r=0,th=0,br=0,bth=0,beta=0;
	bool NoGraph=false;
	TPhaseSpace R,C;
	TBeamParameter P1,P2;

	if (IsSpace(BeamGroup->ItemIndex)) {

		switch (BeamGroup->ItemIndex) {
			case (rpr_chart):{
				P1=R_PAR;
				P2=AR_PAR;
				break;
			}
			case (xpx_chart):{
				P1=X_PAR;
				P2=AX_PAR;
				break;
			}
			case (ypy_chart):{
				P1=Y_PAR;
				P2=AY_PAR;
				break;
			}
			case (xy_chart):{
				P1=X_PAR;
				P2=Y_PAR;
				break;
			}
			case (thpth_chart):{
				P1=TH_PAR;
				P2=ATH_PAR;
				break;
			}
			case (rth_chart):{
				P1=R_PAR;
				P2=TH_PAR;
				break;
			}
			case (wphi_chart):{
				P1=W_PAR;
				P2=PHI_PAR;
				break;
			}

		}
		X0=Solver->GetBeamParameters(0,P1);
		Y0=Solver->GetBeamParameters(0,P2);

		for (int i=0;i<Np;i++){
			switch (BeamGroup->ItemIndex) {
				case (rpr_chart):{
					GChart->Title->Caption="Radial Phase Space";
					GChart->BottomAxis->Title->Caption="r,mm";
					GChart->LeftAxis->Title->Caption="r',mrad";

					X0[i]*=1000;
					Y0[i]*=1000;
					break;
				}
				case (xpx_chart):{
					GChart->Title->Caption="Horizontal Phase Space";
					GChart->BottomAxis->Title->Caption="x,mm";
					GChart->LeftAxis->Title->Caption="x',mrad";

					X0[i]*=1000;
					Y0[i]*=1000;
					break;
				}
				case (ypy_chart):{
					GChart->Title->Caption="Vertical Phase Space";
					GChart->BottomAxis->Title->Caption="y,mm";
					GChart->LeftAxis->Title->Caption="y',mrad";

					X0[i]*=1000;
					Y0[i]*=1000;
					break;
				}
				case (xy_chart):{
					GChart->Title->Caption="Beam Cross Section";
					GChart->BottomAxis->Title->Caption="x,mm";
					GChart->LeftAxis->Title->Caption="y,mm";

					X0[i]*=1000;
					Y0[i]*=1000;
					break;
				}
				case (thpth_chart):{
					GChart->Title->Caption="Azimuthal Phase Space";
					GChart->BottomAxis->Title->Caption="theta,deg";
					GChart->LeftAxis->Title->Caption="theta',mrad";

					X0[i]=RadToDegree(X0[i]);
					Y0[i]*=1000;
					break;
				}
				case (rth_chart):{
					GChart->Title->Caption="Cylindrical Cross Section";
					GChart->BottomAxis->Title->Caption="theta,deg";
					GChart->LeftAxis->Title->Caption="r,mm";

					X0[i]=RadToDegree(X0[i]);
					Y0[i]*=1000;
					break;
				}
				case (wphi_chart):{
					GChart->Title->Caption="Longitudinal Phase Space";
					GChart->BottomAxis->Title->Caption="phi,deg";
					GChart->LeftAxis->Title->Caption="W,MeV";

					X0[i]=RadToDegree(X0[i]);
					//Y0[i]=Y0[i];
					break;
				}
				default:{
					NoGraph=true;
					break;
				}
			}
		}
		if (!NoGraph){
			for (int i=0;i<Np;i++)
				Series0->AddXY(X0[i],Y0[i],"",Col1);
			//Rescale();
		}
	}

	DeleteArray(X0);
	DeleteArray(Y0);
}
//---------------------------------------------------------------------------
void TGeomForm::DrawBarChart(TBarSeries *Series0,TBeamSolver *Solver,TColor Col1)
{
    Series0->Clear();

	double x=0,y=0;
	int n=0;
	TSpectrumBar *Spectrum=NULL;
    double X,dX;
	int Nb=100;//Solver->GetNumberOfBars();
	Solver->SetBarsNumber(Nb);
	bool NoGraph=false,Smooth=EnvelopeCB->Checked;

	if (!IsSpectrum(BeamGroup->ItemIndex)) {
    	return;
	}

	switch (BeamGroup->ItemIndex) {
		case (w_chart):{
			GChart->Title->Caption="Energy Spectrum";
			GChart->BottomAxis->Title->Caption="W,MeV";
			GChart->LeftAxis->Title->Caption="Number of particles";

			Spectrum=Solver->GetEnergySpectrum(0,Smooth);
			break;
		}
		case (phi_chart):{
			GChart->Title->Caption="Phase Spectrum";
			GChart->BottomAxis->Title->Caption="phi,deg";
			GChart->LeftAxis->Title->Caption="Number of particles";

			Spectrum=Solver->GetPhaseSpectrum(0,Smooth);
			break;
		}
		case (r_chart):{
			GChart->Title->Caption="Radial Spectrum";
			GChart->BottomAxis->Title->Caption="r,mm";
			GChart->LeftAxis->Title->Caption="Number of particles";

			Spectrum=Solver->GetRSpectrum(0,Smooth);
			break;
		}
		case (x_chart):{
			GChart->Title->Caption="Horizontal Spectrum";
			GChart->BottomAxis->Title->Caption="x,mm";
			GChart->LeftAxis->Title->Caption="Number of particles";

			Spectrum=Solver->GetXSpectrum(0,Smooth);
			break;
		}
		case (y_chart):{
			GChart->Title->Caption="Vertical Spectrum";
			GChart->BottomAxis->Title->Caption="y,mm";
			GChart->LeftAxis->Title->Caption="Number of particles";

			Spectrum=Solver->GetYSpectrum(0,Smooth);
			break;
		}
		default: { return;}
	}

	for (int i=0;i<Nb;i++){
		x=Spectrum[i].x;
		y=Spectrum[i].y;
		n=Spectrum[i].N;
		switch (BeamGroup->ItemIndex){
			case (phi_chart):{
				x=RadToDegree(x);
				break;
			}
			case (r_chart):{}
			case (x_chart):{}
			case (y_chart):{
				x=1000*x;
				break;
			}
		}
		Series0->AddXY(x,n);
		if (Smooth){
			if (i==0) {
				EnvelopeSeries1->AddXY(x-1e-3,0,clBlue);
			}
			EnvelopeSeries1->AddXY(x,y,clBlue);
			if (i==Nb-1) {
				EnvelopeSeries1->AddXY(x+1e-3,0,clBlue);
			}
		}
	}

	if (Spectrum!=NULL)
		delete[] Spectrum;

}
//---------------------------------------------------------------------------
void TGeomForm::DrawChart(TLineSeries *Series0,TBeamSolver *Solver,TColor Col1, TColor Col2)
{
    Series0->Clear();

    int Np=Solver->GetNumberOfPoints();
 // double lmb=Solver->GetWaveLength();
    //int Nm=Solver->GetMeshPoints();
    double *X0,*Y0;
    double *N0;

    X0= new double[Np];
    Y0= new double[Np];
    N0= new double[Np];

	N0=Solver->GetStructureParameters(NUM_PAR);
    
    switch (ChartGroup->ItemIndex) {
        case (betta_chart):{
            SignChart(CH_BETTA);
			Y0=Solver->GetStructureParameters(SBETA_PAR);
            break;
        }
        case (A_chart):{
            SignChart(CH_A);
			Y0=Solver->GetStructureParameters(A_PAR);
            break;
        }
        case (ELP_chart):{
            SignChart(CH_ELP);
			Y0=Solver->GetStructureParameters(RP_PAR);
            for (int i=0;i<Np;i++)
                Y0[i]=sqrt(2*Y0[i]);
            break;
        }
        case (B_chart):{
            SignChart(CH_B);
			Y0=Solver->GetStructureParameters(E0_PAR);
            for (int i=0;i<Np;i++)
                Y0[i]=1e-6*Y0[i];
            break;
        }
        case (alpha_chart):{
            SignChart(CH_ATT);
			Y0=Solver->GetStructureParameters(ALPHA_PAR);
            break;
        }
        case (Bz_chart):{
            SignChart(CH_BEXT);
			Y0=Solver->GetStructureParameters(B_EXT_PAR);
            for (int i=0;i<Np;i++)
                Y0[i]=10000*Y0[i];
            break;
        }
        case (Ra_chart):{
            SignChart(CH_APP);
            Y0=Solver->GetStructureParameters(RA_PAR);
            for (int i=0;i<Np;i++)
                Y0[i]=100*Y0[i];
            break;
        }
    }
    
	X0=Solver->GetStructureParameters(Z_PAR);

   /*   FILE *F;
    F=fopen("geometry.log","w"); */
    TColor Col=Col1;
    int last=N0[0];

    for (int i=0;i<Np;i++){
        if (N0[i]!=last){
            if (Col==Col1)
                Col=Col2;
            else
                Col=Col1;
            last=N0[i];
        }
       /*   if (i>0 && X0[i]==X0[i-1])
            X0[i]+=1e-6; */
		Series0->AddXY(X0[i]*100,Y0[i],"",Col);
	}

    delete[] X0;
    delete[] Y0;
    delete[] N0;
}
//---------------------------------------------------------------------------
void __fastcall TGeomForm::FormShow(TObject *Sender)
{
    Series1->Visible=!Beam;
    C1=false;
    Series2->Visible=Compare1->Checked && !Beam;
    ChartGroup->Enabled=!Beam;

    BeamGroup->Enabled=Beam;
    BeamSeries1->Visible=Beam;
    
    h=1;//MainSolver->GetKernel();
 // ChartSeries1->Visible=false;

 	Draw();

   /* if (Beam){
        if (BeamBox->Checked)
            DrawBeam(BeamSeries1,MainSolver,Particle_col);
        if (EnvelopeCB->Checked)
			DrawBeamEnvelope(EnvelopeSeries1,MainSolver,Envelope_col);
        SetParameters();
    }
    else{
        DrawChart(Series1,MainSolver,Line1_col_a,Line1_col_b);
        ClearParameters();
	}   */
}
//---------------------------------------------------------------------------

void __fastcall TGeomForm::Exit1Click(TObject *Sender)
{
    GeomForm->Close();  
}
//---------------------------------------------------------------------------

void __fastcall TGeomForm::OpenButton1Click(TObject *Sender)
{
    OpenDialog1->Execute();
    AnsiString File1Name=OpenDialog1->FileName;
    FileEdit1->Text=File1Name;

    CompData1->InputFile=File1Name;
    int Err=CompData1->LoadData();
	CompData1->CreateGeometry();
	if (Beam)
		CompData1->CreateBeam();

	if (Err==ERR_NO){
		//DrawChart(Series2,CompData1,Line2_col_a,Line2_col_b);
		C1=true;
		Draw();
    } else
		C1=false;
}
//---------------------------------------------------------------------------
void __fastcall TGeomForm::FormCreate(TObject *Sender)
{
    Path=Application->ExeName;
    Path=ExtractFileDir(Path);
    
    TIniFile *UserIni;
    UserIni=new TIniFile(Path+"\\hellweg.ini");
	Np_max=UserIni->ReadInteger("OTHER","Maximum Points Output",Np_max);
    CompData1=new TBeamSolver;
}
//---------------------------------------------------------------------------

void __fastcall TGeomForm::FormDestroy(TObject *Sender)
{
	delete CompData1;
}
//---------------------------------------------------------------------------

void __fastcall TGeomForm::Compare1Click(TObject *Sender)
{
	Series2->Visible=Compare1->Checked;// && !Beam;
	BeamSeries2->Visible=Compare1->Checked;
	EnvelopeSeries2->Visible=Compare1->Checked;
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TGeomForm::ChartGroupClick(TObject *Sender)
{
    DrawChart(Series1,MainSolver,Line1_col_a,Line1_col_b);
    if (C1)
		DrawChart(Series2,CompData1,Line2_col_a,Line2_col_b);
}
//---------------------------------------------------------------------------
void TGeomForm::Rescale()
{
	GChart->Update();

	double ymin=GChart->LeftAxis->Minimum;
	double ymax=GChart->LeftAxis->Maximum;
	double xmin=GChart->BottomAxis->Minimum;
	double xmax=GChart->BottomAxis->Maximum;

	xmax=mod(xmax)>mod(xmin)?mod(xmax):mod(xmin);
	ymax=mod(ymax)>mod(ymin)?mod(ymax):mod(ymin);

	GChart->LeftAxis->Automatic=false;
	GChart->LeftAxis->Minimum=-ceil(ymax);
	GChart->LeftAxis->Maximum=ceil(ymax);
	GChart->BottomAxis->Automatic=false;
	GChart->BottomAxis->Minimum=-ceil(xmax);
	GChart->BottomAxis->Maximum=ceil(xmax);

	GChart->Refresh();
}
//---------------------------------------------------------------------------
void TGeomForm::Draw()
{
	if (!Beam) {
        return;
	}
	try{
		SetParameters();
		GChart->LeftAxis->Automatic=true;
		GChart->BottomAxis->Automatic=true;
		if (EnvelopeCB->Checked){
			DrawBeamEnvelope(EnvelopeSeries1,MainSolver,Envelope_col);
            if (C1 && Compare1->Checked) {
				DrawBeamEnvelope(EnvelopeSeries2,CompData1,clNavy);
			}
		}else{
			EnvelopeSeries1->Clear();
			EnvelopeSeries2->Clear();
		}

		if (BeamBox->Checked){
			DrawBeam(BeamSeries1,MainSolver,Particle_col);
			DrawBarChart(BarSeries1,MainSolver,clRed);
			if (C1 && Compare1->Checked) {
				DrawBeam(BeamSeries2,CompData1,clMaroon);
			}
		}else{
			BeamSeries1->Clear();
			BeamSeries2->Clear();
			BarSeries1->Clear();
		}
	}
	catch (...){
		ShowMessage("There's an error while processing the data. May be some parameters are not correct.");
	}
}
//---------------------------------------------------------------------------
void __fastcall TGeomForm::BeamGroupClick(TObject *Sender)
{
	Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGeomForm::BeamBoxClick(TObject *Sender)
{
	Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGeomForm::EnvelopeCBClick(TObject *Sender)
{
	Draw();
}
//---------------------------------------------------------------------------

