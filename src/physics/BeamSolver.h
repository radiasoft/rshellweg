//---------------------------------------------------------------------------

#ifndef BeamSolverH
#define BeamSolverH

// #include "Types.h"       (DLB, 20150819;  creates link conflict)
// #include "Spectrum.h"
// #include "Spline.h"
// #include "Matrix.h"

#include "Functions.h"
#include "Beam.h"
#include "Matrix.h"
#include "Spline.h"
#include "Spectrum.h"

#ifndef RSLINAC
#include "SmartProgressBar.h"
#else
#include "TStringList.hpp"
#endif

using namespace std;

//---------------------------------------------------------------------------
class TBeamSolver
{
private:
    //FLAGS
	bool DataReady;
	AnsiString UserIniPath ;
	TError SolverStop;

	//BEAM
	TBeamInput BeamPar;
	TBeam **Beam;

	//These should be removed sometimes
	double Smooth;
	//double Kernel,AngErr;
	double dh;
	//int Ngraph,Nav;

	int Np_beam,Nstat,Nliv,Ndump;  //Move to TMeshParameters

	TSplineType SplineType;
	//STRUCTURE
	TStructure *Structure;
	TStringList *InputStrings,*ParsedStrings;
	TDump *BeamExport;
	TStructureInput StructPar;
	TFieldMap ExternalMagnetic;
	TFieldMap2D *QuadMaps;

	int ChangeCells(int N);
	void ResetStructure();
	void ResetDump(int Ns);
	void ResetExternal();
	void ResetMaps();
	void ResetStructData();
	void DeleteStructData(TStructData &D);

	int Mode_N,Mode_M,MaxCells,Nmesh,Npoints;// //Move to TMeshParameters

	//int GetSolenoidPoints();
	bool ReadSolenoid(int Nz,double *Z,double* B);

	//OTHER
	#ifndef RSLINAC 
	TSmartProgress *SmartProgress;
	#endif

	//INITIALIZATION & PARSING
	void Initialize();
	void LoadIniConstants();
	double DefaultFrequency(int N);

	AnsiString AddLines(TInputLine *Lines,int N1, int N2);

	TInputLine *ParseFile(int& N);
	TError ParseLines(TInputLine *Lines,int N,bool OnlyParameters=false);

	TError ParseOptions (TInputLine *Line);
	TError ParseSpaceCharge (TInputLine *Line);
	TError ParseSolenoid (TInputLine *Line);
	TError ParseBeam (TInputLine *Line);
	TError ParseCurrent (TInputLine *Line);
	TError ParseParticleType (TInputLine *Line);
	TError ParseStruct (TInputLine *Line, int Ni);
	TError ParseCell (TInputLine *Line,int Ni,int Nsec, bool NewCell);
	TError ParseSingleCell (TInputLine *Line,int Ni,int Nsec, bool NewCell);
	TError ParseMultipleCells (TInputLine *Line,int Ni,int Nsec, bool NewCell);
	TError ParseDrift (TInputLine *Line,int Ni,int Nsec);
	TError ParseQuad (TInputLine *Line,int Ni,int Nsec);
	TError ParsePower (TInputLine *Line,int Nsec);
	TError ParseDump (TInputLine *Line,int Ns, int Ni);

	TError CheckStructFile (TStructData &D, AnsiString &F);
	TError ParseStructFile (TStructData &D, AnsiString &F);
	TError ParsePID (TInputLine *Line, AnsiString &F);
	TError ParsePIT (TInputLine *Line, AnsiString &F);
	TError ParseT2 (TInputLine *Line, AnsiString &F);
	TError ParseFile2R (TInputLine *Line, AnsiString &F, int Nr);
	TError ParseFile1Z (TInputLine *Line, AnsiString &F, int Nz,int Zpos);
	TError ParseFile2Z (TInputLine *Line, AnsiString &F, int Nz,int Zpos);
	TError ParseTwiss2D (TInputLine *Line, AnsiString &F, int Nr);
	TError ParseTwiss4D (TInputLine *Line, AnsiString &F, int Nr);
	TError ParseSPH (TInputLine *Line, AnsiString &F, int Nr);
	TError ParseELL (TInputLine *Line, AnsiString &F, int Nr);
	TError ParseNorm (TInputLine *Line, AnsiString &F, int Nz,int Zpos);

	//STRUCTURE
	void CreateMesh();
	void CreateStrucutre();
	void DeleteMesh();
	void DeleteBeam();

	TImportType ParseSolenoidType(AnsiString &F);
	TDimensions ParseSolenoidLines(TMagnetParameters &P);
	TFieldMap2D ParseMapFile(AnsiString &F); //Try to unite with solenoid parse
	void CreateMaps();
	void CreateMagneticMap();
	void MakePivots();
	void MakeUniformSolenoid();
	void MakeAnalyticalSolenoid();
	void ParseSolenoidFile(TParseStage P);
	void CalculateRadialField();
	void CalculateRadialField(double Rmax);
	void CalculateFieldDerivative();
	void TransformMagneticToCYL1D();
	void TransformMagneticToCYL2D();
	void AdjustMagneticMesh();

	//INTERPOLATION
	double *LinearInterpolation(double *x,double *X,double *Y,int Nbase,int Nint);
	double *SplineInterpolation(double *x,double *X,double *Y,int Nbase,int Nint);
	double *SmoothInterpolation(double *x,double *X,double *Y,int Nbase,int Nint,double p0,double *W=NULL);
	
	void GetDimensions(TCell& Cell);
	void GetDimensionsFromTab(TCell& Cell);
	void GetDimensionsFromFile(TCell& Cell,TStructData *D);
	double FormFactor(double ryrx, double rxrz, TBeamParameter P, double s=0);
	double GetEigenFactor(double x, double y, double z,double a, double b, double c);

