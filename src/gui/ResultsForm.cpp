//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ResultsForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TResForm *ResForm;

using namespace HellwegTypes;
//---------------------------------------------------------------------------
__fastcall TResForm::TResForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TResForm::FormCreate(TObject *Sender)
{
    SolverLoaded=false;

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
    Nb=Solver->GetNumberOfBars();

    PositionTrackBar->Max=Npts-1;
    PositionTrackBar->Position=PositionTrackBar->Max;
    PositionChanged();

    GetTransBoundaries();
    CreateTable();
    DrawField();
    

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
    Table->RowCount=Npar+1;
    Table->ColWidths[colName]=120;
    Table->ColWidths[colValue]=50;

    Table->Cells[colName][pZ]="Position, cm";
    Table->Cells[colName][pWav]="Average Energy, MeV";
    Table->Cells[colName][pWm]="Maximum Energy, MeV";
    Table->Cells[colName][pI]="Beam Current, mA";
    Table->Cells[colName][pkc]="Capture, %";
    Table->Cells[colName][pr]="Beam Radius, mm";
    Table->Cells[colName][pFav]="Average Phase, deg";
    Table->Cells[colName][pdF]="Phase Length, deg";
    Table->Cells[colName][pF]="Frequency, MHz";
    Table->Cells[colName][pRa]="Aperture, mm";
    Table->Cells[colName][pPb]="Beam Power, MW";
    Table->Cells[colName][pvph]="Phase Velocity";
    Table->Cells[colName][peps]="Emittance, cm*rad";
    Table->Cells[colName][palpha]="alpha";
    Table->Cells[colName][pbetta]="betta, cm/rad";
    Table->Cells[colName][pdW]="Energy Spectrum, %";
    Table->Cells[colName][pE]="Field Strength, MV/m";

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
}
//---------------------------------------------------------------------------
void TResForm::ChartActive()
{
    BarsChart->Visible=false;
    BeamChart->Visible=false;
    PieChart->Visible=false;
    SpectrumChart->Visible=false;

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
    EnvelopeSeries->Visible=true;
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
}
//---------------------------------------------------------------------------
void TResForm::LineActive()
{
    ChartActive();
    PackSeries->Visible=false;
    LineSeries->Visible=true;
    AddSeries->Visible=true;
    PackChart->Legend->Visible=true;
}
//---------------------------------------------------------------------------
void TResForm::BarsActive()
{
    PackChart->Visible=false;
    BeamChart->Visible=false;
    PieChart->Visible=false;
    SpectrumChart->Visible=false;

    BarsChart->Visible=true;

    BarSeries1->Clear();
    BarSeries2->Clear();
    BarSeries3->Clear();
    BarSeries4->Clear();
    BarSeries5->Clear();
    BarSeries6->Clear();
}
//---------------------------------------------------------------------------
void TResForm::PieActive()
{
    PackChart->Visible=false;
    BarsChart->Visible=false;
    BeamChart->Visible=false;
    SpectrumChart->Visible=false;

    PieChart->Visible=true;
    PieSeries->Clear();
}
//---------------------------------------------------------------------------
void TResForm::SpectrumActive()
{
    PackChart->Visible=false;
    BarsChart->Visible=false;
    BeamChart->Visible=false;
    PieChart->Visible=false;
    SpectrumChart->Visible=true;
    SpectrumSeries->Clear();
    SpecEnvSeries->Clear();
}
//---------------------------------------------------------------------------
void TResForm::DrawTrace(TBeamParameter P1)
{
	double z=0, y=0;
	TGraph *T=NULL;

	for (int i=0;i<Np;i++){
		PackSeries->AddNullXY(0,0);

		T=Solver->GetTrace(i,Z0_PAR,P1);

		for (int j=0;j<Npts;j++){
			if (!T[j].draw)
				break;

			z=T[j].x;
			y=T[j].y;

			//if (P1==Z0_PAR)
				z*=100;//cm

			switch (P1) {
				case PHI_PAR:{
					y=RadToDegree(y);
					break;
				}
				case R_PAR:{
					y=y*Solver->GetWavelength(j);
					break;
				}
			}
			PackSeries->AddXY(z,y);
		}
		delete[] T;
	}
}
//---------------------------------------------------------------------------
void TResForm::DrawSpace(int Nknot,TBeamParameter P1,TBeamParameter P2,bool sliding)
{
	TGraph *G=NULL;
	G=Solver->GetSpace(Nknot,P1,P2);

	double x=0,y=0,z=0;
	z=100*Solver->GetStructureParameter(Nknot,Z0_PAR);

	for (int i=0;i<Np;i++){
		if (G[i].draw){
			x=G[i].x;
			y=G[i].y;

			switch (P1) {
				case PHI_PAR: {x=RadToDegree(x);break;}
				case R_PAR: {}
				case X_PAR: {}
				case Y_PAR:{x=1000*x;break;}
				case Z0_PAR: {
					x=100*(z+x);
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
				case X_PAR: {}
				case Y_PAR:{x=1000*x;break;}
			}
		}
		BeamSeries->AddXY(x,y);
	}

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
			case PRF_PAR: {}
			case PBEAM_PAR: {}
			case ER_PAR: {}
			case E0_PAR:{
				x=1e-6*x;//MeV
				y=1e-6*y;
				break;
			}
			case RB_PAR: {}
			case RA_PAR: {
				x=100*x;//cm
				y=100*y;
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
void TResForm::DrawEnergy()
{
	gType=F_NONE;
    PackActive();

    PackChart->Title->Caption="Particles Energy";
    PackChart->BottomAxis->Title->Caption="z,cm";
	PackChart->LeftAxis->Title->Caption="W,MeV";

	DrawTrace(W_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawPhase()
{
    gType=F_NONE;
    PackActive();

    PackChart->Title->Caption="Particles Phase";
    PackChart->BottomAxis->Title->Caption="z,cm";
    PackChart->LeftAxis->Title->Caption="phi,deg";

	DrawTrace(PHI_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawRadius()
{
    gType=F_NONE;
    PackActive();

    PackChart->Title->Caption="Particles Radius";
    PackChart->BottomAxis->Title->Caption="z,cm";
    PackChart->LeftAxis->Title->Caption="r,mm";

	DrawTrace(R_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawField()
{
    gType=F_NONE;
    LineActive();

    PackChart->Title->Caption="Field Strength";
    PackChart->BottomAxis->Title->Caption="z,cm";
    PackChart->LeftAxis->Title->Caption="E,MV/m";
    PackSeries->Title="";
    LineSeries->Title="with beam load and losses";
    AddSeries->Title="no beam load and losses";

	DrawPlots(E0_PAR,ER_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawPower()
{
    gType=F_NONE;
    LineActive();

    PackChart->Title->Caption="Power";
    PackChart->BottomAxis->Title->Caption="z,cm";
	PackChart->LeftAxis->Title->Caption="P,MW";
	PackSeries->Title="";
	LineSeries->Title="Field Power";
	AddSeries->Title="Beam Power";

	DrawPlots(PRF_PAR,PBEAM_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawBetta()
{
    gType=F_NONE;
    LineActive();

    PackChart->Title->Caption="Velocity";
    PackChart->BottomAxis->Title->Caption="z,cm";
    PackChart->LeftAxis->Title->Caption="betta";

    LineSeries->Title="Phase Velocity";
    AddSeries->Title="Beam Velocity";

	DrawPlots(SBETA_PAR,BBETA_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawAvEnergy()
{
    gType=F_NONE;
    LineActive();

    PackChart->Title->Caption="Energy";
    PackChart->BottomAxis->Title->Caption="z,cm";
    PackChart->LeftAxis->Title->Caption="W,Mev";

    LineSeries->Title="Average Energy";
    AddSeries->Title="Maximum Energy";

	DrawPlots(WAV_PAR,WMAX_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawAvRadius()
{
    gType=F_NONE;
    LineActive();

    PackChart->Title->Caption="Radius";
    PackChart->BottomAxis->Title->Caption="z,cm";
    PackChart->LeftAxis->Title->Caption="r,cm";

    LineSeries->Title="Beam Radius (RMS)";
    AddSeries->Title="Aperture";

	DrawPlots(RB_PAR,RA_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawLongtSection(bool sliding)
{
    gType=sliding?PHASE_SLID:LONGT_SEC;
    EnvelopeActive();

    AnsiString s;
    int j=PositionTrackBar->Position;
	double z=100*Solver->GetStructureParameter(j,Z0_PAR);

    BeamChart->Title->Caption="Longitudinal Section at position z="+s.FormatFloat("#0.000",z)+" cm";
    BeamChart->BottomAxis->Title->Caption="phi,deg";
    BeamChart->LeftAxis->Title->Caption="r,mm";
    BeamChart->LeftAxis->Automatic=VertFitCheck->Checked;
    BeamChart->BottomAxis->Automatic=HorFitCheck->Checked;
    BeamChart->LeftAxis->Minimum=-1000*Rmax;
    BeamChart->LeftAxis->Maximum=1000*Rmax;
    BeamChart->BottomAxis->Minimum=MinPhase;
    BeamChart->BottomAxis->Maximum=MaxPhase;

	if (EnvelopeCheck->Checked){
        for (int i=0;i<PointsNumber;i++){
			double phi=MinPhase+i*(MaxPhase-MinPhase)/(PointsNumber-1);
			EnvelopeSeries->AddXY(phi,250*Rmax*cos(HellwegTypes::DegToRad(phi)));
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

    AnsiString s;
    int j=PositionTrackBar->Position;
	double z=100*Solver->GetStructureParameter(j,Z0_PAR);


    BeamChart->Title->Caption="Longitudinal Phase Space at position z="+s.FormatFloat("#0.000",z)+" cm";
    BeamChart->BottomAxis->Title->Caption="phi,deg";
    BeamChart->LeftAxis->Title->Caption="W,MeV";
    BeamChart->LeftAxis->Automatic=false;//VertFitCheck->Checked;
    BeamChart->BottomAxis->Automatic=HorFitCheck->Checked;
    BeamChart->LeftAxis->Minimum=0;

    if (VertFitCheck->Checked)
        Wmax=Solver->GetMaxEnergy(j);

	BeamChart->LeftAxis->Maximum=Wmax;

	BeamChart->BottomAxis->Minimum=MinPhase;
	BeamChart->BottomAxis->Maximum=MaxPhase;

	double Phi_min=RadToDegree(Solver->GetMinPhase(j));
	double Phi_max=RadToDegree(Solver->GetMaxPhase(j));

	if (EnvelopeCheck->Checked){
        double H=0,W=0, gamma=0, phi=0;
		double beta_w=Solver->GetStructureParameter(j,SBETA_PAR);
		double A=Solver->GetStructureParameter(j,A_PAR);
        bool sep=true;
        int Nroots=0;

        double Hmin=1e6, Hmax=0;

        for (int k=0; k < SeparatrixNumber; k++) {
			W=k*Wmax/(SeparatrixNumber-1);
			gamma=MeVToGamma(W);
		 // for (int i=0;i<PointsNumber;i++){
			   //   phi=MinPhase+i*(MaxPhase-MinPhase)/(PointsNumber-1);
			   phi=90;
				H=GetH(gamma,phi,beta_w,A);
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
					gamma=MeVToGamma(W);
					H=GetH(gamma,90,beta_w,A);
				} else
					H=Hmin+k*(Hmax-Hmin)/(SeparatrixNumber-1);

                sep=true;

                    for (int i=0;i<PointsNumber;i++){
					//phi=MinPhase+i*(MaxPhase-MinPhase)/(PointsNumber-1);
					phi=Phi_min+i*(Phi_max-Phi_min)/(PointsNumber-1);
					Nroots=n==0?GetPositiveSeparatrix(gamma,phi,beta_w,A,H):GetNegativeSeparatrix(gamma,phi,beta_w,A,H);
					if (((n==0 && Nroots>0) || (n==1 & Nroots==2)) && gamma>1){
						W=GammaToMeV(gamma);
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

    AnsiString s;
    int j=PositionTrackBar->Position;
	double z=100*Solver->GetStructureParameter(j,Z0_PAR);

    BeamChart->Title->Caption="Transverse Section at position z="+s.FormatFloat("#0.000",z)+" cm";
    BeamChart->BottomAxis->Title->Caption="x,mm";
    BeamChart->LeftAxis->Title->Caption="y,mm";
    BeamChart->LeftAxis->Automatic=VertFitCheck->Checked;
    BeamChart->BottomAxis->Automatic=HorFitCheck->Checked;
    BeamChart->LeftAxis->Minimum=-1000*Rmax;
    BeamChart->LeftAxis->Maximum=1000*Rmax;
    BeamChart->BottomAxis->Minimum=-1000*Rmax;
    BeamChart->BottomAxis->Maximum=1000*Rmax;

    double x=0,y=0,k=0,An=0;
    if (EnvelopeCheck->Checked){
		double h=Solver->GetKernel();
		double R=1000*Solver->GetStructureParameter(j,RB_PAR);
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
        }
    }

	if (ChartCheck->Checked)
		DrawSpace(j,X_PAR,Y_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawTransSpace()
{
    gType=TRANS_SPACE;
    EnvelopeActive();

    AnsiString s;
    int j=PositionTrackBar->Position;
    double z=100*Solver->GetStructureParameter(j,Z0_PAR);

    BeamChart->Title->Caption="Transverse Phase Space at position z="+s.FormatFloat("#0.000",z)+" cm";
    BeamChart->BottomAxis->Title->Caption="x,mm";
    BeamChart->LeftAxis->Title->Caption="x',mrad";
   /*   BeamChart->LeftAxis->Automatic=true;
    BeamChart->BottomAxis->Automatic=true; */
    BeamChart->LeftAxis->Automatic=VertFitCheck->Checked;
    BeamChart->BottomAxis->Automatic=HorFitCheck->Checked;
    BeamChart->LeftAxis->Minimum=-1000*Xmax;
    BeamChart->LeftAxis->Maximum=1000*Xmax;
    BeamChart->BottomAxis->Minimum=-1000*Rmax;
    BeamChart->BottomAxis->Maximum=1000*Rmax;
    //BeamChart->SeriesUp(EnvelopeSeries);

   //   sqr(A[j]*Wo/ELPi[j])
	double k=0,x=0,y=0,An=0,x1=0,y1=0;
	TEllipse E;

    if (EnvelopeCheck->Checked){
		E=Solver->GetEllipse(j);
        double h=Solver->GetKernel();
		E.x0*=1e3;
		E.y0*=1e3;
		E.ax*=1e3*h;
		E.by*=1e3*h;
		for (int i=0;i<PointsNumber;i++){
			k=2.0*E.ax/(PointsNumber/2-1);
			if (i<PointsNumber/2){
				x=-E.ax+k*i+E.x0;
				s=1;
			}else{
				x=3*E.ax-k*i+E.x0;
				s=-1;
			}
			An=1-sqr((x-E.x0)/E.ax);
			if (An>=0)
				y=s*E.by*sqrt(An)+E.y0;
			else
				y=E.y0;
			x1=x*cos(E.phi)-y*sin(E.phi);
			y1=x*sin(E.phi)+y*cos(E.phi);
            EnvelopeSeries->AddXY(x1,y1);
        }
    }

	if (ChartCheck->Checked)
		DrawSpace(j,R_PAR,AR_PAR);
}
//---------------------------------------------------------------------------
void TResForm::DrawLongtMotion()
{
    gType=LONGT_MOTION;
    EnvelopeActive();

    AnsiString s;
	int j=PositionTrackBar->Position;
	double z=100*Solver->GetStructureParameter(j,Z0_PAR);
	double ksi=100*Solver->GetStructureParameter(j,KSI_PAR);

    BeamChart->Title->Caption="Longitudinal Motion at position z="+s.FormatFloat("#0.000",z)+" cm";
    BeamChart->BottomAxis->Title->Caption="z,cm";
    BeamChart->LeftAxis->Title->Caption="r,mm";
    BeamChart->LeftAxis->Automatic=VertFitCheck->Checked;
    BeamChart->BottomAxis->Automatic=HorFitCheck->Checked;
    BeamChart->LeftAxis->Minimum=-1000*Rmax;
	BeamChart->LeftAxis->Maximum=1000*Rmax;
	BeamChart->BottomAxis->Minimum=100*Solver->GetStructureParameter(0,Z0_PAR);
	BeamChart->BottomAxis->Maximum=100*Solver->GetStructureParameter(Npts-1,Z0_PAR);

    if (EnvelopeCheck->Checked){
		for (int i=0;i<Npts;i++){
			double ksi0=Solver->GetStructureParameter(i,KSI_PAR);
			double z0=Solver->GetStructureParameter(i,Z0_PAR);
			double phi=(ksi-ksi0)*2*pi;
            if (phi>=DegreeToRad(MinPhase) && phi<=DegreeToRad(MaxPhase))
				EnvelopeSeries->AddXY(100*z0,250*Rmax*cos(phi));
        }
    }

	if (ChartCheck->Checked){
		DrawSpace(j,Z0_PAR,R_PAR);
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
void TResForm::DrawPhaseSpectrum()
{
    gType=F_SPEC;
    SpectrumActive();

    AnsiString s;
    int j=PositionTrackBar->Position;
	double z=100*Solver->GetStructureParameter(j,Z0_PAR);

    SpectrumChart->Title->Caption="Phase Spectrum at position z="+s.FormatFloat("#0.000",z)+" cm";
    SpectrumChart->BottomAxis->Title->Caption="phi,deg";
    SpectrumChart->LeftAxis->Title->Caption="N";
    SpectrumChart->LeftAxis->Automatic=VertFitCheck->Checked;
    SpectrumChart->BottomAxis->Automatic=HorFitCheck->Checked;
    SpectrumChart->LeftAxis->Minimum=0;
    SpectrumChart->LeftAxis->Maximum=1;
    SpectrumChart->BottomAxis->Minimum=MinPhase;
    SpectrumChart->BottomAxis->Maximum=MaxPhase;
   //   SpectrumChart->LeftAxis->Grid->Hide();

    int Nmax=0;
    double Ymax=0;
    for (int i=0;i<Nb;i++){
        if (FSpectrum[i].N>Nmax)
            Nmax=FSpectrum[i].N;
        if (FSpectrum[i].y>Ymax)
            Ymax=FSpectrum[i].y;
    }

    if (EnvelopeCheck->Checked){
        SpecEnvSeries->AddXY(MinPhase,0);
        SpecEnvSeries->AddXY(FSpectrum[0].x-1e-3,0);
        for (int i=0;i<Nb;i++)
            SpecEnvSeries->AddXY(FSpectrum[i].x,FSpectrum[i].y/Ymax);
        SpecEnvSeries->AddXY(FSpectrum[Nb-1].x+1e-3,0);
        SpecEnvSeries->AddXY(MaxPhase,0);
    }

    if (ChartCheck->Checked){
        for (int i=0;i<Nb;i++)
            SpectrumSeries->AddXY(FSpectrum[i].x,1.0*FSpectrum[i].N/Nmax);
    }
    
}
//---------------------------------------------------------------------------
void TResForm::DrawEnergySpectrum()
{
    gType=W_SPEC;
    SpectrumActive();

    AnsiString s;
    int j=PositionTrackBar->Position;
	double z=100*Solver->GetStructureParameter(j,Z0_PAR);

    SpectrumChart->Title->Caption="Energy Spectrum at position z="+s.FormatFloat("#0.000",z)+" cm";
    SpectrumChart->BottomAxis->Title->Caption="W,MeV";
    SpectrumChart->LeftAxis->Title->Caption="N";
    SpectrumChart->LeftAxis->Automatic=VertFitCheck->Checked;
    SpectrumChart->BottomAxis->Automatic=HorFitCheck->Checked;
    SpectrumChart->LeftAxis->Minimum=0;
    SpectrumChart->LeftAxis->Maximum=1;
    SpectrumChart->BottomAxis->Minimum=0;
    SpectrumChart->BottomAxis->Maximum=Wmax;
  //    SpectrumChart->LeftAxis->Grid->Hide();

    double X,dX;

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
            SpecEnvSeries->AddXY(WSpectrum[i].x,WSpectrum[i].y/*/Ymax*/);
        SpecEnvSeries->AddXY(WSpectrum[Nb-1].x+1e-3,0);
        SpecEnvSeries->AddXY(Wmax,0);
    }

    if (ChartCheck->Checked){
        for (int i=0;i<Nb;i++)
            SpectrumSeries->AddXY(WSpectrum[i].x,1.0*WSpectrum[i].N/*/Nmax*/);

    }
    
}
//---------------------------------------------------------------------------
void __fastcall TResForm::EnergyButtonClick(TObject *Sender)
{
    PlayButton->Down=false;
    DrawEnergy();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::PhaseButtonClick(TObject *Sender)
{
    PlayButton->Down=false;
    DrawPhase();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::RadiusButtonClick(TObject *Sender)
{
    PlayButton->Down=false;
    DrawRadius();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::FieldButtonClick(TObject *Sender)
{
    PlayButton->Down=false;
    DrawField();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::PowerButtonClick(TObject *Sender)
{
    PlayButton->Down=false;
    DrawPower();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::BettaButtonClick(TObject *Sender)
{
    PlayButton->Down=false;
    DrawBetta();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::AvEnergyButtonClick(TObject *Sender)
{
    PlayButton->Down=false;
    DrawAvEnergy();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::AvRadiusButtonClick(TObject *Sender)
{
    PlayButton->Down=false;
    DrawAvRadius();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::TransSectionButtonClick(TObject *Sender)
{
    DrawTransSection();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::LongtSectonButtonClick(TObject *Sender)
{
    DrawLongtSection();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::MotionButtonClick(TObject *Sender)
{
    DrawLongtMotion();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::SlidingButtonClick(TObject *Sender)
{
    DrawLongtSection(true);
}
//---------------------------------------------------------------------------
void __fastcall TResForm::TransverseButtonClick(TObject *Sender)
{
    DrawTransSpace();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::LongtitudalButtonClick(TObject *Sender)
{
    DrawLongtSpace();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::FSpectrumButtonClick(TObject *Sender)
{
    gType=F_SPEC;
    PositionChanged();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::WSpectrumButtonClick(TObject *Sender)
{
    gType=W_SPEC;
    PositionChanged();
}
//---------------------------------------------------------------------------
void __fastcall TResForm::LossButtonClick(TObject *Sender)
{
    BarsActive();

    int Nr=0,Nf=0,Nz=0,Nx=0,Ns=0,Nb=0;

    for (int i=0;i<Npts;i++){
        Nr=0;Nf=0;Nz=0;Nx=0;Ns=0;Nb=0;
        for (int j=0;j<Np;j++){
			switch (Solver->GetLossType(i,j)){
                case RADIUS_LOST: Nr++; break;
                case PHASE_LOST: Nf++; break;
                case B_LOST: Nb++; break;
                case BZ_LOST: Nz++; break;
                case BX_LOST: Nx++; break;
                case STEP_LOST: Ns++; break;
            }
        }
        BarSeries1->AddXY(i,100.0*Nr/Np,"",clRed);
        BarSeries2->AddXY(i,100.0*Nf/Np,"",clBlue);
        BarSeries3->AddXY(i,100.0*Nb/Np,"",clGreen);
        BarSeries4->AddXY(i,100.0*Nz/Np,"",clYellow);
        BarSeries5->AddXY(i,100.0*Nx/Np,"",clPurple);
        BarSeries6->AddXY(i,100.0*Ns/Np,"",clGray);
    }
}
//---------------------------------------------------------------------------

void __fastcall TResForm::LossPieButtonClick(TObject *Sender)
{
    PieActive();

        int Nr=0,Nf=0,Nz=0,Nx=0,Ns=0,Nb=0,Nl=0;

        for (int j=0;j<Np;j++){
			switch (Solver->GetLossType(Npts-1,j)){
                case RADIUS_LOST: Nr++; break;
                case PHASE_LOST: Nf++; break;
                case B_LOST: Nb++; break;
                case BZ_LOST: Nz++; break;
                case BX_LOST: Nx++; break;
                case STEP_LOST: Ns++; break;
                case LIVE: Nl++; break;
            }
        }

    PieSeries->AddPie(Nr,"Radius Losses",clRed);
    PieSeries->AddPie(Nf,"Phase Losses",clBlue);
    PieSeries->AddPie(Nb,"Betta Losses",clGreen);
    PieSeries->AddPie(Nz,"Bz Losses",clYellow);
    PieSeries->AddPie(Nx,"Bx Losses",clPurple);
    PieSeries->AddPie(Ns,"Step Losses",clGray);
    PieSeries->AddPie(Nl,"Living",clOlive);
}
//---------------------------------------------------------------------------
void TResForm::ShowParameters()
{
    AnsiString s;
	int j=PositionTrackBar->Position;
	double z=100*Solver->GetStructureParameter(j,Z0_PAR);

	TGauss Gw=Solver->GetEnergyStats(j);
	/*double Ws=0;
	WSpectrum=Solver->GetEnergySpectrum(j,true,W,Ws);
    if (W!=0)
        dW=100*Ws/W;
    else
		dW=100; */

	double Np=Solver->GetLivingNumber(j);
	double N0=Solver->GetLivingNumber(0);

	double Wm=Solver->GetMaxEnergy(j);
	double I=Solver->GetCurrent(j);

    double kc=100*Np/N0;
	double r=1e3*Solver->GetBeamRadius(j);

	TGauss Gphi=Solver->GetPhaseStats(j);
	//FSpectrum=Solver->GetPhaseSpectrum(j,true,F,dF);
	double f=1e-6*c/Solver->GetWavelength(j);
	double Ra=100*Solver->GetStructureParameter(j,RA_PAR);
	//double Ra=1e3*Solver->Structure[j].Ra*Solver->Structure[j].lmb;
	double P=Solver->GetStructureParameter(j,PBEAM_PAR);
	double v=Solver->GetStructureParameter(j,SBETA_PAR);
	double E=1e-6*Solver->GetStructureParameter(j,E0_PAR);

	TTwiss T;
	T=Solver->GetTwiss(j);

    Table->Cells[colValue][pZ]=s.FormatFloat("#0.000",z);
	Table->Cells[colValue][pWav]=s.FormatFloat("#0.000",Gw.mean);
	Table->Cells[colValue][pdW]=s.FormatFloat("#0.00",Gw.sigma);
    Table->Cells[colValue][pWm]=s.FormatFloat("#0.000",Wm);
	Table->Cells[colValue][pI]=s.FormatFloat("#0.00",1e3*I);
    Table->Cells[colValue][pkc]=s.FormatFloat("#0.00",kc);
    Table->Cells[colValue][pr]=s.FormatFloat("#0.00",r);
	Table->Cells[colValue][pFav]=s.FormatFloat("#0.00",Gphi.mean);
	Table->Cells[colValue][pdF]=s.FormatFloat("#0.00",Gphi.sigma);
    Table->Cells[colValue][pF]=s.FormatFloat("#0.0",f);
    Table->Cells[colValue][pRa]=s.FormatFloat("#0.00",Ra);
    Table->Cells[colValue][pPb]=s.FormatFloat("#0.0000",P);
    Table->Cells[colValue][pvph]=s.FormatFloat("#0.000",v);
    Table->Cells[colValue][pE]=s.FormatFloat("#0.00",E);
	Table->Cells[colValue][peps]=s.FormatFloat("#0.0000",100*T.epsilon);
	Table->Cells[colValue][palpha]=s.FormatFloat("#0.000",T.alpha);
    Table->Cells[colValue][pbetta]=s.FormatFloat("#0.000",100*T.beta);
}
//---------------------------------------------------------------------------
void TResForm::DeleteTemp()
{
    delete WSpectrum;
    delete FSpectrum;
}
//---------------------------------------------------------------------------
void TResForm::PositionChanged()
{
	int j=PositionTrackBar->Position;
	double z=100*Solver->GetStructureParameter(j,Z0_PAR);
	int N=Solver->GetStructureParameter(j,NUM_PAR);
    AnsiString s,Z;

    CellLabel->Caption="Cell # "+IntToStr(N+1);
    Z=s.FormatFloat("#0.000",z);
    PositionLabel->Caption="z="+Z+" cm";

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
    }

    DeleteTemp();
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

