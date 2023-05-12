//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ResultsForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TResForm *ResForm;

//using namespace HellwegTypes;
//---------------------------------------------------------------------------
__fastcall TResForm::TResForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TResForm::FormCreate(TObject *Sender)
{
	SolverLoaded=false;
   //	CreateTable();

 /* for (int i=0;i<100;i++)
        Series1->AddXY(i,i*i);
    
    for (int i=0;i<100;i++)
        Series1->AddXY(i,sqrt(i));
    Series1->AddNull(0);
    for (int i=0;i<100;i++)
        Series1->AddXY(i,2*i);
    Series1->AddNull(0);
    for (int i=0;i<100;i++)
        Series1->AddXY(i,8*i);    */
}
//---------------------------------------------------------------------------
void __fastcall TResForm::FormShow(TObject *Sender)
{
    Npts=Solver->GetNumberOfPoints();
	Np=Solver->GetNumberOfParticles();

	for (int i=0;i<Npts;i++){
		int Nliv=Solver->GetLivingNumber(i);
		if (Nliv==0){
			Npts=i;
			break;
		}
	}
	//Nb=Solver->GetNumberOfBars();

	KernelChanged();
	AccuracyChanged();
	BinsChanged();
	SynchChanged();

	PositionTrackBar->Max=Npts-1;
	PositionTrackBar->Position=PositionTrackBar->Max;
 //   PositionChanged();

	GetTransBoundaries();
	CreateTable();
	//DrawField();
	

  /*    for (int i=0;i<Np;i++){
        Series1->AddNullXY(0,0);
        for (int j=0;j<Npoints;j++){
            if (Solver->Beam[j]->Particle[i].lost)
                break;
			Series1->AddXY(j,VelocityToMeV(Solver->Beam[j]->Particle[i].betta));
        }
    }     */
}
//---------------------------------------------------------------------------
void TResForm::CreateTable()
{
	AnsiString EU=GetEnergyUnit(Solver->GetParticleType());

	Table->RowCount=Npar+1;
    Table->ColWidths[colName]=150;
	Table->ColWidths[colValue]=50;

    Table->Cells[colName][pZ]="Position, cm";
	Table->Cells[colName][pWav]="Average Energy, "+EU;
	Table->Cells[colName][pWm]="Maximum Energy, "+EU;
    Table->Cells[colName][pI]="Beam Current, mA";
	Table->Cells[colName][pkc]="Transmitted, %";
	Table->Cells[colName][pFav]="Average Phase, deg";
	Table->Cells[colName][pdF]="Phase Length (rms), deg";
	Table->Cells[colName][pF]="Frequency, MHz";
	Table->Cells[colName][pRa]="Aperture, mm";
	Table->Cells[colName][pPb]="Beam Power, MW";
	Table->Cells[colName][pvph]="Phase Velocity (beta)";
	Table->Cells[colName][pdW]="Energy Spread (FWHM), "+EU;
	Table->Cells[colName][pdWp]="Energy Spectrum (FWHM), %";
	Table->Cells[colName][pE]="Field Strength, MV/m";
	Table->Cells[colName][pcoord]="Coordinate:";
	Table->Cells[colName][pr]="Beam Radius (rms), mm";
	Table->Cells[colName][peps]="Emittance, mm*mrad";
	Table->Cells[colName][pepsn]="Emittance (norm), mm*mrad";
	Table->Cells[colName][palpha]="alpha";
	Table->Cells[colName][pbetta]="betta, cm/rad";

}
//---------------------------------------------------------------------------
void TResForm::GetTransBoundaries()
{
    Rmax=0;
    Xmax=0;
    Wmax=0;
    for (int i=0;i<Npts;i++){
        double Ra=Solver->GetAperture(i);
		double W=Solver->GetMaxEnergy(i);
		double X=Solver->GetMaxDivergence(i);
        if (Ra>Rmax)
			Rmax=Ra;
		if (W>Wmax)
			Wmax=W;
		if (X>Xmax)
			Xmax=X;
    }
    if (Xmax>pi/2)
        Xmax=pi/2;
}
//---------------------------------------------------------------------------
void TResForm::AllPassive()
{
    PackChart->Visible=false;
    BarsChart->Visible=false;
    BeamChart->Visible=false;
	PieChart->Visible=false;
	SpectrumChart->Visible=false;
	MultLineChart->Visible=false;
}
//---------------------------------------------------------------------------
void TResForm::ChartActive()
{
    BarsChart->Visible=false;
    BeamChart->Visible=false;
	PieChart->Visible=false;
	SpectrumChart->Visible=false;
	MultLineChart->Visible=false;

    PackChart->Visible=true;
    PackSeries->Clear();
    LineSeries->Clear();
    AddSeries->Clear();
}
//---------------------------------------------------------------------------
void TResForm::BeamActive()
{
    PackChart->Visible=false;
    BarsChart->Visible=false;
    PieChart->Visible=false;
	SpectrumChart->Visible=false;
	MultLineChart->Visible=false;

    BeamChart->Visible=true;
    BeamSeries->Clear();
}
//---------------------------------------------------------------------------
void TResForm::ParticlesActive()
{
    BeamActive();
    EnvelopeSeries->Visible=false;
}
//---------------------------------------------------------------------------
void TResForm::EnvelopeActive()
{
	BeamActive();
    EnvelopeSeries->Visible=EnvelopeCheck->Checked;
    EnvelopeSeries->Clear();
}
//---------------------------------------------------------------------------
void TResForm::PackActive()
{
    ChartActive();
    PackSeries->Visible=true;
    LineSeries->Visible=false;
    AddSeries->Visible=false;
	PackChart->Legend->Visible=false;
	MultLineChart->Visible=false;
}
//---------------------------------------------------------------------------
void TResForm::LineActive()
{
    ChartActive();
    PackSeries->Visible=false;
    LineSeries->Visible=true;
    AddSeries->Visible=true;
	PackChart->Legend->Visible=true;
	MultLineChart->Visible=false;
	//ExtraSeries->Visible=false;//gType==EPS_PLOT;
}
//---------------------------------------------------------------------------
void TResForm::BarsActive()
{
    PackChart->Visible=false;
    BeamChart->Visible=false;
    PieChart->Visible=false;
	SpectrumChart->Visible=false;
	MultLineChart->Visible=false;

    BarsChart->Visible=true;

    BarSeries1->Clear();
    BarSeries2->Clear();
    BarSeries3->Clear();
	BarSeries4->Clear();
    BarSeries5->Clear();
	BarSeries6->Clear();
	BarSeries7->Clear();
}
//---------------------------------------------------------------------------
void TResForm::PieActive()
{
	PackChart->Visible=false;
	BarsChart->Visible=false;
	BeamChart->Visible=false;
	SpectrumChart->Visible=false;
	MultLineChart->Visible=false;

	PieChart->Visible=true;
	PieSeries->Clear();
}
//---------------------------------------------------------------------------
void TResForm::MultLineActive()
{
	PackChart->Visible=false;
	BarsChart->Visible=false;
	BeamChart->Visible=false;
	SpectrumChart->Visible=false;
	PieChart->Visible=false;
	MultLineChart->Visible=true;

	MultSeries1->Clear();
	MultSeries2->Clear();
	MultSeries3->Clear();
	MultSeries4->Clear();
	MultSeries5->Clear();
	MultSeries6->Clear();
	MultSeries7->Clear();
	MultSeries8->Clear();
}
//---------------------------------------------------------------------------
void TResForm::SpectrumActive()
{
    PackChart->Visible=false;
    BarsChart->Visible=false;
    BeamChart->Visible=false;
	PieChart->Visible=false;
	MultLineChart->Visible=false;
    SpectrumChart->Visible=true;
    SpectrumSeries->Clear();
    SpecEnvSeries->Clear();
}
//---------------------------------------------------------------------------
void TResForm::ResetAxis(TChartAxis *Axis,double NewMin,double NewMax,bool Auto)
{
	Axis->Automatic=Auto;

	double x=0;
	if (NewMin>NewMax) {
		x=NewMin;
		NewMin=NewMax;
		NewMax=x;
	}

	if (NewMin>Axis->Maximum) {
		Axis->Maximum=NewMax;
		Axis->Minimum=NewMin;
	} else {
		Axis->Minimum=NewMin;
		Axis->Maximum=NewMax;
	}
}
//---------------------------------------------------------------------------
void TResForm::DrawTrace(TBeamParameter P1)
{
	double x=0, y=0;
	//int Nliv=0;
	TGraph *T=NULL;
	PackChart->AutoRepaint = false;

	PackSeries->Transparency=TransparentCheck->Checked?70:0;

   /*	DynamicArray<double> X;
	DynamicArray<double> Y;

	X.Length=Np*Npts;
	Y.Length=Np*Npts;   */

/*	PackSeries->XValues->Count = Np*Npts;
	PackSeries->YValues->Count = Np*Npts;   */

	int k=0;

	for (int i=0;i<Np;i++){
		if (SparceFactor!=0 && i%SparceFactor!=0)
			continue;
		PackSeries->AddNullXY(0,0);

		T=Solver->GetTrace(i,Z0_PAR,P1);

	   //	int Ntrace=0;

		for (int j=0;j<Npts;j++){
			if (SparceFactor!=0 && j%SparceFactor!=0)
				continue;

			if (T[j].draw){
				x=100*T[j].x;
				y=T[j].y;
				switch (P1) {
					//case W_PAR:{ }
					case X_PAR:{ }
					case Y_PAR:{ }
					case R_PAR:{
						y=1000*y;
						break;
					}
					case PHI_PAR:{
						y=RadToDegree(y);
						break;
					}
				}
				PackSeries->AddXY(x,y);
			}
	   /*		else{
				x=0;
				y=0;
			}
			X[k]=x;
			Y[k]=y;
			k++;     */
			   //	break;
		 //	Ntrace++;
		}

	 /*	X.Length=Ntrace;
		Y.Length=Ntrace;

		int k=0;
		for (int j=0;j<Npts;j++){
			if (T[j].draw){

				PackSeries->AddXY(X[k],Y[k]);
				k++;
			} else
				break;
		}                   */

	 /*	PackSeries->XValues->Value=X;
		PackSeries->XValues->Count = Np*Npts;
		PackSeries->XValues->Modified = true;
		PackSeries->YValues->Value=Y;
		PackSeries->YValues->Count = Np*Npts;
		PackSeries->YValues->Modified = true;      */

	   	PackChart->AutoRepaint = true;

		delete[] T;
	}


}
//---------------------------------------------------------------------------
void TResForm::DrawSpace(int Nknot,TBeamParameter P1,TBeamParameter P2,bool sliding)
{
	AnsiString s;
	double z=Solver->GetStructureParameter(Nknot,Z_PAR);
	s=BeamChart->Title->Caption+" at position z="+s.FormatFloat("#0.000",100*z)+" cm";
	BeamChart->Title->Caption=s;

	TGraph *G=NULL;
	G=Solver->GetSpace(Nknot,P1,P2);

	double x=0,y=0;

	for (int i=0;i<Np;i++){
		if (G[i].draw){
			x=G[i].x;
			y=G[i].y;

			switch (P1) {
				case PHI_PAR: {x=RadToDegree(x);break;}
				case R_PAR: {}
				case X_PAR: {}
				case Y_PAR:{x=1000*x;break;}
				case ZREL_PAR: {
					x=100*(z+x);
					break;
				}
			}
			switch (P2) {
				case R_PAR: {
					if (sliding)
						y=250*Rmax*cos(DegreeToRad(x)); //cosine
					else
						y=1000*y;//mm
					break;
				}
				case AR_PAR: {}
				case AX_PAR: {}
				case AY_PAR: {}
				case ATH_PAR: {}
				case X_PAR: {}
				case Y_PAR:{y=1000*y;break;}
				//case ATH_PAR: {y=y*x;break;}
			}
		   //	if (i==0)
				//BeamSeries->AddXY(x,y,clBlue);
			//else
				BeamSeries->AddXY(x,y);
		}
	}

	if (sliding)
		BeamChart->ExchangeSeries(1,0);

	delete[] G;
}
//---------------------------------------------------------------------------
void TResForm::DrawPlots(TStructureParameter P1,TStructureParameter P2)
{
	double *Z=NULL,*X=NULL,*Y=NULL;
	double x=0,y=0;

	Z=Solver->GetStructureParameters(Z_PAR);
	X=Solver->GetStructureParameters(P1);
	Y=Solver->GetStructureParameters(P2);

	for (int i=0;i<Npts;i++){
		double x=X[i];
		double y=Y[i];

		switch (P1) {
			//case PBEAM_PAR: {}
			case EREAL_PAR: {}
			case E0_PAR:{
			   //	x=1e-6*x;//MeV
				y=1e-6*y;
			   //	break;
			}
			case PRF_PAR: {
				x=1e-6*x;//MeV
				break;
			}
			case RB_PAR: {}
			case RA_PAR: {
				x=2000*x;//mm
				y=1000*y;
				break;
			}
			case ER_PAR: { }
			case EX_PAR: { }
			case EY_PAR: { }
			case E4D_PAR: { }
			case ET_PAR: { }
			case ENR_PAR: { }
			case ENX_PAR: { }
			case ENY_PAR: { }
			case ENT_PAR: {
				x=1e6*x;  //microns
				y=1e6*y;
				break;
			}
			case E4DN_PAR: {
				x=1e6*x;  //microns
				y=1e6*y;
				break;
			}
		}

		LineSeries->AddXY(100*Z[i],x);
		AddSeries->AddXY(100*Z[i],y);
	}

	delete[] Z;
	delete[] X;
	delete[] Y;
}
//---------------------------------------------------------------------------
void TResForm::DrawSpectrum(int Nknot,TBeamParameter P1)
{
	AnsiString S;

	int j=PositionTrackBar->Position;
	double z=100*Solver->GetStructureParameter(j,Z0_PAR);
	SpectrumChart->Title->Caption+=" at position z="+S.FormatFloat("#0.000",z)+" cm";

	int Nmax=0;
	double Ymax=0,PlotMax=0,x=0;
	bool Smooth=true;
	double *X;
	X=new double[NumBins];

	TSpectrumBar *Spectrum=Solver->GetSpectrum(j,P1,Smooth);
	//=Solver->GetPhaseSpectrum(j,true,F,dF);

	for (int i=0;i<NumBins;i++){
		if (Spectrum[i].N>Nmax)
			Nmax=Spectrum[i].N;
		if (Spectrum[i].y>Ymax)
			Ymax=Spectrum[i].y;
		//X[i]=Spectrum[i].x;
		switch (P1) {
			case R_PAR: {}
			case X_PAR: {}
			case Y_PAR: {
				X[i]=1000*Spectrum[i].x;
				break;
			}
			case TH_PAR:{}
			case PHI_PAR:{
				X[i]=RadToDegree(Spectrum[i].x);
				break;
			}
			default: {
				X[i]=Spectrum[i].x;
			}
		}
	}
	PlotMax=1.0*Nmax>Ymax?1.0*Nmax:Ymax;

	ResetAxis(SpectrumChart->LeftAxis,0,PlotMax,VertFitCheck->Checked);
  //  SpecEnvSeries->Clear();
	if (EnvelopeCheck->Checked){
		/*SpecEnvSeries->AddXY(MinPhase,0);
		SpecEnvSeries->AddXY(MaxPhase,0);    */
		SpecEnvSeries->AddXY(X[0]-1e-3,0);
		for (int i=0;i<NumBins;i++){
			SpecEnvSeries->AddXY(X[i],Spectrum[i].y);
		}
		SpecEnvSeries->AddXY(X[NumBins-1]+1e-3,0);
    }

    if (ChartCheck->Checked){
		for (int i=0;i<NumBins;i++)
			SpectrumSeries->AddXY(X[i],Spectrum[i].N);
	}

	delete[] Spectrum;
	delete[] X;
}
//---------------------------------------------------------------------------
void TResForm::DrawEnergy()
{
	gType=W_TRACE;
	PackActive();

	AnsiString EU=GetEnergyUnit(Solver->GetParticleType());

    PackChart->Title->Caption="Particles Energy";
    PackChart->BottomAxis->Title->Caption="z,cm";
	PackChart->LeftAxis->Title->Caption="W, "+EU;

	DrawTrace(W_PAR);
	//DrawTrace(BZ_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawPhase()
{
	gType=PHI_TRACE;
    PackActive();

    PackChart->Title->Caption="Particles Phase";
    PackChart->BottomAxis->Title->Caption="z,cm";
    PackChart->LeftAxis->Title->Caption="phi,deg";

	DrawTrace(PHI_PAR);
	//DrawTrace(BTH_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawRadius()
{
	gType=R_TRACE;
	PackActive();

	PackChart->Title->Caption="Particles Radius";
	PackChart->BottomAxis->Title->Caption="z,cm";

	TBeamParameter P;
	AnsiString Ytit;
	switch (RadiusGroup->ItemIndex) {
		case r4D_coord:{}
		case th_coord:{}
		case r_coord:{
			P=R_PAR;
			//P=BR_PAR;
			Ytit="r,mm";
			break;
		}
		case x_coord:{
			P=X_PAR;
		   //	P=BR_PAR;
			Ytit="x,mm";
			break;
		}
		case y_coord:{
			P=Y_PAR;
			//P=BTH_PAR;
			Ytit="y,mm";
			break;
		}
	}


	PackChart->LeftAxis->Title->Caption=Ytit;

	DrawTrace(P);
	//AVERAGE RADIUS AS ENVELOPE!!!!
}
//---------------------------------------------------------------------------
void TResForm::DrawField()
{
	gType=E_PLOT;
    LineActive();

    PackChart->Title->Caption="Field Strength";
    PackChart->BottomAxis->Title->Caption="z,cm";
    PackChart->LeftAxis->Title->Caption="E,MV/m";
    PackSeries->Title="";
    LineSeries->Title="with beam load and losses";
    AddSeries->Title="no beam load and losses";

	DrawPlots(E0_PAR,EREAL_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawPower()
{
	gType=P_PLOT;
    LineActive();

    PackChart->Title->Caption="Power";
    PackChart->BottomAxis->Title->Caption="z,cm";
	PackChart->LeftAxis->Title->Caption="P,MW";
	PackSeries->Title="";
	LineSeries->Title="RF Power";
	AddSeries->Title="Beam Power";

	DrawPlots(PRF_PAR,PBEAM_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawBetta()
{
	gType=BETA_PLOT;
    LineActive();

    PackChart->Title->Caption="Velocity";
    PackChart->BottomAxis->Title->Caption="z,cm";
	PackChart->LeftAxis->Title->Caption="Beta";

    LineSeries->Title="Phase Velocity (RF)";
    AddSeries->Title="Beam Velocity";

	DrawPlots(SBETA_PAR,BBETA_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawAvEnergy()
{
	gType=W_PLOT;
	LineActive();

	AnsiString EU=GetEnergyUnit(Solver->GetParticleType());

    PackChart->Title->Caption="Energy";
    PackChart->BottomAxis->Title->Caption="z,cm";
	PackChart->LeftAxis->Title->Caption="W, "+EU;

    LineSeries->Title="Average Energy";
    AddSeries->Title="Maximum Energy";

	DrawPlots(WAV_PAR,WMAX_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawAvRadius()
{
	gType=R_PLOT;
	LineActive();

	PackChart->Title->Caption="Radius";
	PackChart->BottomAxis->Title->Caption="z,cm";
	PackChart->LeftAxis->Title->Caption="r,mm";

	LineSeries->Title="Beam Radius (RMS)";
	AddSeries->Title="Aperture";

	DrawPlots(RB_PAR,RA_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawEmittance()
{
	gType=EPS_PLOT;
	LineActive();
	TStructureParameter P1,P2;
	AnsiString Ytit;

	PackChart->Title->Caption="Emittance";
	PackChart->BottomAxis->Title->Caption="z,cm";

	switch (RadiusGroup->ItemIndex) {
		case r_coord:{
			P1=ER_PAR;
			P2=ENR_PAR;
			Ytit="er,um";
			break;
		}
		case x_coord:{
			P1=EX_PAR;
			P2=ENX_PAR;
			Ytit="ex,um";
			break;
		}
		case y_coord:{
			P1=EY_PAR;
			P2=ENY_PAR;
			Ytit="ey,um";
			break;
		}
		case r4D_coord:{
			P1=E4D_PAR;
			P2=E4DN_PAR;
			Ytit="e4D,um";
			break;
		}
		case th_coord:{
			P1=ET_PAR;
			P2=ENT_PAR;
			Ytit="eth,um";
			break;
		}
	}

	PackChart->LeftAxis->Title->Caption=Ytit;

	LineSeries->Title="Actual (rms)";
	AddSeries->Title="Normalized (rms)";

	DrawPlots(P1,P2);
}
//---------------------------------------------------------------------------
void TResForm::DrawLongtSection(bool sliding)
{
	gType=sliding?PHASE_SLID:LONGT_SEC;
	EnvelopeActive();
	int j=PositionTrackBar->Position;

	BeamChart->Title->Caption="Longitudinal Section";// at position z="+s.FormatFloat("#0.000",z)+" cm";
    BeamChart->BottomAxis->Title->Caption="phi,deg";
	BeamChart->LeftAxis->Title->Caption="r,mm";
	ResetAxis(BeamChart->LeftAxis,-1000*Rmax,1000*Rmax,VertFitCheck->Checked);
	ResetAxis(BeamChart->BottomAxis,MinPhase,MaxPhase,HorFitCheck->Checked);
	/*BeamChart->LeftAxis->Automatic=VertFitCheck->Checked;
	BeamChart->BottomAxis->Automatic=HorFitCheck->Checked;
	BeamChart->LeftAxis->Minimum=-1000*Rmax;
	BeamChart->LeftAxis->Maximum=1000*Rmax;
	BeamChart->BottomAxis->Minimum=MinPhase;
	BeamChart->BottomAxis->Maximum=MaxPhase;  */

	double Phi_min=RadToDegree(Solver->GetMinPhase(j));
	double Phi_max=RadToDegree(Solver->GetMaxPhase(j));

	if (EnvelopeCheck->Checked){
		for (int i=0;i<PointsNumber;i++){
			double phi=Phi_min+i*(Phi_max-Phi_min)/(PointsNumber-1);
			EnvelopeSeries->AddXY(phi,250*Rmax*cos(DegreeToRad(phi)));
        }
    }
   //   sqr(A[j]*Wo/ELPi[j])
	if (ChartCheck->Checked)
		DrawSpace(j,PHI_PAR,R_PAR,sliding);
}
//---------------------------------------------------------------------------
void TResForm::DrawLongtSpace()
{
	gType=LONGT_SPACE;
    //ParticlesActive();
	EnvelopeActive();
	int j=PositionTrackBar->Position;

	AnsiString EU=GetEnergyUnit(Solver->GetParticleType());

	BeamChart->Title->Caption="Longitudinal Phase Space";// at position z="+s.FormatFloat("#0.000",z)+" cm";
    BeamChart->BottomAxis->Title->Caption="phi,deg";
	BeamChart->LeftAxis->Title->Caption="W, "+EU;

	if (VertFitCheck->Checked)
		Wmax=Solver->GetMaxEnergy(j);
	else
		Wmax=7;

	double Phi_min=0;
	double Phi_max=0;

	double W0=Solver->GetParticleRestEnergy();

	if (HorFitCheck->Checked){
		Phi_min=RadToDegree(Solver->GetMinPhase(j));
		Phi_max=RadToDegree(Solver->GetMaxPhase(j));
	}   else {
		Phi_min=MinPhase;
		Phi_max=MaxPhase;
	}

	   // Wmax=0.55;
	ResetAxis(BeamChart->LeftAxis,0,Wmax,false);
	ResetAxis(BeamChart->BottomAxis,Phi_min,Phi_max,HorFitCheck->Checked);

  	BeamChart->LeftAxis->Automatic=VertFitCheck->Checked;
	BeamChart->LeftAxis->Minimum=0;
	BeamChart->LeftAxis->Maximum=Wmax;
	BeamChart->BottomAxis->Automatic=HorFitCheck->Checked;
	BeamChart->BottomAxis->Minimum=Phi_min;
	BeamChart->BottomAxis->Maximum=Phi_max;

	if (EnvelopeCheck->Checked){
		//EnvelopeSeries->Transparency=50;
        double H=0,W=0, gamma=0, phi=0;
		double beta_w=Solver->GetStructureParameter(j,SBETA_PAR);
		double A=Solver->GetStructureParameter(j,A_PAR);
        bool sep=true;
		int Nroots=0;

        double Hmin=1e6, Hmax=0;

        for (int k=0; k < SeparatrixNumber; k++) {
			W=k*Wmax/(SeparatrixNumber-1);
			gamma=MeVToGamma(W,W0);
		 // for (int i=0;i<PointsNumber;i++){
			   //   phi=MinPhase+i*(MaxPhase-MinPhase)/(PointsNumber-1);
			   //phi=90;
				H=GetH(gamma,90,beta_w,A);
				if (H>Hmax)
					Hmax=H;
				if (H<Hmin)
					Hmin=H;

      //        }
		}

        for (int n=0; n < 2; n++) {
			for (int k=0; k < SeparatrixNumber; k++) {

				if (Solver->CheckDrift(j)) {
					W=k*Wmax/(SeparatrixNumber-1);
					gamma=MeVToGamma(W,W0);
					H=GetH(gamma,90,beta_w,A);
				} else
					H=Hmin+k*(Hmax-Hmin)/(SeparatrixNumber-1);

                sep=true;

                    for (int i=0;i<PointsNumber;i++){
					//phi=MinPhase+i*(MaxPhase-MinPhase)/(PointsNumber-1);
					phi=Phi_min+i*(Phi_max-Phi_min)/(PointsNumber-1);
					Nroots=n==0?GetPositiveSeparatrix(gamma,phi,beta_w,A,H):GetNegativeSeparatrix(gamma,phi,beta_w,A,H);
					if (((n==0 && Nroots>0) || (n==1 & Nroots==2)) && gamma>1){
						W=GammaToMeV(gamma,W0);
						if (W>2*Wmax)
							continue;
					 // if (W>0 && W<Wmax) {
							if (sep)
								EnvelopeSeries->AddNullXY(phi,W);
                            else
                                EnvelopeSeries->AddXY(phi,W);
                            sep=false;
                        }  else
                            sep=true;
                    //}
                }
            }
		}
	}

	if (ChartCheck->Checked)
		DrawSpace(j,PHI_PAR,W_PAR);

	BeamChart->ExchangeSeries(1,0);

}
//---------------------------------------------------------------------------
void TResForm::DrawTransSection()
{
    gType=TRANS_SEC;
    EnvelopeActive();
	int j=PositionTrackBar->Position;

	BeamChart->Title->Caption="Transverse Section";
	BeamChart->BottomAxis->Title->Caption="x,mm";
	BeamChart->LeftAxis->Title->Caption="y,mm";

	ResetAxis(BeamChart->LeftAxis,-1000*Rmax,1000*Rmax,VertFitCheck->Checked);
	ResetAxis(BeamChart->BottomAxis,-1000*Rmax,1000*Rmax,HorFitCheck->Checked);

   /*	BeamChart->LeftAxis->Automatic=VertFitCheck->Checked;
	BeamChart->BottomAxis->Automatic=HorFitCheck->Checked;
	BeamChart->LeftAxis->Minimum=-1000*Rmax;
	BeamChart->LeftAxis->Maximum=1000*Rmax;
    BeamChart->BottomAxis->Minimum=-1000*Rmax;
	BeamChart->BottomAxis->Maximum=1000*Rmax; */

	double x=0,y=0,k=0,An=0,s=0;
    if (EnvelopeCheck->Checked){
		//double h=1;//Solver->GetKernel();

		TEllipse Ex,Ey;
		Ex=Solver->GetEllipse(j,X_PAR);
		Ey=Solver->GetEllipse(j,Y_PAR);
		Ex.y0*=1e3;
		Ey.y0*=1e3;
	   /*	Ex.by*=1e3*Hcore;
		Ey.by*=1e3*Hcore;
		double rx=Ex.by;
		double ry=Ey.by;        */
		TTwiss Tx,Ty;
		Tx=Solver->GetTwiss(j,X_PAR);
		Ty=Solver->GetTwiss(j,Y_PAR);
		double rx=1000*Hcore*sqrt(Tx.epsilon*Tx.beta);
		double ry=1000*Hcore*sqrt(Ty.epsilon*Ty.beta);
		for (int i=0;i<PointsNumber;i++){
			k=2.0*rx/(PointsNumber/2-1);
			if (i<PointsNumber/2){
				x=-rx+k*i;//+E.x0;
				s=1;
			}else{
				x=3*rx-k*i;//+E.x0;
				s=-1;
			}
			An=1-sqr((x/*-E.x0*/)/rx);
			if (An>=0)
				y=s*ry*sqrt(An);//+E.y0;
			else
				y=0;//E.y0;
			EnvelopeSeries->AddXY(x+Ex.x0,y+Ey.x0);
		}
	   /*	double R=1000*Hcore*Solver->GetStructureParameter(j,RB_PAR);
		for (int i=0;i<PointsNumber;i++){
			k=2.0*R/(PointsNumber/2-1);
			if (i<PointsNumber/2){
				x=-R+k*i;
				s=1;
			}else{
				x=3*R-k*i;
				s=-1;
			}
			An=1-sqr((x)/R);
			if (An>=0)
				y=s*R*sqrt(An);
			else
				y=0;
			EnvelopeSeries->AddXY(x,y);
		}                                  */
	}

	if (ChartCheck->Checked)
		DrawSpace(j,X_PAR,Y_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawTransSpace()
{
    gType=TRANS_SPACE;
	EnvelopeActive();
	int j=PositionTrackBar->Position;
	TBeamParameter P1,P2;
	AnsiString Xtit,Ytit;
	bool Envelope=EnvelopeCheck->Checked;

	BeamChart->Title->Caption="Transverse Phase Space";// at position z="+s.FormatFloat("#0.000",100*z)+" cm";
	switch (RadiusGroup->ItemIndex) {
		case r4D_coord:{Envelope=false;}
		case r_coord:{
			P1=R_PAR;
			P2=AR_PAR;
			Xtit="r,mm";
			Ytit="r',mrad";
			break;
		}

		case x_coord:{
			P1=X_PAR;
			P2=AX_PAR;
			Xtit="x,mm";
			Ytit="x',mrad";
			break;
		}
		case y_coord:{
			P1=Y_PAR;
			P2=AY_PAR;
			Xtit="y,mm";
			Ytit="y',mrad";
			break;
		}
		case th_coord:{
			Envelope=false;
			P1=R_PAR;
			P2=ATH_PAR;
			Xtit="r,mm";
			Ytit="th'*r,mrad";
			break;
		}
	}
	BeamChart->BottomAxis->Title->Caption=Xtit;
	BeamChart->LeftAxis->Title->Caption=Ytit;
   /*   BeamChart->LeftAxis->Automatic=true;
	BeamChart->BottomAxis->Automatic=true; */
	ResetAxis(BeamChart->LeftAxis,-1000*Xmax,1000*Xmax,VertFitCheck->Checked);
	ResetAxis(BeamChart->BottomAxis,-1000*Rmax,1000*Rmax,HorFitCheck->Checked);

/*	BeamChart->LeftAxis->Automatic=VertFitCheck->Checked;
    BeamChart->BottomAxis->Automatic=HorFitCheck->Checked;
	BeamChart->LeftAxis->Minimum=-1000*;
	BeamChart->LeftAxis->Maximum=1000*Xmax;
	BeamChart->BottomAxis->Minimum=-1000*Rmax;
    BeamChart->BottomAxis->Maximum=1000*Rmax;
	//BeamChart->SeriesUp(EnvelopeSeries);      */

   //   sqr(A[j]*Wo/ELPi[j])
	double k=0,x=0,y=0,An=0,x1=0,y1=0,s=0;
	TEllipse E;

	if (Envelope){
		E=Solver->GetEllipse(j,P1);
		E.x0*=1e3;
		E.y0*=1e3;
		E.ax*=1e3*Hcore;
		E.by*=1e3*Hcore;
		for (int i=0;i<PointsNumber;i++){
			k=2.0*E.ax/(PointsNumber/2-1);
			if (i<PointsNumber/2){
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
			EnvelopeSeries->AddXY(x1+E.x0,y1+E.y0);
        }
    }

	if (ChartCheck->Checked)
		DrawSpace(j,P1,P2);
}
//---------------------------------------------------------------------------
void TResForm::DrawLongtMotion()
{
	gType=LONGT_MOTION;
	EnvelopeActive();
	int j=PositionTrackBar->Position;
	AnsiString Ytit;
	TBeamParameter P;

	double ksi=Solver->GetStructureParameter(j,KSI_PAR);
	double zmin=100*Solver->GetStructureParameter(0,Z_PAR);
	double zmax=100*Solver->GetStructureParameter(Npts-1,Z_PAR);

	BeamChart->Title->Caption="Longitudinal Motion";// at position z="+s.FormatFloat("#0.000",z)+" cm";
	BeamChart->BottomAxis->Title->Caption="z,cm";

	switch (RadiusGroup->ItemIndex) {
		case x_coord:{
			P=X_PAR;
			Ytit="x,mm";
			break;
		}
		case y_coord:{
			P=Y_PAR;
			Ytit="y,mm";
			break;
		}
		case r_coord:{ }
		default :{
			P=R_PAR;
			Ytit="r,mm";
			break;
		}

	}
	BeamChart->LeftAxis->Title->Caption=Ytit;
	ResetAxis(BeamChart->LeftAxis,-1000*Rmax,1000*Rmax,VertFitCheck->Checked);
	ResetAxis(BeamChart->BottomAxis,zmin,zmax,HorFitCheck->Checked);


   /*	BeamChart->LeftAxis->Automatic=VertFitCheck->Checked;
	BeamChart->BottomAxis->Automatic=HorFitCheck->Checked;
	BeamChart->LeftAxis->Minimum=-1000*Rmax;
	BeamChart->LeftAxis->Maximum=1000*Rmax;
	BeamChart->BottomAxis->Minimum=100*Solver->GetStructureParameter(0,Z_PAR);
	BeamChart->BottomAxis->Maximum=100*Solver->GetStructureParameter(Npts-1,Z_PAR);*/

	if (EnvelopeCheck->Checked){
		for (int i=0;i<Npts;i++){
			double ksi0=Solver->GetStructureParameter(i,KSI_PAR);
			double z0=Solver->GetStructureParameter(i,Z_PAR);
			double phi=(ksi-ksi0)*2*pi;
			if (phi>=DegreeToRad(MinPhase) && phi<=DegreeToRad(MaxPhase))
				EnvelopeSeries->AddXY(100*z0,250*Rmax*cos(phi));
		}
	}
  /*			for (int i=0;i<Npts;i++){
			double z0=100*Solver->Structure[i].ksi*Solver->Structure[i].lmb;
			double phi=(z-z0)*2*pi/(100*Solver->Structure[i].lmb);
			if (phi>=HellwegTypes::DegToRad(MinPhase) && phi<=HellwegTypes::DegToRad(MaxPhase))
				EnvelopeSeries->AddXY(z0,250*Rmax*cos(phi));
		}     */

	if (ChartCheck->Checked){
		DrawSpace(j,ZREL_PAR,P);
		/*for (int i=0;i<Np;i++){
			if (Solver->Beam[j]->Particle[i].lost==LIVE){
                double r=1e3*Solver->Beam[j]->Particle[i].r*Solver->Structure[j].lmb;
				double phi=Solver->Beam[j]->Particle[i].phi;
				double z0=z+1e2*Solver->Structure[j].lmb*phi/(2*pi);

                BeamSeries->AddXY(z0,r);
            }
		}    */
	}
}
//---------------------------------------------------------------------------
void TResForm::DrawRadialSpectrum()
{
	gType=R_SPEC;
	SpectrumActive();
	TBeamParameter P;
	AnsiString Xtit;

    int j=PositionTrackBar->Position;
	SpectrumChart->Title->Caption="Radial Spectrum";// at position z="+s.FormatFloat("#0.000",z)+" cm";

	switch (RadiusGroup->ItemIndex) {
		case r4D_coord:{}
		case r_coord:{
			P=R_PAR;
			Xtit="r,mm";
			break;
		}
		case x_coord:{
			P=X_PAR;
			Xtit="x,mm";
			break;
		}
		case y_coord:{
			P=Y_PAR;
			Xtit="y,mm";
			break;
		}
		case th_coord:{
			P=TH_PAR;
			Xtit="theta,deg";
			break;
		}
	}

	SpectrumChart->BottomAxis->Title->Caption=Xtit;
	SpectrumChart->LeftAxis->Title->Caption="N";
	ResetAxis(SpectrumChart->BottomAxis,-1000*Rmax,1000*Rmax,HorFitCheck->Checked);

	DrawSpectrum(j,P);
}
//---------------------------------------------------------------------------
void TResForm::DrawPhaseSpectrum()
{
    gType=F_SPEC;
    SpectrumActive();

    int j=PositionTrackBar->Position;
	SpectrumChart->Title->Caption="Phase Spectrum";// at position z="+s.FormatFloat("#0.000",z)+" cm";
	SpectrumChart->BottomAxis->Title->Caption="phi,deg";
	SpectrumChart->LeftAxis->Title->Caption="N";
	ResetAxis(SpectrumChart->BottomAxis,MinPhase,MaxPhase,HorFitCheck->Checked);

   	DrawSpectrum(j,PHI_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawEnergySpectrum()
{
    gType=W_SPEC;
    SpectrumActive();

	AnsiString s;
	int j=PositionTrackBar->Position;
   //	double z=100*Solver->GetStructureParameter(j,Z0_PAR);

	AnsiString EU=GetEnergyUnit(Solver->GetParticleType());

	SpectrumChart->Title->Caption="Energy Spectrum";// at position z="+s.FormatFloat("#0.000",z)+" cm";
	SpectrumChart->BottomAxis->Title->Caption="W, "+EU;
	SpectrumChart->LeftAxis->Title->Caption="N";

	//ResetAxis(BeamChart->LeftAxis,0,1,VertFitCheck->Checked);
	ResetAxis(BeamChart->BottomAxis,0,Wmax,HorFitCheck->Checked);

   /* SpectrumChart->LeftAxis->Automatic=VertFitCheck->Checked;
    SpectrumChart->BottomAxis->Automatic=HorFitCheck->Checked;
    SpectrumChart->LeftAxis->Minimum=0;
    SpectrumChart->LeftAxis->Maximum=1;
    SpectrumChart->BottomAxis->Minimum=0;
	SpectrumChart->BottomAxis->Maximum=Wmax; */
  //    SpectrumChart->LeftAxis->Grid->Hide();

	DrawSpectrum(j,W_PAR);

 /*   double X,dX;

    int Nmax=0;
    double Ymax=0;
    for (int i=0;i<Nb;i++){
		if (WSpectrum[i].N>Nmax)
            Nmax=WSpectrum[i].N;
        if (WSpectrum[i].y>Ymax)
            Ymax=WSpectrum[i].y;
    }

    SpectrumChart->LeftAxis->Maximum=Nmax;

    if (EnvelopeCheck->Checked){
        SpecEnvSeries->AddXY(0,0);
        SpecEnvSeries->AddXY(WSpectrum[0].x-1e-3,0);
        for (int i=0;i<Nb;i++)
			SpecEnvSeries->AddXY(WSpectrum[i].x,WSpectrum[i].y);
        SpecEnvSeries->AddXY(WSpectrum[Nb-1].x+1e-3,0);
        SpecEnvSeries->AddXY(Wmax,0);
    }

    if (ChartCheck->Checked){
        for (int i=0;i<Nb;i++)
			SpectrumSeries->AddXY(WSpectrum[i].x,1.0*WSpectrum[i].N/);

	}   */
    
}
//---------------------------------------------------------------------------
void __fastcall TResForm::EnergyButtonClick(TObject *Sender)
{
	PlayButton->Down=false;
   //	TrackBox->Enabled=false;
	DrawEnergy();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::PhaseButtonClick(TObject *Sender)
{
	PlayButton->Down=false;
	//TrackBox->Enabled=false;
    DrawPhase();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::RadiusButtonClick(TObject *Sender)
{
	PlayButton->Down=false;
   //	TrackBox->Enabled=false;
	DrawRadius();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::FieldButtonClick(TObject *Sender)
{
	PlayButton->Down=false;
	//TrackBox->Enabled=false;
    DrawField();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::PowerButtonClick(TObject *Sender)
{
	PlayButton->Down=false;
	//TrackBox->Enabled=false;
    DrawPower();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::BettaButtonClick(TObject *Sender)
{
	PlayButton->Down=false;
   //	TrackBox->Enabled=false;
	DrawBetta();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::AvEnergyButtonClick(TObject *Sender)
{
	PlayButton->Down=false;
  //	TrackBox->Enabled=false;
    DrawAvEnergy();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::AvRadiusButtonClick(TObject *Sender)
{
	PlayButton->Down=false;
   //	TrackBox->Enabled=false;
	DrawAvRadius();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::EmittanceButtonClick(TObject *Sender)
{
	PlayButton->Down=false;
	//TrackBox->Enabled=false;
	DrawEmittance();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::TransSectionButtonClick(TObject *Sender)
{
	TrackBox->Enabled=true;
	DrawTransSection();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::LongtSectonButtonClick(TObject *Sender)
{
	TrackBox->Enabled=true;
	DrawLongtSection();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::MotionButtonClick(TObject *Sender)
{
	TrackBox->Enabled=true;
	DrawLongtMotion();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::SlidingButtonClick(TObject *Sender)
{
	TrackBox->Enabled=true;
	DrawLongtSection(true);
}
//---------------------------------------------------------------------------
void __fastcall TResForm::TransverseButtonClick(TObject *Sender)
{
	TrackBox->Enabled=true;
	DrawTransSpace();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::LongtitudalButtonClick(TObject *Sender)
{
    TrackBox->Enabled=true;
	DrawLongtSpace();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::FSpectrumButtonClick(TObject *Sender)
{
	//gType=F_SPEC;
	DrawPhaseSpectrum();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::WSpectrumButtonClick(TObject *Sender)
{
	//gType=W_SPEC;
	DrawEnergySpectrum();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::RSpectrumButtonClick(TObject *Sender)
{
	DrawRadialSpectrum();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::LossButtonClick(TObject *Sender)
{
	BarsActive();

	int Nr=0,Nf=0,Nbz=0,Nbr=0,Nbth=0,Ns=0,Nb=0;

	for (int i=0;i<Npts;i++){
		Nr=0;Nf=0;Nbz=0;Nbr=0;Nbth=0;Ns=0;Nb=0;
		for (int j=0;j<Np;j++){
			switch (Solver->GetLossType(i,j)){
				case RADIUS_LOST: Nr++; break;
				case PHASE_LOST: Nf++; break;
				case BETA_LOST: Nb++; break;
				case BZ_LOST: Nbz++; break;
				case BR_LOST: Nbr++; break;
				case BTH_LOST: Nbth++; break;
				case STEP_LOST: Ns++; break;
			}
		}
		BarSeries1->AddXY(i,100.0*Nr/Np,"",clRed);
		BarSeries2->AddXY(i,100.0*Nf/Np,"",clBlue);
		BarSeries3->AddXY(i,100.0*Nb/Np,"",clGreen);
		BarSeries4->AddXY(i,100.0*Nbz/Np,"",clYellow);
		BarSeries5->AddXY(i,100.0*Nbr/Np,"",clPurple);
		BarSeries6->AddXY(i,100.0*Nbth/Np,"",clOrange);
		BarSeries7->AddXY(i,100.0*Ns/Np,"",clGray);
	}
}
//---------------------------------------------------------------------------

void __fastcall TResForm::LossPieButtonClick(TObject *Sender)
{
	PieActive();

		int Nr=0,Nf=0,Nbz=0,Nbr=0,Nbth=0,Ns=0,Nb=0,Nl=0;

        for (int j=0;j<Np;j++){
			switch (Solver->GetLossType(Npts-1,j)){
                case RADIUS_LOST: Nr++; break;
                case PHASE_LOST: Nf++; break;
				case BETA_LOST: Nb++; break;
				case BZ_LOST: Nbz++; break;
				case BR_LOST: Nbr++; break;
				case BTH_LOST: Nbth	++; break;
				case STEP_LOST: Ns++; break;
                case LIVE: Nl++; break;
            }
        }

    PieSeries->AddPie(Nr,"Radius Losses",clRed);
    PieSeries->AddPie(Nf,"Phase Losses",clBlue);
	PieSeries->AddPie(Nb,"Beta Losses",clGreen);
	PieSeries->AddPie(Nbz,"Bz Losses",clYellow);
	PieSeries->AddPie(Nbr,"Br Losses",clPurple);
	PieSeries->AddPie(Nbth,"Bth Losses",clOrange);
	PieSeries->AddPie(Ns,"Step Losses",clGray);
    PieSeries->AddPie(Nl,"Living",clOlive);
}
//---------------------------------------------------------------------------
void __fastcall TResForm::LossPlotButtonClick(TObject *Sender)
{
	MultLineActive();
	int Nr=0,Nf=0,Nbz=0,Nbr=0,Nbth=0,Ns=0,Nb=0,Ntot=0;

	for (int i=0;i<Npts;i++){
		Nr=0;Nf=0;Nbz=0;Nbr=0;Nbth=0;Ns=0;Nb=0;Ntot=0;
		for (int j=0;j<Np;j++){
			switch (Solver->GetLossType(i,j)){
				case RADIUS_LOST: Nr++; break;
				case PHASE_LOST: Nf++; break;
				case BETA_LOST: Nb++; break;
				case BZ_LOST: Nbz++; break;
				case BR_LOST: Nbr++; break;
				case BTH_LOST: Nbth++; break;
				case STEP_LOST: Ns++; break;
			}
		}
		Ntot=Solver->GetLivingNumber(i);

		MultSeries1->AddXY(i,100.0*Nr/Np,"",clRed);
		MultSeries2->AddXY(i,100.0*Nf/Np,"",clBlue);
		MultSeries3->AddXY(i,100.0*Nb/Np,"",clGreen);
		MultSeries4->AddXY(i,100.0*Nbz/Np,"",clYellow);
		MultSeries5->AddXY(i,100.0*Nbr/Np,"",clPurple);
		MultSeries6->AddXY(i,100.0*Nbth/Np,"",clOrange);
		MultSeries7->AddXY(i,100.0*Ns/Np,"",clGray);
		MultSeries8->AddXY(i,100.0*(1-1.0*Ntot/Np),"",clBlack);
	}
}
//---------------------------------------------------------------------------
void TResForm::ShowParameters()
{
	AnsiString s,Coord;
	int j=PositionTrackBar->Position;
	double z=Solver->GetStructureParameter(j,Z_PAR);
	bool FWHM=true;
	TBeamParameter Par;

	TGauss Gw=Solver->GetEnergyStats(j,FWHM);
	TGauss Gphi=Solver->GetPhaseStats(j);

	double Np=Solver->GetLivingNumber(j);
	double N0=Solver->GetLivingNumber(0);

	double Wm=Solver->GetMaxEnergy(j);
	double I=Solver->GetCurrent(j);

	double W0=Solver->GetParticleRestEnergy();

	double kc=100*Np/N0;

	//FSpectrum=Solver->GetPhaseSpectrum(j,true,F,dF);
	double f=Solver->GetFrequency(j);
	double Ra=Solver->GetStructureParameter(j,RA_PAR);
	//double Ra=1e3*Solver->Structure[j].Ra*Solver->Structure[j].lmb;
	double P=Solver->GetStructureParameter(j,PBEAM_PAR);
	double v=Solver->GetStructureParameter(j,SBETA_PAR);
	double E=Solver->GetStructureParameter(j,E0_PAR);

	TTwiss T;
	int k=1;
	switch (RadiusGroup->ItemIndex){
		case r_coord:{
			Par=R_PAR;
			Coord="R";
			k=2;
			break;
		}
		case x_coord:{
			Par=X_PAR;
			Coord="X";
			break;
		}
		case y_coord:{
			Par=Y_PAR;
			Coord="Y";
			break;
		}
		case r4D_coord:{
			Par=E4D_PAR;
			Coord="4D";
			break;
		}
		case th_coord:{
			Par=TH_PAR;
			Coord="TH";
			break;
		}
	}

	T=Solver->GetTwiss(j,Par);
	//double r=Solver->GetBeamRadius(j,Par);
    double r=k*sqrt(T.epsilon*T.beta);
	double beta_gamma=MeVToVelocity(Gw.mean,W0)*MeVToGamma(Gw.mean,W0);

	Table->Cells[colValue][pZ]=s.FormatFloat("#0.000",100*z);
	Table->Cells[colValue][pWav]=s.FormatFloat("#0.000",Gw.mean);
	Table->Cells[colValue][pdW]=s.FormatFloat("#0.00",Gw.sigma);
	if (Gw.mean!=0)
		Table->Cells[colValue][pdWp]=s.FormatFloat("#0.000",100*Gw.sigma/Gw.mean);
	else
		Table->Cells[colValue][pdWp]=0;
	Table->Cells[colValue][pWm]=s.FormatFloat("#0.000",Wm);
	Table->Cells[colValue][pI]=s.FormatFloat("#0.00",1e3*I);
    Table->Cells[colValue][pkc]=s.FormatFloat("#0.00",kc);
	Table->Cells[colValue][pr]=s.FormatFloat("#0.00",1e3*r);
	Table->Cells[colValue][pFav]=s.FormatFloat("#0.00",RadToDegree(Gphi.mean));
	Table->Cells[colValue][pdF]=s.FormatFloat("#0.00",RadToDegree(Gphi.sigma));
	Table->Cells[colValue][pF]=s.FormatFloat("#0.0",1e-6*f);
	Table->Cells[colValue][pRa]=s.FormatFloat("#0.00",1e3*Ra);
	Table->Cells[colValue][pPb]=s.FormatFloat("#0.0000",P);
	Table->Cells[colValue][pvph]=s.FormatFloat("#0.000",v);
	Table->Cells[colValue][pE]=s.FormatFloat("#0.00",1e-6*E);
	Table->Cells[colValue][pcoord]=Coord;
	Table->Cells[colValue][peps]=s.FormatFloat("#0.0000",1e6*T.epsilon);
	Table->Cells[colValue][pepsn]=s.FormatFloat("#0.0000",1e6*beta_gamma*T.epsilon);
	Table->Cells[colValue][palpha]=s.FormatFloat("#0.000",T.alpha);
	Table->Cells[colValue][pbetta]=s.FormatFloat("#0.000",100*T.beta);
}
//---------------------------------------------------------------------------
/*void TResForm::DeleteTemp()
{
    delete WSpectrum;
	delete FSpectrum;
}   */
//---------------------------------------------------------------------------
void TResForm::BinsChanged()
{
	AnsiString S;
	int Pos=BinsTrack->Position;
	double Nb=pow(10,1+Pos/6.0);
	int Order=1+Pos/6;
	double Nround=round(Nb/pow(10.,Order))*pow(10.,Order);
	NumBins=Nround;
	BinsLabel->Caption=S.FormatFloat("0",NumBins);
    Solver->SetBarsNumber(NumBins);
}
//---------------------------------------------------------------------------
void TResForm::AccuracyChanged()
{
	AnsiString S;
	int Pos=AccuracyTrack->Position;
	AccuracyLabel->Caption=S.FormatFloat("0%",100/(Pos+1));
	SparceFactor=Pos;
}
//---------------------------------------------------------------------------
void TResForm::KernelChanged()
{
	AnsiString S;
	int Pos=KernelTrack->Position;
	Kernel=5*Pos;
	if (Kernel==0)
		Kernel=1;
	if (Kernel==100)
		Kernel=99;

	KernelLabel->Caption=S.FormatFloat("0%",Kernel);

	Kernel/=100.0;
	Hcore=sqrt(-2*ln(1-Kernel));
}
//---------------------------------------------------------------------------
void TResForm::SynchChanged()
{
	AnsiString S;
	int Pos=SynchTrack->Position;
	SynchLabel->Caption=S.FormatFloat("0 deg",5*Pos);
	SynchPhase=5*Pos;
}
//---------------------------------------------------------------------------
void TResForm::PositionChanged()
{
	int j=PositionTrackBar->Position;
	double z=100*Solver->GetStructureParameter(j,Z_PAR);
	int N=Solver->GetStructureParameter(j,NUM_PAR);
	int Np=Solver->GetLivingNumber(j);
	AnsiString s,Z;

	CellLabel->Caption="Cell # "+IntToStr(N+1);
    Z=s.FormatFloat("#0.000",z);
	PositionLabel->Caption="z="+Z+" cm";

	ShowParameters();
	if (Np>0){
		switch (gType) {
			case TRANS_SEC: DrawTransSection();break;
			case LONGT_SEC: DrawLongtSection();break;
			case PHASE_SLID: DrawLongtSection(true);break;
			case LONGT_MOTION: DrawLongtMotion();break;
			case TRANS_SPACE: DrawTransSpace();break;
			case LONGT_SPACE: DrawLongtSpace();break;
			case F_SPEC: DrawPhaseSpectrum();break;
			case W_SPEC: DrawEnergySpectrum();break;
			case R_SPEC: DrawRadialSpectrum();break;
	  /*	case R_TRACE: DrawRadius();break;
		case PHI_TRACE: DrawPhase();break;
		case W_TRACE: DrawEnergy();break;   */
		}
	}

   // DeleteTemp();
}
//---------------------------------------------------------------------------
void TResForm::Draw()
{
	ShowParameters();
	switch (gType) {
		case TRANS_SEC: DrawTransSection();break;
		case LONGT_SEC: DrawLongtSection();break;
		case PHASE_SLID: DrawLongtSection(true);break;
		case LONGT_MOTION: DrawLongtMotion();break;
		case TRANS_SPACE: DrawTransSpace();break;
		case LONGT_SPACE: DrawLongtSpace();break;
		case F_SPEC: DrawPhaseSpectrum();break;
		case W_SPEC: DrawEnergySpectrum();break;
		case R_SPEC: DrawRadialSpectrum();break;
		case R_TRACE: DrawRadius();break;
		case PHI_TRACE: DrawPhase();break;
		case W_TRACE: DrawEnergy();break;
		case W_PLOT: DrawAvEnergy();break;
		case P_PLOT: DrawPower();break;
		case R_PLOT: DrawAvRadius();break;
		case EPS_PLOT: DrawEmittance();break;
		case E_PLOT: DrawField();break;
		case BETA_PLOT: DrawBetta();break;
	}
}
//---------------------------------------------------------------------------
void TResForm::IncrementPosition()
{
    if (PositionTrackBar->Position==PositionTrackBar->Max)
        PositionTrackBar->Position=PositionTrackBar->Min;
    else
        PositionTrackBar->Position++;
}
//---------------------------------------------------------------------------
void __fastcall TResForm::PositionTrackBarChange(TObject *Sender)
{
	PositionChanged();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::PlayButtonClick(TObject *Sender)
{
	if (PlayButton->Down)
        PlayButton->Caption="||";
    else
        PlayButton->Caption=">";

    while(PlayButton->Down){
        IncrementPosition();
        Application->ProcessMessages();
        Sleep(30);
    }
}
//---------------------------------------------------------------------------
void __fastcall TResForm::FFButtonClick(TObject *Sender)
{
    PositionTrackBar->Position=PositionTrackBar->Max;
}
//---------------------------------------------------------------------------
void __fastcall TResForm::RewButtonClick(TObject *Sender)
{
    PositionTrackBar->Position=PositionTrackBar->Min;
}
//---------------------------------------------------------------------------
int TResForm::NextEnd()
{
	int j=PositionTrackBar->Position;
	int N1=Solver->GetStructureParameter(j+1,NUM_PAR);
	//int N1=Solver->Structure[j+1].CellNumber;
    int Max=PositionTrackBar->Max;

    if (j==Max)
        return PositionTrackBar->Min;

    int k=Max;
	for (int i=j+1;i<Max-1;i++){
		int N2=Solver->GetStructureParameter(i,NUM_PAR);
		//int N2=Solver->Structure[i].CellNumber;
        if (N1<N2){
            k=i;
            break;
        }
    }
    return k;
}
//---------------------------------------------------------------------------
int TResForm::PrevEnd()
{
	int j=PositionTrackBar->Position;
	int N1=Solver->GetStructureParameter(j+-1,NUM_PAR);
	//int N1=Solver->Structure[j-1].CellNumber;
    int Min=PositionTrackBar->Min;

    if (j==Min)
        return PositionTrackBar->Max;

    int k=Min;
	for (int i=j-1;i>Min+1;i--){
		int N2=Solver->GetStructureParameter(i,NUM_PAR);
        //int N2=Solver->Structure[i].CellNumber;
        if (N1>N2){
            k=i;
            break;
        }
    }
    return k;
}
//---------------------------------------------------------------------------
void __fastcall TResForm::NextButtonClick(TObject *Sender)
{
    PositionTrackBar->Position=NextEnd();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::PrevButtonClick(TObject *Sender)
{
    PositionTrackBar->Position=PrevEnd();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    PlayButton->Down=false;
    if (SolverLoaded){
        delete Solver;
        SolverLoaded=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TResForm::TableDrawCell(TObject *Sender, int ACol, int ARow,
	  TRect &Rect, TGridDrawState State)
{
    if (ACol==colName){
        Table->Canvas->Brush->Color = cl3DLight;
        Table->Canvas->Font->Color=clBlack;
        Table->Canvas->FillRect(Rect);
        Table->Canvas->TextOut(Rect.Left,Rect.Top,Table->Cells[ACol][ARow]);
    }
}
//---------------------------------------------------------------------------
void __fastcall TResForm::EnvelopeCheckClick(TObject *Sender)
{
    PositionChanged();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::ChartCheckClick(TObject *Sender)
{
    PositionChanged();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::VertFitCheckClick(TObject *Sender)
{
    PositionChanged();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::HorFitCheckClick(TObject *Sender)
{
    PositionChanged();
}
//---------------------------------------------------------------------------

void __fastcall TResForm::Exit1Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TResForm::SaveAs1Click(TObject *Sender)
{
    AnsiString Fname;
    SaveDialog->Execute();
    Fname=SaveDialog->FileName;

    Solver->SaveToFile(Fname);
}
//---------------------------------------------------------------------------
void TResForm::LoadData()
{
    AnsiString Fname;
    OpenDialog->Execute();
    Fname=OpenDialog->FileName;

    Solver=new TBeamSolver;
    SolverLoaded=Solver->LoadFromFile(Fname);

    if (!SolverLoaded){
        delete[] Solver;
        ShowMessage("Error while reading the file");
        Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TResForm::OpenFile1Click(TObject *Sender)
{
    LoadData();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::RadiusGroupClick(TObject *Sender)
{
	Draw();
	//	PositionChanged();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::KernelTrackChange(TObject *Sender)
{
	KernelChanged();
	PositionChanged();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::AccuracyTrackChange(TObject *Sender)
{
	AccuracyChanged();
	PositionChanged();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::TransparentCheckClick(TObject *Sender)
{
	switch (gType) {
		case R_TRACE: DrawRadius();break;
		case PHI_TRACE: DrawPhase();break;
		case W_TRACE: DrawEnergy();break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TResForm::BinsTrackChange(TObject *Sender)
{
	BinsChanged();
	PositionChanged();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::SynchTrackChange(TObject *Sender)
{
	SynchChanged();
	PositionChanged();
}
//---------------------------------------------------------------------------

