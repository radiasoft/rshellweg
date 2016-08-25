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
        double Ra=Solver->Structure[i].Ra*Solver->Structure[i].lmb;
        double W=Solver->Beam[i]->GetMaxEnergy();
        if (Ra>Rmax)
            Rmax=Ra;
        if (W>Wmax)
            Wmax=W;
        for (int j=0;j<Np;j++){
            if (Solver->Beam[i]->Particle[j].lost==LIVE){
                double X=Solver->Beam[i]->Particle[j].Bx/Solver->Beam[i]->Particle[j].betta;
                if (mod(X)>Xmax)
                    Xmax=mod(X);
            }
        }
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
void TResForm::DrawEnergy()
{
    gType=F_NONE;
    PackActive();

    PackChart->Title->Caption="Particles Energy";
    PackChart->BottomAxis->Title->Caption="z,cm";
    PackChart->LeftAxis->Title->Caption="W,MeV";

    for (int i=0;i<Np;i++){
        PackSeries->AddNullXY(0,0);
        for (int j=0;j<Npts;j++){
            if (Solver->Beam[j]->Particle[i].lost!=LIVE)
                break;

            double z=100*Solver->Beam[j]->Particle[i].z;
            double W=VelocityToMeV(Solver->Beam[j]->Particle[i].betta);
            PackSeries->AddXY(z,W);
        }
    }
}
//---------------------------------------------------------------------------
void TResForm::DrawPhase()
{
    gType=F_NONE;
    PackActive();

    PackChart->Title->Caption="Particles Phase";
    PackChart->BottomAxis->Title->Caption="z,cm";
    PackChart->LeftAxis->Title->Caption="phi,deg";

    for (int i=0;i<Np;i++){
        PackSeries->AddNullXY(0,0);
        for (int j=0;j<Npts;j++){
            if (Solver->Beam[j]->Particle[i].lost!=LIVE)
                break;

            double z=100*Solver->Beam[j]->Particle[i].z;
            double phi=HellwegTypes::RadToDeg(Solver->Beam[j]->Particle[i].phi);
            PackSeries->AddXY(z,phi);
        }
    }
}
//---------------------------------------------------------------------------
void TResForm::DrawRadius()
{
    gType=F_NONE;
    PackActive();

    PackChart->Title->Caption="Particles Radius";
    PackChart->BottomAxis->Title->Caption="z,cm";
    PackChart->LeftAxis->Title->Caption="r,mm";

    for (int i=0;i<Np;i++){
        PackSeries->AddNullXY(0,0);
        for (int j=0;j<Npts;j++){
            if (Solver->Beam[j]->Particle[i].lost!=LIVE)
                break;

            double z=100*Solver->Beam[j]->Particle[i].z;
            double r=1e3*Solver->Beam[j]->Particle[i].x*Solver->Structure[j].lmb;
            PackSeries->AddXY(z,r);
        }
    }
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

    for (int j=0;j<Npts;j++){
        double z=100*Solver->Structure[j].ksi*Solver->Structure[j].lmb;
  //       double z=Solver->Structure[j].ksi;
        double E=1e-6*Solver->Structure[j].A*We0/Solver->Structure[j].lmb;
        double P=Solver->Structure[j].P>0;
        P=P>0?P:0;
        double L=1e-6*sqrt(2*Solver->Structure[j].Rp)*sqrt(P)/Solver->Structure[j].lmb;
        LineSeries->AddXY(z,E);
        AddSeries->AddXY(z,L);
    }
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

   //   sqr(A[j]*Wo/ELPi[j])

    for (int j=0;j<Npts;j++){
        double z=100*Solver->Structure[j].ksi*Solver->Structure[j].lmb;
        double E=sqrt(2*Solver->Structure[j].Rp);
        double P=E!=0?1e-6*sqr(Solver->Structure[j].A*We0/E):0;
        double W=Solver->Beam[j]->GetAverageEnergy();
        double I=Solver->Beam[j]->Ib;
        LineSeries->AddXY(z,P);
        AddSeries->AddXY(z,W*I);
    }
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

    //  sqr(A[j]*Wo/ELPi[j])

    for (int j=0;j<Npts;j++){
        double z=100*Solver->Structure[j].ksi*Solver->Structure[j].lmb;
 //   double z=Solver->Structure[j].ksi;
        double betta=Solver->Structure[j].betta;
        double b=MeVToVelocity(Solver->Beam[j]->GetAverageEnergy());
        LineSeries->AddXY(z,betta);
        AddSeries->AddXY(z,b);
    }
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

   //   sqr(A[j]*Wo/ELPi[j])

    for (int j=0;j<Npts;j++){
        double z=100*Solver->Structure[j].ksi*Solver->Structure[j].lmb;
        double W0=Solver->Beam[j]->GetAverageEnergy();
        double Wm=Solver->Beam[j]->GetMaxEnergy();
        LineSeries->AddXY(z,W0);
        AddSeries->AddXY(z,Wm);
    }
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

   //   sqr(A[j]*Wo/ELPi[j])

    for (int j=0;j<Npts;j++){
        double z=100*Solver->Structure[j].ksi*Solver->Structure[j].lmb;
        double R0=100*Solver->Beam[j]->GetBeamRadius();
        double Ra=100*Solver->Structure[j].Ra*Solver->Structure[j].lmb;
        LineSeries->AddXY(z,R0);
        AddSeries->AddXY(z,Ra);
    }
}
//---------------------------------------------------------------------------
void TResForm::DrawLongtSection(bool sliding)
{
    gType=sliding?PHASE_SLID:LONGT_SEC;
    EnvelopeActive();

    AnsiString s;
    int j=PositionTrackBar->Position;
    double z=100*Solver->Structure[j].ksi*Solver->Structure[j].lmb;

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

    if (ChartCheck->Checked){
        for (int i=0;i<Np;i++){
            if (Solver->Beam[j]->Particle[i].lost==LIVE){
                double r=0;
                double phi=HellwegTypes::RadToDeg(Solver->Beam[j]->Particle[i].phi);

                if (sliding)
                    r=250*Rmax*cos(HellwegTypes::DegToRad(phi));
                else
                    r=1e3*Solver->Beam[j]->Particle[i].x*Solver->Structure[j].lmb;

                BeamSeries->AddXY(phi,r);
            }
        }
    }
}
//---------------------------------------------------------------------------
void TResForm::DrawLongtSpace()
{
    gType=LONGT_SPACE;
    //ParticlesActive();
    EnvelopeActive();

    AnsiString s;
    int j=PositionTrackBar->Position;
    double z=100*Solver->Structure[j].ksi*Solver->Structure[j].lmb;


    BeamChart->Title->Caption="Longitudinal Phase Space at position z="+s.FormatFloat("#0.000",z)+" cm";
    BeamChart->BottomAxis->Title->Caption="phi,deg";
    BeamChart->LeftAxis->Title->Caption="W,MeV";
    BeamChart->LeftAxis->Automatic=false;//VertFitCheck->Checked;
    BeamChart->BottomAxis->Automatic=HorFitCheck->Checked;
    BeamChart->LeftAxis->Minimum=0;

    if (VertFitCheck->Checked)
        Wmax=Solver->Beam[j]->GetMaxEnergy();

    BeamChart->LeftAxis->Maximum=Wmax;
    Solver->Beam[j]->GetMaxEnergy();
    BeamChart->BottomAxis->Minimum=MinPhase;
    BeamChart->BottomAxis->Maximum=MaxPhase;

    if (EnvelopeCheck->Checked){
        double H=0,W=0, gamma=0, phi=0;
        double beta_w=Solver->Structure[j].betta;
        double A=Solver->Structure[j].A;
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
                H=Hmin+k*(Hmax-Hmin)/(SeparatrixNumber-1);
                sep=true;

                    for (int i=0;i<4*PointsNumber;i++){
                    phi=MinPhase+i*(MaxPhase-MinPhase)/(PointsNumber-1);
                    Nroots=n==0?GetPositiveSeparatrix(gamma,phi,beta_w,A,H):GetNegativeSeparatrix(gamma,phi,beta_w,A,H);
                    if ((n==0 && Nroots>0) || (n==1 & Nroots==2)){
                        W=GammaToMeV(gamma);
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

    if (ChartCheck->Checked){
        for (int i=0;i<Np;i++){
            if (Solver->Beam[j]->Particle[i].lost==LIVE){
                double W=VelocityToMeV(Solver->Beam[j]->Particle[i].betta);
                double phi=HellwegTypes::RadToDeg(Solver->Beam[j]->Particle[i].phi);
                BeamSeries->AddXY(phi,W);
            }
        }
    }
}
//---------------------------------------------------------------------------
void TResForm::DrawTransSection()
{
    gType=TRANS_SEC;
    EnvelopeActive();

    AnsiString s;
    int j=PositionTrackBar->Position;
    double z=100*Solver->Structure[j].ksi*Solver->Structure[j].lmb;

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
        double R=1000*Solver->Beam[j]->GetBeamRadius();
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

    if (ChartCheck->Checked){
        for (int i=0;i<Np;i++){
            if (Solver->Beam[j]->Particle[i].lost==LIVE){
                double r=1e3*Solver->Beam[j]->Particle[i].x*Solver->Structure[j].lmb;
                if (mod(r)>1e3*Rmax)
                    continue;
                double th=Solver->Beam[j]->Particle[i].Th;

                double x=r*cos(th);
                double y=r*sin(th);

                BeamSeries->AddXY(x,y);
            }
        }
    }
}
//---------------------------------------------------------------------------
void TResForm::DrawTransSpace()
{
    gType=TRANS_SPACE;
    EnvelopeActive();

    AnsiString s;
    int j=PositionTrackBar->Position;
    double z=100*Solver->Structure[j].ksi*Solver->Structure[j].lmb;

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
    double x0=0,y0=0,a=0,b=0,phi=0,Rx=0,Ry=0,k=0,x=0,y=0,An=0,x1=0,y1=0;
    

    if (EnvelopeCheck->Checked){
        Solver->GetEllipticParameters(j,x0,y0,a,b,phi,Rx,Ry);
        double h=Solver->GetKernel();
        x0*=1e3;
        y0*=1e3;
        a*=1e3*h;
        b*=1e3*h;
        for (int i=0;i<PointsNumber;i++){
            k=2.0*a/(PointsNumber/2-1);
            if (i<PointsNumber/2){
                x=-a+k*i+x0;
                s=1;
            }else{
                x=3*a-k*i+x0;
                s=-1;
            }
            An=1-sqr((x-x0)/a);
            if (An>=0)
                y=s*b*sqrt(An)+y0;
            else
                y=y0;
            x1=x*cos(phi)-y*sin(phi);
            y1=x*sin(phi)+y*cos(phi);
            EnvelopeSeries->AddXY(x1,y1);
        }
    }

    if (ChartCheck->Checked){
        for (int i=0;i<Np;i++){
            if (Solver->Beam[j]->Particle[i].lost==LIVE){
                double r=1e3*Solver->Beam[j]->Particle[i].x*Solver->Structure[j].lmb;
                double px=1e3*Solver->Beam[j]->Particle[i].Bx/Solver->Beam[j]->Particle[i].betta;

                BeamSeries->AddXY(r,px);
            }
        }
    }
}
//---------------------------------------------------------------------------
void TResForm::DrawLongtMotion()
{
    gType=LONGT_MOTION;
    EnvelopeActive();

    AnsiString s;
    int j=PositionTrackBar->Position;
    double z=100*Solver->Structure[j].ksi*Solver->Structure[j].lmb;

    BeamChart->Title->Caption="Longitudinal Motion at position z="+s.FormatFloat("#0.000",z)+" cm";
    BeamChart->BottomAxis->Title->Caption="z,cm";
    BeamChart->LeftAxis->Title->Caption="r,mm";
    BeamChart->LeftAxis->Automatic=VertFitCheck->Checked;
    BeamChart->BottomAxis->Automatic=HorFitCheck->Checked;
    BeamChart->LeftAxis->Minimum=-1000*Rmax;
    BeamChart->LeftAxis->Maximum=1000*Rmax;
    BeamChart->BottomAxis->Minimum=100*Solver->Structure[0].ksi*Solver->Structure[0].lmb;
    BeamChart->BottomAxis->Maximum=100*Solver->Structure[Npts-1].ksi*Solver->Structure[Npts-1].lmb;

    if (EnvelopeCheck->Checked){
        for (int i=0;i<Npts;i++){
            double z0=100*Solver->Structure[i].ksi*Solver->Structure[i].lmb;
            double phi=(z-z0)*2*pi/(100*Solver->Structure[i].lmb);
            if (phi>=HellwegTypes::DegToRad(MinPhase) && phi<=HellwegTypes::DegToRad(MaxPhase))
                EnvelopeSeries->AddXY(z0,250*Rmax*cos(phi));
        }
    }

    if (ChartCheck->Checked){
        for (int i=0;i<Np;i++){
            if (Solver->Beam[j]->Particle[i].lost==LIVE){
                double r=1e3*Solver->Beam[j]->Particle[i].x*Solver->Structure[j].lmb;
                double phi=Solver->Beam[j]->Particle[i].phi;
                double z0=z+1e2*Solver->Structure[j].lmb*phi/(2*pi);

                BeamSeries->AddXY(z0,r);
            }
        }
    }
}
//---------------------------------------------------------------------------
void TResForm::DrawPhaseSpectrum()
{
    gType=F_SPEC;
    SpectrumActive();

    AnsiString s;
    int j=PositionTrackBar->Position;
    double z=100*Solver->Structure[j].ksi*Solver->Structure[j].lmb;

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
    double z=100*Solver->Structure[j].ksi*Solver->Structure[j].lmb;

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
            switch (Solver->Beam[i]->Particle[j].lost){
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
            switch (Solver->Beam[Npts-1]->Particle[j].lost){
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
    double z=100*Solver->Structure[j].ksi*Solver->Structure[j].lmb;

    double Ws=0;
    WSpectrum=Solver->GetEnergySpectrum(j,true,W,Ws);
    if (W!=0)
        dW=100*Ws/W;
    else
        dW=100;

    double Np=Solver->Beam[j]->GetLivingNumber();
    double N0=Solver->Beam[0]->GetLivingNumber();

    double Wm=Solver->Beam[j]->GetMaxEnergy();
    double I=Solver->Beam[j]->Ib;
    
    double kc=100*Np/N0;
    double r=1e3*Solver->Beam[j]->GetBeamRadius();

    FSpectrum=Solver->GetPhaseSpectrum(j,true,F,dF);
    double f=1e-6*c/Solver->Structure[j].lmb;
    double Ra=1e3*Solver->Structure[j].Ra*Solver->Structure[j].lmb;
    double P=W*I;
    double v=Solver->Structure[j].betta;
    double E=1e-6*Solver->Structure[j].A*We0/Solver->Structure[j].lmb;

    double alpha=0,betta=0,eps=0;
    Solver->GetCourantSneider(j,alpha,betta,eps);

    Table->Cells[colValue][pZ]=s.FormatFloat("#0.000",z);
    Table->Cells[colValue][pWav]=s.FormatFloat("#0.000",W);
    Table->Cells[colValue][pdW]=s.FormatFloat("#0.00",dW);
    Table->Cells[colValue][pWm]=s.FormatFloat("#0.000",Wm);
    Table->Cells[colValue][pI]=s.FormatFloat("#0.00",1e3*I);
    Table->Cells[colValue][pkc]=s.FormatFloat("#0.00",kc);
    Table->Cells[colValue][pr]=s.FormatFloat("#0.00",r);
    Table->Cells[colValue][pFav]=s.FormatFloat("#0.00",F);
    Table->Cells[colValue][pdF]=s.FormatFloat("#0.00",dF);
    Table->Cells[colValue][pF]=s.FormatFloat("#0.0",f);
    Table->Cells[colValue][pRa]=s.FormatFloat("#0.00",Ra);
    Table->Cells[colValue][pPb]=s.FormatFloat("#0.0000",P);
    Table->Cells[colValue][pvph]=s.FormatFloat("#0.000",v);
    Table->Cells[colValue][pE]=s.FormatFloat("#0.00",E);
    Table->Cells[colValue][peps]=s.FormatFloat("#0.0000",100*eps);
    Table->Cells[colValue][palpha]=s.FormatFloat("#0.000",alpha);
    Table->Cells[colValue][pbetta]=s.FormatFloat("#0.000",100*betta);
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
    double z=100*Solver->Structure[j].ksi*Solver->Structure[j].lmb;
    int N=Solver->Structure[j].CellNumber;
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
    int N1=Solver->Structure[j+1].CellNumber;
    int Max=PositionTrackBar->Max;

    if (j==Max)
        return PositionTrackBar->Min;

    int k=Max;
    for (int i=j+1;i<Max-1;i++){
        int N2=Solver->Structure[i].CellNumber;
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
    int N1=Solver->Structure[j-1].CellNumber;
    int Min=PositionTrackBar->Min;

    if (j==Min)
        return PositionTrackBar->Max;

    int k=Min;
    for (int i=j-1;i>Min+1;i--){
        int N2=Solver->Structure[i].CellNumber;
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

