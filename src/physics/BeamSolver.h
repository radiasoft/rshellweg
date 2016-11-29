//---------------------------------------------------------------------------

#ifndef BeamSolverH
#define BeamSolverH

// #include "Types.h"       (DLB, 20150819;  creates link conflict)
// #include "Spectrum.h"
// #include "Spline.h"
// #include "Matrix.h"

#include "Beam.h"
#include "Matrix.h"
#include "Spline.h"
#include "Spectrum.h"

#ifndef RSLINAC
#include "SmartProgressBar.h"
#else
#include "TStringList.hpp"
#endif

//---------------------------------------------------------------------------
class TBeamSolver
{
private:
    //FLAGS
    bool DataReady;
    AnsiString UserIniPath,SolenoidFile,BeamFile,EnergyFile;
    //INITIAL PARAMETERS
    double F0,P0,lmb;
	double B0,Lmag,Zmag;  //Lmag - length, Zmag -position

	//BEAM
	TBeamInput BeamPar;

	//These should be removed
   /* double W0,Phi0,dW,dPhi,I0;
	double AlphaCS,BettaCS,EmittanceCS;
   	bool NpFromFile;
	int Np,NpEnergy;
	bool Phi_Eq,W_Eq;
	TBeamType BeamType;    */

	double Kernel,Smooth;
	double AngErr;
	double dh;
	int Mode_N,Mode_M,MaxCells,Ncells,Nmesh,Npoints,Nlim;
	int Np_beam,Nstat,Ngraph,Nbars,Nav,Nliv;
	bool Reverse,SpCharge,Coulomb,GWmethod,FSolenoid,Magnetized;

    TSplineType SplineType;
    //STRUCTURE
    TCell *Cells;
    TStringList *InputStrings,*ParsedStrings;

    double I,Rb,Lb,phi0,dphi,w,betta0;
    TIntegration **K;
    TIntParameters *Par;
    
    #ifndef RSLINAC 
    TSmartProgress *SmartProgress;
    #endif
    //INITIALIZATION
    void Initialize();
    void LoadIniConstants();
	int ChangeCells(int N);

    TInputLine *ParseFile(int& N);
	TError ParseLines(TInputLine *Lines,int N,bool OnlyParameters=false);
	AnsiString AddLines(TInputLine *Lines,int N1, int N2);

	//INTERPOLATION
	double *LinearInterpolation(double *x,double *X,double *Y,int Nbase,int Nint);
    double *SplineInterpolation(double *x,double *X,double *Y,int Nbase,int Nint);
    double *SmoothInterpolation(double *x,double *X,double *Y,int Nbase,int Nint,double p0,double *W=NULL);
    void GetDimensions(TCell& Cell);

    void Step(int Si);
    void Integrate(int Si, int Sj);
    void CountLiving(int Si);

    int GetSolenoidPoints();
    bool ReadSolenoid(int Nz,double *Z,double* B);

    bool IsKeyWord(AnsiString &S);
	TInputParameter Parse(AnsiString &S);

	TBeamType ParseDistribution(AnsiString &S);
	bool IsFullFileKeyWord(TBeamType D);
	bool IsTransverseKeyWord(TBeamType D);
	bool IsLongitudinalKeyWord(TBeamType D);
	bool IsFileKeyWord(TBeamType D);

	void ShowError(AnsiString &S);
protected:

public:
    //INITIALIZATION
    __fastcall TBeamSolver(AnsiString _Path);
    __fastcall TBeamSolver();
    __fastcall ~TBeamSolver();

    #ifndef RSLINAC
    void AssignSolverPanel(TObject *SolverPanel);
    #endif

    void Abort();
	bool Stop;
    
    TBeam **Beam;
    TStructure *Structure;
    AnsiString InputFile;

    TError LoadData(int Nl=-1);
    TError MakeBuncher(TCell& iCell);

    void AppendCells(TCell& iCell,int N=1);
    void AddCells(int N=1);
    double GaussIntegration(double r,double z,double Rb,double Lb,int component);
    TCell LastCell();
    TCell GetCell(int j);

    void SaveToFile(AnsiString& Fname);
    bool LoadFromFile(AnsiString& Fname);
   // bool LoadEnergyFromFile(AnsiString& Fname, int NpEnergy);     move to beam.h

    TError CreateBeam();
    int CreateGeometry();

    void ChangeInputCurrent(double Ib);

    //GET INITIAL PARAMETERS
    int GetNumberOfPoints();
    int GetMeshPoints();
    int GetNumberOfParticles();
   //   double GetWaveLength();
    int GetNumberOfChartPoints();
    int GetNumberOfBars();
    int GetNumberOfCells();
    double GetFrequency();
    double GetPower();
    double GetInputCurrent();
   //   double GetMode(int *N=NULL,int *M=NULL);
    double GetSolenoidField();
    double GetSolenoidLength();
    double GetSolenoidPosition();
    double GetInputAverageEnergy();
    double GetInputEnergyDeviation();
    double GetInputAveragePhase();
    double GetInputPhaseDeviation();
    double GetInputAlpha();
    double GetInputBetta();
    double GetInputEpsilon();
    bool IsCoulombAccounted();
    bool IsWaveReversed();
    bool IsEnergyEquiprobable();
    bool IsPhaseEquiprobable();
    //GET OUTPUT PARAMETERS
    void GetCourantSneider(int Nknot, double& alpha,double& betta, double& epsilon);
    void GetEllipticParameters(int Nknot, double& x0,double& y0, double& a,double& b,double& phi,double &Rx,double& Ry);
    TSpectrumBar *GetEnergySpectrum(int Nknot,double& Wav,double& dW);
    TSpectrumBar *GetPhaseSpectrum(int Nknot,double& Fav,double& dF);
    TSpectrumBar *GetEnergySpectrum(int Nknot,bool Env,double& Wav,double& dW);
    TSpectrumBar *GetPhaseSpectrum(int Nknot,bool Env,double& Fav,double& dF);
    void GetBeamParameters(int Nknot,double *X,TBeamParameter Par);
    void GetStructureParameters(double *X,TStructureParameter Par);
    double GetKernel();

    void Solve();
    #ifndef RSLINAC
    TResult Output(AnsiString& FileName,TMemo *Memo=NULL);
    #else
    TResult Output(AnsiString& FileName);
    #endif
};

//---------------------------------------------------------------------------
#endif