	//INTEGRATION
	void DumpHeader(ofstream &fo,TDump *ExportParameters,int jmin,int jmax);
	void DumpFile(ofstream &fo,TDump *ExportParameters,int j);
	void DumpASTRA(ofstream &fo,TDump *ExportParameters,int j,int jref);
	void DumpCST(ofstream &fo,TDump *ExportParameters,int j);
	void DumpT2(ofstream &fo, TDump *ExportParameters, int j);
	void DumpBeam(int Sn);
	void DumpBeam(TDump *ExportParameters);
	void Step(int Si);
	void Integrate(int Si, int Sj);
	void SpaceCharge(int Si, int Sj);
	void CountLiving(int Si);
	void KillParticles(int Si);
        void KillParticles(int Si, int Sj);
	TIntegration **K;
	TIntParameters *Par;

	//TYPE CHECKS
	bool IsKeyWord(AnsiString &S);
	TInputParameter Parse(AnsiString &S);

	TBeamType ParseDistribution(AnsiString &S);
	TSpaceChargeType ParseSpchType(AnsiString &S);
	bool IsFullFileKeyWord(TBeamType D);
	bool IsTransverseKeyWord(TBeamType D);
	bool IsLongitudinalKeyWord(TBeamType D);
	bool IsFileKeyWord(TBeamType D);
	bool IsImportType(TImportType T);

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

    AnsiString InputFile;

    TError LoadData(int Nlim=-1);
    TError LoadData(AnsiString LogFileName, int Nlim=-1);

    TError MakeBuncher(TCell& iCell);

    void AppendCells(TCell& iCell,int N=1);
    void AddCells(int N=1);
    double GaussIntegration(double r,double z,double Rb,double Lb,int component);
    TCell LastCell();
    TCell GetCell(int j);

	void SaveToFile(AnsiString& Fname);
	void SaveOutput(AnsiString& Fname, bool binary=false);
	void SaveTrajectories(AnsiString& Fname);
    bool LoadFromFile(AnsiString& Fname);
   // bool LoadEnergyFromFile(AnsiString& Fname, int NpEnergy);     move to beam.h

    TError CreateBeam();
    TError CreateGeometry();
    void SetBarsNumber(double Nbin);
    void ChangeInputCurrent(double Ib);

	//GET INITIAL PARAMETERS
    int GetNumberOfPoints();
    int GetMeshPoints();
	int GetNumberOfParticles();
	//int GetNumberOfChartPoints();
    //int GetNumberOfBars();
	int GetNumberOfCells();
	int GetNumberOfSections();
	double GetInputCurrent();
   //   double GetMode(int *N=NULL,int *M=NULL);
   	TParticleType GetParticleType();
	double GetParticleMass();
	int GetParticleCharge();
    double GetParticleRestEnergy();

   //DELETE
	TGauss GetInputEnergy();
	TGauss GetInputPhase();
	TTwiss GetInputTwiss(TBeamParameter P);
	double GetInputWavelength();

	//bool CheckMagnetization();
	bool CheckReverse();
	bool CheckDrift(int Nknot);
	TSpaceCharge GetSpaceChargeInfo();
	TMagnetParameters GetSolenoidInfo();

	//GET OUTPUT PARAMETERS
	TTwiss GetTwiss(int Nknot,TBeamParameter P=R_PAR);
	TEllipse GetEllipse(int Nknot,TBeamParameter P=R_PAR);
	TGauss GetEnergyStats(int Nknot, TDeviation D=D_RMS);
	TGauss GetPhaseStats(int Nknot, TDeviation D=D_RMS);
	TSpectrumBar *GetSpectrum(int Nknot,TBeamParameter P, bool Smooth=false);

	//DELETE
	TSpectrumBar *GetEnergySpectrum(int Nknot, bool Smooth=false);
	TSpectrumBar *GetPhaseSpectrum(int Nknot, bool Smooth=false);
	TSpectrumBar *GetRSpectrum(int Nknot, bool Smooth=false);
	TSpectrumBar *GetXSpectrum(int Nknot, bool Smooth=false);
	TSpectrumBar *GetYSpectrum(int Nknot, bool Smooth=false);

	double GetSectionFrequency(int Nsec=0);
	double GetSectionPower(int Nsec=0);
	double GetSectionWavelength(int Nsec=0);
	double GetFrequency(int Ni);
	double GetPower(int Ni);
	double GetWavelength(int Ni);
	double GetMaxAperture();
	double GetAperture(int Ni);
	double GetMaxEnergy(int Ni);
	double GetMaxDivergence(int Ni);
	double GetMaxPhase(int Ni);
	double GetMinPhase(int Ni);
	double GetCurrent(int Ni);
	double GetBeamRadius(int Ni,TBeamParameter P=R_PAR);
	TLoss GetLossType(int Nknot,int Np);
	int GetLivingNumber(int Ni);

	TGraph *GetTrace(int Np,TBeamParameter P1,TBeamParameter P2);
	TGraph *GetSpace(int Nknot,TBeamParameter P1,TBeamParameter P2);
	double *GetBeamParameters(int Nknot,TBeamParameter P);
	double GetStructureParameter(int Nknot,TStructureParameter P);
	double *GetStructureParameters(TStructureParameter P);

	TError Solve();
	#ifndef RSLINAC
	TResult Output(AnsiString& FileName,TMemo *Memo=NULL);
    #else
    TResult Output(AnsiString& FileName);
    #endif
};

//---------------------------------------------------------------------------
#endif
