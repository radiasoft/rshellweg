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

#ifndef RSLINAC
#include <system.hpp>
#include <conio.h>
#include <dir.h>
#include <Vcl.Dialogs.hpp>
#else
#include <AnsiString.hpp>
#define __fastcall 
#endif

#include "ConstUnit.h"

#include "Math.hpp"

//#include "Matrix.h"

namespace HellwegTypes {

enum TError {ERR_NO,ERR_NOFILE,ERR_OPENFILE,ERR_COUPLER,ERR_SOLENOID,ERR_BEAM,
				ERR_CURRENT,ERR_DRIFT,ERR_CELL,ERR_CELLS,ERR_OPTIONS,ERR_DUMP,ERR_FORMAT,ERR_IMPORT,ERR_SPCHARGE};

enum TBeamParameter {R_PAR,TH_PAR,BR_PAR,BTH_PAR,BZ_PAR,PHI_PAR,Z0_PAR,ZREL_PAR,BETA_PAR,X_PAR,Y_PAR,BX_PAR,BY_PAR,
					AR_PAR,ATH_PAR,AX_PAR,AY_PAR,AZ_PAR,W_PAR,NO_PAR,LIVE_PAR} ;
enum TStructureParameter {KSI_PAR,Z_PAR,A_PAR,RP_PAR,ALPHA_PAR,SBETA_PAR,RA_PAR,RB_PAR,B_EXT_PAR,NUM_PAR,
					E0_PAR,EREAL_PAR,PRF_PAR,PBEAM_PAR,BBETA_PAR,WAV_PAR,WMAX_PAR,XB_PAR,YB_PAR,
					ER_PAR,EX_PAR,EY_PAR,ENR_PAR,ENX_PAR,ENY_PAR,E4D_PAR,E4DN_PAR,ET_PAR,ENT_PAR} ;
enum TSplineType {ZSPLINE,LSPLINE,CSPLINE,SSPLINE};
enum TChartType {CH_EMITTANCE,CH_SECTION,CH_PORTRAIT,CH_PHASE,CH_ENERGY,CH_BETTA,CH_A,CH_B,CH_ELP,CH_ATT,CH_APP,CH_BEXT,CH_CLEAR};

enum TInputParameter {POWER,SOLENOID,BEAM,CURRENT,DRIFT,CELL,CELLS,OPTIONS,DUMP,COMMENT,UNDEFINED,SPCHARGE};
enum TTrig {SIN,COS,TG,CTG,SEC,CSC};
enum TDeviation {D_RMS,D_FWHM};
enum TLoss {LIVE,RADIUS_LOST,PHASE_LOST,BZ_LOST,BR_LOST,BTH_LOST,BETA_LOST,STEP_LOST};
enum TGraphType {TRANS_SEC,LONGT_SEC,TRANS_SPACE,LONGT_SPACE,LONGT_MOTION,PHASE_SLID,W_SPEC,F_SPEC,R_SPEC,
				R_TRACE,PHI_TRACE,W_TRACE,E_PLOT,EPS_PLOT,P_PLOT,W_PLOT,BETA_PLOT,R_PLOT,F_NONE};
enum TOptType {BUNCHER,ACCELERATOR};
enum TBeamType {NOBEAM,CST_PID,CST_PIT,TWISS_2D,TWISS_4D,SPH_2D,ELL_2D,FILE_1D,FILE_2D,TWO_FILES_2D,FILE_4D,NORM_1D,NORM_2D};

enum TImportType {NO_ELEMENT,ANALYTIC_0D,IMPORT_1D};
enum TSpaceChargeType {SPCH_NO,SPCH_ELL,SPCH_GW};

const int MaxParameters=14;  //Maximum number of parameters after a keyword. Currently: BEAM
const int NumBessel=6;
int Nslices=1;

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
   //	bool Magnetized;
};

struct TField
{
	double r;
	double th;
	double z;
};

struct TParticle
{
	double r;  //x/lmb (-Rb<x<Rb) - rename to r
	//double Cmag;
	double Th;
	TField beta;
  /*	double Br;
	double Bth;
	double Bz;    */
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
   int N1;
   int N2;
   bool PID;
   bool LiveOnly;
   bool Phase;
   bool Energy;
   bool Radius;
   bool Azimuth;
   bool Vx;
};
struct TCell
{
    double betta;
    double ELP;
    double AL32;
    double AkL;
    double Mode;
    double F0;
    double P0;
	double dF;
	int Mesh;
    bool Drift;
	bool First;
	bool Dump;
	TDump DumpParameters;
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
    double betta;
	double Ra;
	TField Hext;
   /*	double Bz_ext;
	double Br_ext;   */
    bool jump;
    bool drift;
	int CellNumber;
	bool Dump;
	TDump DumpParameters;
};

