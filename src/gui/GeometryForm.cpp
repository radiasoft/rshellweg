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
        case CH_EMITTANCE:{
            GChart->Title->Caption="Beam Emittance";
            GChart->BottomAxis->Title->Caption="x,mm";
            GChart->LeftAxis->Title->Caption="dx,mrad";
            break;
        };
        case CH_SECTION:{
            GChart->Title->Caption="Beam Section";
            GChart->BottomAxis->Title->Caption="x,mm";
            GChart->LeftAxis->Title->Caption="y,mm";
            break;
        };
        case CH_PORTRAIT:{
            GChart->Title->Caption="Beam Portrait";
            GChart->BottomAxis->Title->Caption="phi, deg";
            GChart->LeftAxis->Title->Caption="W, MeV";
            break;
        };
        case CH_ENERGY:{
            GChart->Title->Caption="Energy Spectrum";
            GChart->BottomAxis->Title->Caption="W,MeV";
            GChart->LeftAxis->Title->Caption="N";
            break;
        };
        case CH_PHASE:{
            GChart->Title->Caption="Phase Spectrum";
            GChart->BottomAxis->Title->Caption="phi, deg";
            GChart->LeftAxis->Title->Caption="N";
            break;
        };
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
    double alpha=0,betta=0,eps=0,Rx=0,Ry=0;
    double Wav=0,dW=0,Fav=0,dF=0;
    AnsiString s;

    aParLabel->Visible=true;
    bParLabel->Visible=true;
    eParLabel->Visible=true;
    WavParLabel->Visible=true;
    dWParLabel->Visible=true;
    FavParLabel->Visible=true;
    dFParLabel->Visible=true;

    MainSolver->GetCourantSneider(0,alpha,betta,eps);
    MainSolver->GetEnergySpectrum(0,Wav,dW);
    MainSolver->GetPhaseSpectrum(0,Fav,dF);

    aParLabel->Caption="alpha = "+s.FormatFloat("#0.000##",alpha);
    bParLabel->Caption="betta = "+s.FormatFloat("#0.000##",100*betta)+" cm/rad";
    eParLabel->Caption="epsilon = "+s.FormatFloat("#0.000##",100*eps)+" cm*rad";
    WavParLabel->Caption="Wav = "+s.FormatFloat("#0.000##",Wav)+" MeV";
    dWParLabel->Caption="dW = "+s.FormatFloat("#0.000##",dW)+" MeV";
    FavParLabel->Caption="Phi av = "+s.FormatFloat("#0.000##",Fav)+" deg";
    dFParLabel->Caption="dPhi = "+s.FormatFloat("#0.000##",dF)+" deg";
}
//---------------------------------------------------------------------------
void TGeomForm::DrawBeamEnvelope(TLineSeries *Series0,TBeamSolver *Solver,int env_type,TColor Col1)
{
    double a,b,x0=0,y0=0,x,y,x1,y1,k,phi;
    double alpha,betta,eps,s,An,Rx,Ry;
    int Nb=Solver->GetNumberOfBars();
    TSpectrumBar *Spectrum;
    double X,dX;
    #define Np 500

    Series0->Clear();

    if(BeamGroup->ItemIndex==emittance_chart){
        SignChart(CH_EMITTANCE);
        Solver->GetEllipticParameters(0,x0,y0,a,b,phi,Rx,Ry);
        //Solver->GetCourantSneider(0,alpha,betta,eps);
        x0*=1e3;
        y0*=1e3;
        a*=1e3*h;
        b*=1e3*h;

        if (env_type==ellipse_env){
            for (int i=0;i<Np;i++){
                k=2.0*a/(Np/2-1);
                if (i<Np/2){
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
                Series0->AddXY(x1,y1,"",Col1);
            }
        }
    }
    if(BeamGroup->ItemIndex==energy_chart || BeamGroup->ItemIndex==phase_chart){
        if(BeamGroup->ItemIndex==energy_chart){
            SignChart(CH_ENERGY);
            Spectrum=Solver->GetEnergySpectrum(0,true,X,dX);
        }else if(BeamGroup->ItemIndex==phase_chart){
            SignChart(CH_PHASE);
            Spectrum=Solver->GetPhaseSpectrum(0,true,X,dX);
        }

        for (int i=0;i<Nb;i++)
            Series0->AddXY(Spectrum[i].x,Spectrum[i].y,"",Col1);

        delete[] Spectrum;
    }


    #undef Np

}
//---------------------------------------------------------------------------
void TGeomForm::DrawBeam(TPointSeries *Series0,TBeamSolver *Solver,TColor Col1)
{
    using namespace HellwegTypes ;
    Series0->Clear();

    int Np=Solver->GetNumberOfParticles();
    double *X0,*Y0,*Z0,x,y;
    bool NoGraph=false;;

    X0= new double[Np];
    Y0= new double[Np];
    Z0= new double[Np];

    switch (BeamGroup->ItemIndex) {
        case (emittance_chart):{
            SignChart(CH_EMITTANCE);
            Solver->GetBeamParameters(0,X0,X_PAR);
            Solver->GetBeamParameters(0,Y0,BX_PAR);
            Solver->GetBeamParameters(0,Z0,BETTA_PAR);
            for (int i=0;i<Np;i++){
                double x=X0[i];
                double bx=Y0[i];
                double bz=Z0[i];
                X0[i]=1e3*x*Solver->Structure[0].lmb;
                //Y0[i]/=Z0[i];
                Y0[i]=1e3*bx/bz;
            }
            break;
        }
        case (portrait_chart):{
            SignChart(CH_PORTRAIT);
            Solver->GetBeamParameters(0,X0,PHI_PAR);
            Solver->GetBeamParameters(0,Y0,BETTA_PAR);
            for (int i=0;i<Np;i++){
                X0[i]=HellwegTypes::RadToDeg(X0[i]);
                Y0[i]=VelocityToMeV(Y0[i]);
            }  
            break;
        }
        case (section_chart):{
            SignChart(CH_SECTION);
            Solver->GetBeamParameters(0,X0,X_PAR);
            Solver->GetBeamParameters(0,Y0,TH_PAR);
            for (int i=0;i<Np;i++){
                x=1e3*X0[i]*cos(Y0[i])*Solver->Structure[0].lmb;
                y=1e3*X0[i]*sin(Y0[i])*Solver->Structure[0].lmb;
                X0[i]=x;
                Y0[i]=y;
            }
            break;
        }
        default:{
            //SignChart(CH_CLEAR);
            NoGraph=true;
            break;
        }
    }

    if (!NoGraph){
        for (int i=0;i<Np;i++)
            Series0->AddXY(X0[i],Y0[i],"",Col1);
    }

    delete[] X0;
    delete[] Y0;
    delete[] Z0;
}
//---------------------------------------------------------------------------
void TGeomForm::DrawBarChart(TBarSeries *Series0,TBeamSolver *Solver,TColor Col1)
{
    Series0->Clear();

    double x;
    TSpectrumBar *Spectrum;
    double X,dX;
    int Nb=Solver->GetNumberOfBars();
    bool NoGraph=false;


    if (BeamGroup->ItemIndex==energy_chart){
        SignChart(CH_ENERGY);
        Spectrum=Solver->GetEnergySpectrum(0,X,dX);
    }else if (BeamGroup->ItemIndex==phase_chart){
        SignChart(CH_PHASE);
        Spectrum=Solver->GetPhaseSpectrum(0,X,dX);
    }else{
        //SignChart(CH_CLEAR);
        return;
    }
 
    if (!NoGraph){
        for (int i=0;i<Nb;i++)
            Series0->AddXY(Spectrum[i].x,Spectrum[i].N);
            //EnvelopeSeries1->AddXY(SpectrumArray[i].x,SpectrumArray[i].y,clBlue);
    }
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

    Solver->GetStructureParameters(N0,NUM_PAR);
    
    switch (ChartGroup->ItemIndex) {
        case (betta_chart):{
            SignChart(CH_BETTA);
            Solver->GetStructureParameters(Y0,BETTA_F_PAR);
            break;
        }
        case (A_chart):{
            SignChart(CH_A);
            Solver->GetStructureParameters(Y0,A_PAR);
            break;
        }
        case (ELP_chart):{
            SignChart(CH_ELP);
            Solver->GetStructureParameters(Y0,RP_PAR);
            for (int i=0;i<Np;i++)
                Y0[i]=sqrt(2*Y0[i]);
            break;
        }
        case (B_chart):{
            SignChart(CH_B);
            Solver->GetStructureParameters(Y0,B_PAR);
            for (int i=0;i<Np;i++)
                Y0[i]=1e-6*Y0[i];
            break;
        }
        case (alpha_chart):{
            SignChart(CH_ATT);
            Solver->GetStructureParameters(Y0,ALPHA_PAR);
            break;
        }
        case (Bz_chart):{
            SignChart(CH_BEXT);
            Solver->GetStructureParameters(Y0,B_EXT_PAR);
            for (int i=0;i<Np;i++)
                Y0[i]=10000*Y0[i];
            break;
        }
        case (Ra_chart):{
            SignChart(CH_APP);
            Solver->GetStructureParameters(Y0,RA_PAR);
            for (int i=0;i<Np;i++)
                Y0[i]=100*Y0[i];
            break;
        }
    }
    
    Solver->GetStructureParameters(X0,Z_PAR);
    

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

 /* for (int i=0;i<Np/Nm;i++){
        Col=(i%2)?Col1:Col2;

        for (int j=0;j<Nm;j++)
            
        //fprintf(F,"%i %f %f\n",i,X0[i],Y0[i]);
        //Series0->Add(Y0[i],X0[i]);
        //Series0->Add(i,X0[i]);
        //Series0->Add(Y0[i],i);
        //TColor Col=(i%(2*Nm)==0)?Col1:Col2;
        
    }
    fclose(F);    */


  /*    for (int j=0;j<Np;j+=Nm){
        if (j%(2*Nm)==0)
            Series0->ColorRange(Series0->ValuesList->ValueList[0],j,j+Nm,Col1);
        else
            Series0->ColorRange(Series0->ValuesList->ValueList[0],j,j+Nm,Col2);
    }
          */
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
    
    h=MainSolver->GetKernel();
 // ChartSeries1->Visible=false;

    if (Beam){
        if (BeamBox->Checked)
            DrawBeam(BeamSeries1,MainSolver,Particle_col);
        if (EnvelopeCB->Checked)
            DrawBeamEnvelope(EnvelopeSeries1,MainSolver,ellipse_env,Envelope_col);
        SetParameters();
    }
    else{
        DrawChart(Series1,MainSolver,Line1_col_a,Line1_col_b);
        ClearParameters();
    }
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

    if (Err==ERR_NO){
        DrawChart(Series2,CompData1,Line2_col_a,Line2_col_b);
        C1=true;
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
}
//---------------------------------------------------------------------------

void __fastcall TGeomForm::FormDestroy(TObject *Sender)
{
    delete CompData1;   
}
//---------------------------------------------------------------------------

void __fastcall TGeomForm::Compare1Click(TObject *Sender)
{
    Series2->Visible=Compare1->Checked && !Beam;    
}
//---------------------------------------------------------------------------

void __fastcall TGeomForm::ChartGroupClick(TObject *Sender)
{
    DrawChart(Series1,MainSolver,Line1_col_a,Line1_col_b);
    if (C1)
        DrawChart(Series2,CompData1,Line2_col_a,Line2_col_b);
}
//---------------------------------------------------------------------------
void __fastcall TGeomForm::BeamGroupClick(TObject *Sender)
{
    try{
        if (EnvelopeCB->Checked)
            DrawBeamEnvelope(EnvelopeSeries1,MainSolver,ellipse_env,Envelope_col);
        else
            EnvelopeSeries1->Clear();

        if (BeamBox->Checked){
            DrawBeam(BeamSeries1,MainSolver,Particle_col);
            DrawBarChart(BarSeries1,MainSolver,clRed);
        }else{
            BeamSeries1->Clear();
            BarSeries1->Clear();
        }

    }
    catch (...){
        ShowMessage("There's an error while processing the data. May be some parameters are not correct.");
    }
}
//---------------------------------------------------------------------------




