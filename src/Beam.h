//---------------------------------------------------------------------------

#ifndef BeamH
#define BeamH

#include "Spectrum.h"

const int Ncoef=4;
//---------------------------------------------------------------------------
class TBeam
{
private:
	int Np,Nliv,Nbars;
	double Kernel;
	void CountLiving();
	void TwoRandomGauss(double& x1,double& x2);
	void SetParameters(double *X,TBeamParameter Par);
	TSpectrumBar *GetSpectrum(bool Smooth,double *X,double& Xav,double& dX,bool width=false);
	int CountCSTParticles(char *F);

	FILE *logFile;

	double BesselSum(TIntParameters& Par,TIntegration *I,TTrig Trig);
public:
	__fastcall TBeam(int N);
	__fastcall ~TBeam();
	TParticle *Particle;
	double lmb,h,Cmag;
	double Ib,I0;
	bool Reverse;
	
	void SetKernel(double Ker);
	void SetBarsNumber(int N);

	bool ReadCSTEmittance(TBeamType bType);
	int CountCSTParticles(TBeamType bType);
	void MakeGaussEmittance(double alpha, double betta, double eps);
	void MakeGaussDistribution(double Xav,double sX,TBeamParameter Par);
	void MakeEquiprobableDistribution(double Xav, double dX,TBeamParameter Par);
		//GET OUTPUT PARAMETERS
	void GetCourantSneider(double& alpha,double& betta, double& epsilon);
	void GetEllipticParameters(double& x0,double& y0, double& a,double& b,double& phi, double& Rx, double& Ry);
	TSpectrumBar *GetEnergySpectrum(bool Smooth,double& Wav,double& dW);
	TSpectrumBar *GetPhaseSpectrum(bool Smooth,double& Fav,double& dF);
	void GetParameters(double *X,TBeamParameter Par);

	double GetBeamRadius();
	double GetPhaseLength();
	double GetAveragePhase();
	double GetAverageEnergy();
	double GetMaxEnergy();

	double iGetAverageEnergy(TIntParameters& Par,TIntegration *I);
	double iGetBeamLength(TIntParameters& Par,TIntegration *I);
	double iGetBeamRadius(TIntParameters& Par,TIntegration *I);
	double iGetAveragePhase(TIntParameters& Par,TIntegration *I);

	double SinSum(TIntParameters& Par,TIntegration *I);
	double CosSum(TIntParameters& Par,TIntegration *I);

	int GetLivingNumber();
	double FindEmittanceAngle(double& a,double& b);

	void Integrate(TIntParameters& Par,TIntegration **I,int Si);
	void Next(TBeam *nBeam,TIntParameters& Par,TIntegration **I);
	void Next(TBeam *nBeam);

    void Solve();

};
//---------------------------------------------------------------------------
#endif
