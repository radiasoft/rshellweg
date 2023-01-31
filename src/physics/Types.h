//---------------------------------------------------------------------------

#ifndef TypesH
#define TypesH

#include <IniFiles.hpp>
#include <SysUtils.hpp>

#include <fstream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <cmath>
#include <cstdlib>

#ifndef RSHELLWEG_LINUX

#include <system.hpp>
#include <conio.h>
#include <dir.h>
#include <Vcl.Dialogs.hpp>

#else

#include <AnsiString.hpp>
#define __fastcall
using namespace std;

#endif

#include "ConstUnit.h"

#include "Math.hpp"

//#include "Matrix.h"

//namespace HellwegTypes {

enum TError {ERR_NO,ERR_NOFILE,ERR_OPENFILE,ERR_COUPLER,ERR_SOLENOID,ERR_BEAM,ERR_QUAD,
				ERR_CURRENT,ERR_DRIFT,ERR_CELL,ERR_CELLS,ERR_OPTIONS,ERR_DUMP,ERR_PARTICLE,
				ERR_FORMAT,ERR_IMPORT,ERR_SPCHARGE,ERR_ABORT,ERR_STRUCT,ERR_OTHER};

enum TBeamParameter {R_PAR,TH_PAR,BR_PAR,BTH_PAR,BZ_PAR,PHI_PAR,Z0_PAR,ZREL_PAR,BETA_PAR,X_PAR,Y_PAR,BX_PAR,BY_PAR,
					AR_PAR,ATH_PAR,AX_PAR,AY_PAR,AZ_PAR,W_PAR,RTH_PAR,NO_PAR,LIVE_PAR} ;
enum TStructureParameter {KSI_PAR,Z_PAR,A_PAR,RP_PAR,ALPHA_PAR,SBETA_PAR,RA_PAR,RB_PAR,BZ_EXT_PAR,BR_EXT_PAR,NUM_PAR,
					E0_PAR,EREAL_PAR,PRF_PAR,PBEAM_PAR,BBETA_PAR,WAV_PAR,WMAX_PAR,XB_PAR,YB_PAR,
					ER_PAR,EX_PAR,EY_PAR,ENR_PAR,ENX_PAR,ENY_PAR,E4D_PAR,E4DN_PAR,ET_PAR,ENT_PAR} ;
enum TSplineType {ZSPLINE,LSPLINE,CSPLINE,SSPLINE};
enum TChartType {CH_EMITTANCE,CH_SECTION,CH_PORTRAIT,CH_PHASE,CH_ENERGY,CH_BETA,CH_A,CH_B,CH_ELP,CH_ATT,CH_APP,CH_BEXT,CH_CLEAR};

enum TInputParameter {UNDEFINED,POWER,SOLENOID,BEAM,CURRENT,DRIFT,HALF,CELL,CELLS,OPTIONS,DUMP,COMMENT,SPCHARGE,QUAD,STRUCT,PARTICLES};
enum TTrig {SIN,COS,TG,CTG,SEC,CSC};
enum TDeviation {D_RMS,D_FWHM};
enum TLoss {LIVE,RADIUS_LOST,PHASE_LOST,BZ_LOST,BR_LOST,BTH_LOST,BETA_LOST,STEP_LOST,KILLED};
enum TGraphType {TRANS_SEC,LONGT_SEC,TRANS_SPACE,LONGT_SPACE,LONGT_MOTION,PHASE_SLID,W_SPEC,F_SPEC,R_SPEC,
				R_TRACE,PHI_TRACE,W_TRACE,E_PLOT,EPS_PLOT,P_PLOT,W_PLOT,BETA_PLOT,R_PLOT,F_NONE};
enum TOptType {BUNCHER,ACCELERATOR};
enum TParseStage {DIM_STEP,PIV_STEP,DATA_STEP};
enum TBeamType {NOBEAM,ASTRA,CST_PID,CST_PIT,TWISS_2D,TWISS_4D,SPH_2D,ELL_2D,FILE_1D,FILE_2D,TWO_FILES_2D,FILE_4D,NORM_1D,NORM_2D,PARMELA_T2};

enum TImportType {NO_ELEMENT,ANALYTIC_0D,ANALYTIC_1D,IMPORT_1D,IMPORT_2DC,IMPORT_2DR,IMPORT_3DC,IMPORT_3DR};
enum TMagnetType {MAG_GENERAL,MAG_SOLENOID,MAG_DIPOLE,MAG_QUAD,MAG_NO};
enum TSpaceChargeType {SPCH_NO,SPCH_LPST,SPCH_ELL,SPCH_GW};
enum TParticleType {ELECTRON, PROTON, ION};
enum TCellPosition {FIRST_CELL,END_CELL,REGULAR_CELL};
enum TCellType {TW_CELL, HALF_CELL, DRIFT_CELL};

const int MaxParameters=14;  //Maximum number of parameters after a keyword. Currently: BEAM
const int NumBessel=6;

#ifdef RSHELLWEG_LINUX
typedef long double Extended;

inline Extended DegToRad(const Extended Degrees) {
    return Degrees*M_PI/180;
}
#endif

struct TInputLine{
    TInputParameter P;
    int N;
    AnsiString S[MaxParameters];
};

