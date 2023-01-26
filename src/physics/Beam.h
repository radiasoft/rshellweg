//---------------------------------------------------------------------------

#ifndef BeamH
#define BeamH

#include "Functions.h"
#include "Spectrum.h"
#include "SpectrumPhase.h"
#include <stdlib.h>
#include <stdexcept>

const int Ncoef=4;
//---------------------------------------------------------------------------
class TBeam
{
private:
    int Nliv,Nbars;
	double Kernel;
	double Ib,I0; //beam current
	double W0;//rest energy

	void CountLiving();
	//void TwoRandomGauss(double& x1,double& x2); //Moved to types.h
	void SetParameters(double *X,TBeamParameter Par);

	//DISTRIBUTIONS
	double *MakeRayleighDistribution(TGauss G);
	double *MakeRayleighDistribution(double Xav,double sX);
	double *MakeRayleighDistribution(double Xav,double sX,double Xlim);
	double *MakeGaussDistribution(TGauss G);
	double *MakeGaussDistribution(double Xav,double sX);
	double *MakeGaussDistribution(double Xav,double sX,double Xlim);
	void MakeGaussDistribution(TGauss G,TBeamParameter Par);  //to private
	void MakeGaussDistribution(double Xav,double sX,TBeamParameter Par,double Xlim=0); //move to private!!

	double *MakeEquiprobableDistribution(double Xav, double dX);
	void MakeEquiprobableDistribution(double Xav, double dX,TBeamParameter Par); //to private

	TPhaseSpace *MakeTwissDistribution(TTwiss T);
	TEllipse GetEllipse(TTwiss T);
	TEllipse FindEmittanceAngle(TBeamParameter P);
    //double FindEmittanceAngle(double& a,double& b); //obsolete

	TBeamParameter ComplementaryParameter(TBeamParameter P);
	bool IsRectangular(TBeamParameter P);

	double *GetLivingParameter(TBeamParameter P);
	TGauss GetStatistics(TBeamParameter P,bool FWHM=false);
	TGauss GetStatistics(double *X,bool FWHM=false,bool Core=false);

	TSpectrum *GetSpectrum(TBeamParameter P);
	TSpectrum *GetSpectrum(double *X);
	TSpectrumBar *GetSpectrumBar(double *X,bool Smooth=false);
	double GetDeviation(TBeamParameter P,TDeviation D=D_RMS);
	double GetMean(TBeamParameter P);
	double GetMinValue(TBeamParameter P);
	double GetMaxValue(TBeamParameter P);

	//IMPORT
	double **ImportFromFile(TBeamType BeamType,TBeamInput *BeamPar,bool T=true);

	FILE *logFile;

    double BesselSum(TIntParameters& Par,TIntegration *I,TTrig Trig);
public:
    __fastcall TBeam(int N);
    __fastcall ~TBeam();
    TParticle *Particle;
	double lmb,h,Cmag;
    bool Reverse;
	int Np;
    void SetKernel(double Ker);


	// BEAM CREATION
	bool BeamFromImport(TBeamInput *BeamPar);
	bool BeamFromTwiss(TBeamInput *BeamPar);
	bool BeamFromFile(TBeamInput *BeamPar);
	bool BeamFromSphere(TBeamInput *BeamPar);
	bool BeamFromEllipse(TBeamInput *BeamPar);
	void GenerateEnergy(TGauss G);
	void GeneratePhase(TGauss G);
	void GenerateAzimuth(TGauss G);
	bool ImportEnergy(TBeamInput *BeamPar);
    void ShiftPhase (double phi0);

	void SetCurrent(double I);
	void SetInputCurrent(double I);
	void SetBarsNumber(int N);
	void SetRestEnergy(double W);

		//GET OUTPUT PARAMETERS
	TTwiss GetTwiss(TBeamParameter P=R_PAR,bool Norm=false);
	TTwiss GetTwissDirect(TBeamParameter P=R_PAR);
	double Get4DEmittance(bool Norm=false);
	TEllipse GetEllipse(TBeamParameter P=R_PAR);
	TEllipse GetEllipseDirect(TBeamParameter P=R_PAR);

   //	void GetParameters(double *X,TBeamParameter Par); //remove
	TGauss GetEnergyDistribution(TDeviation D=D_RMS);
	TGauss GetPhaseDistribution(TDeviation D=D_RMS);
	TSpectrumBar *GetEnergySpectrum(bool Smooth);
	TSpectrumBar *GetPhaseSpectrum(bool Smooth);

	double GetParameter(int i,TBeamParameter P);

	double GetRadius(TBeamParameter P=R_PAR,TDeviation D=D_RMS);
	double GetDivergence(TBeamParameter P=AR_PAR,TDeviation D=D_RMS);

	TSpectrumBar *GetSpectrumBar(TBeamParameter P,bool Smooth=false);
	//DELETE
	TSpectrumBar *GetRSpectrum(bool Smooth);
	TSpectrumBar *GetXSpectrum(bool Smooth);
	TSpectrumBar *GetYSpectrum(bool Smooth);

	double GetPhaseLength(TDeviation D=D_RMS);
    double GetAveragePhase();
	double GetAverageEnergy();
	double GetMaxEnergy();
	double GetMinPhase();
	double GetMaxPhase();
	double GetMaxDivergence();
	double GetCurrent();
	double GetInputCurrent();

	double iGetAverageEnergy(TIntParameters& Par,TIntegration *I);
	//double iGetBeamLength(TIntParameters& Par,TIntegration *I, int Nslices, bool SpectrumOutput=false);
	//double iGetBeamRadius(TIntParameters& Par,TIntegration *I, bool SpectrumOutput=false);
	//double iGetAveragePhase(TIntParameters& Par,TIntegration *I);

	TGauss iGetBeamRadius(TIntParameters& Par,TIntegration *I,TBeamParameter P=R_PAR);
	TGauss iGetBeamLength(TIntParameters& Par,TIntegration *I, int Nslices=1);

    double SinSum(TIntParameters& Par,TIntegration *I);
    double CosSum(TIntParameters& Par,TIntegration *I);

	int GetLivingNumber();
	void KillParticles(TParticle *P=NULL);

    void Integrate(TIntParameters& Par,TIntegration **I,int Si);
    void Next(TBeam *nBeam,TIntParameters& Par,TIntegration **I);
    void Next(TBeam *nBeam);

    void Solve();

};
//---------------------------------------------------------------------------
#endif