struct TMagnetParameters
{
	TImportType ImportType;
	double BField;
	double StartPos;
	double Length;
	AnsiString File;
};

struct TSectionParameters
{
	double Frequency;
	//double Wavelength;
	double Power;
	double PhaseShift;
	double NCells;
};

struct TStructureInput
{
	int NSections;
	int NElements;
	int ElementsLimit;
	TSectionParameters *Sections;
	TCell *Cells;
	TMagnetParameters SolenoidPar;
	bool Reverse;
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
	/*double Az;
    double Ar;
	double Hth;
	double br;
	double bth;
	double bz;   */
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
    double dA;
	double B;
	double E;
	TField Hext;
	//TField dHext;
	/*double Bz_ext;
	double Br_ext;  */
	double dH;
	//double Cmag;
    double SumSin;
	double SumCos;
	TField *Eq;
	/*double *Aqz;
	double *Aqr;  */
    double gamma;
    bool drift;
};

//---------------------------------------------------------------------------
#ifndef RSLINAC
inline int round(double x){
	return (x-floor(x))>(ceil(x)-x)?ceil(x):floor(x);
}
#endif
//---------------------------------------------------------------------------
inline double sqr(double x){
    return x*x;
}
//---------------------------------------------------------------------------
inline double cub(double x){
    return x*x*x;
}
//---------------------------------------------------------------------------
inline double arctg(double x){
    return atan(x);
}
//---------------------------------------------------------------------------
inline double tg(double x){
    return tan(x);
}
//---------------------------------------------------------------------------
inline double arc(double x){
    return x*pi/180;
}
//---------------------------------------------------------------------------
inline double ln(double x){
    return log(x);
}
//---------------------------------------------------------------------------
inline double log2(double x){
    return log(x)/log(2.);
}
//---------------------------------------------------------------------------
inline double GammaToMeV(double g){
    return We0*(g-1)*1e-6;;
}
//---------------------------------------------------------------------------
inline double MeVToGamma(double W){
    return 1+W*1e6/We0;
}
//---------------------------------------------------------------------------
inline double EnergyToVelocity(double g){
    return sqrt(1-1/sqr(g));
}
//---------------------------------------------------------------------------
inline double VelocityToEnergy(double b){
    return 1/sqrt(1-sqr(b));
}
//---------------------------------------------------------------------------
inline double VelocityToMeV(double b){
    return GammaToMeV(VelocityToEnergy(b));
}
//---------------------------------------------------------------------------
inline double MeVToVelocity(double W){
	return EnergyToVelocity(MeVToGamma(W));
}
//---------------------------------------------------------------------------
inline double MeVToBetaGamma(double W){
	return MeVToVelocity(W)*MeVToGamma(W);
}
//---------------------------------------------------------------------------
double BzFromOther(double beta,double beta_x,double beta_y){
	double beta_r2=sqr(beta_x)+sqr(beta_y);
	if (sqr(beta)>beta_r2)
		return sqrt(sqr(beta)-beta_r2);
	else
		return -1;
}
//---------------------------------------------------------------------------
inline double RadToDegree(double a){
    return a*180/pi;
}
//---------------------------------------------------------------------------
inline double DegreeToRad(double phi){
	return phi*pi/180;
}
//---------------------------------------------------------------------------
inline double mod(double x){
    return x>=0?x:-x;
}
//---------------------------------------------------------------------------
inline double sign(double x){
    if (x==0)
        return 0;
    else
        return x>0?1:-1;
}
//---------------------------------------------------------------------------
inline double PulseToAngle(double beta_x,double beta_z)
{
	return mod(beta_z>0)?arctg(beta_x/beta_z):sign(beta_x)*pi/2;
 /*   if (mod(bz)>1e-5)
        return arctg(bx/bz);
    else
		return sign(bx)*pi/2;    */

}
//---------------------------------------------------------------------------
inline int Fact(int n){
    int F=1;
    for (int i=1;i<=n;i++)
        F*=i;

    return F;
}
//---------------------------------------------------------------------------
inline double Pow(double x,int n){
    double F=1;

    for (int i=1;i<=n;i++)
        F*=x;

    return F;
}
//---------------------------------------------------------------------------
inline int CountSteps(double delta,double epsilon){
    return ceil(log2(delta/epsilon));
}
//---------------------------------------------------------------------------
inline double Ib0(double x){
    double f=0;

    for (int k=0;k<=NumBessel;k++)
        f+=Pow(x/2,2*k)/sqr(Fact(k));

    return f;
}
//---------------------------------------------------------------------------
inline double Ib1(double x){
	double f=0;

    for (int k=0;k<=NumBessel;k++)
        f+=Pow(x/2,2*k+1)/(Fact(k)*Fact(k+1));

    return f;
}
//---------------------------------------------------------------------------
void TwoRandom(double& Ran1,double& Ran2)
{
	Ran1=((double)rand() / ((double)RAND_MAX + 1));
	Ran2=((double)rand() / ((double)RAND_MAX + 1));

	if (Ran1<1e-5)
		Ran1=1e-5;
}
//---------------------------------------------------------------------------
void TwoRandomGauss(double& x1,double& x2)
{
	double Ran1=0;
	double Ran2=0;

	TwoRandom(Ran1,Ran2);

	x1=sqrt(-2*ln(Ran1))*cos(2*pi*Ran2);
	x2=sqrt(-2*ln(Ran1))*sin(2*pi*Ran2);
}
//---------------------------------------------------------------------------
double RandomGauss()
{
	double Ran1=0,Ran2=0,xx=0;
	TwoRandom(Ran1,Ran2);
	xx=sqrt(-2*ln(Ran1))*cos(2*pi*Ran2);
	return xx;
}
//---------------------------------------------------------------------------
double RandomRayleigh()
{
	double Ran1=0,xx=0;

	Ran1=((double)rand() / ((double)RAND_MAX + 1));

	if (Ran1<1e-5)
		Ran1=1e-5;

	xx=sqrt(-2*ln(1-Ran1));

	return xx;
}
//---------------------------------------------------------------------------
double RandomCos()
{
	double Ran1=0,xx=0;

	Ran1=((double)rand() / ((double)RAND_MAX + 1));

	if (Ran1<1e-5)
		Ran1=1e-5;

	xx=acos(sqrt(Ran1));

	return xx;
}
//---------------------------------------------------------------------------
inline double FormFactor(double p){
    double F1[21]={1.000,0.926,0.861,0.803,0.750,0.704,0.661,0.623,0.588,0.556,
                    0.527,0.500,0.476,0.453,0.432,0.413,0.394,0.378,0.362,0.347,0.333};
    double F2[21]={0.000,0.007,0.020,0.037,0.056,0.075,0.095,0.115,0.135,0.155,
                    0.174,0.192,0.210,0.227,0.244,0.260,0.276,0.291,0.306,0.320,0.333};
    double M=0;
    int i=0;

    if (p<1){
        i=floor(20*p);
        M=F1[i]-(F1[i]-F1[i+1])*(20*p-i);
    }
    else if (p==1)
        M=0.333;
    else if (p>1){
        p=1/p;
        i=floor(20*p);
        M=F2[i]+(F2[i+1]-F2[i])*(20*p-i);
    }

    return M;
}
//---------------------------------------------------------------------------
inline double GetH(double gamma,double phi,double bw,double A)
{
    double H=0;
    H=gamma/bw-sqrt(sqr(gamma)-1)-A*sin(DegToRad(phi))/(2*pi);
    return H;
}
//---------------------------------------------------------------------------
inline int GetSeparatrix(double &gamma,double phi,double bw,double A, double H,bool Neg=false)
{
    double Ah=0, b=0, c=0, D=0;
    int Nroots=0;

    Ah=A*sin(DegToRad(phi))/(2*pi)+H;
    b=Ah/(bw-1/bw);
    c=-(sqr(Ah)+1)/(1-sqr(1/bw));

    D=sqr(b)-c;

    if (D<0)
        Nroots=0;
    else if (D==0) {
        gamma=-b;
        Nroots=1;
    }else {
        if (Neg)
            gamma=-b-sqrt(D);
        else
            gamma=-b+sqrt(D);
        Nroots=2;
    }

    return Nroots;
}
//---------------------------------------------------------------------------
inline int GetPositiveSeparatrix(double &gamma,double phi,double bw,double A, double H)
{
	return GetSeparatrix(gamma,phi,bw,A,H);
}
//---------------------------------------------------------------------------
inline int GetNegativeSeparatrix(double &gamma,double phi,double bw,double A, double H)
{
	return GetSeparatrix(gamma,phi,bw,A,H,1);
}
//---------------------------------------------------------------------------
TPhaseSpace CylinricalToCartesian(double r, double th, double pr, double pth)
{
	TPhaseSpace R;
	double x=0,y=0,px=0,py=0;

	x=r*cos(th);
	y=r*sin(th);

	px=pr*cos(th)-pth*sin(th);
	py=pr*sin(th)+pth*cos(th);

	R.x=x;
	R.y=y;
	R.px=px;
	R.py=py;

	return R;
}
//---------------------------------------------------------------------------
TPhaseSpace CylinricalToCartesian(TPhaseSpace C)
{
	return CylinricalToCartesian(C.x,C.y,C.px,C.py);
}
//---------------------------------------------------------------------------
TPhaseSpace CartesianToCylinrical(double x, double y, double px, double py)
{
	TPhaseSpace C;
	double r=0,th=0,pr=0,pth=0;

	r=sqrt(sqr(x)+sqr(y));
	if (y==0 && x==0)
		th=0;
	else
		th=atan2(y,x);
   /*	th=x==0?pi/2:arctg(y/x);

	if (x<0)
		th+=pi;  */

	pr=px*cos(th)+py*sin(th);
	pth=-px*sin(th)+py*cos(th);

	C.x=r;
	C.y=th;
	C.px=pr;
	C.py=pth;

	return C;
}
//---------------------------------------------------------------------------
TPhaseSpace CartesianToCylinrical(TPhaseSpace R)
{
	return CartesianToCylinrical(R.x,R.y,R.px,R.py);
}
//---------------------------------------------------------------------------
void DeleteArray(double *X)
{
	if (X!=NULL) {
        delete[] X;
	}
}
//---------------------------------------------------------------------------
double **DeleteDoubleArray(double **X,int Nx)
{
	for (int j = 0; j < Nx; j++)
			delete[] X[j];
	delete[] X;
	X=NULL;
	return X;
}
//---------------------------------------------------------------------------
//--------------------TEXT FILES---------------------------------------------
//---------------------------------------------------------------------------
bool IsNumber(AnsiString &S)   //Checks if the string is a number
{
	double x=0;
	bool Success=false;

	try {                  //Check if the data is really a number
		x=S.ToDouble();
		Success=true;
	}
	catch (...){
		Success=false;
	}
	return Success;
}
//---------------------------------------------------------------------------
bool CheckFile(AnsiString &F)   //Checks if the file exists
{
	bool Exists=false;
	std::ifstream f(F.c_str());

	Exists=f.good();

	f.close();

	return Exists;
}
//---------------------------------------------------------------------------
AnsiString GetWord(ifstream &f)   //Reads the next word from fstream
{
   AnsiString S;
   char s[MAX_CHAR];

   f>>s;
   S=AnsiString(s);

   return S;
}
//---------------------------------------------------------------------------
AnsiString GetLine(ifstream &f)   //Reads the next line from fstream
{
   AnsiString S;
   char s[MAX_CHAR];

   f.getline(s, sizeof(s)) ;
   S=AnsiString(s);

   return S;
}
//---------------------------------------------------------------------------
int NumWords(AnsiString &L)  //Counts number of words in the line
{
	int i=0,Nmax=0,N=0;
	bool Space=true; //Several successive spaces

	Nmax=L.Length();

	while (i<Nmax){
		i++;
		if (L[i]==' ' || L[i]=='\t'){
			if (!Space) //Treat successive spaces as a single
				N++;
			Space=true;
		} else
			Space=false;
	}

	if (!Space)
		N++;

	return N;
}
//---------------------------------------------------------------------------
AnsiString ReadWord(AnsiString &L,int N)   //Parses the Nth word from the string
{
	AnsiString S="";
	int i=1,Nmax=0,W=0;

	Nmax=L.Length();

	while (i<Nmax+1){
		if (L[i]==' ' || L[i]=='\t'){
			if (S==""){  //Treat successive spaces as a single
				i++;
				continue;
			} else{
				W++;
				if (W!=N){  //Not the word we searched
					S="";
					i++;
					continue;
				} else
					break;
			}
		}  else {
			S+=L[i];
			i++;
        }
	}

	return S;
}
//---------------------------------------------------------------------------
int NumPointsInFile(AnsiString &F,int NumRow)
{
	//Check number of valid lines in text file F.
	//Line is valid if number of elements in a row == NumRow and they are numbers
	int N=0;
	AnsiString L,S;
	double x;
	bool Num=false;
	std::ifstream fs(F.c_str());

	while (!fs.eof()){
		L=GetLine(fs);
		if (NumWords(L)==NumRow){       //Check if only two numbers in the line
			for (int i = 0; i < NumRow; i++) {
				S=ReadWord(L,i+1);
				Num=IsNumber(S);
				if (!Num)
					break;
			}
			if (Num)
				N++;
		}
	}
    fs.close();

	return N;
}

};
//---------------------------------------------------------------------------
#endif      // TypesH