struct TTwiss{
	double alpha;
	double beta;
	double epsilon;
	double gamma;
};

struct TGauss{
	double mean;
	double sigma;
	double limit;
	double FWHM;
	double core;
};

struct TSphere{
	double Rcath;
	double Rsph;
	double kT;
};

struct TEllipse{
	double ax;
	double by;
	double x0;
	double y0;
	double phi;
	double h;
	double Rx;
	double Ry;
};

struct TSpaceCharge
{
	TSpaceChargeType Type;
	int NSlices;
	double Nrms;
	bool Train;
	double Ltrain;
};

struct TParticlesSpecies
{
	TParticleType Type;
	double A;
	int Q;
};

struct TBeamInput
{
	TBeamType RBeamType;
	TBeamType ZBeamType;
	AnsiString RFile;
	AnsiString YFile;
	AnsiString ZFile;
	TTwiss XTwiss;
	TTwiss YTwiss;
	TGauss XNorm;
	TGauss YNorm;
	TGauss ZNorm;
	TGauss WNorm;
	TSphere Sph;
	TEllipse Ell;
	TSpaceCharge SpaceCharge;
	double Current;
	int NParticles;
	bool ZCompress;
	bool Demagnetize;
	TParticlesSpecies Species;
	double W0; //MeV/u for beta
	double Wnorm; //A/Q for fields
};

struct TStructData
{
	bool Default;
	bool DataReady;
	double Phase;
	int N_bph;
	int N_akl;
	double *Bph;
	double *AKL;
	double **ELP;
	double **AL32;
};

struct TDimensions
{
	int Nx;
	int Ny;
	int Nz;
};

struct TPivot
{
	double *X;
	double *Y;
	double *Z;
};

struct TField
{
	double r;
	double th;
	double z;
};

struct TFieldMap
{
	TDimensions Dim;
	TPivot Piv;
	TField ***Field;
};

struct TFieldMap2D
{
	TDimensions Dim;
	TPivot Piv;
	TField **Field;
};

struct TParticle
{
	double r;  //x/lmb (-Rb<x<Rb) - rename to r
	double th;
	TField beta;
	double phi;
	double z;
	double beta0; //full beta. distinguish beta from bz!
	TLoss lost;
};

struct TPhaseSpace
{
	double x;
	double px;
	double y;
	double py;
};

struct TDump
{
   std::string File;//char *File;
   int NElement;
   int Nmesh;
   int N1;  //limits
   int N2;
   TBeamType SpecialFormat;
   bool Binary;
   bool LiveOnly;
   bool Phase;
   bool Energy;
   bool Radius;
   bool Azimuth;
   bool Divergence;
};

struct TMagnetParameters
{
	TMagnetType MagnetType;
	TImportType ImportType;
	double BField;
	double StartPos;
	double Length;
	double Lfringe;
	AnsiString File;
};

struct TCell
{
    double beta;
    double ELP;
    double AL32;
    double AkL;
    double Mode;
    double F0;
    double P0;
	double dF;
	int Mesh;
    TCellPosition Position;
	TCellType Type;
	TMagnetParameters Magnet;
};
struct TStructure
{
    double ksi;
    double lmb;
    double P;
    double dF;
    double E;
    double A;
    double Rp;
    double B;
    double alpha;
    double beta;
	double Ra;
	TFieldMap2D Bmap;
    bool jump;
    bool drift;
	int CellNumber;
};

struct TSectionParameters
{
	double Frequency;
	double Power;
	double PhaseShift;
	double NCells;
};

struct TStructureInput
{
	int NSections;
	int NElements;
	int NMaps;
	int NStructData;
	int ElementsLimit;
	TSectionParameters *Sections;
	TCell *Cells;
	TStructData *StructData;
	TMagnetParameters SolenoidPar;
	bool Reverse;
    bool CouplerPhase;
};

struct TResult
{
	double Length;
	double MaximumEnergy;
    double InputCurrent;
    double BeamCurrent;
    double Captured;
	double BeamRadius;
	TGauss Energy;
	TGauss Phase;
    double BeamPower;
    double LoadPower;
	TTwiss RTwiss;
	TTwiss XTwiss;
	TTwiss YTwiss;
    double A;
};
struct TOptResult
{
    double x;
    TResult Result;
};
struct TSplineCoef
{
    double X;
    double Y;
    double A;
    double B;
    double C;
    double D;
    double W;
    double F;
};
struct TGraph
{
	double x;
	double y;
	bool draw;
};
struct TSpectrumBar
{
    double x;
    double phase;
    int N;
    double y;  //envelope
    double P;
	double xAv;
	double xRMS;
	double yAv;
	double yRMS;
};
struct TIntegration
{
	double phi;
	TField E;
	TField H;
	TField beta;
	double r;
    double th;
    double A;
};
struct TIntParameters
{
    double h;
    double bw;
    double w;
    double dL;
	double A;
	double B;
	double E;
	TFieldMap2D Hext;
	TFieldMap2D Hmap;
	double dH;
    double SumSin;
	double SumCos;
	TField *Eq;
    double gamma;
    bool drift;
};

//};
//---------------------------------------------------------------------------
#endif      // TypesH
