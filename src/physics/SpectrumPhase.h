//---------------------------------------------------------------------------

#ifndef SpectrumPhaseH
#define SpectrumPhaseH

#include "Spline.h"
// #include "Types.h"

//---------------------------------------------------------------------------
class TSpectrumPhase
{
private:
    FILE *logfile;

    int Nparticle;
    double PhaseMin,PhaseMax,*Phase,*Radius;
    double Mphase,Dphase,Sphase,dPhase;
    bool MeshPhaseSet;
    bool SpectrumPhaseReady,AvPhaseReady,DispPhaseReady,SqrPhaseReady,WidthPhaseReady,EnvelopePhaseReady;
    TSpectrumBar *SpectrumPhase;
    void GetPhaseBoundaries();
    void ResetPhaseStatistics();

    void MakePhaseSpectrum();
    void MakePhaseEnvelope();
    
public:
    __fastcall TSpectrumPhase();
    __fastcall ~TSpectrumPhase();
    
    void SetPhaseMesh(double *R0,double *X0,int Nb,int Ny);
    TSpectrumBar *GetPhaseSpectrum(bool Smooth=false);

    double GetPhaseAverage();
    double GetPhaseDispersion();
    double GetPhaseSquareDeviation();
    double GetPhaseWidth();
};

//---------------------------------------------------------------------------
#endif
