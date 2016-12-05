//---------------------------------------------------------------------------


#pragma hdrstop
#include "Spectrum.h"

//---------------------------------------------------------------------------
__fastcall TSpectrum::TSpectrum()
{
    Nx=1;
    Nbars=1;
    MeshSet=false;
    SpectrumReady=false;
    AvReady=false;
    WidthReady=false;
    EnvelopeReady=false;

    M=0;
    D=0;
    S=0;

    Spectrum=new TSpectrumBar[Nbars];

    //logfile=fopen("spectrum.log","w");
}
//---------------------------------------------------------------------------
__fastcall TSpectrum::~TSpectrum()
{
    delete[] Spectrum;
    //fclose(logfile);
}
//---------------------------------------------------------------------------
void TSpectrum::ResetStatistics()
{
    M=0;
    D=0;
    S=0;
    AvReady=0;
    DispReady=0;
    SqrReady=0;
}
//---------------------------------------------------------------------------
void TSpectrum::GetBoundaries()
{
    double xmin=1e32;
    double xmax=-1e32;
    double x=0;

    for (int i=0;i<Nx;i++){
        x=X[i];
        if (x>xmax)
            xmax=x;
        if (x<xmin)
            xmin=x;
    }

    Xmin=xmin;
    Xmax=xmax;
}
//---------------------------------------------------------------------------
void TSpectrum::SetMesh(double *X0,int Nb,int Ny)
{
    delete[] Spectrum;
    Nbars=Nb;
    Nx=Ny;
    X=X0;

    Spectrum=new TSpectrumBar[Nbars];
    GetBoundaries();

    for (int i=0;i<Nbars;i++){
        Spectrum[i].x=Xmin+i*(Xmax-Xmin)/(Nbars-1);
        Spectrum[i].N=0;
        Spectrum[i].P=0;
        Spectrum[i].y=0;
    }

    MeshSet=true;
    MakeSpectrum();
    ResetStatistics();
}
//---------------------------------------------------------------------------
void TSpectrum::MakeSpectrum()
{
    if (!MeshSet){
        SpectrumReady=false;
        return;
    }

    int j=0,Ny=Nx>0?Nx:1;
    double x=0;

    if (Xmax==Xmin){
        if (Xmax==0){
            Xmax=0.02;
        }else{
            Xmax*=1.01;
            Xmin*=0.99;
        }
    }

    for (int i=0;i<Nx;i++){
        x=X[i];
        j=round((x-Xmin)*(Nbars-1)/(Xmax-Xmin));
        if (j>Nbars-1)
            j=Nbars-1;
        if (j<0)
            j=0;
        Spectrum[j].N++;
    }
    for (int i=0;i<Nbars;i++)
        Spectrum[i].P=1.0*Spectrum[i].N/Ny;

    SpectrumReady=true;

    //MakeEnvelope();
    ResetStatistics();

}
//---------------------------------------------------------------------------
void TSpectrum::MakeEnvelope()
{
    if (!SpectrumReady)
        MakeSpectrum();

    if (!EnvelopeReady){
        for (int i=0;i<Nbars;i++)
            Spectrum[i].y=Spectrum[i].N;

        double *X_base=NULL,*Y_base=NULL,*W=NULL;      

        X_base=new double[Nbars];
        Y_base=new double[Nbars];
        W=new double[Nbars];


        for (int i=0;i<Nbars;i++){
            X_base[i]=Spectrum[i].x;
            Y_base[i]=Spectrum[i].N;
            W[i]=Spectrum[i].P+0.01;
        }

        TSpline *Spline;
        double p=0.8;

        Spline=new TSpline;
        //Spline->MakeSmoothSpline(X_base,Y_base,Nbars,p,W);
        Spline->MakeSmoothSpline(X_base,Y_base,Nbars,p,W);
        for (int i=0;i<Nbars;i++){
            //fprintf(logfile,"%f %f %f\n",Spectrum[i].x,Spectrum[i].y,Spline->F(i));
            Spectrum[i].y=Spline->F(i);
        }

        delete Spline;
        delete[] X_base;
        delete[] Y_base;
        delete[] W;
        EnvelopeReady=true;
    }

}
//---------------------------------------------------------------------------
double TSpectrum::GetAverage()
{
    if (!SpectrumReady)
        MakeSpectrum();

    if (!AvReady){
        M=0;
        for (int i=0;i<Nbars;i++)
            M+=Spectrum[i].P*Spectrum[i].x;
        AvReady=true;
    }
    return M;
}
//---------------------------------------------------------------------------
double TSpectrum::GetDispersion()
{
    if (!DispReady){
        D=0;
        GetAverage();
        for (int i=0;i<Nbars;i++)
            D+=Spectrum[i].P*sqr(Spectrum[i].x-M);
        DispReady=true;
    }
    return D;
}
//---------------------------------------------------------------------------
double TSpectrum::GetSquareDeviation()
{
    if (!SqrReady){
        S=0;
        GetDispersion();
        S=sqrt(D);
        SqrReady=true;
    }
    return S;
}
//---------------------------------------------------------------------------
double TSpectrum::GetWidth()
{
    if (!SpectrumReady)
        MakeSpectrum();
    if (!EnvelopeReady)
        MakeEnvelope();

    if (!WidthReady){
        double *X0,*Y0,Ymax=0;
        double x1=0,x2=0,y0=SpecLevel;

        X0=new double [Nbars+2];
        Y0=new double [Nbars+2];

        for (int i=0;i<Nbars;i++){
            if (Spectrum[i].y>Ymax)
                Ymax=Spectrum[i].y;
        }
        if (Ymax==0)
            Ymax=1e-3;

        for (int i=0;i<Nbars;i++){
            X0[i+1]=Spectrum[i].x;
            Y0[i+1]=Spectrum[i].y/Ymax;
        }

        X0[0]=X0[1]-1e-3;
        Y0[0]=0;

        X0[Nbars+1]=X0[Nbars]+1e-3;
        Y0[Nbars+1]=0;

        for (int i=0;i<Nbars;i++){
            X0[i+1]=Spectrum[i].x;
            Y0[i+1]=Spectrum[i].y/Ymax;
        }

        for (int i=1;i<Nbars+2;i++){
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

        for (int i=Nbars+1;i>=0;i--){
            double u1=0,u2=0,w1=0,w2=0;
            if (Y0[i]>=y0){
                u1=X0[i+1];
                u2=X0[i];
                w1=Y0[i+1];
                w2=Y0[i];

                if (w2-w1!=0)
                    x2=u1+(u2-u1)*(y0-w1)/(w2-w1);
                else
                    x1=(w2+w1)/2;
                break;
            }
        }

        dX=mod(x2-x1);

        WidthReady=true;
    }

    return dX;
}
//---------------------------------------------------------------------------
TSpectrumBar *TSpectrum::GetSpectrum(bool Smooth)
{
    if (Smooth)
        MakeEnvelope();
    return Spectrum;
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
