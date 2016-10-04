//---------------------------------------------------------------------------


#pragma hdrstop
#include "SpectrumPhase.h"

//---------------------------------------------------------------------------
__fastcall TSpectrumPhase::TSpectrumPhase()
{
    Nparticle=1;
//    FILE *F;
//    F=fopen("yeDebug.log","a");
//    fprintf(F,"__factcall TSpectrumPhase: Nslices=%d\n",Nslices);
//    fclose(F);  
    MeshPhaseSet=false;
    SpectrumPhaseReady=false;
    AvPhaseReady=false;
    WidthPhaseReady=false;
    EnvelopePhaseReady=false;

    Mphase=0;
    Dphase=0;
    Sphase=0;

    SpectrumPhase=new TSpectrumBar[Nslices];

    //logfile=fopen("spectrum.log","w");
}
//---------------------------------------------------------------------------
__fastcall TSpectrumPhase::~TSpectrumPhase()
{
    delete[] SpectrumPhase;
    //fclose(logfile);
}
//---------------------------------------------------------------------------
void TSpectrumPhase::ResetPhaseStatistics()
{
    Mphase=0;
    Dphase=0;
    Sphase=0;
    AvPhaseReady=0;
    DispPhaseReady=0;
    SqrPhaseReady=0;
}
//---------------------------------------------------------------------------
void TSpectrumPhase::GetPhaseBoundaries()
{
    double phaseMin=1e32;
    double phaseMax=-1e32;
    double phase=0;

    for (int i=0;i<Nparticle;i++){
        phase=Phase[i];
        if (phase>phaseMax)
            phaseMax=phase;
        if (phase<phaseMin)
            phaseMin=phase;
    }

    PhaseMin=phaseMin;
    PhaseMax=phaseMax;
}
//---------------------------------------------------------------------------
void TSpectrumPhase::SetPhaseMesh(double *R0,double *X0,int Nb,int Ny)
{

    delete[] SpectrumPhase;
    Nslices=Nb;
//    FILE *Fout;
//    Fout=fopen("yeDebug.log","a");
//    fprintf(Fout,"            Nslices(SetPhaseMesh)=%d\n",Nslices);
//    fclose(Fout); 
    Nparticle=Ny;
    Phase=X0;
	Radius=R0;

    SpectrumPhase=new TSpectrumBar[Nslices];
    GetPhaseBoundaries();

    for (int i=0;i<Nslices;i++){
	    if (Nslices != 1) {
            SpectrumPhase[i].phase=PhaseMin+i*(PhaseMax-PhaseMin)/(Nslices-1);
	    } else {
            SpectrumPhase[i].phase=PhaseMin;
		}
        SpectrumPhase[i].N=0;
        SpectrumPhase[i].P=0;
        SpectrumPhase[i].xAv=0;
        SpectrumPhase[i].yAv=0;
        SpectrumPhase[i].xRMS=0;
        SpectrumPhase[i].yRMS=0;
    }

    MeshPhaseSet=true;
    MakePhaseSpectrum();
    ResetPhaseStatistics();
}
//---------------------------------------------------------------------------
void TSpectrumPhase::MakePhaseSpectrum()
{
    if (!MeshPhaseSet){
        SpectrumPhaseReady=false;
        return;
    }

    int j=0,Ny=Nparticle>0?Nparticle:1;
    double phase=0;
	double r=0;
	double xAvOld=0;
	double xRMSold=0;

    if (PhaseMax==PhaseMin){
        if (PhaseMax==0){
            PhaseMax=0.02;
        }else{
            PhaseMax*=1.01;
            PhaseMin*=0.99;
        }
    }

    for (int i=0;i<Nparticle;i++){
        phase=Phase[i];
		r=Radius[i];
		if (Nslices != 1) {
            j=round((phase-PhaseMin)*(Nslices-1)/(PhaseMax-PhaseMin));
            if (j>Nslices-1)
                j=Nslices-1;
            if (j<0)
                j=0;
	    } else {
		    j=0;
		}
        SpectrumPhase[j].N++;
		xAvOld=SpectrumPhase[j].xAv;
		xRMSold=SpectrumPhase[j].xRMS;
		SpectrumPhase[j].xAv=(xAvOld*(SpectrumPhase[j].N-1)+r)/SpectrumPhase[j].N;
		SpectrumPhase[j].xRMS=sqrt(((xRMSold*xRMSold+xAvOld*xAvOld)*(SpectrumPhase[j].N-1)+r*r-
		                            SpectrumPhase[j].xAv*SpectrumPhase[j].xAv*SpectrumPhase[j].N)/SpectrumPhase[j].N);
    }
    for (int i=0;i<Nslices;i++)
        SpectrumPhase[i].P=1.0*SpectrumPhase[i].N/Nparticle;

    SpectrumPhaseReady=true;

    //MakePhaseEnvelope();
    ResetPhaseStatistics();

}
//---------------------------------------------------------------------------
void TSpectrumPhase::MakePhaseEnvelope()
{
    if (!SpectrumPhaseReady)
        MakePhaseSpectrum();

    if (!EnvelopePhaseReady){
        for (int i=0;i<Nslices;i++)
            SpectrumPhase[i].y=SpectrumPhase[i].N;

        double *X_base=NULL,*Y_base=NULL,*W=NULL;      

        X_base=new double[Nslices];
        Y_base=new double[Nslices];
        W=new double[Nslices];


        for (int i=0;i<Nslices;i++){
            X_base[i]=SpectrumPhase[i].phase;
            Y_base[i]=SpectrumPhase[i].N;
            W[i]=SpectrumPhase[i].P+0.01;
        }

        TSpline *Spline;
        double p=0.8;

        Spline=new TSpline;
        //Spline->MakeSmoothSpline(X_base,Y_base,Nslices,p,W);
        Spline->MakeSmoothSpline(X_base,Y_base,Nslices,p,W);
        for (int i=0;i<Nslices;i++){
            //fprintf(logfile,"%f %f %f\n",Spectrum[i].x,Spectrum[i].y,Spline->F(i));
            SpectrumPhase[i].y=Spline->F(i);
        }

        delete Spline;
        delete[] X_base;
        delete[] Y_base;
        delete[] W;
        EnvelopePhaseReady=true;
    }

}
//---------------------------------------------------------------------------
double TSpectrumPhase::GetPhaseAverage()
{
    if (!SpectrumPhaseReady)
        MakePhaseSpectrum();

    if (!AvPhaseReady){
        Mphase=0;
        for (int i=0;i<Nslices;i++)
            Mphase+=SpectrumPhase[i].P*SpectrumPhase[i].phase;
        AvPhaseReady=true;
    }
    return Mphase;
}
//---------------------------------------------------------------------------
double TSpectrumPhase::GetPhaseDispersion()
{
    if (!DispPhaseReady){
        Dphase=0;
        GetPhaseAverage();
        for (int i=0;i<Nslices;i++)
            Dphase+=SpectrumPhase[i].P*sqr(SpectrumPhase[i].phase-Mphase);
        DispPhaseReady=true;
    }
    return Dphase;
}
//---------------------------------------------------------------------------
double TSpectrumPhase::GetPhaseSquareDeviation()
{
    if (!SqrPhaseReady){
        Sphase=0;
        GetPhaseDispersion();
        Sphase=sqrt(Dphase);
        SqrPhaseReady=true;
    }
    return Sphase;
}
//---------------------------------------------------------------------------
double TSpectrumPhase::GetPhaseWidth()
{
    if (!SpectrumPhaseReady)
        MakePhaseSpectrum();
    if (!EnvelopePhaseReady)
        MakePhaseEnvelope();

    if (!WidthPhaseReady){
        double *X0,*Y0,Ymax=0;
        double x1=0,x2=0,y0=SpecLevel;

        X0=new double [Nslices+2];
        Y0=new double [Nslices+2];

        for (int i=0;i<Nslices;i++){
            if (SpectrumPhase[i].y>Ymax)
                Ymax=SpectrumPhase[i].y;
        }
        if (Ymax==0)
            Ymax=1e-3;

        for (int i=0;i<Nslices;i++){
            X0[i+1]=SpectrumPhase[i].phase;
            Y0[i+1]=SpectrumPhase[i].y/Ymax;
        }

        X0[0]=X0[1]-1e-3;
        Y0[0]=0;

        X0[Nslices+1]=X0[Nslices]+1e-3;
        Y0[Nslices+1]=0;

        for (int i=0;i<Nslices;i++){
            X0[i+1]=SpectrumPhase[i].phase;
            Y0[i+1]=SpectrumPhase[i].y/Ymax;
        }

        for (int i=1;i<Nslices+2;i++){
            double u1=0,u2=0,w1=0,w2=0;
            if (Y0[i]>=y0){
                u1=X0[i-1];
                u2=X0[i];
                w1=Y0[i-1];
                w2=Y0[i];
                if (w2-w1!=0)
                    x1=u1+(u2-u1)*(y0-w1)/(w2-w1);
                else
                    x1=(w2+w1)/2;

                break;
            }
        }

        for (int i=Nslices+1;i>=0;i--){
            double u1=0,u2=0,w1=0,w2=0;
            if (Y0[i]>=y0){
                u1=X0[i+1];
                u2=X0[i];
                w1=Y0[i+1];
                w2=Y0[i];

                if (w2-w1!=0)
                    x2=u1+(u2-u1)*(y0-w1)/(w2-w1);
                else
                    x2=(w2+w1)/2;
                break;
            }
        }

        dPhase=mod(x2-x1);

        WidthPhaseReady=true;
    }

    return dPhase;
}
//---------------------------------------------------------------------------
TSpectrumBar *TSpectrumPhase::GetPhaseSpectrum(bool Smooth)
{
    if (Smooth)
        MakePhaseEnvelope();
    return SpectrumPhase;
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
