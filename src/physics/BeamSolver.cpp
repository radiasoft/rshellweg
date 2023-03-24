//---------------------------------------------------------------------------


#pragma hdrstop

#include "BeamSolver.h"

// #include "Types.h"

//---------------------------------------------------------------------------
__fastcall TBeamSolver::TBeamSolver(AnsiString UserIniPath)
{
	this->UserIniPath = UserIniPath;
	Initialize();
}
//---------------------------------------------------------------------------
__fastcall TBeamSolver::TBeamSolver()
{
    Initialize();
}
//---------------------------------------------------------------------------
__fastcall TBeamSolver::~TBeamSolver()
{
	ResetStructure();

	delete[] Structure;

    for (int i=0;i<Npoints;i++)
        delete Beam[i];
    delete[] Beam;

    delete[] Par;
    for (int i=0;i<Ncoef;i++)
        delete[] K[i];
	delete[] K;

    if (BeamExport!=NULL){
		delete[] BeamExport;
		BeamExport=NULL;
	}

    #ifndef RSHELLWEG_LINUX
	if (SmartProgress!=NULL)
        delete SmartProgress;
    #endif

    delete InputStrings;
    delete ParsedStrings;

	//fclose(logFile);
}
//---------------------------------------------------------------------------
void TBeamSolver::Initialize()
{
	MaxCells=DEFAULT_MAX_CELLS;
    Nmesh=DEFAULT_MESH;
	//Kernel=0;
    SplineType=LSPLINE;
    Nstat=100;
    //Ngraph=500;
	//Nbars=100; //default
	//Nav=10;
	Smooth=DEFAULT_SMOOTH;
	Npoints=0;
	Ndump=0;

  /*	Nrmesh=20;
	Nthmesh=36;    */

	//Np=1;
   //	NpEnergy=0;
	Np_beam=1;
	BeamPar.RBeamType=NOBEAM;
	BeamPar.ZBeamType=NOBEAM;
	BeamPar.NParticles=1;
	BeamPar.Demagnetize=false;
	BeamPar.Species.Type=ELECTRON;
	BeamPar.Species.A=1;
	BeamPar.Species.Q=1;
	BeamPar.W0=GetRestEnergy(BeamPar.Species);
	BeamPar.Wnorm=(BeamPar.Species.A/BeamPar.Species.Q)*BeamPar.W0;

	StructPar.NSections=0;
	StructPar.Sections=NULL;
	StructPar.NElements=0;
	StructPar.ElementsLimit=-1;
	StructPar.NStructData=0;
	StructPar.NMaps=0;
	StructPar.StructData=NULL;
	StructPar.Reverse=false;

	ExternalMagnetic.Dim.Nz=0;
	ExternalMagnetic.Dim.Nx=0;
	ExternalMagnetic.Dim.Ny=0;
	ExternalMagnetic.Field=NULL;
	ExternalMagnetic.Piv.X=NULL;
	ExternalMagnetic.Piv.Y=NULL;
	ExternalMagnetic.Piv.Z=NULL;

	QuadMaps=NULL;

	LoadIniConstants();

	DataReady=false;
	SolverStop=ERR_NO;

	K=new TIntegration*[Ncoef];
	for (int i=0;i<Ncoef;i++)
		K[i]=new TIntegration[BeamPar.NParticles];

	Par=new TIntParameters[Ncoef];

	StructPar.Cells = new TCell[1];

	Beam=NULL;
	Structure=NULL;

 /*   Beam=new TBeam*[Npoints];
    for (int i=0;i<Npoints;i++) {
        Beam[i]=new TBeam(1);
    }

    Structure=new TStructure[Npoints];  */

    InputStrings=new TStringList;
	ParsedStrings=new TStringList;

	BeamExport=NULL;

    #ifndef RSHELLWEG_LINUX
    SmartProgress=NULL;
    #endif
}
//---------------------------------------------------------------------------
void TBeamSolver::ResetStructure()
{
	StructPar.NSections=0;
	if (StructPar.Sections!=NULL) {
		delete[] StructPar.Sections;
		StructPar.Sections=NULL;
	}
	StructPar.NElements=0;
	if (StructPar.Cells!=NULL) {
		delete[] StructPar.Cells;
		StructPar.Cells=NULL;
	}

	ResetExternal();
	ResetMaps();
	ResetStructData();
}
//---------------------------------------------------------------------------
void TBeamSolver::DeleteStructData(TStructData &D)
{

	if (D.ELP!=NULL) {
		for (int i = 0; i < D.N_bph; i++)
			delete[] D.ELP[i];
	}
	if (D.AL32!=NULL) {
		for (int i = 0; i < D.N_bph; i++)
			delete[] D.AL32[i];
	}
	if (D.Bph!=NULL)
		delete [] D.Bph ;
	if (D.AKL!=NULL)
		delete [] D.AKL ;

	D.Bph=NULL;
	D.AKL=NULL;
	D.ELP=NULL;
	D.AL32=NULL;

	D.N_bph=0;
	D.N_akl=0;
	D.Phase=-1;
	D.Default=false;

}
//---------------------------------------------------------------------------
void TBeamSolver::ResetStructData()
{
	if (StructPar.StructData!=NULL) {
		for (int i = 0; i < StructPar.NStructData; i++)
			DeleteStructData(StructPar.StructData[i]);
	}
	StructPar.NStructData=0;
	delete [] StructPar.StructData;
	StructPar.StructData=NULL;
}
//---------------------------------------------------------------------------
void TBeamSolver::ResetMaps()
{
	if (QuadMaps!=NULL) {
		for (int i = 0; i < StructPar.NMaps; i++) {
			for (int j = 0; j < QuadMaps[i].Dim.Nx; j++) {
				delete[] QuadMaps[i].Field[j];

			}

			delete[] QuadMaps[i].Field;
			delete[] QuadMaps[i].Piv.X;
			delete[] QuadMaps[i].Piv.Y;

		}
		delete[] QuadMaps;
		QuadMaps=NULL;
		StructPar.NMaps=0;
	}
}
//---------------------------------------------------------------------------
void TBeamSolver::ResetExternal()
{
	if (ExternalMagnetic.Field!=NULL) {
		if (ExternalMagnetic.Dim.Nz!=0) {
			for (int i = 0; i < ExternalMagnetic.Dim.Nz; i++) {
				if (ExternalMagnetic.Dim.Ny!=0) {
					for (int j = 0; j < ExternalMagnetic.Dim.Ny; j++) {
						if (ExternalMagnetic.Dim.Nx!=0) {
							delete[] ExternalMagnetic.Field[i][j];
						}
					}
					delete[] ExternalMagnetic.Field[i];
				}
			}
			delete[] ExternalMagnetic.Field;
			ExternalMagnetic.Dim.Nz=0;
			ExternalMagnetic.Dim.Nx=0;
			ExternalMagnetic.Dim.Ny=0;
			ExternalMagnetic.Field=NULL;
		}
	}

	if (ExternalMagnetic.Piv.X!=NULL) {
		delete[] ExternalMagnetic.Piv.X;
		ExternalMagnetic.Piv.X=NULL;
	}
	if (ExternalMagnetic.Piv.Y!=NULL) {
		delete[] ExternalMagnetic.Piv.Y;
		ExternalMagnetic.Piv.Y=NULL;
	}
	if (ExternalMagnetic.Piv.Z!=NULL) {
		delete[] ExternalMagnetic.Piv.Z;
		ExternalMagnetic.Piv.Z=NULL;
	}
}
//---------------------------------------------------------------------------
void TBeamSolver::ResetDump(int Ns)
{
	if (BeamExport!=NULL){
		delete[] BeamExport;
		BeamExport=NULL;
	}

	Ndump=Ns;

	if (Ns>0) {
		BeamExport=new TDump[Ndump];
	}
}
//---------------------------------------------------------------------------
void TBeamSolver::ShowError(AnsiString &S)
{
	 /*   #ifndef RSHELLWEG_LINUX
	ShowMessage(S);
        #endif           */
	ParsedStrings->Add(S);
}
//---------------------------------------------------------------------------
void TBeamSolver::SaveToFile(AnsiString& Fname)
{
    FILE *F;
    F=fopen(Fname.c_str(),"wb");

    fwrite(&Npoints,sizeof(int),1,F);
	fwrite(&BeamPar.NParticles,sizeof(int),1,F);
	//fwrite(&Nbars,sizeof(int),1,F);

	fwrite(Structure,sizeof(TStructure),Npoints,F);

    for(int i=0;i<Npoints;i++){
        fwrite(&(Beam[i]->lmb),sizeof(double),1,F);
		fwrite(&(Beam[i]->h),sizeof(double),1,F);
		double Ib=Beam[i]->GetCurrent();
		double I0=Beam[i]->GetInputCurrent();
		fwrite(&(Ib),sizeof(double),1,F);
		fwrite(&(I0),sizeof(double),1,F);
		fwrite(Beam[i]->Particle,sizeof(TParticle),BeamPar.NParticles,F);
    }

	fclose(F);
}
//---------------------------------------------------------------------------
bool TBeamSolver::LoadFromFile(AnsiString& Fname)
{
    FILE *F;
    F=fopen(Fname.c_str(),"rb");
    bool Success;

    delete[] Structure;

    if (Beam) {
        for (int i=0;i<Np_beam;i++)
            delete Beam[i];
        delete[] Beam;
    }

    Beam=new TBeam*[Npoints];
    for (int i=0;i<Npoints;i++)
		Beam[i]=new TBeam(BeamPar.NParticles);
	Np_beam=Npoints;

    try{
        fread(&Npoints,sizeof(int),1,F);
		fread(&BeamPar.NParticles,sizeof(int),1,F);
	  //  fread(&Nbars,sizeof(int),1,F);

        Structure=new TStructure[Npoints];
        Beam=new TBeam*[Npoints];
        for (int i=0;i<Npoints;i++)
			Beam[i]=new TBeam(BeamPar.NParticles);

        fread(Structure,sizeof(TStructure),Npoints,F);
      //    fread(Beam,sizeof(TBeam),Npoints,F);


        for (int i=0;i<Npoints;i++){
            fread(&(Beam[i]->lmb),sizeof(double),1,F);
			fread(&(Beam[i]->h),sizeof(double),1,F);
			double Ib=0, I0=0;
			fread(&(Ib),sizeof(double),1,F);
			fread(&(I0),sizeof(double),1,F);
			Beam[i]->SetCurrent(Ib);
            Beam[i]->SetInputCurrent(I0);
			fread(Beam[i]->Particle,sizeof(TParticle),BeamPar.NParticles,F);
        }
        Success=true;
    } catch (...){
        Success=false;
    }

    fclose(F);
    return Success;
}
//---------------------------------------------------------------------------
#ifndef RSHELLWEG_LINUX
void TBeamSolver::AssignSolverPanel(TObject *SolverPanel)
{
    SmartProgress=new TSmartProgress(static_cast <TWinControl *>(SolverPanel));
}
#endif
//---------------------------------------------------------------------------
void TBeamSolver::LoadIniConstants()
{
    TIniFile *UserIni;
    int t;
	//double stat;

	UserIni=new TIniFile(UserIniPath);

	MaxCells=UserIni->ReadInteger("NUMERIC","Maximum Cells",DEFAULT_MAX_CELLS);
	Nmesh=UserIni->ReadInteger("NUMERIC","Number of Mesh Points",DEFAULT_MESH);
  /*	Kernel=UserIni->ReadFloat("NUMERIC","Percent Of Particles in Kernel",Kernel);
    if (Kernel>0)
    	Kernel/=100;
    	else
		Kernel=0.9;      */
	Smooth=UserIni->ReadFloat("NUMERIC","Smoothing Factor",Smooth);

    t=UserIni->ReadInteger("NUMERIC","Spline Interpolation",t);
    switch (t) {
        case (0):{
			SplineType=LSPLINE;
            break;
        }
        case (1):{
			SplineType=CSPLINE;
            break;
        }
        case (2):{
            SplineType=SSPLINE;
            break;
        }
	}

 /*	stat=UserIni->ReadFloat("RESULTS","Statistics Error",stat);
    if (stat<1e-6)
        stat=1e-6;
    if (stat>25)
        stat=25;
	int Nstat=round(100.0/stat);
	AngErr=UserIni->ReadFloat("RESULTS","Angle Error",AngErr);  */
	//Ngraph=UserIni->ReadInteger("RESULTS","Chart Points",Ngraph);
	//Nbars=UserIni->ReadInteger("NUMERIC","Hystogram Bars",Ngraph);
	//Nav=UserIni->ReadInteger("RESULTS","Averaging Points",Nav);
}
//---------------------------------------------------------------------------
int TBeamSolver::GetNumberOfPoints()
{
    return Npoints;
}
/*//---------------------------------------------------------------------------
double TBeamSolver::GetWaveLength()
{
    return lmb;
} */
//---------------------------------------------------------------------------
int TBeamSolver::GetMeshPoints()
{
    return Nmesh;
}
//---------------------------------------------------------------------------
int TBeamSolver::GetNumberOfParticles()
{
	return BeamPar.NParticles;
}
//---------------------------------------------------------------------------
/*int TBeamSolver::GetNumberOfChartPoints()
{
	return Ngraph;
}    */
//---------------------------------------------------------------------------
/*int TBeamSolver::GetNumberOfBars()
{
	return Nbars;
}   */
//---------------------------------------------------------------------------
TParticleType TBeamSolver::GetParticleType()
{
	return BeamPar.Species.Type;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetParticleMass()
{
	return BeamPar.Species.A;
}
//---------------------------------------------------------------------------
int TBeamSolver::GetParticleCharge()
{
	return BeamPar.Species.Q;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetParticleRestEnergy()
{
	return BeamPar.W0;
}
//---------------------------------------------------------------------------
int TBeamSolver::GetNumberOfCells()
{
	return StructPar.NElements;
}
//---------------------------------------------------------------------------
int TBeamSolver::GetNumberOfSections()
{
	return StructPar.NSections;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetInputCurrent()
{
	return BeamPar.Current;
}
//---------------------------------------------------------------------------
TGauss TBeamSolver::GetInputEnergy()
{
	return GetEnergyStats(0);
}
//---------------------------------------------------------------------------
TGauss TBeamSolver::GetInputPhase()
{
	return GetPhaseStats(0);
}
//---------------------------------------------------------------------------
double TBeamSolver::GetInputWavelength()
{
	return GetWavelength(0);
}
//---------------------------------------------------------------------------
TTwiss TBeamSolver::GetInputTwiss(TBeamParameter P)
{
	return GetTwiss(0,P);
}
//---------------------------------------------------------------------------
/*bool TBeamSolver::CheckMagnetization()
{
	return BeamPar.Magnetized;
}    */
//---------------------------------------------------------------------------
bool TBeamSolver::CheckReverse()
{
	return StructPar.Reverse;
}
//---------------------------------------------------------------------------
bool TBeamSolver::CheckDrift(int Nknot)
{
	return Structure[Nknot].drift;
}
//---------------------------------------------------------------------------
TSpaceCharge TBeamSolver::GetSpaceChargeInfo()
{
	return BeamPar.SpaceCharge;
}
//---------------------------------------------------------------------------
TMagnetParameters TBeamSolver::GetSolenoidInfo()
{
	return StructPar.SolenoidPar;
}
//---------------------------------------------------------------------------
bool TBeamSolver::IsKeyWord(AnsiString &S)
{
    const AnsiString u = S.UpperCase();

    return u=="POWER" ||
        u=="SOLENOID" ||
        u=="BEAM" ||
        u=="CURRENT" ||
		u=="DRIFT" ||
		u=="QUAD" ||
        u=="CELL" ||
		u=="CELLS" ||
		u=="SAVE" ||
		u=="OPTIONS" ||
		u=="STRUCT" ||
		u=="SPCHARGE" ||
		u=="PARTICLES";
	  //	u=="COMMENT";
}
//---------------------------------------------------------------------------
TBeamType TBeamSolver::ParseDistribution(AnsiString &S)
{
	const AnsiString u = S.UpperCase();
	 TBeamType D=NOBEAM;
		if (u=="CST_PID")
			D=CST_PID;
		else if (u=="CST_PIT")
			D=CST_PIT;
		else if (u=="TWISS2D")
			D=TWISS_2D;
		else if (u=="TWISS4D")
			D=TWISS_4D;
		else if (u=="SPH2D")
			D=SPH_2D;
		else if (u=="ELL2D")
			D=ELL_2D;
		else if (u=="FILE1D")
			D=FILE_1D;
		else if (u=="FILE2D")
			D=FILE_2D;
		else if (u=="FILE4D")
			D=FILE_4D;
		else if (u=="NORM2D")
			D=NORM_2D;
		else if (u=="PARMELA_T2")
			D=PARMELA_T2;
		/*else if (u=="NORM4D")
			D=NORM_4D;  */
	 return D;
}
//---------------------------------------------------------------------------
bool TBeamSolver::IsFullFileKeyWord(TBeamType D)
{
	bool R=false;

	switch (D) {
    	case PARMELA_T2:{}
		case CST_PID:{}
		case CST_PIT:{R=true;break;}
		default: R=false;
	}
	return R;
}
//---------------------------------------------------------------------------
bool TBeamSolver::IsTransverseKeyWord(TBeamType D)
{
	bool R=false;

	switch (D) {
		case TWISS_2D:{}
		case TWISS_4D:{}
		case FILE_2D:{}
		case FILE_4D:{}
		case TWO_FILES_2D:{}
		case SPH_2D:{}
		case ELL_2D:{R=true;break;}
		default: R=false;
	}
	return R;
}
//---------------------------------------------------------------------------
bool TBeamSolver::IsLongitudinalKeyWord(TBeamType D)
{
	bool R=false;

	switch (D) {
		case FILE_1D:{}
		case FILE_2D:{}
		case NORM_2D:{R=true;break;}
		default: R=false;
	}
	return R;
}
//---------------------------------------------------------------------------
bool TBeamSolver::IsFileKeyWord(TBeamType D)
{
	bool R=false;

	switch (D) {
		case CST_PID:{}
		case CST_PIT: {}
		case PARMELA_T2:{}
		case FILE_1D:{}
		case FILE_2D:{}
		case TWO_FILES_2D:{}
		case FILE_4D:{R=true;break;}
		default: R=false;
	}
	return R;
}
//---------------------------------------------------------------------------
bool TBeamSolver::IsImportType(TImportType T)
{
	bool R=false;

	switch (T) {
		case IMPORT_1D:{}
		case IMPORT_2DC:{}
		case IMPORT_2DR:{}
		case IMPORT_3DC:{}
		case IMPORT_3DR:{R=true; break;}
		default: R=false;
	}
	return R;
}
//---------------------------------------------------------------------------
TInputParameter TBeamSolver::Parse(AnsiString &S)
{
    const AnsiString u = S.UpperCase();
    TInputParameter P;
   // FILE *logFile;

/*    logFile=fopen("BeamSolver.log","a");
	fprintf(logFile,"Parse: S=%s\n",S);
	fclose(logFile); */
	if (u=="POWER")
		P=POWER;
    else if (u=="SOLENOID")
        P=SOLENOID;
    else if (u=="BEAM")
        P=BEAM;
    else if (u=="CURRENT")
		P=CURRENT;
    else if (u=="DRIFT")
		P=DRIFT;
	else if (u=="QUAD")
		P=QUAD;
    else if (u=="CELL")
        P=CELL;
    else if (u=="CELLS")
        P=CELLS;
    else if (u=="OPTIONS")
		P=OPTIONS;
	else if (u=="SAVE")
		P=DUMP;
    else if (u=="SPCHARGE")
		P=SPCHARGE;
	else if (u=="STRUCT")
		P=STRUCT;
	else if (u=="PARTICLES")
		P=PARTICLES;
	else if (u[1]=='!')
		P=COMMENT;
	return  P;
}
//---------------------------------------------------------------------------
TSpaceChargeType TBeamSolver::ParseSpchType(AnsiString &S)
{
	const AnsiString u = S.UpperCase();
	TSpaceChargeType T;;

	if (u=="COULOMB")
		T=SPCH_LPST;
	else if (u=="ELLIPTIC")
		T=SPCH_ELL;
	else if (u=="GWMETHOD")
		T=SPCH_GW;
	else
		T=SPCH_NO;

	return T;
}
//---------------------------------------------------------------------------
void TBeamSolver::GetDimensionsFromTab(TCell& Cell)
{

	int Nbp=0,Nep=0;
    int Nar=0,Nab=0;
	int Mode=Cell.Mode;

    switch (Mode) {
        case 90:
            Nbp=Nb12;
            Nep=Ne12;
            Nar=Nar23;
            Nab=Nab23;
            break;
        case 120:
            Nbp=Nb23;
            Nep=Ne23;
            Nar=Nar23;
            Nab=Nab23;
            break;
        case 240:
            Nbp=Nb43;
            Nep=Ne43;
            Nar=Nar43;
            Nab=Nab23;
            break;
        default:
            return;
    }

    double *Xo,*Yo,*Xi,*Yi;

    Xo=new double[Nep];
    Yo=new double[Nep];
    Xi=new double[Nbp];
    Yi=new double[Nbp];

    //Searching for a/lmb

  /*FILE *T;
	T=fopen("table.log","a");     */

    for (int i=0;i<Nbp;i++){
        for (int j=0;j<Nep;j++){
			switch (Mode) {
                case 90:
                    Xo[j]=E12[Nbp-i-1][j];
                    Yo[j]=R12[Nbp-i-1][j];
                    break;
                case 120:
                    Xo[j]=E23[Nbp-i-1][j];
                    Yo[j]=R23[Nbp-i-1][j];
                    break;
                case 240:
                    Xo[j]=E43[Nbp-i-1][j];
                    Yo[j]=R43[Nbp-i-1][j];
                    break;
                default:
                    return;
            }
        }
        TSpline Spline;
        Spline.SoftBoundaries=false;
        Spline.MakeLinearSpline(Xo,Yo,Nep);
        Yi[i]=Spline.Interpolate(Cell.ELP);
       //   Xi[i]=mode90?B12[i]:B23[i];
        switch (Mode) {
            case 90:
                Xi[i]=B12[i];
                break;
            case 120:
                Xi[i]=B23[i];
                break;
            case 240:
                Xi[i]=B43[i];
                break;
            default:
                return;
        }
    }

    TSpline rSpline;
    rSpline.SoftBoundaries=false;
    rSpline.MakeLinearSpline(Xi,Yi,Nbp);
	Cell.AkL=rSpline.Interpolate(Cell.beta);

    delete[] Xo;
    delete[] Yo;
    delete[] Xi;
    delete[] Yi;

    Xo=new double[Nar];
    Yo=new double[Nar];
    Xi=new double[Nab];
    Yi=new double[Nab];

    for (int i=0;i<Nab;i++){
        for (int j=0;j<Nar;j++){
            switch (Mode) {
                case 90:
                    Xo[j]=AR[j];
                    Yo[j]=A12[i][j];
                    break;
                case 120:
                    Xo[j]=AR[j];
                    Yo[j]=A23[i][j];
                    break;
                case 240:
                    Xo[j]=AR43[j];
                    Yo[j]=A43[i][j];
                    break;
                default:
                    return;
            }

            /*Xo[j]=AR[j];
            Yo[j]=mode90?A12[i][j]:A23[i][j]; */
        }
        TSpline Spline;
        Spline.SoftBoundaries=false;
        Spline.MakeLinearSpline(Xo,Yo,Nar);
        Yi[i]=Spline.Interpolate(Cell.AkL);
        //Xi[i]=AB[i];
        switch (Mode) {
            case 90:
                Xi[i]=AB[i];
                break;
            case 120:
                Xi[i]=AB[i];
                break;
            case 240:
                Xi[i]=AB[i];
                break;
            default:
                return;
        }
    }

    TSpline aSpline;
    aSpline.SoftBoundaries=false;
    aSpline.MakeLinearSpline(Xi,Yi,Nab);
	Cell.AL32=1e-4*aSpline.Interpolate(Cell.beta);

    delete[] Xo;
    delete[] Yo;
    delete[] Xi;
    delete[] Yi;

  /*    fprintf(T,"%f %f %f %f\n",Cell.betta,Cell.ELP,Cell.AkL,Cell.AL32);
    fclose(T);  */
	//ShowError(E12[0][1]);


  /*    for (int i=0;i<Nbt;i++){
        akl[i]=solve(ELP(akl),ELP=E0,bf[i]);
    }
    akl(bf);
    y=interp(akl(bf),bf);    */


}
//---------------------------------------------------------------------------
void TBeamSolver::GetDimensionsFromFile(TCell& Cell,TStructData *D)
{
	double *A, *C, *Einv, *Ainv;
	A=new double [D->N_bph];
	C=new double [D->N_bph];
	Einv=new double [D->N_akl];
	Ainv=new double [D->N_akl];

	for (int j = 0; j < D->N_akl; j++)
		Ainv[j]=D->AKL[D->N_akl-1-j];

	for (int i=0;i<D->N_bph;i++){
		TSpline eSpline, aSpline;
		eSpline.SoftBoundaries=false;
		aSpline.SoftBoundaries=false;

		for (int j = 0; j < D->N_akl; j++)
			Einv[j]=D->ELP[i][D->N_akl-1-j];

		eSpline.MakeLinearSpline(Einv,Ainv,D->N_akl);
		aSpline.MakeLinearSpline(D->AKL,D->AL32[i],D->N_akl);

		A[i]=eSpline.Interpolate(Cell.ELP);
		C[i]=aSpline.Interpolate(A[i]);
    }

	TSpline bSpline, cSpline;
	bSpline.SoftBoundaries=false;
	cSpline.SoftBoundaries=false;

	bSpline.MakeLinearSpline(D->Bph,A,D->N_bph);
	cSpline.MakeLinearSpline(D->Bph,C,D->N_bph);
	Cell.AkL=bSpline.Interpolate(Cell.beta);
	Cell.AL32=cSpline.Interpolate(Cell.beta);

	delete[] A;
	delete[] C;
	delete[] Einv;
	delete[] Ainv;
}
//---------------------------------------------------------------------------
void TBeamSolver::GetDimensions(TCell& Cell)
{
	int i_struct=-1;
	bool Priority=false;
	for (int i=0; i < StructPar.NStructData; i++) {
		if (StructPar.StructData[i].DataReady) {
			if (!Priority && StructPar.StructData[i].Default)
				i_struct=i;
			if (StructPar.StructData[i].Phase==Cell.Mode) {
				i_struct=i;
				Priority=true;
			}
		}
	}

	if (i_struct<0)
		GetDimensionsFromTab(Cell);
	else
		GetDimensionsFromFile(Cell,&StructPar.StructData[i_struct]);
}
//---------------------------------------------------------------------------
TInputLine *TBeamSolver::ParseFile(int& N)
{
	const char *FileName=InputFile.c_str();
	AnsiString S,L,M;

	if (!CheckFile(InputFile)){
		S="ERROR: The input file "+InputFile+" was not found";
		ShowError(S);
		return NULL;
	}

	TInputLine *Lines;
	std::ifstream fs(FileName);
	TInputParameter P;
	int i=-1,j=0,Ns=0;

    FILE *logFile;
	while (!fs.eof()){
		L=GetLine(fs);   //Now reads line by line
		S=ReadWord(L,1);
		//S=S.UpperCase();
	   //	S=GetLine(fs);   //Hid common actions inside the function
		if (S=="")
			continue;
		if(IsKeyWord(S) || S[1]=='!'){
			N++;
			if (S=="SAVE") {
				Ns++;
			}
		} else if (S=="END") {
        	break;
		} else {
			S="WARNING: The keyword "+S+" is not recognized in the input file and will be ignored!";
			ShowError(S);
        }
/*        logFile=fopen("BeamSolver.log","a");
		fprintf(logFile,"ParseFile: N=%i, S=%s\n",N,S);
        fclose(logFile); */
    }

    fs.clear();
    fs.seekg(std::ios::beg);

	Lines=new TInputLine[N];
	ResetDump(Ns);

	while (!fs.eof()){
		L=GetLine(fs);   //Now reads line by line
		S=ReadWord(L,1);  //Gets the first word in the line (should be a key word)
		//S=S.UpperCase();
		if (S=="END")
			break;
		else if (S=="") //empty line
			continue;
		else if(IsKeyWord(S) || S[1]=='!'){ //Key word or comment detected. Start parsing the line
			i++;
			P=Parse(S);
			Lines[i].P=P;

			if (P==COMMENT)       //comment length is not limited
				Lines[i].S[0]=L;
			else{
				Lines[i].N=NumWords(L)-1; //Parse number of words in the line
				if (Lines[i].N>MaxParameters){
					M="WARNING: Too many parameters in Line "+S+"! The excessive parameters will be ignored";
					Lines[i].N=MaxParameters;
					ShowError(M);
				}

				for (j=0;j<Lines[i].N;j++)
					Lines[i].S[j]=ReadWord(L,j+2);
			}
		}
	}

	fs.close();
    return Lines;
}
//---------------------------------------------------------------------------
AnsiString TBeamSolver::AddLines(TInputLine *Lines,int N1,int N2)
{
	AnsiString S="";

	for (int i=N1; i <= N2; i++) {
    	S+=" \t"+Lines->S[i];
	}

	return S;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseT2(TInputLine *Line, AnsiString &F)
{
    return ParsePIT(Line, F);
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParsePID(TInputLine *Line, AnsiString &F)
{
	AnsiString S;

	if (Line->N == 3){
		return ERR_BEAM;
	} else {
		AnsiString FileName=Line->S[1];
		if (CheckFile(FileName)) {
			BeamPar.RFile=FileName;
			F+=AddLines(Line,0,1);
			BeamPar.ZFile=FileName;
			BeamPar.ZBeamType=NORM_1D;

			if (Line->N > 3) {
				BeamPar.ZNorm.mean=DegreeToRad(Line->S[2].ToDouble());
				BeamPar.ZNorm.limit=DegreeToRad(Line->S[3].ToDouble());
				F+=AddLines(Line,2,3);

				if (Line->N > 4) {
					BeamPar.ZNorm.sigma=DegreeToRad(Line->S[4].ToDouble());
					F+=" \t"+Line->S[4];
				} else {
					BeamPar.ZNorm.sigma=100*BeamPar.ZNorm.limit;
				}
			} else {
				BeamPar.ZNorm.mean=pi;
				BeamPar.ZNorm.limit=pi;
				BeamPar.ZNorm.sigma=100*pi;
			}
		} else {
			S="ERROR: The file "+FileName+" is missing!";
			ShowError(S);
			return ERR_BEAM;
		}
	}

	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParsePIT(TInputLine *Line, AnsiString &F)
{
	AnsiString FileName=Line->S[1],S;

	if (CheckFile(FileName)) {
		BeamPar.ZBeamType=BeamPar.RBeamType;
		BeamPar.RFile=FileName;
		BeamPar.ZFile=FileName;

		F+=AddLines(Line,0,1);

		if (Line->N == 3){
			if (Line->S[2].UpperCase()=="COMPRESS") {
				BeamPar.ZCompress=true;
				F+=" COMPRESS";
			} else {
				S="ERROR: Wrong keyword "+Line->S[2]+" in BEAM line";
				ShowError(S);
				return ERR_BEAM;
			}
		}
	} else {
		S="ERROR: The file "+FileName+" is missing!";
		ShowError(S);
		return ERR_BEAM;
	}

	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseFile2R(TInputLine *Line, AnsiString &F, int Nr)
{
	AnsiString FileName=Line->S[1],S;

	if (CheckFile(FileName)) {
		BeamPar.RFile=FileName;
		F+=AddLines(Line,0,1);
	} else {
		S="ERROR: The file "+FileName+" is missing!";
		ShowError(S);
		return ERR_BEAM;
	}

	if (Nr==2){
		FileName=Line->S[2];
		if (CheckFile(FileName)) {
			BeamPar.YFile=FileName;
			F+=" \t"+FileName;
			BeamPar.RBeamType=TWO_FILES_2D;
		} else {
			S="ERROR: The file "+FileName+" is missing!";
			ShowError(S);
			return ERR_BEAM;
		}
	}
	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseTwiss2D(TInputLine *Line, AnsiString &F, int Nr)
{
	AnsiString S;

	if (Nr != 3){
		S="ERROR: Wrong number of Twiss parameters in BEAM line!";
		ShowError(S);
		return ERR_BEAM;
	}else{
		BeamPar.XTwiss.alpha=Line->S[1].ToDouble();
		BeamPar.XTwiss.beta=Line->S[2].ToDouble()/100; //m/rad
		BeamPar.XTwiss.epsilon=Line->S[3].ToDouble()/100; //m*rad
		BeamPar.YTwiss=BeamPar.XTwiss;
		F+=AddLines(Line,0,3);
	}
	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseTwiss4D(TInputLine *Line, AnsiString &F, int Nr)
{
	AnsiString S;

	if (Nr != 6){
		S="ERROR: Wrong number of Twiss parameters in BEAM line!";
		ShowError(S);
		return ERR_BEAM;
	}
	else{
		BeamPar.XTwiss.alpha=Line->S[1].ToDouble();
		BeamPar.XTwiss.beta=Line->S[2].ToDouble()/100; //cm/rad
		BeamPar.XTwiss.epsilon=Line->S[3].ToDouble()/100; //cm*rad
		BeamPar.YTwiss.alpha=Line->S[4].ToDouble();
		BeamPar.YTwiss.beta=Line->S[5].ToDouble()/100;
		BeamPar.YTwiss.epsilon=Line->S[6].ToDouble()/100;
		F+=AddLines(Line,0,6);
	}

	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseSPH(TInputLine *Line, AnsiString &F, int Nr)
{
	AnsiString S;

    if (Nr>3){
		S="ERROR: Too many Spherical parameters in BEAM line!";
		ShowError(S);
		return ERR_BEAM;
	} else{
		BeamPar.Sph.Rcath=Line->S[1].ToDouble()/100; //Rcath cm
		F+=AddLines(Line,0,1);
		if (Nr>1) {
			BeamPar.Sph.Rsph=Line->S[2].ToDouble()/100;  //Rsph cm
			F+=" \t"+Line->S[2];
			if (Nr>2){
				BeamPar.Sph.kT=Line->S[3].ToDouble(); //kT
				F+=" \t"+Line->S[3];
			} else
				BeamPar.Sph.kT=0;
		} else {
			BeamPar.Sph.Rsph=0;
			BeamPar.Sph.kT=0;
		}
	}

	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseELL(TInputLine *Line, AnsiString &F, int Nr)
{
	AnsiString S;

	if (Nr>4){
		S="ERROR: Too many Elliptical parameters in BEAM line!";
		ShowError(S);
		return ERR_BEAM;
	}else{
		BeamPar.Ell.ax=Line->S[1].ToDouble()/100; //x cm
		F+=AddLines(Line,0,1);
		if (Nr>1) {
			BeamPar.Ell.by=Line->S[2].ToDouble()/100; //y cm
			F+=" \t"+Line->S[2];
			if (Nr>2){
				BeamPar.Ell.phi=DegreeToRad(Line->S[3].ToDouble()); //phi
				F+=" \t"+Line->S[3];
				if (Nr>3){
					BeamPar.Ell.h=Line->S[4].ToDouble(); //h
					F+=" \t"+Line->S[4];
				} else
					BeamPar.Ell.h=1;
			} else {
				BeamPar.Ell.phi=0;
				BeamPar.Ell.h=1;
			}
		} else {
			BeamPar.Ell.by=BeamPar.Ell.ax; //y
			BeamPar.Ell.phi=0;
		 	BeamPar.Ell.h=1;
		}
	}
	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseFile1Z(TInputLine *Line, AnsiString &F, int Nz,int Zpos)
{
	AnsiString S;

	if (Nz != 3 && Nz != 4){
		S="ERROR: Wrong number of Import File (Z) or Phase Distribution parameters in BEAM line!";
		ShowError(S);
		return ERR_BEAM;
	}

	AnsiString FileName=Line->S[Zpos+1];
	if (CheckFile(FileName)) {
		BeamPar.ZFile=FileName;
		F+=AddLines(Line,Zpos,Zpos+1);
	} else {
		S="ERROR: The file "+FileName+" is missing!";
		ShowError(S);
		return ERR_BEAM;
	}

	BeamPar.ZNorm.mean=DegreeToRad(Line->S[Zpos+2].ToDouble());
	BeamPar.ZNorm.limit=DegreeToRad(Line->S[Zpos+3].ToDouble());
	F+=AddLines(Line,Zpos+2,Zpos+3);

	if (Nz == 3) {
		BeamPar.ZNorm.sigma=100*BeamPar.ZNorm.limit;
	} else  if (Nz == 4) {
		BeamPar.ZNorm.sigma=DegreeToRad(Line->S[Zpos+4].ToDouble());
		F+=Line->S[Zpos+4];
	}

	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseFile2Z(TInputLine *Line, AnsiString &F, int Nz,int Zpos)
{
	AnsiString S;

	if (Nz != 1){
		S="ERROR: Too many Import File (Z) parameters in BEAM line!";
		ShowError(S);
		return ERR_BEAM;
	}

	AnsiString FileName=Line->S[Zpos+1];
	if (CheckFile(FileName)) {
		BeamPar.ZFile=FileName;
		F+=AddLines(Line,Zpos,Zpos+1);
	} else {
		S="ERROR: The file "+FileName+" is missing!";
		ShowError(S);
		return ERR_BEAM;
	}

	return ERR_NO;
}
 //---------------------------------------------------------------------------
TError TBeamSolver::ParseNorm(TInputLine *Line, AnsiString &F, int Nz,int Zpos)
{
	AnsiString S;

	if (Nz != 4 && Nz != 6){
		S="Wrong number of Longitudinal Distribution parameters in BEAM line!";
		ShowError(S);
		return ERR_BEAM;
	}else{
		if (Nz == 4) {
			BeamPar.WNorm.mean=Line->S[Zpos+1].ToDouble();
			BeamPar.WNorm.limit=Line->S[Zpos+2].ToDouble();
			BeamPar.WNorm.sigma=100*BeamPar.WNorm.limit;
			BeamPar.ZNorm.mean=DegreeToRad(Line->S[Zpos+3].ToDouble());
			BeamPar.ZNorm.limit=DegreeToRad(Line->S[Zpos+4].ToDouble());
			BeamPar.ZNorm.sigma=100*BeamPar.ZNorm.limit;
			F+=AddLines(Line,Zpos,Zpos+4);
		} else  if (Nz == 6) {
			BeamPar.WNorm.mean=Line->S[Zpos+1].ToDouble();
			BeamPar.WNorm.limit=Line->S[Zpos+2].ToDouble();
			BeamPar.WNorm.sigma=Line->S[Zpos+3].ToDouble();
			BeamPar.ZNorm.mean=DegreeToRad(Line->S[Zpos+4].ToDouble());
			BeamPar.ZNorm.limit=DegreeToRad(Line->S[Zpos+5].ToDouble());
			BeamPar.ZNorm.sigma=DegreeToRad(Line->S[Zpos+6].ToDouble());
			F+=AddLines(Line,Zpos,Zpos+6);
		}
	}

	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseOptions(TInputLine *Line)
{
	TError Error=ERR_NO;
	AnsiString F="OPTIONS ";

	for (int j=0;j<Line->N;j++){
		const AnsiString s = Line->S[j].UpperCase();
		if (s=="REVERSE")
			StructPar.Reverse=true;
		if (s=="DEMAGNETIZE")
			BeamPar.Demagnetize=true;

	/*	if (s=="MAGNETIZED")
			BeamPar.Magnetized=true;   */

		F=F+"\t"+s;
	}
	ParsedStrings->Add(F);

	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseParticleType(TInputLine *Line)
{
	TError Error=ERR_NO;
	AnsiString S="", F="PARTICLES ";
	const AnsiString s = Line->S[0].UpperCase();


	if (s=="ELECTRON" || s=="ELECTRONS"){
		BeamPar.Species.Type=ELECTRON;
		BeamPar.Species.A=1;
		BeamPar.Species.Q=1;
		F+="ELECTRONS ";
		if (Line->N > 1)
			S="!WARNING: Excessive parameters for electrons will be ignored\n";
	} else if (s=="PROTON" || s=="PROTONS"){
		BeamPar.Species.Type=PROTON;
		BeamPar.Species.A=1;
		BeamPar.Species.Q=1;
		F+="PROTONS ";
		if (Line->N > 1)
			S="!WARNING: Excessive parameters for protons will be ignored\n";
	} else if (s=="ION" || s=="IONS"){
		F+="IONS ";
		BeamPar.Species.Type=ION;
		if (Line->N == 1) {
			S="!WARNING:Ion mass and charge are not specified. Assuming protons\n";
			BeamPar.Species.Type=PROTON;
			BeamPar.Species.A=1;
			BeamPar.Species.Q=1;
		} else {
			BeamPar.Species.A=Line->S[1].ToDouble();
			BeamPar.Species.Q=1;
			F+=Line->S[1];
			if (Line->N >= 3) {
				BeamPar.Species.Q=abs(Line->S[2].ToInt());
				F=F+" "+Line->S[2];
				if (Line->N > 3)
					S+="!WARNING:Excessive parameters for ions will be ignored\n";

				if (BeamPar.Species.A==1 && BeamPar.Species.Q==1){
					S="!WARNING: Ion mass and charge are equal to 1. Assuming protons\n";
					BeamPar.Species.Type=PROTON;
				}
			}
		}
	} else {
		S="ERROR: Unrecognized keyword in PARTCILES line!\n";
		Error=ERR_PARTICLE;
	}

	if (BeamPar.Species.A/BeamPar.Species.Q<1)
		S+="!WARNING:A/Q ratio is less than 1! Check input parameters!\n";

	BeamPar.W0=GetRestEnergy(BeamPar.Species);
	BeamPar.Wnorm=(BeamPar.Species.A/BeamPar.Species.Q)*BeamPar.W0;

	ParsedStrings->Add(F);

	if (S!="")
		ShowError(S);

	return Error;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseSpaceCharge(TInputLine *Line)
{
	AnsiString F="SPCHARGE ",S;
	BeamPar.SpaceCharge.Type=SPCH_NO;
	BeamPar.SpaceCharge.NSlices=1;
	BeamPar.SpaceCharge.Nrms=2.4;
	BeamPar.SpaceCharge.Train=false;
	BeamPar.SpaceCharge.Ltrain=0;

	int Ntr=0;

	if (Line->N==0){
		BeamPar.SpaceCharge.Type=SPCH_ELL;
		F=F+"\t"+"ELLIPTIC";
	}else{
		for (int i = 1; i < Line->N; i++) {
			if (Line->S[i].UpperCase()=="TRAIN") {
				//Ntr++;//=i+1;
				Ntr=Line->N-i;
				break;
			}
		}
		int Nsp=Line->N-Ntr;
		if (Nsp>2) {
			S="ERROR: Too many parameters in SPCHARGE line!";
			ShowError(S);
			return ERR_SPCHARGE;
		}else{
			BeamPar.SpaceCharge.Type=ParseSpchType(Line->S[0]);
			F=F+"\t"+Line->S[0];
			switch (BeamPar.SpaceCharge.Type) {
				case SPCH_GW: {
					if (Nsp==2){
						BeamPar.SpaceCharge.NSlices=Line->S[1].ToInt();
						F=F+"\t"+Line->S[1];
					}
					break;
				}
				case SPCH_LPST: { }
				case SPCH_ELL: {
					if (Nsp==2){
						BeamPar.SpaceCharge.Nrms=Line->S[1].ToDouble();
						F=F+"\t"+Line->S[1];
					}
					break;
				}
				case SPCH_NO: { break;}
				default: {return ERR_SPCHARGE;};
			}
		}

		if (Ntr>0) {
			BeamPar.SpaceCharge.Train=true;
			F=F+"\t"+"TRAIN";
			if (Ntr==1){
				BeamPar.SpaceCharge.Ltrain=0;
			} else if (Ntr==2){
				BeamPar.SpaceCharge.Ltrain=1e-2*Line->S[Nsp+1].ToDouble();
				F=F+"\t"+Line->S[Nsp+1];
			}else {
				S="ERROR: Too many parameters after TRAIN keyword!";
				ShowError(S);
				return ERR_SPCHARGE;
			}
		}
	}

	ParsedStrings->Add(F);
	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseSolenoid(TInputLine *Line)
{
	AnsiString F="SOLENOID",SolenoidFile,S;

	StructPar.SolenoidPar.MagnetType=MAG_SOLENOID;
	StructPar.SolenoidPar.ImportType=NO_ELEMENT;
	StructPar.SolenoidPar.BField=0; //[T]
	StructPar.SolenoidPar.Length=0; //[m]
	StructPar.SolenoidPar.StartPos=0; //[m]

	if (Line->N>=1 && Line->N<=4) {
		if (IsNumber(Line->S[0])) {
			StructPar.SolenoidPar.BField=Line->S[0].ToDouble()/10000; //[Gs]
			F=F+"\t"+Line->S[0];
			if (Line->N>=2) {
				StructPar.SolenoidPar.ImportType=ANALYTIC_1D;
				StructPar.SolenoidPar.Length=Line->S[1].ToDouble()/100; //[cm]
				F=F+"\t"+Line->S[1];
				if (Line->N>=3){
					StructPar.SolenoidPar.StartPos=Line->S[2].ToDouble()/100; //[cm]
					F=F+"\t"+Line->S[2];
					if (Line->N==4){
						StructPar.SolenoidPar.Lfringe=Line->S[3].ToDouble()/100; //[cm]
						F=F+"\t"+Line->S[3];
					}  else
						StructPar.SolenoidPar.Lfringe=0.01; //[m]
				}else{
					StructPar.SolenoidPar.StartPos=0; //[m]
					StructPar.SolenoidPar.Lfringe=0.01; //[m]
				}
			} else {
				StructPar.SolenoidPar.ImportType=ANALYTIC_0D;
			}
		} else {
			SolenoidFile=Line->S[0];
			F+="\t"+SolenoidFile;
			if (CheckFile(SolenoidFile)){
				StructPar.SolenoidPar.ImportType=ParseSolenoidType(SolenoidFile);
				StructPar.SolenoidPar.File=SolenoidFile;
			}else{
				S="ERROR: The file "+SolenoidFile+" is missing!";
				ShowError(S);
				return ERR_SOLENOID;
			}
			if (Line->N==2){
				StructPar.SolenoidPar.StartPos=Line->S[1].ToDouble()/100;
				F=F+"\t"+Line->S[1];
			}else if (Line->N>2){
				S="WARNING: Excessive parameters in SOLENOID line. They will be ingored!";
				ShowError(S);
			}
		}
		ParsedStrings->Add(F);
	} else
		return ERR_SOLENOID;

	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseBeam(TInputLine *Line)
{
	TError Error=ERR_NO;
	AnsiString F="BEAM",S;

	if (Line->N < 1)
		return ERR_BEAM;

	AnsiString KeyWord=Line->S[0];
	BeamPar.RBeamType=ParseDistribution(KeyWord);
	BeamPar.ZCompress=false;

	if (BeamPar.RBeamType==NOBEAM) {
		S="ERROR: Wrong KEYWORD in BEAM line";
		ShowError(S);
		return ERR_BEAM;
	 }

	 if (IsFullFileKeyWord(BeamPar.RBeamType)) {
		if (Line->N < 2 || Line->N >5)
			return ERR_BEAM;

			switch (BeamPar.RBeamType) {
				case CST_PID: {
					Error=ParsePID(Line,F);
					break;
				}
				case CST_PIT: {
					Error=ParsePIT(Line,F);
					break;
				}
				case PARMELA_T2: {
					Error=ParseT2(Line,F);
					break;
				}
				default: {
					S="ERROR: Unexpected Input File Type";
					ShowError(S);
					return ERR_BEAM;
				}
			}
	 } else if (IsTransverseKeyWord(BeamPar.RBeamType)) {
		int Zpos=0;
		for (int j=1; j < Line->N; j++) {
			if (IsLongitudinalKeyWord(ParseDistribution(Line->S[j]))) {
				Zpos=j;
				break;
			}
		}
		if (Zpos==0) {
			S="Longitudinal Distribution Type is not defined in BEAM line";
			ShowError(S);
			return ERR_BEAM;
		}
		int Nr=Zpos-1; // Number of R-parameters
		int Nz=Line->N-Zpos-1; //Number of Z-parameters

		if (Nr==0 || Nz==0) {
			S="Too few parametes in BEAM line!";
			ShowError(S);
			return ERR_BEAM;
		}

		switch (BeamPar.RBeamType) {
			case FILE_4D: {
				if (Nr!=1){
					S="Too many Import File (R) parameters in BEAM line!";
					ShowError(S);
					return ERR_BEAM;
				}
			}
			case FILE_2D: {
				if (Nr>2){
					S="Too many Import File (R) parameters in BEAM line!";
					ShowError(S);
					return ERR_BEAM;
				}
				Error=ParseFile2R(Line,F,Nr);
				break;
			}
			case TWISS_2D: {
				Error=ParseTwiss2D(Line,F,Nr);
				break;
			}
			case TWISS_4D: {
				Error=ParseTwiss4D(Line,F,Nr);
				break;
			}
			case SPH_2D: {
				Error=ParseSPH(Line,F,Nr);
				break;
			}
			case ELL_2D: {
				Error=ParseELL(Line,F,Nr);
				break;
			}
			default: {
				S="Unexpected BEAM Transversal Distribution Type";
				ShowError(S);
				return ERR_BEAM;
			}
		}
		KeyWord=Line->S[Zpos];
		BeamPar.ZBeamType=ParseDistribution(KeyWord);

		if (IsLongitudinalKeyWord(BeamPar.ZBeamType)) {
			switch (BeamPar.ZBeamType) {
				case FILE_1D: {
					Error=ParseFile1Z(Line,F,Nz,Zpos);
					break;
				}
				case FILE_2D: {
					Error=ParseFile2Z(Line,F,Nz,Zpos);
					break;
				}
				case NORM_2D: {
					Error=ParseNorm(Line,F,Nz,Zpos);
					break;
				}
				default: {
					S="ERROR:Unexpected BEAM Longitudinal Distribution Type";
					ShowError(S);
					return ERR_BEAM;
				}
			}
		} else{
			S="ERROR: Unexpected BEAM Longitudinal Distribution Type";
			ShowError(S);
			return ERR_BEAM;
		}
	 }  else  {
		S="ERROR: Wrong Format of BEAM line";
		ShowError(S);
		return ERR_BEAM;
	 }
	 ParsedStrings->Add(F);

	 return Error;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseCurrent(TInputLine *Line)
{
	AnsiString F="CURRENT",S;
	if (Line->N<1 || Line->N>2){
		return ERR_CURRENT;
	}  else {
		BeamPar.Current=Line->S[0].ToDouble();
		F+="\t"+Line->S[0];
		if (!IsFileKeyWord(BeamPar.RBeamType) && !IsFileKeyWord(BeamPar.ZBeamType)) {
			if (Line->N!=2){
				S="ERROR: Number of particles is not defined!";
				ShowError(S);
				return ERR_CURRENT;
			} else {
				BeamPar.NParticles=Line->S[1].ToInt();
				F+="\t"+Line->S[1];
			}
		} else {
			int Nr=0, Ny=0, Nz=0, Np=0;

			if (IsFullFileKeyWord(BeamPar.RBeamType)){
				switch (BeamPar.RBeamType) {
					case CST_PID:{
						Nr=NumPointsInFile(BeamPar.RFile,PID_LENGTH);
						break;
					}
					case CST_PIT:{
						Nr=NumPointsInFile(BeamPar.RFile,PIT_LENGTH);
						break;
					}
					case PARMELA_T2:{
						Nr=NumPointsInFile(BeamPar.RFile,T2_LENGTH);
						break;
					}
					default:  {
						S="ERROR: Unexpected BEAM File Format!";
						ShowError(S);
						return ERR_IMPORT;
					}
				}

				Nz=Nr;
			} else {
				switch (BeamPar.RBeamType) {
					case FILE_2D: {
						Nr=NumPointsInFile(BeamPar.RFile,2);
						break;
					}
					case TWO_FILES_2D: {
						Nr=NumPointsInFile(BeamPar.RFile,2);
						Ny=NumPointsInFile(BeamPar.YFile,2);
						if (Nr!=Ny) {
							S="ERROR: The numbers of imported particles from two Transverse Beam Files don't match!";
							ShowError(S);
							return ERR_IMPORT;
						}
						break;
					}
					case FILE_4D: {
						Nr=NumPointsInFile(BeamPar.RFile,4);
						break;
					}
					default:  {
						Nr=-1;
						break;
					}
				}

				switch (BeamPar.ZBeamType) {
					case FILE_1D: {
						Nz=NumPointsInFile(BeamPar.ZFile,1);
						break;
					}
					case FILE_2D: {
						Nz=NumPointsInFile(BeamPar.ZFile,2);
						break;
					}
					default:  {
						Nz=Nr;
						break;
					}
				}

				if (Nr==-1)
					Nr=Nz;


			}
			//check Np mismatch
			if (IsFileKeyWord(BeamPar.RBeamType) && IsFileKeyWord(BeamPar.ZBeamType)){
				if (Nr!=Nz) {
					S="ERROR: The number of imported particles from Longintudinal Beam File doesn't match with the number of particles imported from Transversal Beam File!";
					ShowError(S);
					return ERR_IMPORT;
				}
			}

			//check Np overrride
			if (Line->N==2) {
				Np=Line->S[1].ToInt();

				if (Np>Nr) {
					S="WARNING: The number of defined particles exceeds the number of imported particles, and will be ignored!";
					ShowError(S);
				}  else if (Np<Nr) {
					S="WARNING: The number of defined particles is less than the number of imported particles! Only the first "+Line->S[1]+" particles will be simulated";
					ShowError(S);
					Nr=Np;
				}
				F+=" "+Line->S[1];
			}
			BeamPar.NParticles=Nr;
		}
	}

	ParsedStrings->Add(F);

	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::CheckStructFile(TStructData &D, AnsiString &F)
{
	TError Error=ERR_NO;
	AnsiString S,L;
	std::ifstream fs(F.c_str());
	int N=0;

	L=GetLine(fs);
	D.N_bph=NumWords(L);
	L=GetLine(fs);
	D.N_akl=NumWords(L);

	try {
		for (int i=0; i < 2*D.N_bph; i++) {
			L=GetLine(fs);
			N=NumWords(L);
			if (N!=D.N_akl){
				Error=ERR_STRUCT;
				break;
			}
		}
	} catch (...) {
		Error=ERR_STRUCT;
	}

	fs.close();

	if (Error!=ERR_NO) {
		S="ERROR: The format of file "+F+" is not correct! Please, refer to manual for the correct format. This line will be ignored.";
		ShowError(S);
	}

	return Error;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseStructFile(TStructData &D, AnsiString &F)
{
	TError Error=ERR_NO;
	AnsiString S,L;

	Error=CheckStructFile(D,F);
	if (Error==ERR_NO) {
		D.Bph=new double [D.N_bph];
		D.AKL=new double [D.N_akl];
		D.ELP=new double *[D.N_bph];
		D.AL32=new double *[D.N_bph];
		for (int i = 0; i < D.N_bph; i++){
			D.ELP[i]=new double [D.N_akl];
			D.AL32[i]=new double [D.N_akl];
		}
		std::ifstream fs(F.c_str());
		try {
		L=GetLine(fs);      //Betas
		for (int j=0;j<D.N_bph;j++){
			S=ReadWord(L,j+1);
			D.Bph[j]=S.ToDouble();
		}

		L=GetLine(fs);     //akls
		for (int j=0;j<D.N_akl;j++){
			S=ReadWord(L,j+1);
			D.AKL[j]=S.ToDouble();
		}

		for (int i = 0; i < D.N_bph; i++){
			L=GetLine(fs);     //ELPs
			for (int j=0;j<D.N_akl;j++){
				S=ReadWord(L,j+1);
				D.ELP[i][j]=S.ToDouble();
			}
		}
		for (int i = 0; i < D.N_bph; i++){
			L=GetLine(fs);     //AL32s
			for (int j=0;j<D.N_akl;j++){
				S=ReadWord(L,j+1);
				D.AL32[i][j]=S.ToDouble();
			}
		}
		} catch (...) {
			Error=ERR_STRUCT;
		}
		fs.close();
	}

	D.DataReady=Error==ERR_NO;

 	return Error;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseStruct(TInputLine *Line,int Ni)
{
	AnsiString F="STRUCT",StructFile,S;
	TError Error=ERR_NO;

	if (Line->N>0){
		//Read File
		StructFile=Line->S[0];
		F+="\t"+StructFile;
		if (CheckFile(StructFile)){
			Error=ParseStructFile(StructPar.StructData[Ni],StructFile);
			if (Error==ERR_NO) {
				if (Line->N==1)
					StructPar.StructData[Ni].Default=true;
				else {
					StructPar.StructData[Ni].Phase=Line->S[1].ToDouble();
					F+="\t"+Line->S[1];
					if (Line->N>2){
						S="WARNING: Excessive parameters in STRUCT line. They will be ingored!";
						ShowError(S);
					}
				}
			}
		}else {
			S="ERROR: The file "+StructFile+" is missing!";
			ShowError(S);
			Error=ERR_STRUCT;
		}
	 } else
		Error=ERR_STRUCT;

	ParsedStrings->Add(F);
	return Error;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseCell(TInputLine *Line,int Ni, int Nsec, bool NewCell)
{
	if (Line->N==3 || Line->N==5){
		StructPar.Cells[Ni].Mode=Line->S[0].ToDouble();
		StructPar.Cells[Ni].beta=Line->S[1].ToDouble();
		StructPar.Cells[Ni].ELP=Line->S[2].ToDouble();
		StructPar.Cells[Ni].Mesh=Nmesh;

		if (Line->N==3){
			GetDimensions(StructPar.Cells[Ni]);
		} else if (Line->N==5){
			StructPar.Cells[Ni].AL32=Line->S[3].ToDouble();
			StructPar.Cells[Ni].AkL=Line->S[4].ToDouble();
		}
	 } else{
		return ERR_CELL;
	 }

	 StructPar.Cells[Ni].F0=StructPar.Sections[Nsec-1].Frequency;
	 StructPar.Cells[Ni].P0=StructPar.Sections[Nsec-1].Power;
	 StructPar.Cells[Ni].First=NewCell;
	 StructPar.Cells[Ni].Magnet.MagnetType=MAG_NO;
	 StructPar.Cells[Ni].dF=NewCell?StructPar.Sections[Nsec-1].PhaseShift:0;
	 StructPar.Sections[Nsec-1].NCells++;

	 return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseSingleCell(TInputLine *Line,int Ni, int Nsec, bool NewCell)
{
	 AnsiString F="CELL ",S;
	 TError Err;

	 Err=ParseCell(Line,Ni,Nsec,NewCell);

	 F+=Line->S[0]+"\t"+S.FormatFloat("#0.000",StructPar.Cells[Ni].beta)+"\t"+S.FormatFloat("#0.000",StructPar.Cells[Ni].ELP)+"\t"+S.FormatFloat("#0.000000",StructPar.Cells[Ni].AL32)+"\t"+S.FormatFloat("#0.000000",StructPar.Cells[Ni].AkL);
	 ParsedStrings->Add(F);

	 return Err;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseMultipleCells(TInputLine *Line,int Ni, int Nsec, bool NewCell)
{
	AnsiString F="CELLS ",S;

	TError Err;
	TInputLine ParsedLine;
	ParsedLine=*Line;
	int Ncells=Line->S[0].ToInt();
	for (int i = 0; i < Line->N-1; i++) {
		ParsedLine.S[i]=Line->S[i+1];
	}
	ParsedLine.N=Line->N-1;

	for (int i = 0; i < Ncells; i++) {
		Err=ParseCell(&ParsedLine, Ni+i,Nsec,NewCell);
		NewCell=false;
	}

	F+=Line->S[0]+"\t"+Line->S[1]+"\t"+S.FormatFloat("#0.000",StructPar.Cells[Ni].beta)+"\t"+S.FormatFloat("#0.000",StructPar.Cells[Ni].ELP)+"\t"+S.FormatFloat("#0.000000",StructPar.Cells[Ni].AL32)+"\t"+S.FormatFloat("#0.000000",StructPar.Cells[Ni].AkL);
	ParsedStrings->Add(F);

	return Err;
}
//---------------------------------------------------------------------------
double TBeamSolver::DefaultFrequency(int N)
{
	double f=0;
	//f=N>0?arc(StructPar.Sections[N-1].Frequency):c;
	f=N>0?StructPar.Sections[N-1].Frequency:c;
	return f;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseDrift(TInputLine *Line,int Ni, int Nsec)
{
	AnsiString F="DRIFT ",S;
	int m=0;
	TError Err;

	if (Line->N<2 || Line->N>3){
		return ERR_DRIFT;
	} else {
		StructPar.Cells[Ni].Drift=true;
		StructPar.Cells[Ni].Magnet.MagnetType=MAG_NO;
		StructPar.Cells[Ni].beta=Line->S[0].ToDouble()/100;//D, cm
		StructPar.Cells[Ni].AkL=Line->S[1].ToDouble()/100;//Ra, cm
		F+=Line->S[0]+" \t"+Line->S[1];
		if (Line->N==3){
			m=Line->S[2].ToInt();
			if (m>1)
				StructPar.Cells[Ni].Mesh=m;
			else{
				S="WARNING: Number of mesh points must be more than 1";
				ShowError(S);
				StructPar.Cells[Ni].Mesh=Nmesh;
			}
			F+=" \t"+Line->S[2];
		} else
			StructPar.Cells[Ni].Mesh=Nmesh;

		StructPar.Cells[Ni].ELP=0;
		StructPar.Cells[Ni].AL32=0;
		StructPar.Cells[Ni].First=true;
		StructPar.Cells[Ni].F0=DefaultFrequency(Nsec);
		StructPar.Cells[Ni].dF=0;//arc(StructPar.Sections[Nsec-1].PhaseShift);

		ParsedStrings->Add(F);
	}

	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseQuad(TInputLine *Line,int Ni, int Nsec)
{
	AnsiString F="QUAD",S,QuadFile;
	int m=0;
	TError Err;

	if (Line->N<3 || Line->N>5){
		return ERR_QUAD;
	} else {
		StructPar.Cells[Ni].Drift=true;
		StructPar.Cells[Ni].Magnet.MagnetType=MAG_QUAD;

		QuadFile=Line->S[0];
		F+="\t"+QuadFile;
		if (CheckFile(QuadFile))
			StructPar.Cells[Ni].Magnet.File=QuadFile;
		else{
			S="ERROR: The file "+QuadFile+" is missing!";
			ShowError(S);
			return ERR_QUAD;
		}

		StructPar.Cells[Ni].beta=Line->S[1].ToDouble()/100;//L, cm
		F+=" \t"+Line->S[1];

		if (Line->N>=3){
			StructPar.Cells[Ni].Magnet.BField=Line->S[2].ToDouble();
			F+=" \t"+Line->S[2];
			if (Line->N>=4){
				m=Line->S[3].ToInt();
				if (m>1)
					StructPar.Cells[Ni].Mesh=m;
				else{
					S="WARNING: Number of mesh points must be more than 1";
					ShowError(S);
					StructPar.Cells[Ni].Mesh=Nmesh;
				}
				F+=" \t"+Line->S[3];
			}else
				StructPar.Cells[Ni].Mesh=Nmesh;
		} else{
			StructPar.Cells[Ni].Magnet.BField=1.0;
			StructPar.Cells[Ni].Mesh=Nmesh;
		}


		StructPar.Cells[Ni].AkL=-1;//defined by mesh

		StructPar.Cells[Ni].ELP=0;
		StructPar.Cells[Ni].AL32=0;
		StructPar.Cells[Ni].First=true;
		StructPar.Cells[Ni].F0=DefaultFrequency(Nsec);
		StructPar.Cells[Ni].dF=0;//arc(StructPar.Sections[Nsec-1].PhaseShift);
		ParsedStrings->Add(F);
	}

	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParsePower(TInputLine *Line,int Nsec)
{
	AnsiString F="POWER ",S;
	if (Line->N>=2 && Line->N<=3){
		if (Nsec>StructPar.NSections) {
			S="ERROR: Inconsistent number of sections!";
			ShowError(S);
			return ERR_FORMAT;
		}  else{
			StructPar.Sections[Nsec].Power=1e6*Line->S[0].ToDouble();
			StructPar.Sections[Nsec].Frequency=1e6*Line->S[1].ToDouble();
			StructPar.Sections[Nsec].NCells=0;
			F+=Line->S[0]+"\t"+Line->S[1]+"\t";
			if(Line->N==3){
				StructPar.Sections[Nsec].PhaseShift=arc(Line->S[2].ToDouble());
				F+=Line->S[2];
			}else
				StructPar.Sections[Nsec].PhaseShift=0;
		}
	} else
		return ERR_COUPLER;

	ParsedStrings->Add(F);

	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseDump(TInputLine *Line, int Ns, int Ni)
{
	bool DefaultSet=true;
	int Nkey=1; // the word from which the flags are checked
	AnsiString F="SAVE ",F0, S;

	if (Line->N>=1 && Line->N<=9){
		BeamExport[Ns].NElement=Ni;
		BeamExport[Ns].File=Line->S[0].c_str();
		BeamExport[Ns].LiveOnly=true;
		BeamExport[Ns].SpecialFormat=NOBEAM;
		BeamExport[Ns].Phase=false;
		BeamExport[Ns].Energy=false;
		BeamExport[Ns].Radius=false;
		BeamExport[Ns].Azimuth=false;
		BeamExport[Ns].Divergence=false;
        BeamExport[Ns].Binary=false;

		F+=Line->S[0];
		F0=F;
			if (Line->N>=2 && IsNumber(Line->S[1])) {
				BeamExport[Ns].N1=Line->S[1].ToDouble();
				F=F+" "+Line->S[1];
				Nkey=2;
				if (Line->N>=3 && IsNumber(Line->S[2])){
					BeamExport[Ns].N2=Line->S[2].ToDouble();
					F=F+" "+Line->S[2];
					Nkey=3;
				} else
					BeamExport[Ns].N2=0;
			}  else {
				BeamExport[Ns].N1=0;
				BeamExport[Ns].N2=0;
			}
			if (Line->N>=2 && Line->N<=9) {
				for (int j=Nkey;j<Line->N;j++){
					const AnsiString s = Line->S[j].UpperCase();
					if (s=="PID") {
						BeamExport[Ns].SpecialFormat=CST_PID;
						F=F0+" PID";
						DefaultSet=false;
						break;
					}
					else if (s=="PIT") {
						BeamExport[Ns].SpecialFormat=CST_PIT;
						F=F0+" PIT";
						DefaultSet=false;
						break;
					}
					else if (s=="ASTRA") {
						BeamExport[Ns].SpecialFormat=ASTRA;
						F=F0+" ASTRA";
						DefaultSet=false;
						break;
					}
					else if (s=="LOST") {
						BeamExport[Ns].LiveOnly=false;
						F=F+" LOST";
					}
					else if (s=="PHASE") {
						BeamExport[Ns].Phase=true;
						F=F+" PHASE";
						DefaultSet=false;
					}
					else if (s=="ENERGY") {
						BeamExport[Ns].Energy=true;
						F=F+" ENERGY";
						DefaultSet=false;
					}
					else if (s=="RADIUS") {
						BeamExport[Ns].Radius=true;
						F=F+" RADIUS";
						DefaultSet=false;
					}
					else if (s=="AZIMUTH") {
						BeamExport[Ns].Azimuth=true;
						F=F+" AZIMUTH";
						DefaultSet=false;
					}
					else if (s=="DIVERGENCE") {
						BeamExport[Ns].Divergence=true;
						F=F+" DIVERGENCE";
						DefaultSet=false;
					} else {
						S="WARNING: The keyword "+s+" in SAVE line is not recognized and will be ignored";
						ShowError(S);
					}
				}
			}

			if (DefaultSet) {
				BeamExport[Ns].Phase=true;
				BeamExport[Ns].Energy=true;
				BeamExport[Ns].Radius=true;
				BeamExport[Ns].Azimuth=true;
				BeamExport[Ns].Divergence=true;
			}

		 /*	if (BeamExport[Ns].SpecialFormat!=NOBEAM) {
				BeamExport[Ns].Phase=false;
				BeamExport[Ns].Energy=false;
				BeamExport[Ns].Radius=false;
				BeamExport[Ns].Azimuth=false;
				BeamExport[Ns].Divergence=false;
			}         */

			//check the numbers
			if (BeamExport[Ns].N1<0 || BeamExport[Ns].N2<0) {
				S="WARNING: The particle numbers in SAVE line can not be negative. The region will be ignored.";
				ShowError(S);
				BeamExport[Ns].N1=0;
				BeamExport[Ns].N2=0;
			} else if (BeamExport[Ns].N2!=0 && BeamExport[Ns].N1>BeamExport[Ns].N2) {
				int temp=BeamExport[Ns].N2; //switch the numbers
				BeamExport[Ns].N2=BeamExport[Ns].N1;
				BeamExport[Ns].N1=temp;
			}
		ParsedStrings->Add(F);
	} else {    // else skip the line
		S="WARNING: The format of SAVE line is wrong and will be ignored.";
		ShowError(S);
	}

	return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::ParseLines(TInputLine *Lines,int N,bool OnlyParameters)
{
	int Ni=0, Nsec=0, Ns=0, Nq=0, Ndat=0;
	double dF=0;
	double F_last=0, P_last=0, master_phase=0;

	TError Error=ERR_NO;

	bool BeamDefined=false;
	bool CurrentDefined=false;
	bool CellDefined=false;
	bool PowerDefined=false;

	bool NewCell=true;
	AnsiString F,S,s;
    ParsedStrings->Clear();

   // FILE *logFile;
	for (int k=0;k<N;k++){
	 /*   logFile=fopen("BeamSolver.log","a");
        fprintf(logFile,"ParseLines: Line %i \n",k);
		fclose(logFile);     */
		switch (Lines[k].P) {
			case UNDEFINED: {
				throw std::logic_error("Unhandled TInputParameter UNDEFINED in TBeamSolver::ParseLines");
			}
			case OPTIONS:{
				Error=ParseOptions(&Lines[k]);
				break;
			}
			case SPCHARGE:{
				Error=ParseSpaceCharge(&Lines[k]);
				break;
			}
			case SOLENOID:{
				Error=ParseSolenoid(&Lines[k]);
				break;
			}
			case BEAM:{
				Error=ParseBeam(&Lines[k]);
				BeamDefined=Error==ERR_NO;
				break;
			}
			case CURRENT:{
				Error=ParseCurrent(&Lines[k]);
				CurrentDefined=Error==ERR_NO;
				break;
			}
			case PARTICLES:{
				Error=ParseParticleType(&Lines[k]);
				break;
			}
			case STRUCT:{
				Error=ParseStruct(&Lines[k],Ndat);
				Ndat++;
				break;
			}
			case CELL:{ }
			case CELLS:{
				if(!OnlyParameters){
					if (NewCell && !PowerDefined) {
						S="ERROR: The RF power source must be defined before each RF section. Put POWER line in correct format into the input file before the end of each RF section. Note that DRIFT element terminates the defined RF power!";
						ShowError(S);
						return ERR_FORMAT;
					}
					if (Lines[k].P==CELL) {
						Error=ParseSingleCell(&Lines[k],Ni,Nsec,NewCell);
						Ni++;
					} else if (Lines[k].P==CELLS) {
						Error=ParseMultipleCells(&Lines[k],Ni,Nsec,NewCell);
						Ni+=Lines[k].S[0].ToInt();
					}

					CellDefined=Error==ERR_NO;

					NewCell=false;

					if (StructPar.ElementsLimit>-1 &&Ni>=StructPar.ElementsLimit)
						OnlyParameters=true;
				}

				break;
			}
			case QUAD: {}
			case DRIFT:{
				if(!OnlyParameters){
					if (Lines[k].P==QUAD){
						Error=ParseQuad(&Lines[k],Ni,Nsec);
						Nq++;
					}
					else
						Error=ParseDrift(&Lines[k],Ni,Nsec);
					NewCell=true;
					PowerDefined=false;
					Ni++;
				}

				if (StructPar.ElementsLimit>-1 && Ni>=StructPar.ElementsLimit)
					OnlyParameters=true;

				break;
			}
			case POWER:{
				Error=ParsePower(&Lines[k],Nsec);
				if (Error==ERR_NO){
					double phi=StructPar.Sections[Nsec].PhaseShift;
					StructPar.Sections[Nsec].PhaseShift-=master_phase;
					master_phase=phi;

					Nsec++;
					NewCell=true;
					PowerDefined=true;
				} else
					return ERR_COUPLER;
				break;
			}

			case DUMP:{
				Error=ParseDump(&Lines[k],Ns,Ni);
				Ns++;
				break;
			}
			case COMMENT:{
				ParsedStrings->Add(Lines[k].S[0]);
				break;
			}
		}

		if (Error!=ERR_NO)
			return Error;
	}

	if (!BeamDefined) {
		S="ERROR: The beam particles distribution not defined. Put BEAM line in correct format into the input file";
		ShowError(S);
		Error=ERR_FORMAT;
	}
	if (!CurrentDefined) {
		S="ERROR: The beam current is not defined. Put CURRENT line in correct format into the input file";
		ShowError(S);
		Error=ERR_FORMAT;
	}
	if (Ni<1) {
		S="ERROR: The accelerating structure is not defined. Put at least one element (CELL, DRIFT) line in correct format into the input file";
		ShowError(S);
		Error=ERR_FORMAT;
	}
   /*	if (CellDefined && !PowerDefined) {
		ShowError("ERROR: The RF power source must be defined before each RF section. Put POWER line in correct format into the input file before the end of each RF section");
		Error=ERR_FORMAT;
	}       */

	return Error;
}
//---------------------------------------------------------------------------
TError TBeamSolver::LoadData(int Nlim)
{
	return LoadData("PARSED.TXT",Nlim);
}
//---------------------------------------------------------------------------
TError TBeamSolver::LoadData(AnsiString LogFileName, int Nlim)
{
    const char *FileName=InputFile.c_str();
	LoadIniConstants();
    InputStrings->Clear();
	StructPar.ElementsLimit=Nlim; //# of cells limit. Needed for optimizer. Don't remove

    DataReady=false;

    if (strncmp(FileName, "", 1) == 0) {
        return ERR_NOFILE;
    }
    if (!FileExists(FileName)) {
        return ERR_OPENFILE;
    }

	AnsiString S;
    TInputLine *Lines;
    int i=-1,j=0,N=0;
    TError Parsed;

    Lines=ParseFile(N);
	//InputStrings->LoadFromFile(InputFile);

	ResetStructure();

	for (int k=0;k<N;k++){
		if (Lines[k].P==CELL || Lines[k].P==DRIFT)
			StructPar.NElements++;
		else if (Lines[k].P==CELLS)
			StructPar.NElements+=Lines[k].S[0].ToInt();
		else if (Lines[k].P==POWER)
			StructPar.NSections++;
		else if (Lines[k].P==QUAD) {
			StructPar.NElements++;
			StructPar.NMaps++;
		} else if (Lines[k].P==STRUCT)
			StructPar.NStructData++;
	}

	if (StructPar.ElementsLimit>-1 && StructPar.NElements>=StructPar.ElementsLimit){  //# of cells limit. Needed for optimizer
		StructPar.NElements=StructPar.ElementsLimit;
		S="The number of elements exceeds the user-defined limit. Check the hellweg.ini file";
		ShowError(S);
	}


	StructPar.Cells = new TCell[StructPar.NElements+1];  //+1 - end point
	for (int k=0;k<StructPar.NElements+1;k++){
		//StructPar.Cells[k].Dump=false;
		StructPar.Cells[k].Drift=false;
		StructPar.Cells[k].First=false;
	}

	if (StructPar.NSections==0){
		StructPar.NSections=1; //default section!
		StructPar.Sections=new TSectionParameters [StructPar.NSections];
		//StructPar.Sections[0].Wavelength=1;
		StructPar.Sections[0].Frequency=c;
		StructPar.Sections[0].Power=0;
		StructPar.Sections[0].NCells=0;
	} else
		StructPar.Sections=new TSectionParameters [StructPar.NSections];

	if (StructPar.NStructData!=0) {
		StructPar.StructData=new TStructData [StructPar.NStructData];
		for (int k = 0; k < StructPar.NStructData; k++) {
			StructPar.StructData[k].Default=false;
			StructPar.StructData[k].Phase=0;
			StructPar.StructData[k].DataReady=false;
		}
	}

	Parsed=ParseLines(Lines,N);
    ParsedStrings->Add("END");
    InputStrings->AddStrings(ParsedStrings);

	if (LogFileName!="")
		ParsedStrings->SaveToFile(LogFileName);

    delete[] Lines;
    DataReady=true;
    return Parsed;
}
//---------------------------------------------------------------------------
TError TBeamSolver::MakeBuncher(TCell& iCell)
{
    const char *FileName=InputFile.c_str();
    InputStrings->Clear();
    //LoadIniConstants();

    DataReady=false;
    TError Error;

    if (strncmp(FileName, "", 1) == 0) {
        return ERR_NOFILE;
    }
    if (!FileExists(FileName)) {
        return ERR_OPENFILE;
    }

	ResetStructure();

    AnsiString F,s;
    TInputLine *Lines;
    int i=-1,j=0,N=0;

    Lines=ParseFile(N);
    Error=ParseLines(Lines,N,true);

	//NOT CLEAR. MAY NEED TO UNCOMMENT AND REWRITE
   if (StructPar.Sections[0].Frequency*1e6!=iCell.F0 || StructPar.Sections[0].Power*1e6!=iCell.P0){
		StructPar.Sections[0].Frequency=iCell.F0*1e-6;
		StructPar.Sections[0].Power=iCell.P0*1e-6;
		F="POWER "+s.FormatFloat("#0.00",StructPar.Sections[0].Power)+"\t"+s.FormatFloat("#0.00",StructPar.Sections[0].Frequency);
        ParsedStrings->Add(F);
	}

    ParsedStrings->Add("");

    double k1=0,k2=0,k3=0,k4=0,k5=0;
	double Am=iCell.ELP*sqrt(StructPar.Sections[0].Power*1e6)/BeamPar.Wnorm;
    k1=3.8e-3*(Power(10.8,Am)-1);
    k2=1.25*Am+2.25;
    k3=0.5*Am+0.15*sqrt(Am);
    k4=0.5*Am-0.15*sqrt(Am);
    k5=1/sqrt(1.25*sqrt(Am));

	double b=0,A=0,ksi=0,lmb=0,th=0;
	double W0=Beam[0]->GetAverageEnergy();
	double b0=MeVToVelocity(W0,BeamPar.W0);

	lmb=1e-6*c/StructPar.Sections[0].Frequency;
    if (iCell.Mode==90)
        th=pi/2;
    else if (iCell.Mode==120)
        th=2*pi/3;

	StructPar.NElements=0;
	StructPar.NSections=1;

	TSectionParameters Sec;
	Sec=StructPar.Sections[0];
	delete [] StructPar.Sections;
	StructPar.Sections=new TSectionParameters [StructPar.NSections];
	StructPar.Sections[0]=Sec;
   //	Ncells=0;
    int iB=0;

    do {
        b=(2/pi)*(1-b0)*arctg(k1*Power(ksi,k2))+b0;
       //   b=(2/pi)*arctg(0.25*sqr(10*ksi*lmb)+0.713);
		iB=10000*b;
        ksi+=b*th/(2*pi);
		StructPar.NElements++;
    } while (iB<9990);
    //} while (ksi*lmb<1.25);

	StructPar.Cells=new TCell[StructPar.NElements];

    ksi=0;
	for (int i=0;i<StructPar.NElements;i++){
        ksi+=0.5*b*th/(2*pi);

        b=(2/pi)*(1-b0)*arctg(k1*Power(ksi,k2))+b0;
       //   b=(2/pi)*arctg(0.25*sqr(10*ksi*lmb)+0.713);

        A=k3-k4*cos(pi*ksi/k5);
        //A=3.0e6*sin(0.11*sqr(10*ksi*lmb)+0.64);
       /*
        if (ksi*lmb>0.3)
            A=3.00e6;     */

        if (ksi>k5)
            A=Am;

        ksi+=0.5*b*th/(2*pi);

		StructPar.Cells[i].Mode=iCell.Mode;
		StructPar.Cells[i].beta=(2/pi)*(1-b0)*arctg(k1*Power(ksi,k2))+b0;
        //Cells[i].betta=(2/pi)*arctg(0.25*sqr(10*ksi*lmb)+0.713);

		StructPar.Cells[i].ELP=A*BeamPar.Wnorm/sqrt(1e6*StructPar.Sections[0].Power);
       //   Cells[i].ELP=A*lmb/sqrt(1e6*P0);

		GetDimensions(StructPar.Cells[i]);

		F="CELL "+s.FormatFloat("#0",iCell.Mode)+"\t"+s.FormatFloat("#0.000",StructPar.Cells[i].beta)+"\t"+s.FormatFloat("#0.000",StructPar.Cells[i].ELP)+"\t"+s.FormatFloat("#0.000000",StructPar.Cells[i].AL32)+"\t"+s.FormatFloat("#0.000000",StructPar.Cells[i].AkL);
		ParsedStrings->Add(F);

		StructPar.Cells[i].F0=StructPar.Sections[0].Frequency*1e6;
		StructPar.Cells[i].P0=StructPar.Sections[0].Power*1e6;
		StructPar.Cells[i].dF=0;
		StructPar.Cells[i].Drift=false;
		StructPar.Cells[i].First=false;
    }
    StructPar.Cells[0].First=true;

    ParsedStrings->Add("END");
    InputStrings->AddStrings(ParsedStrings);
    ParsedStrings->SaveToFile("PARSED.TXT");

    return Error;
}
//---------------------------------------------------------------------------
int TBeamSolver::ChangeCells(int N)
{
    TCell *iCells;
	iCells=new TCell[StructPar.NElements];

	for (int i=0;i<StructPar.NElements;i++)
		iCells[i]=StructPar.Cells[i];

	delete[] StructPar.Cells;

	int Nnew=StructPar.NElements<N?StructPar.NElements:N;
	int Nprev=StructPar.NElements;
	StructPar.NElements=N;

	StructPar.Cells=new TCell[StructPar.NElements];

    for (int i=0;i<Nnew;i++)
        StructPar.Cells[i]=iCells[i];

    delete[] iCells;

    return Nprev;
}
//---------------------------------------------------------------------------
void TBeamSolver::AppendCells(TCell& iCell,int N)
{
	int Nprev=ChangeCells(StructPar.NElements+N);
    int i=InputStrings->Count-1;
    InputStrings->Delete(i);

    i=Nprev;
	StructPar.Cells[i]=iCell;
	GetDimensions(StructPar.Cells[i]);

	for (int j=Nprev;j<StructPar.NElements;j++)
		StructPar.Cells[j]=StructPar.Cells[i];

    AnsiString F,F1,s;
	F1=F=s.FormatFloat("#0",iCell.Mode)+"\t"+s.FormatFloat("#0.000",StructPar.Cells[i].beta)+"\t"+s.FormatFloat("#0.000",StructPar.Cells[i].ELP)+"\t"+s.FormatFloat("#0.000000",StructPar.Cells[i].AL32)+"\t"+s.FormatFloat("#0.000000",StructPar.Cells[i].AkL);
	if (N==1)
		F="CELL "+F1;
    else
        F="CELLS "+s.FormatFloat("#0",N)+F1;

    InputStrings->Add(F);
    InputStrings->Add("END");

}
//---------------------------------------------------------------------------
void TBeamSolver::AddCells(int N)
{
	TCell pCell=StructPar.Cells[StructPar.NElements-1];
	AppendCells(pCell,N);
}
//---------------------------------------------------------------------------
TCell TBeamSolver::GetCell(int j)
{
    if (j<0)
        j=0;
	if (j>=StructPar.NElements)
		j=StructPar.NElements-1;

	return StructPar.Cells[j];
}
//---------------------------------------------------------------------------
TCell TBeamSolver::LastCell()
{
    return GetCell(StructPar.NElements-1);
}
//---------------------------------------------------------------------------
void TBeamSolver::ChangeInputCurrent(double Ib)
{
    BeamPar.Current=Ib;
}
//---------------------------------------------------------------------------
void TBeamSolver::SetBarsNumber(double Nbin)
{
	for (int i=0;i<Npoints;i++)
		Beam[i]->SetBarsNumber(Nbin);
}
//---------------------------------------------------------------------------
double *TBeamSolver::SmoothInterpolation(double *x,double *X,double *Y,int Nbase,int Nint,double p0,double *W)
{
    TSpline *Spline;
    double *y;
    //y=new double[Nint];

    Spline=new TSpline;
    Spline->MakeSmoothSpline(X,Y,Nbase,p0,W);
    y=Spline->Interpolate(x,Nint);

    delete Spline;

    return y;
}
//---------------------------------------------------------------------------
double *TBeamSolver::SplineInterpolation(double *x,double *X,double *Y,int Nbase,int Nint)
{
    TSpline *Spline;
    double *y;
    y=new double[Nint];

    Spline=new TSpline;
    Spline->MakeCubicSpline(X,Y,Nbase);
    y=Spline->Interpolate(x,Nint);

    delete Spline;

    return y;
}
//---------------------------------------------------------------------------
double *TBeamSolver::LinearInterpolation(double *x,double *X,double *Y,int Nbase,int Nint)
{
    TSpline *Spline;
    double *y;
	//y=new double[Nint];

    Spline=new TSpline;
    Spline->MakeLinearSpline(X,Y,Nbase);
    y=Spline->Interpolate(x,Nint);

    delete Spline;

    return y;
}
//---------------------------------------------------------------------------
void TBeamSolver::DeleteMesh()
{
	delete[] Structure;
	Structure=NULL;
	Npoints=0;
}
//---------------------------------------------------------------------------
void TBeamSolver::CreateMesh()
{
	if (Npoints!=0)
		DeleteMesh();

   //	int Njmp=0;
	for(int i=0;i<StructPar.NElements;i++){
		Npoints+=StructPar.Cells[i].Mesh;
		if (StructPar.Cells[i].First){
			Npoints++;
			//Njmp++;
		}
	}

	Structure=new TStructure[Npoints];
}
//---------------------------------------------------------------------------
TFieldMap2D TBeamSolver::ParseMapFile(AnsiString &F)
{
	TFieldMap2D Map;

	int Nmax=0;
	int NumRow=4,NumDim=2; //x y Bx By
	double **Unq=NULL;
	int Nx=0,Ny=0;

	int N=0;
	AnsiString L,S;
	double x=0,y=0;
	bool Num=false;

	Nmax=NumPointsInFile(F,NumRow);
	Unq=new double*[NumRow];
	for (int i = 0; i < NumRow; i++) {
		Unq[i]=new double[Nmax];
	}

	std::ifstream fs(F.c_str());

	while (!fs.eof()){
		L=GetLine(fs);
		if (NumWords(L)==NumRow){       //Check if only two numbers in the line
			for (int i = 0; i < NumRow; i++) {
				S=ReadWord(L,i+1);
				Num=IsNumber(S);
				if (Num)
					Unq[i][N]=S.ToDouble();
				else
					break;
				if (i<NumDim) {
					Unq[i][N]/=100; //from [cm]
				} else
					Unq[i][N]/=10000; //from [Gs]
			}
			if (Num)
				N++;
		}
	}

	Map.Dim.Nx=CountUnique(Unq[0],Nmax);
	Map.Dim.Ny=CountUnique(Unq[1],Nmax);

	Map.Field=new TField* [Map.Dim.Nx];
	for (int i = 0; i < Map.Dim.Nx; i++)
		Map.Field[i]=new TField [Map.Dim.Ny];

	Map.Piv.X=MakePivot(Unq[0],Nmax,Map.Dim.Nx);
	Map.Piv.Y=MakePivot(Unq[1],Nmax,Map.Dim.Ny);

	for (int j = 0; j < Map.Dim.Nx; j++) {
		x=Map.Piv.X[j];
		for (int k = 0; k < Map.Dim.Ny; k++) {
			y=Map.Piv.Y[k];
			for (int s = 0; s < Nmax; s++) {
				if (Unq[0][s]==x && Unq[1][s]==y) {
					Map.Field[j][k].r=Unq[2][s];
					Map.Field[j][k].th=Unq[3][s];
					break;
				}
			}
		}
	}

	fs.close();
	DeleteDoubleArray(Unq,NumRow);

	return Map;
}
//---------------------------------------------------------------------------
void TBeamSolver::CreateMaps()
{
	ResetMaps();
	double kc=0;
	int q=0;

    if(StructPar.NMaps <= 0) {
        return;
    }

	QuadMaps=new TFieldMap2D [StructPar.NMaps];
	for(int i=0;i<StructPar.NElements;i++){
		if (StructPar.Cells[i].Magnet.MagnetType==MAG_QUAD){
			kc=sqr(c)/(BeamPar.Wnorm*StructPar.Cells[i].F0);
			QuadMaps[q]=ParseMapFile(StructPar.Cells[i].Magnet.File);
			for (int j = 0; j < QuadMaps[q].Dim.Nx; j++) {
				QuadMaps[q].Piv.X[j]*=StructPar.Cells[i].F0/c;
				for (int k = 0; k < QuadMaps[q].Dim.Ny; k++) {
					QuadMaps[q].Field[j][k].r*=StructPar.Cells[i].Magnet.BField*kc;
					QuadMaps[q].Field[j][k].th*=StructPar.Cells[i].Magnet.BField*kc;
				}
			}
			for (int k = 0; k < QuadMaps[q].Dim.Ny; k++) {
				QuadMaps[q].Piv.Y[k]*=StructPar.Cells[i].F0/c;
			}
			q++;
		} /* else {
		   QuadMaps[q].Piv.X=NULL;
           QuadMaps[q].Piv.Y=NULL;
        }     */
	}
}
//---------------------------------------------------------------------------
void TBeamSolver::CreateStrucutre()
{
	int Extra=0,k0=0,k=0,q=0,first_drift=0;
	double z=0,zm=0,D=0,x=0,lmb=0,theta=0;
	bool isInput=false;
	TFieldMap2D Map;

	for (int i=0;i<StructPar.NElements;i++){
		Extra=0;
		Map.Field=NULL;
		if (i==StructPar.NElements-1 || StructPar.Cells[i+1].First)
			Extra=1;

		if (StructPar.Cells[i].First)
			z-=zm;

		//double lmb=1;
		if (StructPar.Cells[i].Drift){
			D=StructPar.Cells[i].beta;
			isInput=false;
		   /*	for (int j=i;j<StructPar.NElements;j++){
				if (!StructPar.Cells[j].Drift){
					StructPar.Cells[i].betta=StructPar.Cells[j].betta;
					lmb=c/StructPar.Cells[j].F0;
					isInput=true;
					break;
				}
			}  */
			//ADD QUAD FIELD
			if (StructPar.Cells[i].Magnet.MagnetType==MAG_QUAD) {
				Map=QuadMaps[q];
				q++;
			}

			if (!isInput){
				for (int j=i;j>=0;j--){
					if (!StructPar.Cells[j].Drift){
						StructPar.Cells[i].beta=StructPar.Cells[j].beta;
						lmb=c/StructPar.Cells[j].F0;
						isInput=true; //Same wavelength as previously defined
						break;
					} else
						first_drift=j;
				}
			}
			if (!isInput){
				StructPar.Cells[i].beta=StructPar.Cells[first_drift].beta;
				lmb=c/StructPar.Cells[first_drift].F0;
				StructPar.Cells[i].beta=1;
				//lmb=1;
			}
        }else{
			lmb=c/StructPar.Cells[i].F0;
			theta=StructPar.Cells[i].Mode*pi/180;
			D=StructPar.Cells[i].beta*lmb*theta/(2*pi);
		}
		zm=D/StructPar.Cells[i].Mesh;
		k0=k;

		Structure[k].dF=StructPar.Cells[i].dF;

		double P0=StructPar.Cells[i].P0;
		double beta=StructPar.Cells[i].beta;
		double E=StructPar.Cells[i].ELP;
		double Rp=sqr(E)/2;
		double A=P0>0?E*sqrt(P0)/BeamPar.Wnorm:0;
		double B=Rp/(2*BeamPar.Wnorm);
		double alpha=StructPar.Cells[i].AL32/(lmb*sqrt(lmb));

		for (int j=0;j<StructPar.Cells[i].Mesh+Extra;j++){
		   //	X_int[k]=z/lmb;
			Structure[k].ksi=z/lmb;
			Structure[k].lmb=lmb;
			Structure[k].P=P0;
			if (j>0)
				Structure[k].dF=0;

		   //	if (StructPar.Cells[i].beta<1)
			Structure[k].beta=beta;
		  //	else
			  //	Structure[k].betta=MeVToVelocity(EnergyLimit,BeamPar.W0);   */

			Structure[k].E=E;
			Structure[k].A=A;
			Structure[k].Rp=Rp;
			Structure[k].B=B;
			Structure[k].alpha=alpha;

			Structure[k].drift=StructPar.Cells[i].Drift;
			if (StructPar.Cells[i].Drift)
				Structure[k].Ra=StructPar.Cells[i].AkL/lmb;
			else
				Structure[k].Ra=StructPar.Cells[i].AkL;//*lmb;

			Structure[k].jump=false;
			Structure[k].CellNumber=i;

			Structure[k].Bmap=Map;
			z+=zm;
			k++;
		}
		if (StructPar.Cells[i].First)
			Structure[k0].jump=true;
		for (int j=0; j < Ndump; j++) {
			if (BeamExport[j].NElement==0)
				BeamExport[j].Nmesh=0;
			else if (BeamExport[j].NElement==i+1)
				BeamExport[j].Nmesh=k-1;

		}
	}
}
//---------------------------------------------------------------------------
void TBeamSolver::TransformMagneticToCYL1D()
{
	TPhaseSpace R,C;
	double x=0, y=0;
	for (int k = 0; k < ExternalMagnetic.Dim.Nz; k++) {
		for (int i=0;i<ExternalMagnetic.Dim.Nx;i++){
			x=ExternalMagnetic.Piv.X[i];
			y=0;

			R.x=x;
			R.y=y;

			R.py=ExternalMagnetic.Field[k][0][i].r;
			R.px=ExternalMagnetic.Field[k][0][i].th;

			C=CartesianToCylinrical(R);

			ExternalMagnetic.Field[k][0][i].r=C.px;
			ExternalMagnetic.Field[k][0][i].th=C.py;
		}
	}

}
//---------------------------------------------------------------------------
void TBeamSolver::TransformMagneticToCYL2D()
{
	double Bx=0,By=0,Br=0,Bth=0;
	double x=0,y=0,r=0,th=0,dx=1e32,dy=1e32;
	TField **B;

	//FIND MESH RESOLUTION
	double xmax=0,dxmin=1e32;
	double ymax=0,dymin=1e32;
	for (int i=0;i<ExternalMagnetic.Dim.Nx;i++){
		if (i>0)
			dx=ExternalMagnetic.Piv.X[i]-x;
		x=ExternalMagnetic.Piv.X[i];
		if (mod(x)>xmax)
			xmax=mod(x);
		if (mod(dx)<dxmin)
			dxmin=mod(dx);
	}

	for (int i=0;i<ExternalMagnetic.Dim.Ny;i++){
		if (i>0)
			dy=ExternalMagnetic.Piv.Y[i]-y;
		y=ExternalMagnetic.Piv.Y[i];
		if (mod(y)>ymax)
			ymax=mod(y);
		if (mod(dy)<dymin)
			dymin=mod(dy);
	}

	double rmax=0,dr=0,thmax=0,dth=0,dth1=0,dth2=0;
	rmax=sqrt(sqr(xmax)+sqr(ymax));
	dr=sqrt(sqr(dxmin)+sqr(dymin));
	thmax=2*pi;
	dth1=arctg(dymin/xmax);
	dth2=arctg(dxmin/ymax);
	dth=dth1<dth2?dth1:dth2;

	//TRANSFORM X,Y PIVOTS TO R,TH
	int Nr=0, Nth=0;
	Nr=ceil(rmax/dr);
	Nth=ceil(thmax/dth);

	double *Rpiv=new double[Nr];
	double *Thpiv=new double[Nth];

	for (int i = 0; i < Nr; i++)
		Rpiv[i]=i*rmax/(Nr-1);

	for (int i = 0; i < Nth; i++)
		Thpiv[i]=i*thmax/(Nth-1);

	//ADJUST TO NEW PIVOTS
	TPhaseSpace R,C;
	for (int k = 0; k < ExternalMagnetic.Dim.Nz; k++) {
		B=new TField*[Nth];
		for (int i = 0; i < Nth; i++)
			B[i]=new TField[Nr];

		for (int i=0;i<Nth;i++){
			th=Thpiv[i];
			for (int j=0;j<Nr;j++){
				r=Rpiv[j];

				x=r*cos(th);
				y=r*sin(th);

				R.x=x;
				R.y=y;
				B[i][j]=BiLinearInterpolation(y,x,ExternalMagnetic.Piv.Y,ExternalMagnetic.Piv.X,
				ExternalMagnetic.Dim.Ny,ExternalMagnetic.Dim.Nx,ExternalMagnetic.Field[k]);

				R.px=B[i][j].r;
				R.py=B[i][j].th;

				C=CartesianToCylinrical(R);

				B[i][j].r=C.px;
				B[i][j].th=C.py;
			}

		}
		for (int j=0;j<ExternalMagnetic.Dim.Ny;j++)
			delete[] ExternalMagnetic.Field[k][j];
		delete[] ExternalMagnetic.Field[k];
		ExternalMagnetic.Field[k]=B;
	}

	delete[] ExternalMagnetic.Piv.X;
	delete[] ExternalMagnetic.Piv.Y;
	ExternalMagnetic.Piv.X=Rpiv;
	ExternalMagnetic.Piv.Y=Thpiv;
	ExternalMagnetic.Dim.Nx=Nr;
	ExternalMagnetic.Dim.Ny=Nth;
}
//---------------------------------------------------------------------------
void TBeamSolver::AdjustMagneticMesh()
{
	double *Zpiv_new=new double[Npoints];
	TField *B0=new TField[ExternalMagnetic.Dim.Nz];
	for (int i = 0; i < Npoints; i++)
		Zpiv_new[i]=Structure[i].ksi*Structure[i].lmb;

	TField ***B=new TField**[Npoints];

	for (int i = 0; i < Npoints; i++) {
		B[i]=new TField*[ExternalMagnetic.Dim.Ny];
		for (int j = 0; j < ExternalMagnetic.Dim.Ny; j++) {
			B[i][j]=new TField[ExternalMagnetic.Dim.Nx];
		}
	}

	for (int j = 0; j < ExternalMagnetic.Dim.Ny; j++) {
		for (int k = 0; k < ExternalMagnetic.Dim.Nx; k++) {
			for (int p = 0; p < ExternalMagnetic.Dim.Nz; p++) {
				B0[p]=ExternalMagnetic.Field[p][j][k];
			}
			for (int i = 0; i < Npoints; i++){
				B[i][j][k]=LinInterpolation(Zpiv_new[i],ExternalMagnetic.Piv.Z,ExternalMagnetic.Dim.Nz,B0);
			}
		}
	}

	for (int i = 0; i < ExternalMagnetic.Dim.Nz; i++) {
		for (int j = 0; j < ExternalMagnetic.Dim.Ny; j++)
				delete[] ExternalMagnetic.Field[i][j];
		delete[] ExternalMagnetic.Field[i];
	}

	delete[] ExternalMagnetic.Field;
	ExternalMagnetic.Field=B;

	delete[] ExternalMagnetic.Piv.Z;
	ExternalMagnetic.Piv.Z=Zpiv_new;
	ExternalMagnetic.Dim.Nz=Npoints;

	delete[] B0;
}
//---------------------------------------------------------------------------
void TBeamSolver::CalculateFieldDerivative()
{
	if (BeamPar.Demagnetize)
		ExternalMagnetic.Field[0][0][0].r=ExternalMagnetic.Field[0][0][0].z/0.01;
	else
		ExternalMagnetic.Field[0][0][0].r=0;

	ExternalMagnetic.Field[ExternalMagnetic.Dim.Nz-1][0][0].r=0;

	double z1=0,z2=0,B1=0,B2=0,dBdr=0;

	for (int i=1;i<ExternalMagnetic.Dim.Nz-1;i++){
		z1=ExternalMagnetic.Piv.Z[i-1];
		z2=ExternalMagnetic.Piv.Z[i+1];
		B1=ExternalMagnetic.Field[i-1][0][0].z;
		B2=ExternalMagnetic.Field[i+1][0][0].z;
		dBdr=(B2-B1)/(z2-z1);

		ExternalMagnetic.Field[i][0][0].r=dBdr;
	}
}
//---------------------------------------------------------------------------
void TBeamSolver::CalculateRadialField()
{
	double Rmax=GetMaxAperture();
	CalculateRadialField(Rmax);
}
//---------------------------------------------------------------------------
void TBeamSolver::CalculateRadialField(double Rmax)
{
	for (int j = ExternalMagnetic.Dim.Nx-1; j >= 0; j--) {
		double r=Rmax*j/(ExternalMagnetic.Dim.Nx-1);
		ExternalMagnetic.Piv.X[j]=r;
		for (int i = 0; i < ExternalMagnetic.Dim.Nz; i++) {
			ExternalMagnetic.Field[i][0][j].z=ExternalMagnetic.Field[i][0][0].z;
			ExternalMagnetic.Field[i][0][j].r=ExternalMagnetic.Field[i][0][0].r;
			ExternalMagnetic.Field[i][0][j].r*=-(r/2); //Er=-r/2 * dEz/dz
			ExternalMagnetic.Field[i][0][j].th=0;
		}
	}
}
//---------------------------------------------------------------------------
void TBeamSolver::MakePivots()
{
	ExternalMagnetic.Piv.X=new double[ExternalMagnetic.Dim.Nx];
	ExternalMagnetic.Piv.Y=new double[ExternalMagnetic.Dim.Ny];
	ExternalMagnetic.Piv.Z=new double[ExternalMagnetic.Dim.Nz];
}
//---------------------------------------------------------------------------
void TBeamSolver::MakeUniformSolenoid()
{
	MakePivots();
	ExternalMagnetic.Piv.Z[0]=Structure[0].ksi*Structure[0].lmb;
	ExternalMagnetic.Field[0][0][0].z=StructPar.SolenoidPar.BField;
	ExternalMagnetic.Field[0][0][0].r=0;
	ExternalMagnetic.Field[0][0][0].th=0;

	ExternalMagnetic.Piv.Z[1]=Structure[Npoints-1].ksi*Structure[Npoints-1].lmb;
	ExternalMagnetic.Field[1][0][0].z=StructPar.SolenoidPar.BField;;
	ExternalMagnetic.Field[1][0][0].r=0;
	ExternalMagnetic.Field[1][0][0].th=0;

	CalculateRadialField();
	ExternalMagnetic.Piv.Y[0]=0;
}
//---------------------------------------------------------------------------
void TBeamSolver::MakeAnalyticalSolenoid()
{
	MakePivots();
	if (StructPar.SolenoidPar.Length<0)
		StructPar.SolenoidPar.Length=Structure[Npoints-1].ksi*Structure[Npoints-1].lmb;

	ExternalMagnetic.Piv.Z[0]=StructPar.SolenoidPar.StartPos-StructPar.SolenoidPar.Lfringe-DerAccuracy;
	ExternalMagnetic.Field[0][0][0].z=0;
	ExternalMagnetic.Field[0][0][0].r=0; //dEz/dz
	ExternalMagnetic.Field[0][0][0].th=0;

	ExternalMagnetic.Piv.Z[1]=StructPar.SolenoidPar.StartPos-StructPar.SolenoidPar.Lfringe;
	ExternalMagnetic.Field[1][0][0].z=0;
	ExternalMagnetic.Field[1][0][0].r=StructPar.SolenoidPar.BField/StructPar.SolenoidPar.Lfringe;
	ExternalMagnetic.Field[1][0][0].th=0;

	ExternalMagnetic.Piv.Z[2]=StructPar.SolenoidPar.StartPos;
	ExternalMagnetic.Field[2][0][0].z=StructPar.SolenoidPar.BField;
	if (StructPar.SolenoidPar.StartPos==0)
		ExternalMagnetic.Field[2][0][0].r=0;
	else
		ExternalMagnetic.Field[2][0][0].r=StructPar.SolenoidPar.BField/StructPar.SolenoidPar.Lfringe;
	ExternalMagnetic.Field[2][0][0].th=0;

	ExternalMagnetic.Piv.Z[3]=StructPar.SolenoidPar.StartPos+DerAccuracy;
	ExternalMagnetic.Field[3][0][0].z=StructPar.SolenoidPar.BField;
	ExternalMagnetic.Field[3][0][0].r=0;
	ExternalMagnetic.Field[3][0][0].th=0;

	ExternalMagnetic.Piv.Z[4]=StructPar.SolenoidPar.StartPos+StructPar.SolenoidPar.Length-DerAccuracy;
	ExternalMagnetic.Field[4][0][0].z=StructPar.SolenoidPar.BField;
	ExternalMagnetic.Field[4][0][0].r=0;
	ExternalMagnetic.Field[4][0][0].th=0;

	ExternalMagnetic.Piv.Z[5]=StructPar.SolenoidPar.StartPos+StructPar.SolenoidPar.Length;
	ExternalMagnetic.Field[5][0][0].z=StructPar.SolenoidPar.BField;
	ExternalMagnetic.Field[5][0][0].r=-StructPar.SolenoidPar.BField/StructPar.SolenoidPar.Lfringe;
	ExternalMagnetic.Field[5][0][0].th=0;

	ExternalMagnetic.Piv.Z[6]=StructPar.SolenoidPar.StartPos+StructPar.SolenoidPar.Length+StructPar.SolenoidPar.Lfringe;
	ExternalMagnetic.Field[6][0][0].z=0;
	ExternalMagnetic.Field[6][0][0].r=-StructPar.SolenoidPar.BField/StructPar.SolenoidPar.Lfringe;
	ExternalMagnetic.Field[6][0][0].th=0;

	ExternalMagnetic.Piv.Z[7]=StructPar.SolenoidPar.StartPos+StructPar.SolenoidPar.Length+StructPar.SolenoidPar.Lfringe+DerAccuracy;
	ExternalMagnetic.Field[7][0][0].z=0;
	ExternalMagnetic.Field[7][0][0].r=0;
	ExternalMagnetic.Field[7][0][0].th=0;

	CalculateRadialField();
	ExternalMagnetic.Piv.Y[0]=0;
}
//---------------------------------------------------------------------------
void TBeamSolver::ParseSolenoidFile(TParseStage P)
{
	TDimensions D0=ParseSolenoidLines(StructPar.SolenoidPar);
	int NumDim=D0.Nx;
	int NumRow=D0.Ny;

	int Nmax=0;
	double **Unq=NULL;
	int Nx=0,Ny=0,Nz=0;

	int N=0;
	AnsiString L,S;
	double x;
	bool Num=false;

	Nmax=NumPointsInFile(StructPar.SolenoidPar.File,NumRow);
	Unq=new double*[NumRow];
	for (int i = 0; i < NumRow; i++) {
		Unq[i]=new double[Nmax];
	}

	std::ifstream fs(StructPar.SolenoidPar.File.c_str());

	while (!fs.eof()){
		L=GetLine(fs);
		if (NumWords(L)==NumRow){       //Check if only two numbers in the line
			for (int i = 0; i < NumRow; i++) {
				S=ReadWord(L,i+1);
				Num=IsNumber(S);
				if (Num)
					Unq[i][N]=S.ToDouble();
				else
					break;
				if (i<NumDim) {
					Unq[i][N]/=100; //from [cm]
				} else
					Unq[i][N]/=10000; //from [Gs]
			}
			if (Num)
				N++;
		}
	}

	fs.close();

	switch (P) {
		case DIM_STEP:{
			if (NumDim==1) {
				ExternalMagnetic.Dim.Nz=CountUnique(Unq[0],Nmax);
				ExternalMagnetic.Dim.Nx=RMESH_DEFAULT;
				ExternalMagnetic.Dim.Ny=1;
			} else if (NumDim==2) {
				ExternalMagnetic.Dim.Nx=CountUnique(Unq[0],Nmax);
				ExternalMagnetic.Dim.Nz=CountUnique(Unq[1],Nmax);
				ExternalMagnetic.Dim.Ny=1;
			} else {
				ExternalMagnetic.Dim.Nx=CountUnique(Unq[0],Nmax);
				ExternalMagnetic.Dim.Ny=CountUnique(Unq[1],Nmax);
				ExternalMagnetic.Dim.Nz=CountUnique(Unq[2],Nmax);
			}
			break;
		}
		case PIV_STEP:{
			if (NumDim==1) {
				ExternalMagnetic.Piv.Z=MakePivot(Unq[0],Nmax,ExternalMagnetic.Dim.Nz);
				ExternalMagnetic.Piv.X=new double[ExternalMagnetic.Dim.Nx];
				ExternalMagnetic.Piv.Y=new double[ExternalMagnetic.Dim.Ny];
			} else if (NumDim==2) {
				ExternalMagnetic.Piv.X=MakePivot(Unq[0],Nmax,ExternalMagnetic.Dim.Nx);
				ExternalMagnetic.Piv.Z=MakePivot(Unq[1],Nmax,ExternalMagnetic.Dim.Nz);
				ExternalMagnetic.Piv.Y=new double[ExternalMagnetic.Dim.Ny];
			} else {
				ExternalMagnetic.Piv.X=MakePivot(Unq[0],Nmax,ExternalMagnetic.Dim.Nx);
				ExternalMagnetic.Piv.Y=MakePivot(Unq[1],Nmax,ExternalMagnetic.Dim.Ny);
				ExternalMagnetic.Piv.Z=MakePivot(Unq[2],Nmax,ExternalMagnetic.Dim.Nz);
			}
			//break;
		}
		case DATA_STEP:{
			double x=0,y=0,z=0,r=0;
			if (NumDim==1) {
				for (int i = 0; i < ExternalMagnetic.Dim.Nz; i++) {
					z=ExternalMagnetic.Piv.Z[i];
					for (int s = 0; s < Nmax; s++) {
						if (Unq[0][s]==z) {
							ExternalMagnetic.Field[i][0][0].z=Unq[1][s];
							ExternalMagnetic.Field[i][0][0].r=0;
							ExternalMagnetic.Field[i][0][0].th=0;
							break;
						}
					}
				}
			   /*	CalculateFieldDerivative();
				CalculateRadialField();
                // PERFORM AFTER MESH ADJUSTMENT
				*/
			} else if (NumDim==2) {
				for (int i = 0; i < ExternalMagnetic.Dim.Nz; i++) {
					z=ExternalMagnetic.Piv.Z[i];
					for (int j = 0; j < ExternalMagnetic.Dim.Nx; j++) {
						r=ExternalMagnetic.Piv.X[j];
						for (int s = 0; s < Nmax; s++) {
							if (Unq[0][s]==r && Unq[1][s]==z) {
								if (StructPar.SolenoidPar.ImportType==IMPORT_2DC) {
									ExternalMagnetic.Field[i][0][j].z=Unq[3][s];
									ExternalMagnetic.Field[i][0][j].r=Unq[2][s];
									ExternalMagnetic.Field[i][0][j].th=0;
								}   else if (StructPar.SolenoidPar.ImportType==IMPORT_2DR) {
									ExternalMagnetic.Field[i][0][j].z=Unq[4][s];
									ExternalMagnetic.Field[i][0][j].r=Unq[2][s];
									ExternalMagnetic.Field[i][0][j].th=Unq[3][s];;
								}
								break;
							}
						}
					}
				}
			} else {
              for (int i = 0; i < ExternalMagnetic.Dim.Nz; i++) {
					z=ExternalMagnetic.Piv.Z[i];
					for (int j = 0; j < ExternalMagnetic.Dim.Nx; j++) {
						x=ExternalMagnetic.Piv.X[j];
						for (int k = 0; k < ExternalMagnetic.Dim.Ny; k++) {
							y=ExternalMagnetic.Piv.Y[k];
							for (int s = 0; s < Nmax; s++) {
								if (Unq[0][s]==x && Unq[1][s]==y && Unq[2][s]==z) {
									ExternalMagnetic.Field[i][k][j].z=Unq[5][s];
									ExternalMagnetic.Field[i][k][j].r=Unq[3][s];
									ExternalMagnetic.Field[i][k][j].th=Unq[4][s];;
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	DeleteDoubleArray(Unq,NumRow);
}
//---------------------------------------------------------------------------
void TBeamSolver::CreateMagneticMap()
{
	ResetExternal();

	int Nlines=0;
	TDimensions D;

	// DEFINE MAGNETIC ARRAY DIMENSIONS
	switch (StructPar.SolenoidPar.ImportType) {
		case ANALYTIC_0D:{
			ExternalMagnetic.Dim.Ny=1;
			ExternalMagnetic.Dim.Nx=RMESH_DEFAULT;
			ExternalMagnetic.Dim.Nz=2;
			break;
		}
		case ANALYTIC_1D:{
			ExternalMagnetic.Dim.Ny=1;
			ExternalMagnetic.Dim.Nx=RMESH_DEFAULT;
			ExternalMagnetic.Dim.Nz=ZMESH_0D; //Trapeze
			break;
		}
		case IMPORT_1D:{}
		case IMPORT_2DC:{}
		case IMPORT_2DR:{}
		case IMPORT_3DR:{
			ParseSolenoidFile(DIM_STEP);
			break;
		}
		default:
			return;
        ;
	}

 	D=ExternalMagnetic.Dim;

	// CREATE MAGNETIC MESH
	ExternalMagnetic.Field=new TField**[D.Nz];
	for (int i = 0; i < D.Nz; i++) {
		ExternalMagnetic.Field[i]=new TField*[D.Ny];
		for (int j = 0; j < D.Ny; j++) {
			ExternalMagnetic.Field[i][j]=new TField[D.Nx];
		}
	}

	//FILL MAGNETIC ARRAY
	switch (StructPar.SolenoidPar.ImportType) {
		case ANALYTIC_0D:{
			MakeUniformSolenoid();
			break;
		}
		case ANALYTIC_1D:{
			MakeAnalyticalSolenoid();
			break;
		}
		case IMPORT_1D:{}
		case IMPORT_2DC:{}
		case IMPORT_2DR:{}
		case IMPORT_3DR:{
			ParseSolenoidFile(PIV_STEP);
			//ParseSolenoidFile(DATA_STEP); //Embedded in PIV_STEP
			for (int i = 0; i < ExternalMagnetic.Dim.Nz; i++)
				ExternalMagnetic.Piv.Z[i]+=StructPar.SolenoidPar.StartPos;
			break;
		}
		default:
			return;
        ;
	}

	//TRASFORM TO CYLINDRICAL COORDINATES
	if (StructPar.SolenoidPar.ImportType==IMPORT_2DR)
		TransformMagneticToCYL1D();
	else if (StructPar.SolenoidPar.ImportType==IMPORT_3DR)
		TransformMagneticToCYL2D();

	bool PreAdjust=false;

	//CALCULATE RADIAL FIELD FOR 1D FIELD
	if (StructPar.SolenoidPar.ImportType==IMPORT_1D) {
		PreAdjust=Npoints>MESH_TOLERANCE*ExternalMagnetic.Dim.Nz; //If solenoid mesh from file is too coarse, differentiate after interpolation
		if (PreAdjust)
			AdjustMagneticMesh();

		CalculateFieldDerivative();
		CalculateRadialField();
	}

	//ADJUST MESH TO Z-NODES
	if (!PreAdjust)
		AdjustMagneticMesh();

	//NORMALIZE
	D=ExternalMagnetic.Dim;
	for (int i = 0; i < D.Nz; i++) {
		double knorm=c*Structure[i].lmb/BeamPar.Wnorm;
		for (int j = 0; j < D.Ny; j++) {
			for (int k = 0; k < D.Nx; k++) {
				ExternalMagnetic.Field[i][j][k].z*=knorm;
				ExternalMagnetic.Field[i][j][k].r*=knorm;
				ExternalMagnetic.Field[i][j][k].th*=knorm;
            }
        }
    }
}
//---------------------------------------------------------------------------
TError TBeamSolver::CreateGeometry()
{
	CreateMesh();
	CreateMaps();
	CreateStrucutre();
	CreateMagneticMap();

    return ERR_NO;
}
//---------------------------------------------------------------------------
void TBeamSolver::DeleteBeam()
{
	for (int i=0;i<Np_beam;i++)
		delete Beam[i];

	delete[] Beam;
	Beam=NULL;
}
//---------------------------------------------------------------------------
TError TBeamSolver::CreateBeam()
{
    double sx=0,sy=0,r=0;
	double b0=0,db=0;

	AnsiString S;

	if (Beam!=NULL)
		DeleteBeam();

	Beam=new TBeam*[Npoints];
	for (int i=0;i<Npoints;i++){
		Beam[i]=new TBeam(BeamPar.NParticles);
	  //  Beam[i]->SetBarsNumber(Nbars);
		//Beam[i]->SetKernel(Kernel);
		Beam[i]->lmb=Structure[i].lmb;
		Beam[i]->SetInputCurrent(BeamPar.Current);
		Beam[i]->SetRestEnergy(BeamPar.W0);
		Beam[i]->Reverse=StructPar.Reverse;
		//Beam[i]->Cmag=c*Cmag/(Structure[i].lmb*We0); //Cmag = c*B*lmb/Wo * (1/lmb^2 from r normalization)
		for (int j=0;j<BeamPar.NParticles;j++){
			Beam[i]->Particle[j].lost=LIVE;
			Beam[i]->Particle[j].beta0=0;
			Beam[i]->Particle[j].beta.z=0;
			Beam[i]->Particle[j].beta.r=0;
			Beam[i]->Particle[j].beta.th=0;
			//Beam[i]->Particle[j].Br=0;
			Beam[i]->Particle[j].phi=0;
		   //	Beam[i]->Particle[j].Bth=0;
			Beam[i]->Particle[j].r=0;
			//Beam[i]->Particle[j].Cmag=0;
			Beam[i]->Particle[j].th=0;
		}
	}
	Beam[0]->SetCurrent(BeamPar.Current);
	Np_beam=Npoints;

	for (int i=0;i<BeamPar.NParticles;i++)
        Beam[0]->Particle[i].z=Structure[0].ksi*Structure[0].lmb;

	//Create Distributions:

	if (!IsFullFileKeyWord(BeamPar.RBeamType)){
		switch (BeamPar.ZBeamType){
			case NORM_2D: {
				Beam[0]->GenerateEnergy(BeamPar.WNorm);
				Beam[0]->GeneratePhase(BeamPar.ZNorm);
				break;
			}
			case FILE_1D: {
				Beam[0]->GeneratePhase(BeamPar.ZNorm);
			}
			case FILE_2D: {
				if (!Beam[0]->ImportEnergy(&BeamPar))
					return ERR_BEAM;
				break;
			}
			default: {
				S="Unexpected Distribution Type";
				ShowError(S);
				return ERR_BEAM;
			}
		}
	}

	switch (BeamPar.RBeamType) {
		case CST_PID:{
			Beam[0]->GeneratePhase(BeamPar.ZNorm);
		}
        case PARMELA_T2:{
		}
		case CST_PIT:{
			if (!Beam[0]->BeamFromImport(&BeamPar))
				return ERR_BEAM;
			break;
		}

		case TWISS_2D:{}
		case TWISS_4D:{
			Beam[0]->BeamFromTwiss(&BeamPar);
			break;;
		}
		case SPH_2D:{
			Beam[0]->BeamFromSphere(&BeamPar);
			break;
		}
		case ELL_2D:{
			Beam[0]->BeamFromEllipse(&BeamPar);
			break;
		}
		case FILE_2D: {
			TGauss A;
			A.mean=0;
			A.limit=pi;
			A.sigma=100*pi;
			Beam[0]->GenerateAzimuth(A);
		}
		case TWO_FILES_2D: { }
		case FILE_4D:{
			if (!Beam[0]->BeamFromFile(&BeamPar))
				return ERR_BEAM;
			break;
		}
		default: {
			S="Unexpected Distribution Type";
			ShowError(S);
			return ERR_BEAM;
		}
	}

	Beam[0]->ShiftPhase(StructPar.Cells[0].dF);

	return ERR_NO;
}
//---------------------------------------------------------------------------
TImportType TBeamSolver::ParseSolenoidType(AnsiString &F)
{
	int Npar=0;
	TImportType T=NO_ELEMENT;

	AnsiString L,S;
	bool AllNumbers=false;

	std::ifstream fs(F.c_str());

	while (!AllNumbers){
		L=GetLine(fs);
		Npar=NumWords(L);
		AllNumbers=true;
		for (int i = 1; i < Npar; i++) {
			S=ReadWord(L,i);
			if (!IsNumber(S)) {
				AllNumbers=false;
				break;
			}
		}
	}

	if (Npar==2)  //1D: z,Bz
		T=IMPORT_1D;
	else if (Npar==4)  //2D cyl: r,z,Br,Bz
		T=IMPORT_2DC;
	else if (Npar==5)  //2D rec: r,z,Bx,By,Bz
		T=IMPORT_2DR;
	else if (Npar==6)  //3D: x,y,z,Bx,By,Bz
		T=IMPORT_3DR;

	fs.close();

	return T;
}
//---------------------------------------------------------------------------
TDimensions TBeamSolver::ParseSolenoidLines(TMagnetParameters &P)
{
	int Npar=0, Ndim=0;
	TDimensions Dim;

	switch (P.ImportType) {
		case IMPORT_1D: {
			Npar=2;
			Ndim=1;
			break;
		}
		case IMPORT_2DC: {
			Npar=4;
			Ndim=2;
			break;
		}
		case IMPORT_2DR: {
			Npar=5;
			Ndim=2;
			break;
		}
		case IMPORT_3DR: {
			Npar=6;
			Ndim=3;
			break;
		}
		default:{};
	}

	/*if (Npar>0)
		Dim=NumDimensions(P.File,Ndim,Npar); */
	Dim.Nx=Ndim;
	Dim.Ny=Npar;

	return Dim;
}
//---------------------------------------------------------------------------
/*int TBeamSolver::GetSolenoidPoints()
{
	return NumPointsInFile(StructPar.SolenoidPar.File,2);
}    */
//---------------------------------------------------------------------------
bool TBeamSolver::ReadSolenoid(int Nz,double *Z,double* B)
{
	std::ifstream fs(StructPar.SolenoidPar.File.c_str());
	float z=0,Bz=0;
	int i=0;
	bool Success=false;
	AnsiString S,L;

	while (!fs.eof()){
		L=GetLine(fs);

		if (NumWords(L)==2){       //Check if only two numbers in the line
			try {                  //Check if the data is really a number
				S=ReadWord(L,1);
				z=S.ToDouble()/100; //[cm]
				S=ReadWord(L,2);
				Bz=S.ToDouble()/10000;  //[Gs]
			}
			catch (...){
				continue;          //Skip incorrect line
			}
			if (i==Nz){  //if there is more data than expected
				i++;
				break;
			}
			Z[i]=z;
			B[i]=Bz;
			i++;
		}
	}

	fs.close();

	Success=(i==Nz);
	return Success;
}
//---------------------------------------------------------------------------
TEllipse TBeamSolver::GetEllipse(int Nknot,TBeamParameter P)
{
	//return Beam[Nknot]->GetEllipseDirect(P);
	return Beam[Nknot]->GetEllipse(P);
}
//---------------------------------------------------------------------------
TTwiss TBeamSolver::GetTwiss(int Nknot,TBeamParameter P)
{
	//return Beam[Nknot]->GetTwissDirect(P);
	return Beam[Nknot]->GetTwiss(P);
}
//---------------------------------------------------------------------------
TGauss TBeamSolver::GetEnergyStats(int Nknot, TDeviation D)
{
	return Beam[Nknot]->GetEnergyDistribution(D);
}
//---------------------------------------------------------------------------
TGauss TBeamSolver::GetPhaseStats(int Nknot, TDeviation D)
{
	return Beam[Nknot]->GetPhaseDistribution(D);
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeamSolver::GetSpectrum(int Nknot,TBeamParameter P,bool Smooth)
{
	return Beam[Nknot]->GetSpectrumBar(P,Smooth);
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeamSolver::GetEnergySpectrum(int Nknot,bool Smooth)
{
	return Beam[Nknot]->GetEnergySpectrum(Smooth);
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeamSolver::GetPhaseSpectrum(int Nknot,bool Smooth)
{
	return Beam[Nknot]->GetPhaseSpectrum(Smooth);
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeamSolver::GetRSpectrum(int Nknot,bool Smooth)
{
	return Beam[Nknot]->GetRSpectrum(Smooth);
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeamSolver::GetXSpectrum(int Nknot,bool Smooth)
{
	return Beam[Nknot]->GetXSpectrum(Smooth);
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeamSolver::GetYSpectrum(int Nknot,bool Smooth)
{
	return Beam[Nknot]->GetYSpectrum(Smooth);
}
//---------------------------------------------------------------------------
double TBeamSolver::GetSectionFrequency(int Nsec)
{
	return StructPar.Sections[Nsec].Frequency;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetSectionPower(int Nsec)
{
	return StructPar.Sections[Nsec].Power;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetSectionWavelength(int Nsec)
{
	return c/StructPar.Sections[Nsec].Frequency;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetFrequency(int Ni)
{
	return c/Structure[Ni].lmb;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetPower(int Ni)
{
	return Structure[Ni].P;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetWavelength(int Ni)
{
	return Structure[Ni].lmb;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetMaxAperture()
{
	double Amax=0,a=0;
	for (int i=0; i < Npoints; i++) {
		a=GetAperture(i);
		if (a>Amax)
			Amax=a;
	}
	return Amax;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetAperture(int Ni)
{
	return Structure[Ni].Ra*Structure[Ni].lmb;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetMaxEnergy(int Ni)
{
	return Beam[Ni]->GetMaxEnergy();
}
//---------------------------------------------------------------------------
double TBeamSolver::GetMaxDivergence(int Ni)
{
	return Beam[Ni]->GetMaxDivergence();
}
//---------------------------------------------------------------------------
double TBeamSolver::GetMaxPhase(int Ni)
{
	return Beam[Ni]->GetMaxPhase();
}
//---------------------------------------------------------------------------
double TBeamSolver::GetMinPhase(int Ni)
{
	return Beam[Ni]->GetMinPhase();
}
//---------------------------------------------------------------------------
double TBeamSolver::GetCurrent(int Ni)
{
	return Beam[Ni]->GetCurrent();
}
//---------------------------------------------------------------------------
double TBeamSolver::GetBeamRadius(int Ni,TBeamParameter P)
{
	return Beam[Ni]->GetRadius(P);
}
//---------------------------------------------------------------------------
TLoss TBeamSolver::GetLossType(int Nknot,int Np)
{
	return Beam[Nknot]->Particle[Np].lost;
}
//---------------------------------------------------------------------------
int TBeamSolver::GetLivingNumber(int Ni)
{
	return Beam[Ni]->GetLivingNumber();
}
//---------------------------------------------------------------------------
TGraph *TBeamSolver::GetTrace(int Np,TBeamParameter P1,TBeamParameter P2)
{
	TGraph *G;
	G=new TGraph[Npoints];
	bool live=false;

	for (int i = 0; i < Npoints; i++) {
		live=Beam[i]->GetParameter(Np,LIVE_PAR)==1;
		if (live) {
			G[i].x=Beam[i]->GetParameter(Np,P1);
			G[i].y=Beam[i]->GetParameter(Np,P2);
		}
		G[i].draw=live;
	}

	return G;
}
//---------------------------------------------------------------------------
TGraph *TBeamSolver::GetSpace(int Nknot,TBeamParameter P1,TBeamParameter P2)
{
	TGraph *G;
	G=new TGraph[BeamPar.NParticles];
	bool live=false;

	for (int i = 0; i < BeamPar.NParticles; i++) {
		live=Beam[Nknot]->GetParameter(i,LIVE_PAR)==1;
		if (live) {
			G[i].x=Beam[Nknot]->GetParameter(i,P1);
			G[i].y=Beam[Nknot]->GetParameter(i,P2);
		}
		G[i].draw=live;
	}

	return G;
}
/*
//OBSOLETE
//---------------------------------------------------------------------------
TSpectrumBar *TBeamSolver::GetEnergySpectrum(int Nknot,double& Wav,double& dW)
{
    TSpectrumBar *Spectrum;
    Spectrum=GetEnergySpectrum(Nknot,false,Wav,dW);
    return Spectrum;
}

//---------------------------------------------------------------------------
TSpectrumBar *TBeamSolver::GetPhaseSpectrum(int Nknot,double& Fav,double& dF)
{
	TSpectrumBar *Spectrum;
	Spectrum=GetPhaseSpectrum(Nknot,false,Fav,dF);
	return Spectrum;
}
*/
//---------------------------------------------------------------------------
double *TBeamSolver::GetBeamParameters(int Nknot,TBeamParameter P)
{
	double *X;
	X=new double[BeamPar.NParticles];

	for (int i= 0; i < BeamPar.NParticles; i++) {
		X[i]=Beam[Nknot]->GetParameter(i,P);
	}

	return X;

}
//---------------------------------------------------------------------------
double TBeamSolver::GetStructureParameter(int Nknot, TStructureParameter P)
{
	double x=0;
	TTwiss T;

	switch (P) {
		case (KSI_PAR):{
			x=Structure[Nknot].ksi;
			break;
		}
		case (Z_PAR):{
			x=Structure[Nknot].ksi*Structure[Nknot].lmb;
			break;
		}
		case (A_PAR):{
			x=Structure[Nknot].A;
			break;
		}
		case (RP_PAR):{
			x=Structure[Nknot].Rp;
			break;
		}
		case (E0_PAR):{
			x=Structure[Nknot].A*BeamPar.Wnorm/Structure[Nknot].lmb;
			break;
		}
		case (EREAL_PAR):{
			double P=Structure[Nknot].P>0?Structure[Nknot].P:0;
			x=sqrt(2*Structure[Nknot].Rp)*sqrt(P)/Structure[Nknot].lmb;
			break;
		}
		case (PRF_PAR):{
			double E=sqrt(2*Structure[Nknot].Rp);
			x=E!=0?sqr(Structure[Nknot].A*BeamPar.Wnorm/E):0;
			break;
		}
		case (PBEAM_PAR):{
			x=Beam[Nknot]->GetAverageEnergy()*Beam[Nknot]->GetCurrent();
			break;
		}
		case (ALPHA_PAR):{
			x=Structure[Nknot].alpha;
			break;
		}
		case (SBETA_PAR):{
			x=Structure[Nknot].beta;
			break;
		}
		case (BBETA_PAR):{
			x=MeVToVelocity(Beam[Nknot]->GetAverageEnergy(),BeamPar.W0);
			break;
		}
		case (WAV_PAR):{
			x=Beam[Nknot]->GetAverageEnergy();
			break;
		}
		case (WMAX_PAR):{
			x=Beam[Nknot]->GetMaxEnergy();
			break;
		}
		case (RA_PAR):{
			x=Structure[Nknot].Ra*Structure[Nknot].lmb;
			break;
		}
		case (RB_PAR):{
			x=Beam[Nknot]->GetRadius();
			break;
		}
		case (BZ_EXT_PAR):{
			//x=Structure[Nknot].Hext[Nknot][0][0].Field.z;
			if (ExternalMagnetic.Field!=NULL)
				x=ExternalMagnetic.Field[Nknot][0][0].z*BeamPar.Wnorm/(c*Structure[Nknot].lmb);
			else x=0;

			break;
		}
		case (BR_EXT_PAR):{
			//x=Structure[Nknot].Hext[Nknot][0][0].Field.z;
			if (ExternalMagnetic.Field!=NULL){
				int ri=ExternalMagnetic.Dim.Nx-1;
				double r=ExternalMagnetic.Piv.X[ri];
				x=ExternalMagnetic.Field[Nknot][0][ri].r*BeamPar.Wnorm/(c*Structure[Nknot].lmb);
				x/=r;
			} else
				x=0;
			break;
		}
		case (NUM_PAR):{
			x=Structure[Nknot].CellNumber;
			break;
		}
		case (ER_PAR):{
			T=Beam[Nknot]->GetTwiss(R_PAR,false);
			x=T.epsilon;
			break;
		}
		case (EX_PAR):{
			T=Beam[Nknot]->GetTwiss(X_PAR,false);
			x=T.epsilon;
			break;
		}
		case (EY_PAR):{
			T=Beam[Nknot]->GetTwiss(Y_PAR,false);
			x=T.epsilon;
			break;
		}
		case (ENR_PAR):{
			T=Beam[Nknot]->GetTwiss(R_PAR,true);
			x=T.epsilon;
			break;
		}
		case (ENX_PAR):{
			T=Beam[Nknot]->GetTwiss(X_PAR,true);
			x=T.epsilon;
			break;
		}
		case (ENY_PAR):{
			T=Beam[Nknot]->GetTwiss(Y_PAR,true);
			x=T.epsilon;
			break;
		}
		case (E4D_PAR):{
			x=Beam[Nknot]->Get4DEmittance(false);
			break;
		}
		case (E4DN_PAR):{
			x=Beam[Nknot]->Get4DEmittance(true);
			break;
		}
		case (ET_PAR):{
			T=Beam[Nknot]->GetTwiss(TH_PAR,false);
			x=T.epsilon;
			break;
		}
		case (ENT_PAR):{
			T=Beam[Nknot]->GetTwiss(TH_PAR,true);
			x=T.epsilon;
			break;
		}
                default: {
                        throw runtime_error("GetStructureParameter error: Unhandled TStructureParameter value");
                }
	}

	return x;
}
//---------------------------------------------------------------------------
double *TBeamSolver::GetStructureParameters(TStructureParameter P)
{
	double *X;
	X=new double [Npoints];

	for (int i=0;i<Npoints;i++){
		X[i]=GetStructureParameter(i,P);
	}

	return X;
}
//---------------------------------------------------------------------------
/*double TBeamSolver::GetKernel()
{
	return Beam[0]->h;
} */

//---------------------------------------------------------------------------
void TBeamSolver::Abort()
{
	SolverStop=ERR_ABORT;
	#ifndef RSHELLWEG_LINUX
	SmartProgress->ShowMessage("Aborted");
	#endif
}
//---------------------------------------------------------------------------
double TBeamSolver::GetEigenFactor(double x, double y, double z,double a, double b, double c)
{
	double s=0,s0=0,s1=0,s2=0,s3=0;
	double A=0, B=0, C=0, D=0;
	double AA=0, BB=0, CC=0;
	double q=0, p=0, p3=0, Q=0;
	double rho=0, r=0, cph=0, phi=0;
	double err=1e-10;

	double xx=sqr(x/a),yy=sqr(y/b),zz=sqr(z/c),bb=sqr(a/b),cc=sqr(a/c);

	A=bb*cc;
	//B=(bb*cc+bb+cc)-(xx*bb*cc+yy*cc+zz*bb);
	B=A*(1-xx)+bb*(1-zz)+cc*(1-yy);
	//C=1-yy-zz+bb*(1-xx-zz)+cc*(1-xx-yy);
	//C=(1-xx)*(bb+cc)-yy*(1+cc)-zz*(1+bb);
	//C=1+bb+cc-xx*(bb+cc)-yy*(1+cc)-zz*(1+bb);
	C=1+bb+cc-xx*(bb+cc)-yy*(1+cc)-zz*(1+bb);
	D=1-(xx+yy+zz);

	AA=B/A;
	BB=C/A;
	CC=D/A;

	//p=C/A-sqr(B/A)/3;
	p=BB-sqr(AA)/3;

	//q=(2*cub(B)-9*A*B*C+27*sqr(A)*D)/(27*cub(A));
	//q=2*cub(B/A)/27-B*C/(3*sqr(A))+D/A;;
	q=2*cub(AA/3)-AA*BB/3+CC;
	p3=cub(p/3);
	Q=p3+sqr(q/2);

	if (Q > err) {
		s0=cubrt(sqrt(Q)-q/2)-cubrt(sqrt(Q)+q/2);
	} else {
		if (mod(Q)<err) {
			s1=cubrt(q/2);
			s2=-2*cubrt(q/2);
			s3=s2;//-2*cubrt(q/2);
		} else {
			rho=sqrt(-p/3);
			cph=-q/(2*cub(rho));

			if (mod(cph)<=1)
				phi=acos(cph);
			else if (cph>=1)
				phi=0;
			else if (cph<=-1)
				phi=pi;
			else
				assert(false);

			r=2*rho;
			s1=r*cos(phi/3);
			s2=r*cos(phi/3+2*pi/3);
			s3=r*cos(phi/3-2*pi/3);
		}
		s0=s2>s1?s2:s1;
		s0=s3>s0?s3:s0;
	}
	s=s0-AA/3;

	return s;
}
//---------------------------------------------------------------------------
double TBeamSolver::FormFactor(double ryrx, double rxrz, TBeamParameter P, double s)
{
	int kx=0, ky=0, kz=0, Nx=Nryrx, Ny=Nrxrz, Nz=Nlmb, Ndim=8;
	double *M=NULL;
	double *X=(double *)RYRX;
	double *Y=(double *)RXRZ;
	double *Z=(double *)LMB;
	double F=0;
	double x=ryrx, y=rxrz, z=s;

	//M=new double** [Ny];


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

	if (z<=Z[0]){
		z=Z[0];
		kz=0;
	}else if (z>=Z[Nz-1]){
		z=Z[Nz-1];
		kz=Nz-2;
		return 0;
	}else {
		for (int i = 1; i < Nz; i++) {
			if (z<Z[i]) {
				kz=i-1;
				break;
			}
		}
	}

	M=new double [Ndim]; //4*kz + 2*ky + kx

	for (int i=0; i<=1; i++){
		for (int j=0; j<=1; j++){
			for (int k=0; k<=1; k++){
				switch (P) {
					case X_PAR:{
						M[4*k+2*j+i]=MX[kz+k][ky+j][kx+i];
						break;
					}
					case Y_PAR:{
						M[4*k+2*j+i]=MY[kz+k][ky+j][kx+i];
						break;
					}
					case Z0_PAR:{
						M[4*k+2*j+i]=MZ[kz+k][ky+j][kx+i];
						//M[4*k+2*j+i]=MZ[kz+k][ky+j][kx+i];
						break;
					}
					default:{
						M[4*k+2*j+i]=0;
					}
				}
			//	if (M[4*k+2*j+i]>0)
					M[4*k+2*j+i]=log(M[4*k+2*j+i]);
			 //	else
				 //	ShowMessage("Error!");
			}
		}
	}

	//M[0]=M[kz][ky][kx];
	//M[1=M[kz][ky][kx+1];
	//M[2]=M[kz][ky+1][kx];
	//M[3]=M[kz][ky+1][kx+1];
	//M[4]=M[kz+1][ky][kx];
	//M[5]=M[kz+1][ky][kx+1];
	//M[6]=M[kz+1][ky+1][kx];
	//M[7]=M[kz+1][ky+1][kx+1];

	/*F=(M[ky][kx]*(X[kx+1]-x)*(Y[ky+1]-y)+M[ky][kx+1]*(x-X[kx])*(Y[ky+1]-y)+
	M[ky+1][kx]*(X[kx+1]-x)*(y-Y[ky])+M[ky+1][kx+1]*(x-X[kx])*(y-Y[ky]))/((X[kx+1]-X[kx])*(Y[ky+1]-Y[ky]));*/

	F=M[0]*(X[kx+1]-x)*(Y[ky+1]-y)*(Z[kz+1]-z)+
	M[1]*(x-X[kx])*(Y[ky+1]-y)*(Z[kz+1]-z)+
	M[2]*(X[kx+1]-x)*(y-Y[ky])*(Z[kz+1]-z)+
	M[3]*(x-X[kx])*(y-Y[ky])*(Z[kz+1]-z)+
	M[4]*(X[kx+1]-x)*(Y[ky+1]-y)*(z-Z[kz])+
	M[5]*(x-X[kx])*(Y[ky+1]-y)*(z-Z[kz])+
	M[6]*(X[kx+1]-x)*(y-Y[ky])*(z-Z[kz])+
	M[7]*(x-X[kx])*(y-Y[ky])*(z-Z[kz]);

	F/=(X[kx+1]-X[kx])*(Y[ky+1]-Y[ky])*(Z[kz+1]-Z[kz]);

	delete[] M;

	return exp(F);
}
//---------------------------------------------------------------------------
void TBeamSolver::SpaceCharge(int Si, int Sj)
{
	double gamma0=1, Mr=0,Nr=0,/*phic=0,*/Qbunch=0,Rho=0,lmb=0,Icur=0;
	double Mx=0,My=0,Mz=0,Mxx=0,Myy=0,Mzz=0,ix=0,iy=0,p=0,M=0;
	double Ltrain=0,s_head=1,s_tail=1,Mz_tail=0,Mz_head=0;
	int component=0;

	double x0=0,y0=0,z0=0,rx=0,ry=0,rz=0/*,rb=0*/,V=0,Mcore=1,Ncore=0;
	double Nrms=BeamPar.SpaceCharge.Nrms;

	TParticle *Particle=Beam[Si]->Particle;
	Ncore=0;
	gamma0=Par[Sj].gamma;

	switch (BeamPar.SpaceCharge.Type) {   //SPACE CHARGE
		case SPCH_LPST: {}
		case SPCH_ELL: {
			TGauss Gx,Gy,Gz;
			Gx=Beam[Si]->iGetBeamRadius(Par[Sj],K[Sj],X_PAR);
			Gy=Beam[Si]->iGetBeamRadius(Par[Sj],K[Sj],Y_PAR);
			Gz=Beam[Si]->iGetBeamLength(Par[Sj],K[Sj]);//BeamPar.SpaceCharge.NSlices);

			x0=Gx.mean;
			y0=Gy.mean;
			z0=Gz.mean;

			rx=Nrms*Gx.sigma;
			ry=Nrms*Gy.sigma;
			rz=Nrms*Gz.sigma;

			lmb=Beam[Si]->lmb;
			Icur=Beam[Si]->GetCurrent();
			Qbunch=Icur*lmb/c;
		 //	rx=0.2e-2;
		   //	ry=rx;
		   //	rz=rx/0.1;
			V=mod(4*pi*rx*ry*rz/3);
			Rho=Qbunch/V;
			//Rho*=2*pi/eps0;
			Rho/=2*eps0;

			if (BeamPar.SpaceCharge.Train) {
				Ltrain=BeamPar.SpaceCharge.Ltrain>0?BeamPar.SpaceCharge.Ltrain:lmb;
			}

			if (V!=0) {
				//ACTUAL CODE!!!
				if (BeamPar.SpaceCharge.Type==SPCH_LPST) {  //Lapostolle
					p=gamma0*rz/sqrt(rx*ry);
					M=FormFactorLpst(p);
				} else {     //Elliptic
					ix=ry/rx;
					iy=rx/rz;
					Mx=FormFactor(ix,iy,X_PAR);
					My=FormFactor(ix,iy,Y_PAR);
					Mz=FormFactor(ix,iy,Z0_PAR);
				}
				//END ACTUAL CODE!!

				for (int i=0;i<BeamPar.NParticles;i++){
					if (Particle[i].lost==LIVE){
						double x=0,y=0,z=0,r=0,th=0,phi=0,gamma=1,beta_z=1,beta=1,g2=1;
						double Ex=0,Ey=0,Ez=0,Lx=0,Ly=0,Lz=0;
						double r3=0,r2=0,rv=0,s=0;

						phi=Particle[i].phi+K[Sj][i].phi*Par[Sj].h;
						r=(Particle[i].r+K[Sj][i].r*Par[Sj].h)*lmb;
						th=Particle[i].th+K[Sj][i].th*Par[Sj].h;

						beta_z=Particle[i].beta.z+K[Sj][i].beta.z*Par[Sj].h;
						beta=Particle[i].beta0;
						//+K[Sj][i].beta.z*Par[Sj].h;

						x=r*cos(th)-x0;
						y=r*sin(th)-y0;
						z=phi*beta*lmb/(2*pi)-z0;

						r3=sqr(x/rx)+sqr(y/ry)+sqr(z/rz);
//					   	z=rz;

						gamma=VelocityToEnergy(beta); //change to beta
						g2=1/sqr(gamma);

						if  (BeamPar.SpaceCharge.Type==SPCH_LPST) { //Lapostolle
							Lx=2*(1-M)*ry/(rx+ry);
							Ly=2*(1-M)*rx/(rx+ry);
							Lz=2*M;
							if (r3<1) {
								Ncore++;
								Mxx=Lx;
								Myy=Ly;
								Mzz=Lz;
							} else {
								r2=sqrt(sqr(x)+sqr(y)+sqr(z));
								rv=rx*ry*rz;
								Mxx=(2.0/3.0)*rv/pow(r2,3.0);
								Myy=Mxx;
								Mzz=Mxx;
							}
						} else {//Elliptic
							if (r3<1){
								Mxx=Mx;
								Myy=My;
								Mzz=Mz;
								Lz=2*M;
								Ncore++;
							}else{
								r2=sqrt(sqr(x)+sqr(y)+sqr(z));
								rv=rx*ry*rz;
								s=GetEigenFactor(x,y,z,rx,ry,rz);
								Mxx=FormFactor(ix,iy,X_PAR,s);
								Myy=FormFactor(ix,iy,Y_PAR,s);
								Mzz=FormFactor(ix,iy,Z0_PAR,s);


								Lz=(2.0/3.0)*rv/pow(r2,3.0);
							}
						}
						Ex=Mxx*x;
						Ey=Myy*y;
						Ez=Mzz*z;

						if (BeamPar.SpaceCharge.Train) {
							if (rz<=lmb/2) {
								s_tail=GetEigenFactor(x,y,z+Ltrain,rx,ry,rz);
								s_head=GetEigenFactor(x,y,z-Ltrain,rx,ry,rz);

								Mz_head=FormFactor(ix,iy,Z0_PAR,s_head);
								Mz_tail=FormFactor(ix,iy,Z0_PAR,s_tail);

								Ez+=Mz_head*(z-Ltrain)+Mz_tail*(z+Ltrain);
							}  else
								Ez=0;

						}

						Ex*=Rho/2;
						Ey*=Rho/2;
						Ez*=Rho/2;

						Ex*=(g2*lmb/BeamPar.Wnorm);
						Ey*=(g2*lmb/BeamPar.Wnorm);
					  /*	Ex*=lmb/BeamPar.W0;
						Ey*=lmb/BeamPar.W0;  */
						Ez*=lmb/BeamPar.Wnorm;

						Par[Sj].Eq[i].z=Ez;
						Par[Sj].Eq[i].r=Ex*cos(th)+Ey*sin(th);
						Par[Sj].Eq[i].th=-Ex*sin(th)+Ey*cos(th);
					}
				}

			  /*	if (BeamPar.SpaceCharge.Type==SPCH_ELL) {// elliptic
					Mcore=Ncore/Nliv;
					for (int i=0;i<BeamPar.NParticles;i++){
						Par[Sj].Eq[i].z*=Mcore;
						Par[Sj].Eq[i].r*=Mcore;
						Par[Sj].Eq[i].th*=Mcore;
					}
				}      */
			}  // V!=0 loop

			break;

		}
		case SPCH_GW: {
		  /*	for (int i=0;i<BeamPar.NParticles;i++){
				Par[Sj].Eq[i].z=kFc*Icur*lmb*GaussIntegration(r,z,Rb,Lb,3);  // E;  (YuE) expression
				Par[Sj].Eq[i].z*=(lmb/We0);                     // A
				Par[Sj].Eq[i].r=kFc*Icur*lmb*GaussIntegration(r,z,Rb,Lb,1);  // E;  (YuE) expression
				Par[Sj].Eq[i].r*=(lmb/We0);                     // A
			}     */
			break;
		}
		case SPCH_NO:{}
		default: { };
	}
}
//---------------------------------------------------------------------------
void TBeamSolver::Integrate(int Si, int Sj)
{
	double gamma0=1;

	//phic=Beam[Si]->iGetAveragePhase(Par[Sj],K[Sj]);
	Par[Sj].SumSin=0;
	Par[Sj].SumCos=0;
	Par[Sj].SumSin=Beam[Si]->SinSum(Par[Sj],K[Sj]);
	Par[Sj].SumCos=Beam[Si]->CosSum(Par[Sj],K[Sj]);

	gamma0=Beam[Si]->iGetAverageEnergy(Par[Sj],K[Sj]);
	Par[Sj].gamma=gamma0;

	Par[Sj].Eq=new TField[BeamPar.NParticles];

	for (int i=0;i<BeamPar.NParticles;i++){
		Par[Sj].Eq[i].z=0;
		Par[Sj].Eq[i].r=0;
		Par[Sj].Eq[i].th=0;
	}

	SpaceCharge(Si,Sj);

	Beam[Si]->Integrate(Par[Sj],K,Sj);

	delete[] Par[Sj].Eq;
	//delete[] Par[Sj].Aqr;
}
//---------------------------------------------------------------------------
 double TBeamSolver::GaussIntegration(double r,double z,double Rb,double Lb,int component)
{
  //MOVED CONSTANTS TO CONST.H!

	double Rb2,Lb2,d,d2,ksi,s,t,func,GInt;
	int i;

	Rb2=sqr(Rb);
	Lb2=sqr(Lb);
	d=pow(Rb2*Lb,1/3);
	d2=sqr(d);
	GInt=0;
	for (int i=0;i<11;i++) {
		ksi=.5*(psi12[i]+1);
		s=d2*(1/ksi-1);
		t=sqr(r)/(Rb2+s)+sqr(z)/(Lb2+s);
		if (t <= 5) {
//           func=sqrt(ksi/((Lb2/d2-1)*ksi+1))/abs(((Rb2/d2-1)*ksi+1));
           func=sqrt(ksi/((Lb2/d2-1)*ksi+1))/((Rb2/d2-1)*ksi+1);
		   if (component < 3) {
//		        GInt +=.5*r*w12[i]*func/abs(((Rb2/d2-1)*ksi+1));
				GInt +=.5*r*w12[i]*func/((Rb2/d2-1)*ksi+1);
		   }
		   if (component == 3) {
//		        GInt +=.5*z*w12[i]*func/abs(((Lb2/d2-1)*ksi+1));
		        GInt +=.5*z*w12[i]*func/((Lb2/d2-1)*ksi+1);
		   }
		}
	}
	return GInt;
}
//---------------------------------------------------------------------------
void TBeamSolver::CountLiving(int Si)
{
	Nliv=Beam[Si]->GetLivingNumber();
    if (Nliv==0){
	  /*    FILE *F;
		F=fopen("beam.log","w");
		for (int i=Si;i<Npoints;i++){
			for (int j=0;j<Np;j++)
				fprintf(F,"%i ",Beam[i]->Particle[j].lost);
			fprintf(F,"\n");
		}
		fclose(F);   */
		KillParticles(Si+1,Npoints-1);
		#ifndef RSHELLWEG_LINUX
		AnsiString S="Beam Lost!";
		ShowError(S);
        SmartProgress->ShowMessage(S.c_str());
        #endif
		SolverStop=ERR_BEAM;
        return;
    }
}
//---------------------------------------------------------------------------
void TBeamSolver::KillParticles(int Si)
{
	if (Si>0)
		Beam[Si]->KillParticles(Beam[Si-1]->Particle);
	else
		Beam[Si]->KillParticles();
}
//---------------------------------------------------------------------------
void TBeamSolver::KillParticles(int Si, int Sj)
{
	for (int i=Si;i<=Sj;i++)
		KillParticles(i);
}
//---------------------------------------------------------------------------
void TBeamSolver::DumpHeader(ofstream &fo,TDump *ExportParameters,int jmin,int jmax)
{
	AnsiString s, w;
	AnsiString EU=GetEnergyUnit(BeamPar.Species.Type);

	w="Energy ["+EU+"]\t";

	fo<<"List of ";

	int Si=ExportParameters->Nmesh;

	if (jmin==0 && jmax==BeamPar.NParticles)
		fo<<"ALL ";
	if (ExportParameters->LiveOnly)
		fo<<"LIVE ";

	fo<<"particles ";

	if (!(jmin==0 && jmax==BeamPar.NParticles)){
		fo<<" from #";
		fo<<jmin+1;
		fo<<" to #";
		fo<<jmax;
	}

	fo<<" at z=";
	s=s.FormatFloat("#0.00",100*Structure[Si].ksi*Structure[Si].lmb);
	fo<<s.c_str();
	fo<<" cm\n";

	fo<<"Particle #\t";
	if (!ExportParameters->LiveOnly)
		fo<<"Lost\t";
	if (ExportParameters->Phase)
		fo<<"Phase [deg]\t";
	if (ExportParameters->Energy)
		fo<<w;
	if (ExportParameters->Radius)
		fo<<"Radius [mm]\t";
	if (ExportParameters->Azimuth)
		fo<<"Azimuth [deg]\t";
	if (ExportParameters->Divergence)
		fo<<"Divergence	[mrad]\t";
				//fo<<"Vth [m/s]\t";
	fo<<"\n";
}
//---------------------------------------------------------------------------
void TBeamSolver::DumpFile(ofstream &fo,TDump *ExportParameters,int j)
{
	AnsiString s;
	int Si=ExportParameters->Nmesh;

	if(!ExportParameters->LiveOnly || (ExportParameters->LiveOnly && !Beam[Si]->Particle[j].lost)){
		s=s.FormatFloat("#0000\t\t",j+1);
		fo<<s.c_str();

		if (!ExportParameters->LiveOnly) {
			if (Beam[Si]->Particle[j].lost)
				fo<<"LOST\t";
			else
				fo<<"LIVE\t";
		}
		if (ExportParameters->Phase){
			s=s.FormatFloat("#0.00\t\t",RadToDegree(Beam[Si]->GetParameter(j,PHI_PAR)));
			fo<<s.c_str();
		}
		if (ExportParameters->Energy){
			s=s.FormatFloat("#0.00\t\t\t",Beam[Si]->GetParameter(j,W_PAR));
			fo<<s.c_str();
		}
		if (ExportParameters->Radius){
			s=s.FormatFloat("#0.00\t\t\t",1000*Beam[Si]->GetParameter(j,R_PAR));
			fo<<s.c_str();
		}
		if (ExportParameters->Azimuth){
			s=s.FormatFloat("#0.00\t\t",RadToDegree(Beam[Si]->GetParameter(j,TH_PAR)));
			fo<<s.c_str();
		}
		if (ExportParameters->Divergence){
			s=s.FormatFloat("#0.00\t\t",1000*Beam[Si]->GetParameter(j,AR_PAR));
			fo<<s.c_str();
		}
		fo<<"\n";
	}
}
//---------------------------------------------------------------------------
void TBeamSolver::DumpASTRA(ofstream &fo,TDump *ExportParameters,int j,int jref)
{
	AnsiString s;
	double x=0, y=0, z=0;
	double px=0, py=0, pz=0;
	double q=0, I=0, clock=0;
	int  status=0, index=1;   //1 - electrons, 2 - positrons, 3 - protons, 4 - heavy ions;

	switch (BeamPar.Species.Type){
		case PROTON: {index=3; break;}
		case ION: {index=4; break;}
		case ELECTRON: {}
		default: index=1;
	}

    //NEED TO CHECK ASTRA format for ions

	int Si=ExportParameters->Nmesh;

	x=Beam[Si]->GetParameter(j,X_PAR);
	y=Beam[Si]->GetParameter(j,Y_PAR);
	z=Beam[Si]->GetParameter(j,PHI_PAR)*Structure[Si].lmb/(2*pi);
	clock=Beam[Si]->GetParameter(j,PHI_PAR)*Structure[Si].lmb/(2*pi*c);

	double W=Beam[Si]->GetParameter(j,W_PAR);
	double gamma=MeVToGamma(W,BeamPar.W0);

	px=BeamPar.W0*gamma*Beam[Si]->GetParameter(j,BX_PAR);
	py=BeamPar.W0*gamma*Beam[Si]->GetParameter(j,BY_PAR);
	pz=BeamPar.W0*gamma*Beam[Si]->GetParameter(j,BZ_PAR);

	I=BeamPar.Current/BeamPar.NParticles;
	q=I*Structure[Si].lmb/c;

	if (j==jref) {
	   x=0;
	   y=0;
	   z=0;

	   clock=0;
	   status=5;  //reference particle
	}  else {
	   z=z-Beam[Si]->GetParameter(jref,PHI_PAR)*Structure[Si].lmb/(2*pi);
	   pz=pz-BeamPar.W0*gamma*Beam[Si]->GetParameter(jref,BZ_PAR);
	   clock=clock-Beam[Si]->GetParameter(jref,PHI_PAR)*Structure[Si].lmb/(2*pi*c);

	   status=3; //regular
	}

	s=s.FormatFloat("#.##############e+0 ",x);
	fo<<s.c_str();
	s=s.FormatFloat("#.##############e+0 ",y);
	fo<<s.c_str();
	s=s.FormatFloat("#.##############e+0 ",z);
	fo<<s.c_str();
	s=s.FormatFloat("#.##############e+0 ",px);
	fo<<s.c_str();
	s=s.FormatFloat("#.##############e+0 ",py);
	fo<<s.c_str();
	s=s.FormatFloat("#.##############e+0 ",pz);
	fo<<s.c_str();
	s=s.FormatFloat("#.##############e+0 ",1e9*clock);
	fo<<s.c_str();
	s=s.FormatFloat("#.##############e+0 ",1e9*q);
	fo<<s.c_str();
	s=s.FormatFloat("0 ",index);
	fo<<s.c_str();
	s=s.FormatFloat("0 ",status);
	fo<<s.c_str();

	fo<<"\n";
}
//---------------------------------------------------------------------------
void TBeamSolver::DumpT2(ofstream &fo,TDump *ExportParameters,int j)
{
	AnsiString s;
	double x=0, y=0;
	double px=0, py=0;
	double phi=0, W=0;

	int Si=ExportParameters->Nmesh;

	if (Beam[Si]->Particle[j].lost!=LIVE)
		return;

	x=100*Beam[Si]->GetParameter(j,X_PAR);
	y=100*Beam[Si]->GetParameter(j,Y_PAR);
	phi=RadToDegree(Beam[Si]->GetParameter(j,PHI_PAR));
	W=Beam[Si]->GetParameter(j,W_PAR);

	px=1000*Beam[Si]->GetParameter(j,BX_PAR);
	py=1000*Beam[Si]->GetParameter(j,BY_PAR);

	if (ExportParameters->Binary){
		fo.write(reinterpret_cast<char*>(&x), sizeof(double));
		fo.write(reinterpret_cast<char*>(&px), sizeof(double));
		fo.write(reinterpret_cast<char*>(&y), sizeof(double));
		fo.write(reinterpret_cast<char*>(&py), sizeof(double));
		fo.write(reinterpret_cast<char*>(&phi), sizeof(double));
		fo.write(reinterpret_cast<char*>(&W), sizeof(double));
	} else {
		s=s.FormatFloat("#0.0000\t",x);
		fo<<s.c_str();
		s=s.FormatFloat("#0.000\t",px);
		fo<<s.c_str();
		s=s.FormatFloat("#0.0000\t",y);
		fo<<s.c_str();
		s=s.FormatFloat("#0.000\t",py);
		fo<<s.c_str();
		s=s.FormatFloat("#0.000\t",phi);
		fo<<s.c_str();
		s=s.FormatFloat("#0.000\t",W);
		fo<<s.c_str();

		fo<<"\n";
	}
}
//---------------------------------------------------------------------------
void TBeamSolver::DumpCST(ofstream &fo,TDump *ExportParameters,int j)
{
	AnsiString s;
	double x=0, y=0, z=0;
	double px=0, py=0, pz=0;
	double m=0, q=0, I=0,t=0;

	int Si=ExportParameters->Nmesh;
	if (!Beam[Si]->GetParameter(j,LIVE_PAR)) {
		return;
	}
//	double l=Structure[Si].lmb;
	double beta=Beam[Si]->GetParameter(j,BETA_PAR);

	x=Beam[Si]->GetParameter(j,X_PAR);
	y=Beam[Si]->GetParameter(j,Y_PAR);
	//z=Beam[Si]->GetParameter(j,PHI_PAR)*beta*Structure[Si].lmb/(2*pi);
	t = -Beam[Si]->GetParameter(j,PHI_PAR)*Structure[Si].lmb/(2.0*pi*c);

   	//z=4.0*z*1.1;

	double W=Beam[Si]->GetParameter(j,W_PAR);
	double p=MeVToBetaGamma(W,BeamPar.W0);

	px=p*Beam[Si]->GetParameter(j,BX_PAR)/beta;
	py=p*Beam[Si]->GetParameter(j,BY_PAR)/beta;
	pz=p*Beam[Si]->GetParameter(j,BZ_PAR)/beta;

	q=qe;
	switch (BeamPar.Species.Type){
		case PROTON: {
			m=mp;
			break;
		}
		case ION: {
			m=mu*BeamPar.Species.A;
			q*=BeamPar.Species.Q;
            break;
        }
		case ELECTRON: {}
		default: {
			m=me;
			break;
		}
	}

	//NEED TO CHECK if Q matters for ions for PID/PIT

	I=BeamPar.Current/BeamPar.NParticles;
	s=s.FormatFloat("#.##############e+0 ",x);
	fo<<s.c_str();
	s=s.FormatFloat("#.##############e+0 ",y);
	fo<<s.c_str();
	s=s.FormatFloat("#.##############e+0 ",z);
	fo<<s.c_str();
	s=s.FormatFloat("#.##############e+0 ",px);
	fo<<s.c_str();
	s=s.FormatFloat("#.##############e+0 ",py);
	fo<<s.c_str();
	s=s.FormatFloat("#.##############e+0 ",pz);
	fo<<s.c_str();
	s=s.FormatFloat("#.##############e+0 ",m);
	fo<<s.c_str();
	s=s.FormatFloat("#.##############e+0 ",q);
	fo<<s.c_str();
	if (ExportParameters->SpecialFormat==CST_PID) {
		s=s.FormatFloat("#.##############e+0",I);
		fo<<s.c_str();
	} else if (ExportParameters->SpecialFormat==CST_PIT) {
		s=s.FormatFloat("#.##############e+0 ",I*Structure[Si].lmb/c);
		fo<<s.c_str();
		s=s.FormatFloat("#.##############e+0",t);
		fo<<s.c_str();
	}
	fo<<"\n";

}
//---------------------------------------------------------------------------
void TBeamSolver::DumpBeam(int Sn)
{
	TDump *ExportParameters=NULL;
	ExportParameters=&BeamExport[Sn];
	DumpBeam(ExportParameters);
}
//---------------------------------------------------------------------------
void TBeamSolver::DumpBeam(TDump *ExportParameters)
{
	int Si=ExportParameters->Nmesh;
	int Nlive=0;
	AnsiString F=ExportParameters->File.c_str();
	AnsiString s;

	switch (ExportParameters->SpecialFormat) {
		case CST_PID:{F+=".pid";break;}
		case CST_PIT:{F+=".pit";break;}
		case ASTRA:{F+=".ini";break;}
		case PARMELA_T2:{
			if (ExportParameters->Binary)
				F+=".bin";
			else
				F+=".out";
			break;
		}
		case NOBEAM:{}
		default: {F+=".dat";break;}
	}

	std::ofstream fo;
	if (ExportParameters->Binary)
		fo.open(F.c_str(),std::ios::out | std::ios::binary);
	else
		fo.open(F.c_str());


	int jmin=0;
	int jmax=BeamPar.NParticles;

	if (ExportParameters->N1>0 && ExportParameters->N2==0) {
		jmin=0;
		jmax=ExportParameters->N1;
	} else if (ExportParameters->N1>0 && ExportParameters->N2>0) {
		jmin=ExportParameters->N1-1;
		jmax=ExportParameters->N2;
	}

	if (jmin>BeamPar.NParticles || jmax>BeamPar.NParticles) {
		if (ExportParameters->SpecialFormat==NOBEAM)
			fo<<"WARNING: The defined range of particle numbers exceeds the number of available particles. The region was set to default.\n";
		jmin=0;
		jmax=BeamPar.NParticles;
	}
	switch (ExportParameters->SpecialFormat) {
		case CST_PIT: {}
		case CST_PID: {
			for (int j=jmin;j<jmax;j++)
				DumpCST(fo,ExportParameters,j);
			break;
		}
		case ASTRA: {
			for (int j=jmin;j<jmax;j++)
				DumpASTRA(fo,ExportParameters,j,jmin);
			break;
		}
		case PARMELA_T2: {
			Nlive=GetLivingNumber(Si);
			std::time_t timestamp = std::time(0);
			fo << std::ctime(&timestamp);
			if (ExportParameters->Binary){
				//fo.write(reinterpret_cast<char*>(&timestamp), sizeof(std::time_t ));
				fo.write(reinterpret_cast<char*>(&Nlive), sizeof(int));
			}else {
				//fo << std::ctime(&timestamp);
				s=s.FormatFloat("0",Nlive);
				fo<<s.c_str();
				fo<<"\n";
			}
			for (int j=jmin;j<jmax;j++)
				DumpT2(fo,ExportParameters,j);

			break;
		}
		case NOBEAM:{}
		default:{
			DumpHeader(fo,ExportParameters,jmin,jmax);
			for (int j=jmin;j<jmax;j++)
				DumpFile(fo,ExportParameters,j);
		}
	}
	fo.close();
}
//---------------------------------------------------------------------------
void TBeamSolver::SaveOutput(AnsiString& Fname, bool binary)
{
	TDump ExportParameters;

	ExportParameters.File=Fname.c_str();
	ExportParameters.SpecialFormat=PARMELA_T2;

	ExportParameters.Nmesh=Npoints-1;
	ExportParameters.N1=BeamPar.NParticles;;
	ExportParameters.N2=0;

    ExportParameters.Binary=binary;

	DumpBeam(&ExportParameters);
}
//---------------------------------------------------------------------------
void TBeamSolver::SaveTrajectories(AnsiString& Fname)
{
	std::time_t timestamp = std::time(0);
	int Np=GetNumberOfParticles();
	double z=0;
	std::ofstream fo(Fname.c_str(),std::ios::out | std::ios::binary);

    fo << std::ctime(&timestamp);
	//fo.write(reinterpret_cast<char*>(&timestamp), sizeof(std::time_t ));
	fo.write(reinterpret_cast<char*>(&Np), sizeof(int));
	fo.write(reinterpret_cast<char*>(&Npoints), sizeof(int));

	for (int k = 0; k < Npoints; k++) {
		z=100*GetStructureParameter(k,Z_PAR);
        fo.write(reinterpret_cast<char*>(&z), sizeof(double));
    }

	for (int j = 0; j < Np; j++) {
		for (int k = 0; k < Npoints; k++) {
			double x=0,px=0,y=0,py=0,phi=0,W=0,L=0;
			bool live=false;
			live=Beam[k]->Particle[j].lost==LIVE;

			if (live){
				x=100*Beam[k]->GetParameter(j,X_PAR);
				y=100*Beam[k]->GetParameter(j,Y_PAR);
				phi=RadToDegree(Beam[k]->GetParameter(j,PHI_PAR));
				W=Beam[k]->GetParameter(j,W_PAR);
				px=1000*Beam[k]->GetParameter(j,BX_PAR);
				py=1000*Beam[k]->GetParameter(j,BY_PAR);
			}

			L=live?1:0;

			//fo.write(reinterpret_cast<char*>(&live), sizeof(bool));
			fo.write(reinterpret_cast<char*>(&L), sizeof(double));
			fo.write(reinterpret_cast<char*>(&x), sizeof(double));
			fo.write(reinterpret_cast<char*>(&px), sizeof(double));
			fo.write(reinterpret_cast<char*>(&y), sizeof(double));
			fo.write(reinterpret_cast<char*>(&py), sizeof(double));
			fo.write(reinterpret_cast<char*>(&phi), sizeof(double));
			fo.write(reinterpret_cast<char*>(&W), sizeof(double));
		}
	}

	fo.close();
}
//---------------------------------------------------------------------------
void TBeamSolver::Step(int Si)
{
    bool drift=false;
	double lmb=Structure[Si].lmb;
    Beam[Si]->lmb=lmb;
	CountLiving(Si);
   //	I=BeamPar.Current*Nliv/BeamPar.NParticles;
  /*
    Rb=Beam[i]->GetBeamRadius();
    phi0=Beam[i]->GetAveragePhase();
    dphi=Beam[i]->GetPhaseLength();
    Lb=dphi*lmb/(2*pi);
    betta0=Beam[i]->GetAverageEnergy();

    w=Structure[i]->alpha*lmb;        */
    drift=(Structure[Si].drift);
    for (int i=0;i<4;i++)
        Par[i].drift=Structure[Si].drift;
    //Par[3].drift=Structure[Si+1].drift;

    dh=Structure[Si+1].ksi-Structure[Si].ksi;
    Par[0].h=0;
    Par[1].h=dh/2;
    Par[2].h=Par[1].h;
	Par[3].h=dh;

	double db=Structure[Si+1].beta-Structure[Si].beta;
	Par[0].bw=Structure[Si].beta;
	Par[1].bw=Structure[Si].beta+db/2;
    Par[2].bw=Par[1].bw;
	Par[3].bw=Structure[Si+1].beta;

	double dw=Structure[Si+1].alpha-Structure[Si].alpha;
    Par[0].w=Structure[Si].alpha*lmb;
    Par[1].w=(Structure[Si].alpha+dw/2)*lmb;
    Par[2].w=Par[1].w;
    Par[3].w=Structure[Si+1].alpha*lmb;

    double dE=Structure[Si+1].E-Structure[Si].E;
    Par[0].E=Structure[Si].E;
    Par[1].E=Structure[Si].E+dE/2;
    Par[2].E=Par[1].E;
    Par[3].E=Structure[Si+1].E;

    double dA=Structure[Si+1].A-Structure[Si].A;
    Par[0].A=Structure[Si].A;
    Par[1].A=Structure[Si].A;//+dA/2;
    Par[2].A=Par[1].A;
    Par[3].A=Structure[Si].A;

    double dB=Structure[Si+1].B-Structure[Si].B;
    Par[0].B=Structure[Si].B;
    Par[1].B=Structure[Si].B+dB/2;
    Par[2].B=Par[1].B;
    Par[3].B=Structure[Si+1].B;

   /*   for(int i=0;i<4;i++)
        Par[i].B*=I;  */

    double d2E=0;
    double d2A=0;
	double d2h=0;
	double d2B=0;
	double dR=0;
    if (Structure[Si+1].Rp!=0 && Structure[Si].Rp!=0)
        dR=ln(Structure[Si+1].Rp)-ln(Structure[Si].Rp);
    double d2R=0;

    if (drift){
        for (int i=0;i<4;i++){
            Par[i].dL=0;
            Par[i].dA=0;
        }
	} else {
		if (Si==0 || (Si!=0 && Structure[Si].jump)){
			Par[0].dL=dE/(Structure[Si].E*dh);
			//Par[0].dL=dR/dh;
			Par[0].dA=dA/dh;
		}else{
			d2E=Structure[Si+1].E-Structure[Si-1].E;
			d2A=Structure[Si+1].A-Structure[Si-1].A;
			d2h=Structure[Si+1].ksi-Structure[Si-1].ksi;
			Par[0].dL=d2E/(Structure[Si].E*d2h);
		   //   d2R=ln(Structure[Si+1].Rp)-ln(Structure[Si-1].Rp);
		   //   Par[0].dL=d2R/d2h;
			Par[0].dA=d2A/d2h;
		}

        Par[1].dL=dE/((Structure[Si].E+dE/2)*dh);
        //Par[1].dL=dR/dh;
        Par[2].dL=Par[1].dL;

		Par[1].dA=dA/dh;
		Par[2].dA=Par[1].dA;

		if (Si==Npoints-2 || (Si<Npoints-2 && Structure[Si+2].jump)){
            Par[3].dL=dE/(Structure[Si+1].E*dh);
            //Par[3].dL=dR/dh;
			Par[3].dA=dA/dh;
		}else{
           //   d2E=Structure[Si+2].E-Structure[Si].E;
			d2A=Structure[Si+2].A-Structure[Si].A;
			d2h=Structure[Si+2].ksi-Structure[Si].ksi;
            Par[3].dL=d2E/(Structure[Si+1].E*d2h);
            //d2R=ln(Structure[Si+2].Rp)-ln(Structure[Si].Rp);
            ///Par[0].dL=d2R/d2h;
			Par[3].dA=d2A/d2h;
        }
	}

	Par[0].Hmap=Structure[Si].Bmap;
	Par[1].Hmap=Structure[Si].Bmap;
	Par[2].Hmap=Structure[Si].Bmap;
	Par[3].Hmap=Structure[Si+1].Bmap;

	if (ExternalMagnetic.Field!=NULL) {

		for (int k = 0; k < 4; k++) {
			Par[k].Hext.Field=new TField*[ExternalMagnetic.Dim.Ny];
			Par[k].Hext.Dim=ExternalMagnetic.Dim;
			Par[k].Hext.Piv=ExternalMagnetic.Piv;
			for (int i = 0; i < ExternalMagnetic.Dim.Ny; i++) {
				Par[k].Hext.Field[i]=new TField[ExternalMagnetic.Dim.Nx];
			}
		}

		for (int i = 0; i < ExternalMagnetic.Dim.Ny; i++) {
			for (int j = 0; j < ExternalMagnetic.Dim.Nx; j++) {
				Par[0].Hext.Field[i][j].z=ExternalMagnetic.Field[Si][i][j].z;
				Par[1].Hext.Field[i][j].z=(ExternalMagnetic.Field[Si+1][i][j].z+ExternalMagnetic.Field[Si][i][j].z)/2;
				Par[2].Hext.Field[i][j].z=Par[1].Hext.Field[i][j].z;
				Par[3].Hext.Field[i][j].z=ExternalMagnetic.Field[Si+1][i][j].z;

				Par[0].Hext.Field[i][j].r=ExternalMagnetic.Field[Si][i][j].r;
				Par[1].Hext.Field[i][j].r=(ExternalMagnetic.Field[Si+1][i][j].r+ExternalMagnetic.Field[Si][i][j].r)/2;
				Par[2].Hext.Field[i][j].r=Par[1].Hext.Field[i][j].r;
				Par[3].Hext.Field[i][j].r=ExternalMagnetic.Field[Si+1][i][j].r;

				Par[0].Hext.Field[i][j].th=ExternalMagnetic.Field[Si][i][j].th;
				Par[1].Hext.Field[i][j].th=(ExternalMagnetic.Field[Si+1][i][j].th+ExternalMagnetic.Field[Si][i][j].th)/2;
				Par[2].Hext.Field[i][j].th=Par[1].Hext.Field[i][j].th;
				Par[3].Hext.Field[i][j].th=ExternalMagnetic.Field[Si+1][i][j].th;
			}
		}

	}  else {
		Par[0].Hext.Field=NULL;
		Par[1].Hext.Field=NULL;
		Par[2].Hext.Field=NULL;
		Par[3].Hext.Field=NULL;
    }

  /*	double dBzx=Structure[Si+1].Hext.z-Structure[Si].Hext.z;
	Par[0].Hext.z=Structure[Si].Hext.z;
	Par[1].Hext.z=Structure[Si].Hext.z+dBzx/2;
	Par[2].Hext.z=Par[1].Hext.z;
	Par[3].Hext.z=Structure[Si+1].Hext.z;

	double dBrx=Structure[Si+1].Hext.r-Structure[Si].Hext.r;
	Par[0].Hext.r=Structure[Si].Hext.r;
	Par[1].Hext.r=Structure[Si].Hext.r+dBrx/2;
	Par[2].Hext.r=Par[1].Hext.r;
	Par[3].Hext.r=Structure[Si+1].Hext.r;

	double dBthx=Structure[Si+1].Hext.th-Structure[Si].Hext.th;
	Par[0].Hext.th=Structure[Si].Hext.th;
	Par[1].Hext.th=Structure[Si].Hext.th+dBthx/2;
	Par[2].Hext.th=Par[1].Hext.th;
	Par[3].Hext.th=Structure[Si+1].Hext.th;     */


  /*	if (Si==0)
		Par[0].dH=dBzx/dh;
	else{
		d2Bzx=Structure[Si+1].Hext.z-Structure[Si-1].Hext.z;
		d2h=Structure[Si+1].ksi-Structure[Si-1].ksi;
		Par[0].dH=d2Bzx/d2h;
	}
	Par[1].dH=dBzx/dh;
	Par[2].dH=Par[1].dH;
	if (Si==Npoints-2){
		Par[3].dH=dBzx/dh;
	} else {
		d2Bzx=Structure[Si+2].Bz_ext-Structure[Si].Bz_ext;
		d2h=Structure[Si+2].ksi-Structure[Si].ksi;
		Par[3].dH=d2Bzx/d2h;
	}

   	FILE *logFile=fopen("beam.log","a");
	//fprintf(logFile,"Phase Radius Betta\n");
    //for (int i=0;i<Np;i++)
		fprintf(logFile,"%i %f %f %f %f\n",Si,Par[0].dH,Par[1].dH,Par[2].dH,Par[3].dH);

	fclose(logFile);   */

    for (int j=0;j<Ncoef;j++)
		Integrate(Si,j);

	for (int k = 0; k < 4; k++) {
		for (int i = 0; i < ExternalMagnetic.Dim.Ny; i++) {
			delete[] Par[k].Hext.Field[i];
		}
		delete[] Par[k].Hext.Field;
	}
}
//---------------------------------------------------------------------------
TError TBeamSolver::Solve()
{
	#ifndef RSHELLWEG_LINUX
	if (SmartProgress==NULL){
		//ShowMessage("System Message: ProgressBar not assigned! Code needs to be corrected");
        return ERR_OTHER;
    }
	SmartProgress->Reset(Npoints-1/*Np*/);
    SmartProgress->ShowMessage("Running...");
	#endif

	double phi0=0, phi_s=0, phi_p=0;
	bool DriftOverride=false;

  //    logFile=fopen("beam.log","w");
 /* for (int i=0;i<Np;i++){
      //    fprintf(logFile,"%f %f\n",Beam[0]->Particle[i].x,Beam[0]->Particle[i].x/Structure[0].lmb);
        Beam[0]->Particle[i].x/=Structure[0].lmb;
    }                                             */
 // fclose(logFile);

    for (int i=0;i<Ncoef;i++){
        delete[] K[i];
		K[i]=new TIntegration[BeamPar.NParticles];
    }
    K[0][0].A=Structure[0].A;
   //   Beam[0]->Particle[j].z=Structure[0].ksi*Structure[0].lmb;
   //
	for (int i=0;i<Npoints;i++){
        //for (int j=0;j<Np;j++){

            //if (i==0)
           //   Nliv=Beam[i]->GetLivingNumber();
		for (int j = 0; j < Ndump; j++) {
			if (BeamExport[j].Nmesh==i) {
				DumpBeam(j);
			}
		}

		if (i==Npoints-1) break; // Nowhere to iterate

		if (!Structure[i+1].jump){
			Step(i);
			Structure[i+1].A=Structure[i].A+dh*(K[0][0].A+K[1][0].A+2*K[2][0].A+2*K[3][0].A)/6;
			//  fprintf(logFile,"%f %f %f %f %f\n",K[1][0].A,K[2][0].A,K[3][0].A,K[0][0].A,Structure[i+1].A);
			Beam[i]->Next(Beam[i+1],Par[3],K);
		} else {
			//Structure[i+1].A=Structure[i].A ;
			Beam[i]->Next(Beam[i+1]);
		}

		if (Structure[i].drift)
			DriftOverride=true;

		for (int j=0;j<BeamPar.NParticles;j++){
			phi_s=Structure[i+1].ksi*2*pi;
			if (Beam[i+1]->Particle[j].lost==LIVE){
				if (Structure[i+1].Bmap.Field==NULL) {
					if (mod(Beam[i+1]->Particle[j].r)>=Structure[i+1].Ra)
						Beam[i+1]->Particle[j].lost=RADIUS_LOST;
				} else {
					TPhaseSpace R;
					double xmin=0,xmax=0,ymin=0,ymax=0;
					R.x=Beam[i+1]->Particle[j].r*cos(Beam[i+1]->Particle[j].th);
					R.y=Beam[i+1]->Particle[j].r*sin(Beam[i+1]->Particle[j].th);
					xmin=Structure[i+1].Bmap.Piv.X[0];
					xmax=Structure[i+1].Bmap.Piv.X[Structure[i+1].Bmap.Dim.Nx-1];
					ymin=Structure[i+1].Bmap.Piv.Y[0];
					ymax=Structure[i+1].Bmap.Piv.Y[Structure[i+1].Bmap.Dim.Ny-1];
					if (R.x<xmin || R.x>xmax || R.y<ymin || R.y>ymax) {
						Beam[i+1]->Particle[j].lost=RADIUS_LOST;
					}
				}
				phi0=Beam[0]->Particle[j].phi;
				phi_p=Beam[i+1]->Particle[j].phi;
				if ((phi_p-phi0+phi_s)<-pi/2 && !DriftOverride) {
					Beam[i+1]->Particle[j].lost=PHASE_LOST;
				}
			}
			Beam[i+1]->Particle[j].z=Structure[i+1].ksi*Structure[i+1].lmb;
			Beam[i+1]->Particle[j].phi+=Structure[i+1].dF;
		}

		#ifndef RSHELLWEG_LINUX
		SmartProgress->operator ++();
		Application->ProcessMessages();
		#endif
		if (SolverStop!=ERR_NO){

			#ifndef RSHELLWEG_LINUX
			AnsiString S="Solve Process Aborted!";
			ShowError(S);
			if (SolverStop==ERR_ABORT)
				SmartProgress->Reset();
			else if (SolverStop==ERR_BEAM)
				SmartProgress->TerminateTime();
			#endif

			SolverStop=ERR_NO;
			return ERR_ABORT;
        }
        for (int i=0;i<Ncoef;i++)
            memset(K[i], 0, sizeof(TIntegration));
        //}
    }

   //

    #ifndef RSHELLWEG_LINUX
    SmartProgress->SetPercent(100);
	SmartProgress->SetTime(0);
	SmartProgress->TerminateTime();
    SmartProgress->ShowMessage("Finished");
	#endif

	return ERR_NO;
}
//---------------------------------------------------------------------------
#ifndef RSHELLWEG_LINUX
TResult TBeamSolver::Output(AnsiString& FileName,TMemo *Memo)
#else
TResult TBeamSolver::Output(AnsiString& FileName)
#endif
{
	AnsiString Line,s;
	AnsiString EU=GetEnergyUnit(BeamPar.Species.Type);
    TStringList *OutputStrings;
    OutputStrings= new TStringList;
	TResult Result;
  /*    int Nliv=0;
    Nliv=Beam[Npoints-1]->GetLivingNumber();
                     */
   // OutputStrings->Clear();
   //	OutputStrings->Add("========================================");
   // OutputStrings->Add("INPUT DATA from:");
   //	OutputStrings->Add(InputFile);
	OutputStrings->Add("========================================");

  /*    TStringList *InputStrings;
    InputStrings= new TStringList;
	InputStrings->LoadFromFile(InputFile);    */
	OutputStrings->AddStrings(InputStrings);
	//OutputStrings->AddStrings(ParsedStrings);
	OutputStrings->Add("========================================");
	OutputStrings->Add("");
    OutputStrings->Add("RESULTS");
	OutputStrings->Add("========================================");
   //OutputStrings->Add("");

    double Ws=0;
   //   AnsiString s;
    int j=Npoints-1;
    double z=100*Structure[j].ksi*Structure[j].lmb;

	TGauss Gw=GetEnergyStats(j,D_FWHM);

	double Wm=Beam[j]->GetMaxEnergy();
	double I=Beam[j]->GetCurrent();
	double I0=Beam[0]->GetCurrent();
    double kc=100.0*Beam[j]->GetLivingNumber()/Beam[0]->GetLivingNumber();
	double r=1e3*Beam[j]->GetRadius();

	TGauss Gphi=GetPhaseStats(j,D_FWHM);

    double f=1e-6*c/Structure[j].lmb;
	double Ra=1e3*Structure[j].Ra*Structure[j].lmb;
	double P=Gw.mean*I;

	double W0=Beam[0]->GetAverageEnergy();
	//double Wout=Beam[j]->GetAverageEnergy();
	double Pin=W0*I0;


	double v=Structure[j].beta;
	double E=sqrt(2*Structure[j].Rp);
	double Pb=E!=0?1e-6*sqr(Structure[j].A*BeamPar.Wnorm/E):0;

	double beta_gamma=MeVToVelocity(Gw.mean,BeamPar.W0)*MeVToGamma(Gw.mean,BeamPar.W0);

	/*double Pw=P0;
    for(int i=1;i<Npoints;i++)
        Pw=Pw*exp(-2*(Structure[i].ksi-Structure[i-1].ksi)*Structure[i].alpha*Structure[i].lmb);  */
	//double Pw=1e-6*P0-(P-Pin+Pb);

	TTwiss TR,TX,TY;
	TR=GetTwiss(j,R_PAR);
	TX=GetTwiss(j,X_PAR);
	TY=GetTwiss(j,Y_PAR);
  /*  double A=0;
  	int Na=j-Nmesh/2;
	if (Na>0)
		A=Structure[Na].A;     */

    Result.Length=z;
	Result.MaximumEnergy=Wm;
	Result.Energy=Gw;
	Result.InputCurrent=1e3*I0;
    Result.BeamCurrent=1e3*I;
    Result.Captured=kc;
	Result.BeamRadius=r;
	Result.Phase=Gphi;
    Result.BeamPower=P;
	Result.LoadPower=Pb;
	Result.RTwiss=TR;
	Result.XTwiss=TX;
	Result.YTwiss=TY;
  //  Result.A=A;

	Line="Total Length = "+s.FormatFloat("#0.000",Result.Length)+" cm";
	OutputStrings->Add(Line);
	Line="Average Energy = "+s.FormatFloat("#0.000 ",Result.Energy.mean)+EU;
	if (BeamPar.Species.Type==ION)
		Line=Line+" ("+s.FormatFloat("#0.000 ",BeamPar.Species.A*Result.Energy.mean)+" MeV)";
	OutputStrings->Add(Line);

	Line="Maximum Energy = "+s.FormatFloat("#0.000 ",Result.MaximumEnergy)+EU;
	if (BeamPar.Species.Type==ION)
		Line=Line+" ("+s.FormatFloat("#0.000 ",BeamPar.Species.A*Result.MaximumEnergy)+" MeV)";
	OutputStrings->Add(Line);

	Line="Energy Spectrum (FWHM) = "+s.FormatFloat("#0.00",100*Result.Energy.FWHM/Result.Energy.mean)+" %";
	OutputStrings->Add(Line);

	Line="Input Current = "+s.FormatFloat("#0.00",Result.InputCurrent)+" mA";
	if (BeamPar.Species.Type==ION){
		Line=Line+" ("+s.FormatFloat("#0.00",Result.InputCurrent/BeamPar.Species.Q)+" pmA)";
	}
	OutputStrings->Add(Line);

	Line="Beam Current = "+s.FormatFloat("#0.00",Result.BeamCurrent)+" mA";
	if (BeamPar.Species.Type==ION){
		Line=Line+" ("+s.FormatFloat("#0.00",Result.BeamCurrent/BeamPar.Species.Q)+" pmA)";
	}
	OutputStrings->Add(Line);

	Line="Particles Transmitted = "+s.FormatFloat("#0.00",Result.Captured)+" %";
	OutputStrings->Add(Line);
    Line="Beam Radius (RMS) = "+s.FormatFloat("#0.00",Result.BeamRadius)+" mm";
    OutputStrings->Add(Line);
	Line="Average Phase = "+s.FormatFloat("#0.00",RadToDegree(Result.Phase.mean))+" deg";
	OutputStrings->Add(Line);
	Line="Phase Length (FWHM) = "+s.FormatFloat("#0.00",RadToDegree(Result.Phase.FWHM))+" deg";
	OutputStrings->Add(Line);
	OutputStrings->Add("Twiss Parameters (R):");
	Line="alpha= "+s.FormatFloat("#0.00000",Result.RTwiss.alpha);
	OutputStrings->Add(Line);
	Line="beta = "+s.FormatFloat("#0.00000",100*Result.RTwiss.beta)+" cm/rad";
	OutputStrings->Add(Line);
	Line="emittance = "+s.FormatFloat("#0.000000",1e6*Result.RTwiss.epsilon)+" um";
	OutputStrings->Add(Line);
	Line="emittance (norm) = "+s.FormatFloat("#0.000000",1e6*beta_gamma*Result.RTwiss.epsilon)+" um";
	OutputStrings->Add(Line);
	OutputStrings->Add("Twiss Parameters (X):");
	Line="alpha= "+s.FormatFloat("#0.00000",Result.XTwiss.alpha);
	OutputStrings->Add(Line);
	Line="beta = "+s.FormatFloat("#0.00000",100*Result.XTwiss.beta)+" cm/rad";
	OutputStrings->Add(Line);
	Line="emittance = "+s.FormatFloat("#0.000000",1e6*Result.XTwiss.epsilon)+" um";
	OutputStrings->Add(Line);
	Line="emittance (norm) = "+s.FormatFloat("#0.000000",1e6*beta_gamma*Result.XTwiss.epsilon)+" um";
	OutputStrings->Add(Line);
	OutputStrings->Add("Twiss Parameters (Y):");
	Line="alpha= "+s.FormatFloat("#0.00000",Result.YTwiss.alpha);
	OutputStrings->Add(Line);
	Line="beta = "+s.FormatFloat("#0.00000",100*Result.YTwiss.beta)+" cm/rad";
	OutputStrings->Add(Line);
	Line="emittance = "+s.FormatFloat("#0.000000",1e6*Result.YTwiss.epsilon)+" um";
	OutputStrings->Add(Line);
	Line="emittance (norm) = "+s.FormatFloat("#0.000000",1e6*beta_gamma*Result.YTwiss.epsilon)+" um";
	OutputStrings->Add(Line);
	//OutputStrings->Add("========================================");

	int Nsec=0;
	double Pbeam0=0, Wb0=0;
	double Ib=0, Wb=0, P0=0;
	for (int i=0;i<=j;i++){
		if ((Structure[i].jump && !Structure[i].drift) || i==j) {
			Ib=Beam[i]->GetCurrent();
			Wb=Beam[i]->GetAverageEnergy();

			if (Nsec>0) {
				double Pload=E!=0?1e-6*sqr(Structure[i-1].A*BeamPar.Wnorm/E):0;
				double Pbeam=Ib*Wb;
				double F0=1e-6*GetSectionFrequency(Nsec-1);

				Line="===========Section #"+s.FormatFloat("#",Nsec)+" ======================";
				OutputStrings->Add(Line);
				Line="Input Power = "+s.FormatFloat("#0.0000",P0)+" MW";
				OutputStrings->Add(Line);
				Line="Frequency = "+s.FormatFloat("#0.0000",F0)+" MHz";
				OutputStrings->Add(Line);
			   /*	Line="Beam Power = "+s.FormatFloat("#0.0000",Pbeam)+" MW";
				OutputStrings->Add(Line);
				Line="Beam Energy = "+s.FormatFloat("#0.0000",Wb)+" MeV";
				OutputStrings->Add(Line);   */
				Line="Beam Energy Gain = "+s.FormatFloat("#0.0000 ",Wb-Wb0)+EU;
                if (BeamPar.Species.Type==ION)
					Line=Line+" ("+s.FormatFloat("#0.000 ",BeamPar.Species.A*(Wb-Wb0))+" MeV)";
				OutputStrings->Add(Line);

				Line="Beam Power Gain = "+s.FormatFloat("#0.0000",Pbeam-Pbeam0)+" MW";
				OutputStrings->Add(Line);
				Line="Load Power = "+s.FormatFloat("#0.0000",Pload)+" MW";
				OutputStrings->Add(Line);
				Line="Loss Power = "+s.FormatFloat("#0.0000",P0-(Pbeam-Pbeam0+Pload))+" MW";
				OutputStrings->Add(Line);
			}

			Pbeam0=Ib*Wb;
			P0=1e-6*Structure[i].P;
			Wb0=Wb;
			Nsec++;
		}
	}
	OutputStrings->Add("========================================");


    #ifndef RSHELLWEG_LINUX
	if (Memo!=NULL){
		Memo->Lines->AddStrings(OutputStrings);
                Memo->Lines->SaveToFile(FileName);
        }
    #endif
  	OutputStrings->SaveToFile(FileName);
        delete OutputStrings;
   //   delete Strings;



    return Result;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
