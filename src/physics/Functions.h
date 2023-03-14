//---------------------------------------------------------------------------

#ifndef FunctionsH
#define FunctionsH

#include "Types.h"
#include <cstring>

//#endif

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
inline double cubrt(double x){
    return x>0?pow(x,1.0/3.0):-pow(-x,1.0/3.0);
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
inline double asinh(double x){
	return ln(x+sqrt(sqr(x)+1));
}
//---------------------------------------------------------------------------
inline double acosh(double x){
	return ln(x+sqrt(sqr(x)-1));
}
//---------------------------------------------------------------------------
inline double atanh(double x){
	return 0.5*ln((1+x)/(1-x));
}
//---------------------------------------------------------------------------
inline double GetRestEnergy(TParticlesSpecies R){
	double W0=0;
	switch (R.Type){
		case PROTON :{W0=Wp0;break;}
		case ION :{W0=Wu0;break;}
		case ELECTRON:{}
		default:{W0=We0;break;}
	}
	return W0;
}
//---------------------------------------------------------------------------
static AnsiString GetEnergyUnit(TParticleType P){
	AnsiString S;
	if (P==ION)
		S="MeV/u";
	else
		S="MeV";
	return S;
}
//---------------------------------------------------------------------------
inline double GammaToMeV(double g, double W0){
	return W0*(g-1)*1e-6;
}
//---------------------------------------------------------------------------
inline double MeVToGamma(double W, double W0){
	return 1+W*1e6/W0;
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
inline double VelocityToMeV(double b, double W0){
	return GammaToMeV(VelocityToEnergy(b),W0);
}
//---------------------------------------------------------------------------
inline double MeVToVelocity(double W, double W0){
	return EnergyToVelocity(MeVToGamma(W,W0));
}
//---------------------------------------------------------------------------
inline double MeVToBetaGamma(double W, double W0){
	return MeVToVelocity(W,W0)*MeVToGamma(W,W0);
}
//---------------------------------------------------------------------------
inline double BzFromOther(double beta,double beta_x,double beta_y){
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
	return mod(beta_z) > 0? arctg(beta_x/beta_z) : sign(beta_x)*pi/2;
}
//---------------------------------------------------------------------------
inline void CompressPhase(double &phi)
{
	int phase_dig=-DigitConst*phi;
	int max_dig=DigitConst*2*pi;
	int phase_trunc=phase_dig%max_dig;
	phi=(1.0*phase_trunc)/DigitConst;
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
inline double Ib0_beta(double r,double b2){  //I0(r*x)
        double f=0;

        for (int k=0;k<=NumBessel;k++)
                //f+=Pow(sqr(r)*b2/2,k)/sqr(Fact(k));
		f+=Pow(sqr(r)*b2/4,k)/sqr(Fact(k));

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
inline double Ib1_beta(double r,double b2){   //I1(r*x)/x
        double f=0;

        for (int k=0;k<=NumBessel;k++)
                f+=Pow(r/2,2*k+1)*Pow(b2,k)/(Fact(k)*Fact(k+1));

        return f;
}
//---------------------------------------------------------------------------
inline void TwoRandom(double& Ran1,double& Ran2)
{
	Ran1=((double)rand() / ((double)RAND_MAX + 1));
	Ran2=((double)rand() / ((double)RAND_MAX + 1));

	if (Ran1<1e-5)
		Ran1=1e-5;
}
//---------------------------------------------------------------------------
inline void TwoRandomGauss(double& x1,double& x2)
{
	double Ran1=0;
	double Ran2=0;

	TwoRandom(Ran1,Ran2);

	x1=sqrt(-2*ln(Ran1))*cos(2*pi*Ran2);
	x2=sqrt(-2*ln(Ran1))*sin(2*pi*Ran2);
}
//---------------------------------------------------------------------------
inline double RandomGauss()
{
	double Ran1=0,Ran2=0,xx=0;
	TwoRandom(Ran1,Ran2);
	xx=sqrt(-2*ln(Ran1))*cos(2*pi*Ran2);
	return xx;
}
//---------------------------------------------------------------------------
inline double RandomRayleigh()
{
	double Ran1=0,xx=0;

	Ran1=((double)rand() / ((double)RAND_MAX + 1));

	if (Ran1<1e-5)
		Ran1=1e-5;

	xx=sqrt(-2*ln(1-Ran1));

	return xx;
}
//---------------------------------------------------------------------------
inline double RandomCos()
{
	double Ran1=0,xx=0;

	Ran1=((double)rand() / ((double)RAND_MAX + 1));

	if (Ran1<1e-5)
		Ran1=1e-5;

	xx=acos(sqrt(Ran1));

	return xx;
}
//---------------------------------------------------------------------------
/*inline double FormFactorLpst(double p){
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
}      */
//---------------------------------------------------------------------------
inline double FormFactorLpst(double p)
{
	double M=0;

	if (p>1) {
		M=(1-acosh(p)*p/sqrt(sqr(p)-1))/(sqr(p)-1);
	} else if (p<1) {
		M=(1-acos(p)*p/sqrt(1-sqr(p)))/(sqr(p)-1);
	} else
		M=-1.0/3.0;

	return -M;
}//
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
inline TPhaseSpace CylinricalToCartesian(double r, double th, double pr, double pth)
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
inline TPhaseSpace CylinricalToCartesian(TPhaseSpace C)
{
	return CylinricalToCartesian(C.x,C.y,C.px,C.py);
}
//---------------------------------------------------------------------------
inline TPhaseSpace CartesianToCylinrical(double x, double y, double px, double py)
{
	TPhaseSpace C;
	double r=0, th=0, pr=0, pth=0;

	r = sqrt(sqr(x)+sqr(y));
	if (y==0 && x==0)
		th=0;
	else
		th = atan2(y,x);
   /*	th=x==0?pi/2:arctg(y/x);

	if (x<0)
		th+=pi;  */

	pr  =  px*cos(th) +py*sin(th);
	pth = -px*sin(th) +py*cos(th);

	C.x = r;
	C.y = th;
	C.px = pr;
	C.py = pth;

	return C;
}
//---------------------------------------------------------------------------
inline TPhaseSpace CartesianToCylinrical(TPhaseSpace R)
{
	return CartesianToCylinrical(R.x,R.y,R.px,R.py);
}
//---------------------------------------------------------------------------
inline void DeleteArray(double *X)
{
	if (X!=NULL) {
        delete[] X;
	}
}
//---------------------------------------------------------------------------
inline double **DeleteDoubleArray(double **X,int Nx)
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
static bool IsNumber(AnsiString &S)   //Checks if the string is a number
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
static AnsiString GetFileName(AnsiString &F)   //Gets the file name from path
{
    std::string s = std::string(F.c_str());
    return s.substr(s.find_last_of("/\\") + 1);
}
//---------------------------------------------------------------------------
static AnsiString GetFileCaption(AnsiString &F)   //Gets the file name without extension
{
    std::string s = std::string(F.c_str());
	return s.substr(0,s.find_last_of("/.")-1);
}
//---------------------------------------------------------------------------
static bool CheckFile(AnsiString &F)   //Checks if the file exists
{
	bool Exists=false;
	std::ifstream f(F.c_str());

	Exists=f.good();

	f.close();

	return Exists;
}
//---------------------------------------------------------------------------
static AnsiString GetWord(ifstream &f)   //Reads the next word from fstream
{
   AnsiString S;
   char s[MAX_CHAR];

   f>>s;
   S=AnsiString(s);

   return S;
}
//---------------------------------------------------------------------------
static AnsiString GetLine(ifstream &f)   //Reads the next line from fstream
{
	AnsiString S;
	char s[MAX_CHAR];

	do {
		if (f.eof()) {
			return AnsiString("");
		}
		f.getline(s, sizeof(s)) ;
		S=AnsiString(s);
	} while(S.IsEmpty() || S==" ");

	return S;
}
//---------------------------------------------------------------------------
static int NumWords(AnsiString &L)  //Counts number of words in the line
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
static AnsiString ReadWord(AnsiString &L,int N)   //Parses the Nth word from the string
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
static int NumPointsInFile(AnsiString &F,int NumRow)
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
//---------------------------------------------------------------------------
static int CountUnique(double *X, int N)
{
	int Nunq=0;

	for (int i = 0; i < N; i++) {
		bool unique=true;
		for (int j = 0; j < i; j++) {
			if (X[j]==X[i]){
				unique=false;
				break;
			}
		}
		if (unique)
			Nunq++;
	}

	return Nunq;
}
//---------------------------------------------------------------------------
static void SortArray(double *X, int Nmax, bool Desc=false)
{
	double x=0;
	for (int i = Nmax-1; i>=0; i--) {
		for (int j = 0; j < i; j++) {
			if (X[j]>X[i]) {
				x=X[i];
				X[i]=X[j];
				X[j]=x;
			}
		}
	}
}
//---------------------------------------------------------------------------
static double *MakePivot(double *X, int Nmax, int Npiv)
{
	int Nunq=0;
	double *Piv=new double[Npiv];

	for (int i = 0; i < Nmax; i++) {
		bool unique=true;
		for (int j = 0; j < i; j++) {
			if (X[j]==X[i]){
				unique=false;
				break;
			}

		}
		if (unique){
			Piv[Nunq]=X[i];
			Nunq++;
		}
	}

	SortArray(Piv,Nunq);

	return Piv;
}
//---------------------------------------------------------------------------
static TField LinInterpolation(double x,double *X,int Nx,TField *A)
{
	int kx=0;
	TField F;

  /*	if (x<=X[0]){
		x=X[0];
		kx=0;
	}else if (x>=X[Nx-1]){
		x=X[Nx-1];
		kx=Nx-2;  */
	if (x<X[0] || x>X[Nx-1]){
		F.r=0;
		F.th=0;
		F.z=0;
	}else {
		for (int i = 1; i < Nx; i++) {
			if (x<X[i]) {
				kx=i-1;
				break;
			}
		}


	double x1=0,x2=0,xx=0;
	x2=X[kx+1]-x;
	x1=x-X[kx];
	xx=X[kx+1]-X[kx];

	F.r=A[kx].r*x2+A[kx+1].r*x1;
	F.r/=xx;

	F.th=A[kx].th*x2+A[kx+1].th*x1;
	F.th/=xx;

	F.z=A[kx].z*x2+A[kx+1].z*x1;
	F.z/=xx;
	}

	return F;
}
//---------------------------------------------------------------------------
static TField BiLinearInterpolation(double x,double y,double *X,double *Y,int Nx,int Ny, TField **A)
{
	int kx=0, ky=0;
	TField F;

	if (x<=X[0]){
		x=X[0];
		kx=0;
	}else if (x>=X[Nx-1]){
		x=X[Nx-1];
		kx=Nx-2;
	}else {
		for (int i = 1; i < Nx; i++) {
			if (x<X[i]) {
				kx=i-1;
				break;
			}
		}
	}

	if (y<=Y[0]){
		y=Y[0];
		ky=0;
	}else if (y>=Y[Ny-1]){
		y=Y[Ny-1];
		ky=Ny-2;
	}else {
		for (int i = 1; i < Ny; i++) {
			if (y<Y[i]) {
				ky=i-1;
				break;
			}
		}
	}

	double x1=0,x2=0,y1=0,y2=0,xy=0;
	x2=X[kx+1]-x;
	x1=x-X[kx];
	y2=Y[ky+1]-y;
	y1=y-Y[ky];
	xy=(X[kx+1]-X[kx])*(Y[ky+1]-Y[ky]);

	F.r=A[kx][ky].r*x2*y2+A[kx+1][ky].r*x1*y2+A[kx][ky+1].r*x2*y1+A[kx+1][ky+1].r*x1*y1;
	F.r/=xy;

	F.th=A[kx][ky].th*x2*y2+A[kx+1][ky].th*x1*y2+A[kx][ky+1].th*x2*y1+A[kx+1][ky+1].th*x1*y1;
	F.th/=xy;

	F.z=A[kx][ky].z*x2*y2+A[kx+1][ky].z*x1*y2+A[kx][ky+1].z*x2*y1+A[kx+1][ky+1].z*x1*y1;
	F.z/=xy;

	return F;
}
//---------------------------------------------------------------------------
#endif
