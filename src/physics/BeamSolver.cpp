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
    delete[] Cells;
    delete[] Structure;

    for (int i=0;i<Npoints;i++)
        delete Beam[i];
    delete[] Beam;

    delete[] Par;
    for (int i=0;i<Ncoef;i++)
        delete[] K[i];
    delete[] K;

    #ifndef RSLINAC
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
    MaxCells=500;
    Nmesh=20;
    Kernel=0.9;
    SplineType=LSPLINE;
    Nstat=100;
    Ngraph=500;
    Nbars=100;
    Nav=10;
    Smooth=0.95;
    Npoints=1;

	//Np=1;
   //	NpEnergy=0;
	Np_beam=1;
	BeamPar.RBeamType=NOBEAM;
	BeamPar.ZBeamType=NOBEAM;
	BeamPar.NParticles=1;

	Nlim=-1;
	Magnetized=false;

	LoadIniConstants();

	DataReady=false;
	Stop=false;

	K=new TIntegration*[Ncoef];
	for (int i=0;i<Ncoef;i++)
		K[i]=new TIntegration[BeamPar.NParticles];

	Par=new TIntParameters[Ncoef];

	Cells = new TCell[1];


    Beam=new TBeam*[Npoints];
    for (int i=0;i<Npoints;i++) {
        Beam[i]=new TBeam(1);
    }

    Structure=new TStructure[Npoints];

    InputStrings=new TStringList;
    ParsedStrings=new TStringList;

    #ifndef RSLINAC
    SmartProgress=NULL;
    #endif
}
//---------------------------------------------------------------------------
void TBeamSolver::ShowError(AnsiString &S)
{
	ShowMessage(S);
	ParsedStrings->Add(S);
}
//---------------------------------------------------------------------------
void TBeamSolver::SaveToFile(AnsiString& Fname)
{
    FILE *F;
    F=fopen(Fname.c_str(),"wb");

    fwrite(&Npoints,sizeof(int),1,F);
	fwrite(&BeamPar.NParticles,sizeof(int),1,F);
    fwrite(&Nbars,sizeof(int),1,F);

    fwrite(Structure,sizeof(TStructure),Npoints,F);

    for(int i=0;i<Npoints;i++){
        fwrite(&(Beam[i]->lmb),sizeof(double),1,F);
        fwrite(&(Beam[i]->h),sizeof(double),1,F);
        fwrite(&(Beam[i]->Ib),sizeof(double),1,F);
        fwrite(&(Beam[i]->I0),sizeof(double),1,F);
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

    for (int i=0;i<Np_beam;i++)
        delete Beam[i];
    delete[] Beam;

    Beam=new TBeam*[Npoints];
    for (int i=0;i<Npoints;i++)
		Beam[i]=new TBeam(BeamPar.NParticles);
	Np_beam=Npoints;

    try{
        fread(&Npoints,sizeof(int),1,F);
		fread(&BeamPar.NParticles,sizeof(int),1,F);
        fread(&Nbars,sizeof(int),1,F);

        Structure=new TStructure[Npoints];
        Beam=new TBeam*[Npoints];
        for (int i=0;i<Npoints;i++)
			Beam[i]=new TBeam(BeamPar.NParticles);

        fread(Structure,sizeof(TStructure),Npoints,F);
      //    fread(Beam,sizeof(TBeam),Npoints,F);


        for (int i=0;i<Npoints;i++){
            fread(&(Beam[i]->lmb),sizeof(double),1,F);
            fread(&(Beam[i]->h),sizeof(double),1,F);
            fread(&(Beam[i]->Ib),sizeof(double),1,F);
            fread(&(Beam[i]->I0),sizeof(double),1,F);
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
#ifndef RSLINAC
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
    double stat;

    UserIni=new TIniFile(UserIniPath);
    MaxCells=UserIni->ReadInteger("OTHER","Maximum Cells",MaxCells);
    Nmesh=UserIni->ReadInteger("NUMERIC","Number of Mesh Points",Nmesh);
    Kernel=UserIni->ReadFloat("Beam","Percent Of Particles in Kernel",Kernel);
    if (Kernel>0)
    	Kernel/=100;
    	else
    	Kernel=0.9;

    
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

    stat=UserIni->ReadFloat("NUMERIC","Statistics Error",stat);
    if (stat<1e-6)
        stat=1e-6;
    if (stat>25)
        stat=25;
	int Nstat=round(100.0/stat);
	AngErr=UserIni->ReadFloat("NUMERIC","Angle Error",AngErr);
    Smooth=UserIni->ReadFloat("NUMERIC","Smoothing",Smooth);
    Ngraph=UserIni->ReadInteger("OTHER","Chart Points",Nbars);
    Nbars=UserIni->ReadInteger("NUMERIC","Hystogram Bars",Ngraph);
    Nav=UserIni->ReadInteger("NUMERIC","Averaging Points",Nav);
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
int TBeamSolver::GetNumberOfChartPoints()
{
    return Ngraph;
}
//---------------------------------------------------------------------------
int TBeamSolver::GetNumberOfBars()
{
    return Nbars;
}
//---------------------------------------------------------------------------
int TBeamSolver::GetNumberOfCells()
{
    return Ncells;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetFrequency()
{
    return F0;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetPower()
{
    return P0;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetInputCurrent()
{
	return BeamPar.Current;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetSolenoidField()
{
    return B0;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetSolenoidPosition()
{
    return Zmag;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetSolenoidLength()
{
    return Lmag;
}
/*//---------------------------------------------------------------------------
double TBeamSolver::GetMode(int *N,int *M)
{
    if (M!=NULL)
        *M=Mode_M;
    if (N!=NULL)
        *N=Mode_N;
    return Mode_N*pi/Mode_M;
}     */
//---------------------------------------------------------------------------
double TBeamSolver::GetInputAverageEnergy()
{
	return Beam[0]->GetAverageEnergy();
}
//---------------------------------------------------------------------------
double TBeamSolver::GetInputEnergyDeviation()
{
	return -1;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetInputAveragePhase()
{
	return Beam[0]->GetAveragePhase();
}
//---------------------------------------------------------------------------
double TBeamSolver::GetInputPhaseDeviation()
{
	return -1;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetInputAlpha()
{
	return -1;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetInputBetta()
{
	return -1;
}
//---------------------------------------------------------------------------
double TBeamSolver::GetInputEpsilon()
{
	return -1;
}
//---------------------------------------------------------------------------
bool TBeamSolver::IsCoulombAccounted()
{
	return Coulomb;
}
//---------------------------------------------------------------------------
bool TBeamSolver::IsWaveReversed()
{
    return Reverse;
}
//---------------------------------------------------------------------------
bool TBeamSolver::IsEnergyEquiprobable()
{
	return 0;
}
//---------------------------------------------------------------------------
bool TBeamSolver::IsPhaseEquiprobable()
{
	return 0;
}
//---------------------------------------------------------------------------
bool TBeamSolver::IsKeyWord(AnsiString &S)
{
    return S=="POWER" ||
        S=="SOLENOID" ||
        S=="BEAM" ||
        S=="CURRENT" ||
        S=="DRIFT" ||
        S=="CELL" ||
		S=="CELLS" ||
		S=="SAVE" ||
        S=="OPTIONS" ||
		S=="SPCHARGE"; 
	  //	S=="COMMENT";
}
//---------------------------------------------------------------------------
TBeamType TBeamSolver::ParseDistribution(AnsiString &S)
{
	 TBeamType D=NOBEAM;
		if (S=="CST_PID")
			D=CST_PID;
		else if (S=="CST_PIT")
			D=CST_PIT;
		else if (S=="TWISS2D")
			D=TWISS_2D;
		else if (S=="TWISS4D")
			D=TWISS_4D;
		else if (S=="SPH2D")
			D=SPH_2D;
		else if (S=="ELL2D")
			D=ELL_2D;
		else if (S=="FILE1D")
			D=FILE_1D;
		else if (S=="FILE2D")
			D=FILE_2D;
		else if (S=="FILE4D")
			D=FILE_4D;
		else if (S=="NORM2D")
			D=NORM_2D;
		/*else if (S=="NORM4D")
			D=NORM_4D;  */
	 return D;
}
//---------------------------------------------------------------------------
bool TBeamSolver::IsFullFileKeyWord(TBeamType D)
{
	bool R=false;

	switch (D) {
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
		case FILE_1D:{}
		case FILE_2D:{}
		case TWO_FILES_2D:{}
		case FILE_4D:{R=true;break;}
		default: R=false;
	}
	return R;
}
//---------------------------------------------------------------------------
TInputParameter TBeamSolver::Parse(AnsiString &S)
{
    TInputParameter P;
    FILE *logFile;
	
/*    logFile=fopen("BeamSolver.log","a");
    fprintf(logFile,"Parse: S=%s\n",S);
    fclose(logFile); */
	if (S=="POWER")
		P=POWER;
    else if (S=="SOLENOID")
        P=SOLENOID;
    else if (S=="BEAM")
        P=BEAM;
    else if (S=="CURRENT")
		P=CURRENT;
    else if (S=="DRIFT")
        P=DRIFT;
    else if (S=="CELL")
        P=CELL;
    else if (S=="CELLS")
        P=CELLS;
    else if (S=="OPTIONS")
		P=OPTIONS;
	else if (S=="SAVE")
		P=DUMP;
    else if (S=="SPCHARGE")
		P=SPCHARGE;
    else if (S[1]=='!')
		P=COMMENT;
	return  P;
}
//---------------------------------------------------------------------------
void TBeamSolver::GetDimensions(TCell& Cell)
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
    Cell.AkL=rSpline.Interpolate(Cell.betta);

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
    Cell.AL32=1e-4*aSpline.Interpolate(Cell.betta);

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
    int i=-1,j=0;

    FILE *logFile;
    while (!fs.eof()){
		S=GetWord(fs);   //Hid common actions inside the function
		if (S=="")
			continue;
		if(IsKeyWord(S) || S[1]=='!')
            N++;
/*        logFile=fopen("BeamSolver.log","a");
        fprintf(logFile,"ParseFile: N=%i, S=%s\n",N,S);
        fclose(logFile); */
    }

    fs.clear();
    fs.seekg(std::ios::beg);

    Lines=new TInputLine[N];

	while (!fs.eof()){
		L=GetLine(fs);   //Now reads line by line
		S=ReadWord(L,1);  //Gets the first word in the line (should be a key word)

		   /*	fs>>s;
			S=AnsiString(s);    */
/*            logFile=fopen("BeamSolver.log","a");
			fprintf(logFile,"ParseFile: s=%s, S=%s\n",s,S);
			fclose(logFile); */
		if (S=="END")
			break;
		else if (S=="") //empty line
			continue;
		else if(IsKeyWord(S) || S[1]=='!'){ //Key word or comment detected. Start parsing the line
			i++;
			P=Parse(S);
/*                logFile=fopen("BeamSolver.log","a");
				fprintf(logFile,"ParseFile: P=%s\n",P);
				fclose(logFile); */
/*				if(S=="COMMENT"){
					logFile=fopen("BeamSolver.log","a");
					fprintf(logFile,"ParseFile: Comment Line\n");
					fclose(logFile);
				}                  */
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
TError TBeamSolver::ParseLines(TInputLine *Lines,int N,bool OnlyParameters)
{
    int Ni=0;
    float dF=0;

    Coulomb=false;
    SpCharge=false;
	GWmethod=false;
    Reverse=false;
	Magnetized=false;
	//BeamType=RANDOM;
	FSolenoid=false;
	SolenoidFile=Solenoid_File;
	//BeamFile=CST_FileX;
	//EnergyFile="";
	TError Error=ERR_NO;

	bool BeamDefined=false;
	bool CurrentDefined=false;
	bool CellDefined=false;
	bool PowerDefined=false;
	bool NpFromFile=false;

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
				F="OPTIONS ";
				for (int j=0;j<Lines[k].N;j++){
					if (Lines[k].S[j]=="REVERSE")
						Reverse=true;
					if (Lines[k].S[j]=="MAGNETIZED")
						Magnetized=true;
					F=F+"\t"+Lines[k].S[j];
				}
				SpCharge=false;
				Coulomb=false;
//				GWmethod=false;
				ParsedStrings->Add(F);
				break;
			}
			case SPCHARGE:{
			    F="SPCHARGE ";
				SpCharge=true;
		        F=F+"\t"+Lines[k].S[0];
				Nslices=1;
                if (Lines[k].N==0){
				    Coulomb=true;
				    GWmethod=false;
				}
                if (Lines[k].N==1){
                    if (Lines[k].S[0]=="COULOMB"){
				        Coulomb=true;
				        GWmethod=false;
					    F=F+"\t"+Lines[k].S[0];
                    } 
					if (Lines[k].S[0]=="GWMETHOD") {
				        Coulomb=false;
				        GWmethod=true;
//					    F=F+"\t"+Lines[k].S[0];
                    } 
					if ((Lines[k].S[0]!="COULOMB") && ((Lines[k].S[0]!="GWMETHOD"))) {					
						return ERR_SPCHARGE;
					    break;
					}
				}
                if (Lines[k].N==2){
                    if (Lines[k].S[0]!="GWMETHOD"){
						return ERR_SPCHARGE;
					    break;
					} else {
				        Coulomb=false;
				        GWmethod=true;
						Nslices=Lines[k].S[1].ToInt();
						F+=AddLines(&Lines[k],0,1);
					  /*  F=F+"\t"+Lines[k].S[0];
						F=F+"\t"+Lines[k].S[1];     */
					}
				}
				ParsedStrings->Add(F);
				break;				
			}
            case SOLENOID:{
                if (Lines[k].N==3){
                    B0=Lines[k].S[0].ToDouble();
                    Lmag=Lines[k].S[1].ToDouble();
                    Zmag=Lines[k].S[2].ToDouble();
					F="SOLENOID";
					F+=AddLines(&Lines[k],0,2);
				   //	+Lines[k].S[0]+"\t"+Lines[k].S[1]+"\t"+Lines[k].S[2];
                    ParsedStrings->Add(F);
				}else if (Lines[k].N==1) {
					if (Lines[k].S[0]=="BFILE")
						F="SOLENOID BFILE";
					else {
						SolenoidFile=Lines[k].S[0];
						F="SOLENOID "+SolenoidFile;
					}
					if (CheckFile(SolenoidFile)){
						FSolenoid=true;
						ParsedStrings->Add(F);
					}else{
						S="ERROR: The file "+SolenoidFile+" is missing!";
						ShowError(S);
						return ERR_SOLENOID;
					}
				}else
					return ERR_SOLENOID;
                break;
			}

			case BEAM:{
				if (Lines[k].N < 1)
					return ERR_BEAM;

				F="BEAM";
				AnsiString KeyWord=Lines[k].S[0];
				BeamPar.RBeamType=ParseDistribution(KeyWord);
				BeamPar.ZCompress=false;

				if (BeamPar.RBeamType==NOBEAM) {
					S="ERROR: Wrong KEYWORD in BEAM line";
					ShowError(S);
					return ERR_BEAM;
				}

				if (IsFullFileKeyWord(BeamPar.RBeamType)) {
					if (Lines[k].N < 2 || Lines[k].N >5)
						return ERR_BEAM;

					switch (BeamPar.RBeamType) {
						case CST_PID: {
							if (Lines[k].N == 3){
								return ERR_BEAM;
							} else {
                            	AnsiString FileName=Lines[k].S[1];
								if (CheckFile(FileName)) {
									BeamPar.RFile=FileName;
									F+=AddLines(&Lines[k],0,1);
								   //	F="BEAM "+KeyWord+" \t"+FileName;
									BeamPar.ZFile=FileName;
									BeamPar.ZBeamType=NORM_1D;

									if (Lines[k].N > 3) {
										BeamPar.ZNorm.mean=DegreeToRad(Lines[k].S[2].ToDouble());
										BeamPar.ZNorm.limit=DegreeToRad(Lines[k].S[3].ToDouble());
										F+=AddLines(&Lines[k],2,3);
									  //	F+=" \t"+Lines[k].S[2]+" \t"+Lines[k].S[3];
										if (Lines[k].N > 4) {
											BeamPar.ZNorm.sigma=DegreeToRad(Lines[k].S[4].ToDouble());
											F+=" \t"+Lines[k].S[4];
										} else {
											BeamPar.ZNorm.sigma=100*BeamPar.ZNorm.limit;
										}
									}
									else {
										BeamPar.ZNorm.mean=pi;
										BeamPar.ZNorm.limit=pi;
										BeamPar.ZNorm.sigma=100*pi;
                                    }
								}
								BeamDefined=true;
								ParsedStrings->Add(F);
                            }
							break;
						}
						case CST_PIT: {
							AnsiString FileName=Lines[k].S[1];
							if (CheckFile(FileName)) {
								BeamPar.ZBeamType=BeamPar.RBeamType;
								BeamPar.RFile=FileName;
								BeamPar.ZFile=FileName;

								//F="BEAM "+KeyWord+" \t"+FileName;
								F+=AddLines(&Lines[k],0,1);

								if (Lines[k].N == 3){
									if (Lines[k].S[2]=="COMPRESS") {
										BeamPar.ZCompress=true;
										F+=" COMPRESS";
									} else {
										S="ERROR: Wrong keyword "+Lines[k].S[2]+" in BEAM line";
										ShowError(S);
										return ERR_BEAM;
                                    }
								}

								BeamDefined=true;
								ParsedStrings->Add(F);
							} else {
								S="ERROR: The file "+FileName+" is missing!";
								ShowError(S);
								return ERR_BEAM;
							}
							break;
						}
						default: {
							S="Unexpected Input File Type";
							ShowError(S);
							return ERR_BEAM;
						}
					}
				} else if (IsTransverseKeyWord(BeamPar.RBeamType)) {
					int Zpos=0;
					for (int j=1; j < Lines[k].N; j++) {
						if (IsLongitudinalKeyWord(ParseDistribution(Lines[k].S[j]))) {
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
					int Nz=Lines[k].N-Zpos-1; //Number of Z-parameters

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
							AnsiString FileName=Lines[k].S[1];
							if (CheckFile(FileName)) {
								BeamPar.RFile=FileName;
								//F="BEAM "+KeyWord+" \t"+FileName+" \t";
								F+=AddLines(&Lines[k],0,1);
							} else {
								S="ERROR: The file "+FileName+" is missing!";
								ShowError(S);
								return ERR_BEAM;
							}
							if (Nr==2){
								FileName=Lines[k].S[2];
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
							break;
						}

						case TWISS_2D: {
							if (Nr != 3){
								S="Wrong number of Twiss parameters in BEAM line!";
								ShowError(S);
								return ERR_BEAM;
							}else{
								BeamPar.XTwiss.alpha=Lines[k].S[1].ToDouble();
								BeamPar.XTwiss.beta=Lines[k].S[2].ToDouble();
								BeamPar.XTwiss.epsilon=Lines[k].S[3].ToDouble();
								BeamPar.YTwiss=BeamPar.XTwiss;
							 //	Zpos=4;
								//F="BEAM";
								F+=AddLines(&Lines[k],0,3);
								//+Lines[k].S[0]+" \t"+Lines[k].S[1]+" \t"+Lines[k].S[2]+" \t"+Lines[k].S[3]+" \t";
							}
							break;
						}
						case TWISS_4D: {
							if (Nr != 6){
								S="Wrong number of Twiss parameters in BEAM line!";
								ShowError(S);
								return ERR_BEAM;
							}
							else{
								BeamPar.XTwiss.alpha=Lines[k].S[1].ToDouble();
								BeamPar.XTwiss.beta=Lines[k].S[2].ToDouble();
								BeamPar.XTwiss.epsilon=Lines[k].S[3].ToDouble();
								BeamPar.YTwiss.alpha=Lines[k].S[4].ToDouble();
								BeamPar.YTwiss.beta=Lines[k].S[5].ToDouble();
								BeamPar.YTwiss.epsilon=Lines[k].S[6].ToDouble();
								Zpos=7;
								F+=AddLines(&Lines[k],0,6);
								//F="BEAM "+Lines[k].S[0]+" \t"+Lines[k].S[1]+" \t"+Lines[k].S[2]+" \t"+Lines[k].S[3]+" \t"+Lines[k].S[4]+" \t"+Lines[k].S[5]+" \t"+Lines[k].S[6]+" \t";
							}
							break;
						}
						case SPH_2D: {
							if (Nr>3){
								S="Too many Spherical parameters in BEAM line!";
								ShowError(S);
								return ERR_BEAM;
							}
							else{
								BeamPar.Sph.Rcath=Lines[k].S[1].ToDouble()/100; //Rcath
								F+=AddLines(&Lines[k],0,1);
								//F="BEAM "+Lines[k].S[0]+" \t"+Lines[k].S[1];
							   //	Zpos=2;
								if (Nr>1) {
									BeamPar.Sph.Rsph=Lines[k].S[2].ToDouble()/100;  //Rsph
									F+=" \t"+Lines[k].S[1];
									//Zpos=3;
									if (Nr>2){
										BeamPar.Sph.kT=Lines[k].S[3].ToDouble(); //kT
										F+=" \t"+Lines[k].S[1];
									 //	Zpos=4;
									 } else
										BeamPar.Sph.kT=0;
								} else {
									 BeamPar.Sph.Rsph=0;
									 BeamPar.Sph.kT=0;
								}
							   //	BeamPar.YNorm=BeamPar.XNorm;
							}
							break;
						}
						case ELL_2D: {
							if (Nr>4){
								S="Too many Elliptical parameters in BEAM line!";
								ShowError(S);
								return ERR_BEAM;
							}
							else{
								BeamPar.Ell.ax=Lines[k].S[1].ToDouble()/100; //x
								F+=AddLines(&Lines[k],0,1);
								if (Nr>1) {
									BeamPar.Ell.by=Lines[k].S[2].ToDouble()/100; //y
									F+=" \t"+Lines[k].S[2];
									if (Nr>2){
										BeamPar.Ell.phi=DegreeToRad(Lines[k].S[3].ToDouble()); //phi
										F+=" \t"+Lines[k].S[3];
										if (Nr>3){
											BeamPar.Ell.h=Lines[k].S[4].ToDouble(); //h
											F+=" \t"+Lines[k].S[4];
										} else
											BeamPar.Ell.h=1e-6;
									} else {
										BeamPar.Ell.phi=0;
										BeamPar.Ell.h=1e-6;
									}
								} else {
									BeamPar.Ell.by=BeamPar.Ell.ax; //y
									BeamPar.Ell.phi=0;
									BeamPar.Ell.h=1e-6;
								}
							}
							break;
						}
						default: {
							S="Unexpected BEAM Transversal Distribution Type";
							ShowError(S);
							return ERR_BEAM;
						}
					}
				   /*	if (Lines[k].N < Zpos+1)
						return ERR_BEAM;    */

					KeyWord=Lines[k].S[Zpos];
					BeamPar.ZBeamType=ParseDistribution(KeyWord);

					if (IsLongitudinalKeyWord(BeamPar.ZBeamType)) {
						switch (BeamPar.ZBeamType) {
							case FILE_1D: {
								if (Nz != 3 && Nz != 4){
									S="Wrong number of Import File (Z) or Phase Distribution parameters in BEAM line!";
									ShowError(S);
									return ERR_BEAM;
								}
								AnsiString FileName=Lines[k].S[Zpos+1];
								if (CheckFile(FileName)) {
									BeamPar.ZFile=FileName;
									F+=AddLines(&Lines[k],Zpos,Zpos+1);
								   //	F+=KeyWord+" \t"+FileName+" \t";
							   //	BeamDefined=true;
								} else {
									S="ERROR: The file "+FileName+" is missing!";
									ShowError(S);
									return ERR_BEAM;
								}
								BeamPar.ZNorm.mean=DegreeToRad(Lines[k].S[Zpos+2].ToDouble());
								BeamPar.ZNorm.limit=DegreeToRad(Lines[k].S[Zpos+3].ToDouble());
								F+=AddLines(&Lines[k],Zpos+2,Zpos+3);
								//F+=Lines[k].S[Zpos+2]+" \t"+Lines[k].S[Zpos+3];
								if (Nz == 3) {
									BeamPar.ZNorm.sigma=100*BeamPar.ZNorm.limit;
								} else  if (Nz == 4) {
									BeamPar.ZNorm.sigma=DegreeToRad(Lines[k].S[Zpos+4].ToDouble());
									F+=Lines[k].S[Zpos+4];
								}
                                break;
                            }
							case FILE_2D: {
								if (Nz != 1){
									S="Too many Import File (Z) parameters in BEAM line!";
									ShowError(S);
									return ERR_BEAM;
								}

								AnsiString FileName=Lines[k].S[Zpos+1];
								if (CheckFile(FileName)) {
									BeamPar.ZFile=FileName;
									F+=AddLines(&Lines[k],Zpos,Zpos+1);
								   //	F+=KeyWord+" \t"+FileName;
							   //	BeamDefined=true;
								} else {
									S="ERROR: The file "+FileName+" is missing!";
									ShowError(S);
									return ERR_BEAM;
								}
								break;
							}
							case NORM_2D: {
								if (Nz != 4 && Nz != 6){
									S="Wrong number of Longitudinal Distribution parameters in BEAM line!";
									ShowError(S);
									return ERR_BEAM;
								}else{
									if (Nz == 4) {
										BeamPar.WNorm.mean=Lines[k].S[Zpos+1].ToDouble();
										BeamPar.WNorm.limit=Lines[k].S[Zpos+2].ToDouble();
										BeamPar.WNorm.sigma=100*BeamPar.WNorm.limit;
										BeamPar.ZNorm.mean=DegreeToRad(Lines[k].S[Zpos+3].ToDouble());
										BeamPar.ZNorm.limit=DegreeToRad(Lines[k].S[Zpos+4].ToDouble());
										BeamPar.ZNorm.sigma=100*BeamPar.ZNorm.limit;
										F+=AddLines(&Lines[k],Zpos,Zpos+4);
									   //	F+=Lines[k].S[Zpos]+" \t"+Lines[k].S[Zpos+1]+" \t"+Lines[k].S[Zpos+2]+" \t"+Lines[k].S[Zpos+3]+" \t"+Lines[k].S[Zpos+4];

									} else  if (Nz == 6) {
										BeamPar.WNorm.mean=Lines[k].S[Zpos+1].ToDouble();
										BeamPar.WNorm.limit=Lines[k].S[Zpos+2].ToDouble();
										BeamPar.WNorm.sigma=Lines[k].S[Zpos+3].ToDouble();
										BeamPar.ZNorm.mean=DegreeToRad(Lines[k].S[Zpos+4].ToDouble());
										BeamPar.ZNorm.limit=DegreeToRad(Lines[k].S[Zpos+5].ToDouble());
										BeamPar.ZNorm.sigma=DegreeToRad(Lines[k].S[Zpos+6].ToDouble());
										F+=AddLines(&Lines[k],Zpos,Zpos+6);
									   //	F+=Lines[k].S[Zpos]+" \t"+Lines[k].S[Zpos+1]+" \t"+Lines[k].S[Zpos+2]+" \t"+Lines[k].S[Zpos+3]+" \t"+Lines[k].S[Zpos+4]+" \t"+Lines[k].S[Zpos+5]+" \t"+Lines[k].S[Zpos+6];
									}
								}
								break;
							}
							default: {
								S="Unexpected BEAM Longitudinal Distribution Type";
								ShowError(S);
								return ERR_BEAM;
							}
						}
					}
					else{
						S="Unexpected BEAM Longitudinal Distribution Type";
						ShowError(S);
						return ERR_BEAM;
					}
                    BeamDefined=true;
					ParsedStrings->Add(F);
				}  else  {
					S="Wrong Format of BEAM line";
					ShowError(S);
					return ERR_BEAM;

				}

			// OLD CODE
			  /*  if (Lines[k].N < 4 && Lines[k].N > 6){
					return ERR_BEAM;
				}
				Phi0=Lines[k].S[0].ToDouble();
				dPhi=Lines[k].S[1].ToDouble();
				Phi_Eq=(Lines[k].S[2]!="NORM");
                if (Lines[k].N == 6){
				    W0=Lines[k].S[3].ToDouble();
                    dW=Lines[k].S[4].ToDouble();
                    W_Eq=(Lines[k].S[5]!="NORM");
					NpEnergy=0;
					F="BEAM "+Lines[k].S[0]+" \t"+Lines[k].S[1]+" \t"+Lines[k].S[2]+" \t"+Lines[k].S[3]+" \t"+Lines[k].S[4]+" \t"+Lines[k].S[5];
				    ParsedStrings->Add(F);
				    BeamDefined=true;
				    break;
				} else {
					if (CheckFile(Lines[k].S[3])) {
 					    EnergyFile=Lines[k].S[3];
                        F="BEAM "+Lines[k].S[0]+" \t"+Lines[k].S[1]+" \t"+Lines[k].S[2]+" \t"+Lines[k].S[3];
						NpEnergy=NumPointsInFile(EnergyFile,2);
                        if (Lines[k].N == 5){
					        NpEnergy=Lines[k].S[4].ToInt();
							F += " \t"+Lines[k].S[4];
						}
						ParsedStrings->Add(F);
						BeamDefined=true;
						break;
					} else {
						S="ERROR: Format of the file "+Lines[k].S[1]+" is wrong!";
						ShowError(S);
						return ERR_BEAM;
					}
				}       */
				break;
			}

			case CURRENT:{
				if (Lines[k].N<1 || Lines[k].N>2){
					return ERR_CURRENT;
				}  else {
					BeamPar.Current=Lines[k].S[0].ToDouble();
					F="CURRENT "+Lines[k].S[0];

					if (!IsFileKeyWord(BeamPar.RBeamType) && !IsFileKeyWord(BeamPar.ZBeamType)) {
						if (Lines[k].N!=2){
							S="ERROR: Number of particles is not defined!";
							ShowError(S);
							return ERR_CURRENT;
						} else {
							BeamPar.NParticles=Lines[k].S[1].ToInt();
							F+=" "+Lines[k].S[1];
							CurrentDefined=true;
						}
					} else {
						int Nr=0;
						int Ny=0;
						int Nz=0;
						int Np=0;
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
								/*	S="ERROR: Unexpected BEAM Transversal Distribution File Format!";
									ShowError(S);
									return ERR_CURRENT;  */
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


						} //check Np mismatch
						if (IsFileKeyWord(BeamPar.RBeamType) && IsFileKeyWord(BeamPar.ZBeamType)){
							if (Nr!=Nz) {
								S="ERROR: The number of imported particles from Longintudinal Beam File doesn't match with the number of particles imported from Transversal Beam File!";
								ShowError(S);
								return ERR_IMPORT;
							}
						}
						if (Lines[k].N==2) {
							Np=Lines[k].S[1].ToInt();
							//check Np overrride
							if (Np>Nr) {
								S="WARNING: The number of defined particles exceeds the number of imported particles, and will be ignored!";
								ShowError(S);
							}  else if (Np<Nr) {
								S="WARNING: The number of defined particles is less than the number of imported particles! Only the first "+Lines[k].S[1]+" particles will be simulated";
								ShowError(S);
								Nr=Np;
							}
							F+=" "+Lines[k].S[1];
						}
						BeamPar.NParticles=Nr;
					}
					ParsedStrings->Add(F);
					CurrentDefined=true;
                }

			// OLD CODE
			 /*	if (Lines[k].N!=5 && Lines[k].N!=2 && Lines[k].N!=3){
                    return ERR_CURRENT;
                }
				I0=Lines[k].S[0].ToDouble();
				F="CURRENT "+Lines[k].S[0]+"\t"+Lines[k].S[1]+"\t";//+Lines[k].S[2]+"\t";
				if (Lines[k].N==5 ){
					Np=Lines[k].S[1].ToInt();
					AlphaCS=Lines[k].S[2].ToDouble();
					BettaCS=Lines[k].S[3].ToDouble();
					EmittanceCS=Lines[k].S[4].ToDouble();
					F+=Lines[k].S[2]+"\t"+Lines[k].S[3]+" \t"+Lines[k].S[4];
				} else if (Lines[k].N==2){
					if (Lines[k].S[1]=="CST_X" || Lines[k].S[1]=="CST")
						BeamFile=CST_FileX;
					else
						BeamFile=Lines[k].S[1];;

					if (CheckFile(BeamFile))
						BeamType=CST_X;
					else{
						S="ERROR: The file "+BeamFile+" is missing!";
						ShowError(S);
						return ERR_CURRENT;
					}
					BeamType=CST_X;
					NpFromFile=true;
					Np=NumPointsInFile(BeamFile,2);
				} else if (Lines[k].N==3){
					if (Lines[k].S[1]=="CST_X" || Lines[k].S[1]=="CST")
						BeamFile=CST_FileX;
					else
						BeamFile=Lines[k].S[1];

					if (CheckFile(BeamFile))
						BeamType=CST_X;
					else{
						S="ERROR: The file "+BeamFile+" is missing!";
						ShowError(S);
						return ERR_CURRENT;
					}
					BeamType=CST_X;
					Np=Lines[k].S[2].ToInt();
					NpFromFile=false;
				}
				if (BeamType!=RANDOM)
					F="CURRENT "+Lines[k].S[0]+" \t"+Lines[k].S[1];
				if (!NpFromFile)
					F+=" \t"+Lines[k].S[2];
				ParsedStrings->Add(F);
				CurrentDefined=true;   */
				break;
			}

            case CELL:{
				if(OnlyParameters)
                    break;
				if (Lines[k].N==3 || Lines[k].N==5){
					Cells[Ni].Mode=Lines[k].S[0].ToDouble();
                    Cells[Ni].betta=Lines[k].S[1].ToDouble();
					Cells[Ni].ELP=Lines[k].S[2].ToDouble();
					Cells[Ni].Mesh=Nmesh;

					if (Lines[k].N==3){
						GetDimensions(Cells[Ni]);
					} else if (Lines[k].N==5){
						Cells[Ni].AL32=Lines[k].S[3].ToDouble();
						Cells[Ni].AkL=Lines[k].S[4].ToDouble();
					}
				} else{
                    return ERR_CELL;
                }

                F="CELL "+Lines[k].S[0]+"\t"+s.FormatFloat("#0.000",Cells[Ni].betta)+"\t"+s.FormatFloat("#0.000",Cells[Ni].ELP)+"\t"+s.FormatFloat("#0.000000",Cells[Ni].AL32)+"\t"+s.FormatFloat("#0.000000",Cells[Ni].AkL);
                ParsedStrings->Add(F);

                Cells[Ni].F0=F0*1e6;
                Cells[Ni].P0=P0*1e6;
                Cells[Ni].dF=arc(dF);
			   //	Cells[Ni].Drift=false;
				Cells[Ni].First=NewCell;
				if (NewCell && !PowerDefined) {
					S="ERROR: The RF power source must be defined before each RF section. Put POWER line in correct format into the input file before the end of each RF section. Note that DRIFT element terminates the defined RF power!";
					ShowError(S);
					Error=ERR_FORMAT;
				}
				NewCell=false;
				dF=0;

				Ni++;
				if (Nlim>-1 &&Ni>=Nlim)
					OnlyParameters=true;

				CellDefined=true;		
                break;
            }

            case CELLS:{
                if(OnlyParameters)
                    break;
				if (Lines[k].N==4 || Lines[k].N==6){
					for (int j=0;j<Lines[k].S[0].ToInt();j++){
                        Cells[Ni].Mode=Lines[k].S[1].ToDouble();
                        Cells[Ni].betta=Lines[k].S[2].ToDouble();
						Cells[Ni].ELP=Lines[k].S[3].ToDouble();
						Cells[Ni].Mesh=Nmesh;
						if (Lines[k].N==4)
							GetDimensions(Cells[Ni]);
						else if (Lines[k].N==6){
							Cells[Ni].AL32=Lines[k].S[4].ToDouble();
							Cells[Ni].AkL=Lines[k].S[5].ToDouble();
						}
                        Cells[Ni].F0=F0*1e6;
						Cells[Ni].P0=P0*1e6;
                        Cells[Ni].dF=arc(dF);
					 //   Cells[Ni].Drift=false;
						Cells[Ni].First=NewCell;
						if (NewCell && !PowerDefined) {
							S="ERROR: The RF power source must be defined before each RF section. Put POWER line in correct format into the input file before the end of each RF section. Note that DRIFT element terminates the defined RF power!";
							ShowError(S);
							Error=ERR_FORMAT;
						}
						NewCell=false;
						dF=0;

                        Ni++;
                        if (Nlim>-1 &&Ni>=Nlim){
                            OnlyParameters=true;
							break;
                        }
                    }
				} else{
					return ERR_CELLS;
				}
				if (Ni>0){
					F="CELLS "+Lines[k].S[0]+"\t"+Lines[k].S[1]+"\t"+s.FormatFloat("#0.000",Cells[Ni-1].betta)+"\t"+s.FormatFloat("#0.000",Cells[Ni-1].ELP)+"\t"+s.FormatFloat("#0.000000",Cells[Ni-1].AL32)+"\t"+s.FormatFloat("#0.000000",Cells[Ni-1].AkL);
					ParsedStrings->Add(F);
				}

				CellDefined=true;
                break;
            }
            case DRIFT:{
				if(OnlyParameters)
					break;
				if (Lines[k].N<2 || Lines[k].N>3){
					return ERR_DRIFT;
				}
				Cells[Ni].Drift=true;
				Cells[Ni].betta=Lines[k].S[0].ToDouble()/100;//D, cm
				Cells[Ni].AkL=Lines[k].S[1].ToDouble()/100;//Ra, cm
				if (Lines[k].N==3){
					double m=Lines[k].S[2].ToDouble();
					if (m>1)
						Cells[Ni].Mesh=m;
					else{
						S="WARNING: Number of mesh points must be more than 1";
						ShowError(S);
						Cells[Ni].Mesh=Nmesh;
                    }
				}
				else
					Cells[Ni].Mesh=Nmesh;
				Cells[Ni].ELP=0;
				Cells[Ni].AL32=0;
				Cells[Ni].First=true;
				Cells[Ni].F0=c;
				Cells[Ni].dF=arc(dF);
				dF=0;
				NewCell=true;
				PowerDefined=false;

				F="DRIFT "+Lines[k].S[0]+" \t"+Lines[k].S[1];
				ParsedStrings->Add(F);

				Ni++;
				if (Nlim>-1 && Ni>=Nlim)
					OnlyParameters=true;
				break;
			}
			case POWER:{
				if (Lines[k].N>=2 && Lines[k].N<=3){
					P0=Lines[k].S[0].ToDouble();
					F0=Lines[k].S[1].ToDouble();
					if(Lines[k].N==3)
						dF=Lines[k].S[2].ToDouble();
					NewCell=true;
				} else {
					return ERR_COUPLER;
				}

				F="POWER "+Lines[k].S[0]+"\t"+Lines[k].S[1]+"\t"+s.FormatFloat("#0.00",dF);
				ParsedStrings->Add(F);

				PowerDefined=true;
				break;
			}
			case DUMP:{
				if (Lines[k].N>=1 && Lines[k].N<=9){
					Cells[Ni].Dump=true;
					Cells[Ni].DumpParameters.File=Lines[k].S[0].c_str();
					Cells[Ni].DumpParameters.LiveOnly=true;
					Cells[Ni].DumpParameters.Phase=false;
					Cells[Ni].DumpParameters.Energy=false;
					Cells[Ni].DumpParameters.Radius=false;
					Cells[Ni].DumpParameters.Azimuth=false;
					Cells[Ni].DumpParameters.Vx=false;
					bool DefaultSet=true;
					int Nkey=1; // the word from which the flags are checked

					F="SAVE "+Lines[k].S[0];
					if (Lines[k].N>=2 && IsNumber(Lines[k].S[1])) {
						Cells[Ni].DumpParameters.N1=Lines[k].S[1].ToDouble();
						F=F+" "+Lines[k].S[1];
						Nkey=2;
						if (Lines[k].N>=3 && IsNumber(Lines[k].S[2])){
							Cells[Ni].DumpParameters.N2=Lines[k].S[2].ToDouble();
							F=F+" "+Lines[k].S[2];
							Nkey=3;
						}
						else
							Cells[Ni].DumpParameters.N2=0;
					}  else {
						Cells[Ni].DumpParameters.N1=0;
						Cells[Ni].DumpParameters.N2=0;
					}
					if (Lines[k].N>=2 && Lines[k].N<=9) {
						for (int j=Nkey;j<Lines[k].N;j++){
							if (Lines[k].S[j]=="LOST") {
								Cells[Ni].DumpParameters.LiveOnly=false;
								F=F+" LOST";
							}
							else if (Lines[k].S[j]=="PHASE") {
								Cells[Ni].DumpParameters.Phase=true;
								F=F+" PHASE";
								DefaultSet=false;
							}
							else if (Lines[k].S[j]=="ENERGY") {
								Cells[Ni].DumpParameters.Energy=true;
								F=F+" ENERGY";
								DefaultSet=false;
							}
							else if (Lines[k].S[j]=="RADIUS") {
								Cells[Ni].DumpParameters.Radius=true;
								F=F+" RADIUS";
								DefaultSet=false;
							}
							else if (Lines[k].S[j]=="AZIMUTH") {
								Cells[Ni].DumpParameters.Azimuth=true;
								F=F+" AZIMUTH";
								DefaultSet=false;
							}
							else if (Lines[k].S[j]=="VX") {
								Cells[Ni].DumpParameters.Vx=true;
								F=F+" VX";
								DefaultSet=false;
							} else {
								S="WARNING: The keyword "+Lines[k].S[j]+" in SAVE line is not recognized and will be ignored";
								ShowError(S);
							}
						}
					}
					if (DefaultSet) {
						Cells[Ni].DumpParameters.Phase=true;
						Cells[Ni].DumpParameters.Energy=true;
						Cells[Ni].DumpParameters.Radius=true;
						Cells[Ni].DumpParameters.Azimuth=true;
						Cells[Ni].DumpParameters.Vx=true;
					}

					//check the numbers
					if (Cells[Ni].DumpParameters.N1<0 || Cells[Ni].DumpParameters.N2<0) {
						S="WARNING: The particle numbers in SAVE line can not be negative. The region will be ignored.";
						ShowError(S);
						Cells[Ni].DumpParameters.N1=0;
						Cells[Ni].DumpParameters.N2=0;
					} else if (Cells[Ni].DumpParameters.N2!=0 && Cells[Ni].DumpParameters.N1>Cells[Ni].DumpParameters.N2) {
						int temp=Cells[Ni].DumpParameters.N2; //switch the numbers
						Cells[Ni].DumpParameters.N2=Cells[Ni].DumpParameters.N1;
						Cells[Ni].DumpParameters.N1=temp;
					}

					ParsedStrings->Add(F);
				} else {    // else skip the line
					S="WARNING: The format of SAVE line is wrong and will be ignored.";
					ShowError(S);
				}
				break;
			}
			case COMMENT:{
				ParsedStrings->Add(Lines[k].S[0]);
				break;
			}

		}
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
TError TBeamSolver::LoadData(int Nl)
{
    const char *FileName=InputFile.c_str();
    LoadIniConstants();
    InputStrings->Clear();
	Nlim=Nl; //# of cells limit. Consider removing

    DataReady=false;

    if (strncmp(FileName, "", 1) == 0) {
        return ERR_NOFILE;
    }
    if (!FileExists(FileName)) {
        return ERR_OPENFILE;
    }

    delete[] Cells;

    AnsiString S;
    TInputLine *Lines;
    int i=-1,j=0,N=0;
    TError Parsed;

    Lines=ParseFile(N);
    //InputStrings->LoadFromFile(InputFile);

	Ncells=0; 
	for (int k=0;k<N;k++){
		if (Lines[k].P==CELL || Lines[k].P==DRIFT)
            Ncells++;
        else if (Lines[k].P==CELLS)
			Ncells+=Lines[k].S[0].ToInt();
	}

	if (Nlim>-1 && Ncells>=Nlim){  //# of cells limit. Consider removing
		Ncells=Nlim;
		S="The number of elements exceeds the user-defined limit. Check the hellweg.ini file";
		ShowError(S);
	}
	Cells = new TCell[Ncells+1];  //+1 - end point
	for (int k=0;k<Ncells+1;k++){
		Cells[k].Dump=false;
		Cells[k].Drift=false;   
		Cells[k].First=false; 
	}

	Parsed=ParseLines(Lines,N);
    ParsedStrings->Add("END");
    InputStrings->AddStrings(ParsedStrings);
    
    ParsedStrings->SaveToFile("PARSED.TXT");

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

    delete[] Cells;

    AnsiString F,s;
    TInputLine *Lines;
    int i=-1,j=0,N=0;

    Lines=ParseFile(N);
    Error=ParseLines(Lines,N,true);

	if (F0*1e6!=iCell.F0 || P0*1e6!=iCell.P0){
        F0=iCell.F0*1e-6;
        P0=iCell.P0*1e-6;
        F="POWER "+s.FormatFloat("#0.00",P0)+"\t"+s.FormatFloat("#0.00",F0);
        ParsedStrings->Add(F);
    }

    ParsedStrings->Add("");

    double k1=0,k2=0,k3=0,k4=0,k5=0;
    double Am=iCell.ELP*sqrt(P0*1e6)/We0;
    k1=3.8e-3*(Power(10.8,Am)-1);
    k2=1.25*Am+2.25;
    k3=0.5*Am+0.15*sqrt(Am);
    k4=0.5*Am-0.15*sqrt(Am);
    k5=1/sqrt(1.25*sqrt(Am));

	double b=0,A=0,ksi=0,lmb=0,th=0;
	double W0=Beam[0]->GetAverageEnergy();
	double b0=MeVToVelocity(W0);

    lmb=1e-6*c/F0;
    if (iCell.Mode==90)
        th=pi/2;
    else if (iCell.Mode==120)
        th=2*pi/3;

    Ncells=0;
    int iB=0;

    do {
        b=(2/pi)*(1-b0)*arctg(k1*Power(ksi,k2))+b0;
       //   b=(2/pi)*arctg(0.25*sqr(10*ksi*lmb)+0.713);
        iB=10000*b;
        ksi+=b*th/(2*pi);
        Ncells++;
    } while (iB<9990);
    //} while (ksi*lmb<1.25);

    Cells=new TCell[Ncells];

    ksi=0;
    for (int i=0;i<Ncells;i++){
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

        Cells[i].Mode=iCell.Mode;
        Cells[i].betta=(2/pi)*(1-b0)*arctg(k1*Power(ksi,k2))+b0;
        //Cells[i].betta=(2/pi)*arctg(0.25*sqr(10*ksi*lmb)+0.713);

        Cells[i].ELP=A*We0/sqrt(1e6*P0);
       //   Cells[i].ELP=A*lmb/sqrt(1e6*P0);

        GetDimensions(Cells[i]);

        F="CELL "+s.FormatFloat("#0",iCell.Mode)+"\t"+s.FormatFloat("#0.000",Cells[i].betta)+"\t"+s.FormatFloat("#0.000",Cells[i].ELP)+"\t"+s.FormatFloat("#0.000000",Cells[i].AL32)+"\t"+s.FormatFloat("#0.000000",Cells[i].AkL);
        ParsedStrings->Add(F);

        Cells[i].F0=F0*1e6;
        Cells[i].P0=P0*1e6;
        Cells[i].dF=0;
        Cells[i].Drift=false;
        Cells[i].First=false;
    }
    Cells[0].First=true;

    ParsedStrings->Add("END");
    InputStrings->AddStrings(ParsedStrings);
    ParsedStrings->SaveToFile("PARSED.TXT");

    return Error;
}
//---------------------------------------------------------------------------
int TBeamSolver::ChangeCells(int N)
{
    TCell *iCells;
    iCells=new TCell[Ncells];

    for (int i=0;i<Ncells;i++)
        iCells[i]=Cells[i];

    delete[] Cells;

    int Nnew=Ncells<N?Ncells:N;
    int Nprev=Ncells;
    Ncells=N;

    Cells=new TCell[Ncells];
    
    for (int i=0;i<Nnew;i++)
        Cells[i]=iCells[i];

    delete[] iCells;
    
    return Nprev;
}
//---------------------------------------------------------------------------
void TBeamSolver::AppendCells(TCell& iCell,int N)
{
    int Nprev=ChangeCells(Ncells+N);
    int i=InputStrings->Count-1;
    InputStrings->Delete(i);

    i=Nprev;
    Cells[i]=iCell;
    GetDimensions(Cells[i]);

    for (int j=Nprev;j<Ncells;j++)
        Cells[j]=Cells[i];

    AnsiString F,F1,s;
    F1=F=s.FormatFloat("#0",iCell.Mode)+"\t"+s.FormatFloat("#0.000",Cells[i].betta)+"\t"+s.FormatFloat("#0.000",Cells[i].ELP)+"\t"+s.FormatFloat("#0.000000",Cells[i].AL32)+"\t"+s.FormatFloat("#0.000000",Cells[i].AkL);
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
    TCell pCell=Cells[Ncells-1];
    AppendCells(pCell,N);
}
//---------------------------------------------------------------------------
TCell TBeamSolver::GetCell(int j)
{
    if (j<0)
        j=0;
    if (j>=Ncells)
        j=Ncells-1;

    return Cells[j];
}
//---------------------------------------------------------------------------
TCell TBeamSolver::LastCell()
{
    return GetCell(Ncells-1);
}
//---------------------------------------------------------------------------
void TBeamSolver::ChangeInputCurrent(double Ib)
{
    BeamPar.Current=Ib;
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
    y=new double[Nint];

    Spline=new TSpline;
    Spline->MakeLinearSpline(X,Y,Nbase);
    y=Spline->Interpolate(x,Nint);

    delete Spline;

    return y;
}
//---------------------------------------------------------------------------
int TBeamSolver::CreateGeometry()
{
    F0*=1e6;
    P0*=1e6;
    double theta=0;
    double *X_base,*B_base,*E_base,*Al_base,*A_base;
	double *X_int,*B_int,*E_int,*Al_int,*A_int;
	AnsiString S;

    bool Solenoid_success=false;

	int Njmp=0,k0=0;

	Npoints=0;
	for(int i=0;i<Ncells;i++){
		Npoints+=Cells[i].Mesh;
		if (Cells[i].First){
			Npoints++;
			Njmp++;
		}
    }

   //	Npoints=Ncells*Nmesh+Njmp;//add+1 for the last point;
	TSplineType Spl;

    X_base = new double[Ncells]; X_int=new double[Npoints];
    B_base = new double[Ncells]; B_int=new double[Npoints];
    E_base = new double[Ncells]; E_int=new double[Npoints];
    Al_base = new double[Ncells]; Al_int=new double[Npoints];

    delete[] Structure;

    double z=0,zm=0,D=0,x=0;

    Structure=new TStructure[Npoints];

    int k=0;
	for (int i=0;i<Ncells;i++){
        int Extra=0;

        if (i==Ncells-1)
            Extra=1;
        else if (Cells[i+1].First)
            Extra=1;

        if (Cells[i].First)
            z-=zm;

        double lmb=1;
        if (Cells[i].Drift){
            D=Cells[i].betta;
            bool isInput=false;
            for (int j=i;j<Ncells;j++){
                if (!Cells[j].Drift){
                    Cells[i].betta=Cells[j].betta;
                    lmb=c/Cells[j].F0;
                    isInput=true;
                    break;
                }
            }
            if (!isInput){
                for (int j=i;j>=0;j--){
                    if (!Cells[j].Drift){
                        Cells[i].betta=Cells[j].betta;
                        lmb=c/Cells[j].F0;
                        isInput=true;
                        break;
                    }
                }
            }
            if (!isInput){
                Cells[i].betta=1;
                lmb=1;
            }
        }else{
            lmb=c/Cells[i].F0;
            theta=Cells[i].Mode*pi/180;
			D=Cells[i].betta*lmb*theta/(2*pi);
        }
        x+=D/2;
        X_base[i]=x/lmb;
		x+=D/2;
		B_base[i]=Cells[i].betta;
        E_base[i]=Cells[i].ELP;
        Al_base[i]=Cells[i].AL32;
		//zm=D/Nmesh;
		zm=D/Cells[i].Mesh;
		k0=k;
        Structure[k].dF=Cells[i].dF;
		//for (int j=0;j<Nmesh+Extra;j++){
		for (int j=0;j<Cells[i].Mesh+Extra;j++){
            X_int[k]=z/lmb;
            Structure[k].ksi=z/lmb;
            Structure[k].lmb=lmb;
            Structure[k].P=Cells[i].P0;
            Structure[k].dF=0;
            Structure[k].drift=Cells[i].Drift;
			if (Cells[i].Drift)
                Structure[k].Ra=Cells[i].AkL/lmb;
            else
                Structure[k].Ra=Cells[i].AkL;//*lmb;
            Structure[k].jump=false;
			Structure[k].CellNumber=i;
			Structure[k].Dump=false;
			z+=zm;
            k++;
        }
		if (Cells[i].First)
			Structure[k0].jump=true;
		if (Cells[i].Dump){ 
			Structure[k0].Dump=true;
			Structure[k0].DumpParameters=Cells[i].DumpParameters;
		  //	strcpy(Structure[k0].DumpParameters.File, Cells[i].DumpParameters.File);
		} 
	}
	if (Cells[Ncells].Dump){
		Structure[k-1].Dump=true;  //was k0+Nmesh
		Structure[k-1].DumpParameters=Cells[Ncells].DumpParameters; //was k0+Nmesh
	}

	//   int Njmp=0;
//  Structure[0].jump=true;

    double *Xo=NULL, *Bo=NULL, *Eo=NULL, *Ao=NULL;
	double *Xi=NULL, *Bi=NULL, *Ei=NULL, *Ai=NULL;
    int Ncls=0;
    int Npts=0;

    Njmp=0;
    int iJmp=0;

    bool EndOfBlock=false;

  /*    FILE *F;
    F=fopen("cells.log","w");*/

    for (int i=0;i<=Ncells;i++){
        if (i==Ncells)
            EndOfBlock=true;
        else if (Cells[i].First && i!=0)
            EndOfBlock=true;
        else
            EndOfBlock=false;

        if (EndOfBlock/*Cells[i].First && i!=0 || i==Ncells*/){
			Ncls=i-Njmp;
			Npts=0;
			for (int j=Njmp;j<i;j++) {
				Npts+=Cells[j].Mesh;
			}
			Npts++;
			//int Npts0=Ncls*Nmesh+1;

			/*if (i!=Ncells)
				Structure[i*Nmesh].jump=true;*/

            Xo=new double[Ncls];
            Bo=new double[Ncls];
            Eo=new double[Ncls];
            Ao=new double[Ncls];
            Xi=new double[Npts];

			for (int j=0;j<Ncls;j++){
                Xo[j]=X_base[Njmp+j];
                Bo[j]=B_base[Njmp+j];
                Eo[j]=E_base[Njmp+j];
                Ao[j]=Al_base[Njmp+j];
            }

			int pos=0;
			for (int j=0;j<Njmp;j++)
				pos+=Cells[j].Mesh;

			for (int j=0;j<Npts;j++)
				Xi[j]=X_int[/*Njmp*Nmesh*/pos+iJmp+j];

            Spl=(Ncls<4)?LSPLINE:SplineType;

            if (Ncls==1)
                Spl=ZSPLINE;

            switch (Spl) {
                case ZSPLINE:{
                    Bi=new double[Npts];
                    Ei=new double[Npts];
                    Ai=new double[Npts];
                    for (int j=0;j<Npts;j++){
                        Bi[j]=Bo[0];
                        Ei[j]=Eo[0];
                        Ai[j]=Ao[0];
                    }
                    break;
                }
                case(LSPLINE):{
                    Bi=LinearInterpolation(Xi,Xo,Bo,Ncls,Npts);
                    Ei=LinearInterpolation(Xi,Xo,Eo,Ncls,Npts);
                    Ai=LinearInterpolation(Xi,Xo,Ao,Ncls,Npts);
                    break;
                }
                case(CSPLINE):{
                    Bi=SplineInterpolation(Xi,Xo,Bo,Ncls,Npts);
                    Ei=SplineInterpolation(Xi,Xo,Eo,Ncls,Npts);
                    Ai=SplineInterpolation(Xi,Xo,Ao,Ncls,Npts);
                    break;
                }
                case(SSPLINE):{
                    Bi=SmoothInterpolation(Xi,Xo,Bo,Ncls,Npts,Smooth);
                    Ei=SmoothInterpolation(Xi,Xo,Eo,Ncls,Npts,Smooth);
                    Ai=SmoothInterpolation(Xi,Xo,Ao,Ncls,Npts,Smooth);
                    break;
                }
            }

            for (int j=0;j<Npts;j++){
				B_int[/*Njmp*Nmesh*/pos+iJmp+j]=Bi[j];
				E_int[/*Njmp*Nmesh*/pos+iJmp+j]=Ei[j];
				Al_int[/*Njmp*Nmesh*/pos+iJmp+j]=Ai[j];
			}


        /*  for (int i=0;i<Npts;i++){
                fprintf(F,"%i %f %f\n",Njmp*Nmesh+i,Xi[i],Ei[i]);
            }     */
            

            delete[] Xo;
            delete[] Xi;
            delete[] Bo;
            delete[] Bi;
			delete[] Eo;
			delete[] Ei;
            delete[] Ao;
			delete[] Ai;

			Njmp=i;
			iJmp++;
		}

	}
	// fclose(F);

	if (FSolenoid) {
		int NSol=0;
		double *Xz=NULL;
		double *Bz=NULL;

		NSol=GetSolenoidPoints();
		if(NSol<1){
            Zmag=0;
            B0=0;
            Lmag=0;
			FSolenoid=false;
			S="The format of "+SolenoidFile+" is wrong or too few points available";
			ShowError(S);
        } else {
            Xz=new double[NSol];
            Bz=new double[NSol];

			ReadSolenoid(NSol,Xz,Bz);
			if (NSol==1) {
                Zmag=0;
                Lmag=Structure[Npoints-1].ksi*Structure[Npoints-1].lmb;
                B0=Bz[0];
                FSolenoid=false;
                delete[] Xz;
                delete[] Bz;
            } else{
                Xi=new double[Npoints];
				for (int i=0; i<Npoints; i++)
                    Xi[i]=Structure[i].ksi*Structure[i].lmb;

                Bi=LinearInterpolation(Xi,Xz,Bz,NSol,Npoints);
                delete[] Xz;
                delete[] Bz;
                delete[] Xi;
            }
        }
    }

	for (int i=0;i<Npoints;i++){
        //int s=0;
        double lmb=1;
		lmb=Structure[i].lmb;
	   if (B_int[i]!=1)
			Structure[i].betta=B_int[i];
		else
            Structure[i].betta=MeVToVelocity(EnergyLimit);

        Structure[i].E=E_int[i];
        Structure[i].A=Structure[i].P>0?E_int[i]*sqrt(Structure[i].P)/We0:0;
/*      if (i==20)
            s=1;*/
        Structure[i].Rp=sqr(E_int[i])/2;;
        Structure[i].B=Structure[i].Rp/(2*We0);
        Structure[i].alpha=Al_int[i]/(lmb*sqrt(lmb));
        if (FSolenoid) {
            Structure[i].B_ext=Bi[i]/10000;
        } else {
            if (Structure[i].ksi>=Zmag/lmb && Structure[i].ksi<=(Zmag+Lmag)/lmb)
                Structure[i].B_ext=B0;
            else
                Structure[i].B_ext=0;
        }
    }

    if (FSolenoid) {
        delete[] Bi;
    }

    delete[] X_base; delete[] X_int;
    delete[] B_base; delete[] B_int;
    delete[] E_base; delete[] E_int;
    delete[] Al_base; delete[] Al_int;

    return ERR_NO;
}
//---------------------------------------------------------------------------
TError TBeamSolver::CreateBeam()
{
    double sx=0,sy=0,r=0;
	double b0=0,db=0;

	AnsiString S;

	//Npoints=Ncells*Nmesh;

	//Np=BeamPar.NParticles;

	//OLD CODE
   /* if (BeamType!=RANDOM){
	    if (NpFromFile)
		    Np=Beam[0]->CountCSTParticles(BeamFile);
        if(Np<0) {
            return ERR_CURRENT;
		}
    }

	if (NpEnergy != 0) {
	    if (NpEnergy != Np) { 
            return ERR_NUMBPARTICLE;
		}
	} */

   for (int i=0;i<Np_beam;i++){
        delete Beam[i];
    }
    delete[] Beam;

    Beam=new TBeam*[Npoints];
    for (int i=0;i<Npoints;i++){
		Beam[i]=new TBeam(BeamPar.NParticles);
        Beam[i]->SetBarsNumber(Nbars);
        Beam[i]->SetKernel(Kernel);
        Beam[i]->lmb=Structure[i].lmb;
        Beam[i]->I0=BeamPar.Current;
        Beam[i]->Reverse=Reverse;
        //Beam[i]->Cmag=c*Cmag/(Structure[i].lmb*We0); //Cmag = c*B*lmb/Wo * (1/lmb^2 from r normalization)
		for (int j=0;j<BeamPar.NParticles;j++){
            Beam[i]->Particle[j].lost=LIVE;
            Beam[i]->Particle[j].betta=0;
            Beam[i]->Particle[j].Bx=0;
            Beam[i]->Particle[j].phi=0;
            Beam[i]->Particle[j].Bth=0;
            Beam[i]->Particle[j].x=0;
            Beam[i]->Particle[j].x0=0;
            Beam[i]->Particle[j].Th=0;
        }
    }
	Beam[0]->Ib=BeamPar.Current;
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
		case CST_PIT:{
			if (!Beam[0]->BeamFromCST(&BeamPar))
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

   //OLD CODE
/*
	if (NpEnergy == 0) {
        if (W_Eq) {
            Beam[0]->MakeEquiprobableDistribution(W0,dW,BETTA_PAR);
        } else {
            Beam[0]->MakeGaussDistribution(W0,dW,BETTA_PAR);
        }
	} else {
		LoadEnergyFromFile(EnergyFile,NpEnergy);
	}

    for (int i=0;i<Np;i++) {
        Beam[0]->Particle[i].betta=MeVToVelocity(Beam[0]->Particle[i].betta);
    }

    if (Phi_Eq) { 
        Beam[0]->MakeEquiprobableDistribution(HellwegTypes::DegToRad(Phi0)-Structure[0].dF,HellwegTypes::DegToRad(dPhi),PHI_PAR);
    } else {
        Beam[0]->MakeGaussDistribution(HellwegTypes::DegToRad(Phi0)-Structure[0].dF,HellwegTypes::DegToRad(dPhi),PHI_PAR);
    }

	if (BeamType==RANDOM) {
		Beam[0]->MakeGaussEmittance(AlphaCS,BettaCS,EmittanceCS);
    } else {
		Beam[0]->ReadCSTEmittance(BeamFile,Np);
	}

   //	if (BeamType!=CST_Y) {  //not used yet
        Beam[0]->MakeEquiprobableDistribution(pi,pi,TH_PAR);
   //	}
    Beam[0]->MakeEquiprobableDistribution(0,0,BTH_PAR);

    for (int i=0;i<Npoints;i++){
        for (int j=0;j<Np;j++) {
            Beam[i]->Particle[j].x0=Beam[0]->Particle[j].x;
        }
	}    */
    
 /* for (int i=0;i<Np;i++){
        Beam[0]->Particle[i].x=0;//-0.001+0.002*i/(Np-1);
        //Beam[0]->Particle[i].phi=0;
        Beam[0]->Particle[i].Bx=0;
        Beam[0]->Particle[i].Th=0;
        Beam[0]->Particle[i].Bth=0;

      //    Beam[0]->Particle[i].phi=DegToRad(-90+i);
       //   Beam[0]->Particle[i].betta=MeVToVelocity(0.05);
    }           */
    return ERR_NO;
}
//---------------------------------------------------------------------------
//MOVED TO BEAM.CPP
/*bool TBeamSolver::LoadEnergyFromFile(AnsiString &F, int NpEnergy)
{
	std::ifstream fs(F.c_str());
	float enrg=0;
	int i=0, np=0;
	bool Success=false;
	AnsiString S,L;

	while (!fs.eof()){
		L=GetLine(fs);

		if (NumWords(L)==2){       //Check if only two numbers in the line
			try {                  //Check if the data is really a number
				S=ReadWord(L,1);
				np=S.ToInt();
				S=ReadWord(L,2);
				enrg=S.ToDouble();
			}
			catch (...){
				continue;          //Skip incorrect line
			}
			if (i==NpEnergy){     //if there is more data than expected
				i++;
				break;
			}
			Beam[0]->Particle[i].betta=enrg;
			i++;
		}
	}

	fs.close();

	Success=(i==NpEnergy);
	return Success;
}       */
//---------------------------------------------------------------------------
int TBeamSolver::GetSolenoidPoints()
{
	return NumPointsInFile(SolenoidFile,2);
}
//---------------------------------------------------------------------------
bool TBeamSolver::ReadSolenoid(int Nz,double *Z,double* B)
{
	std::ifstream fs(SolenoidFile.c_str());
	float z=0,Bz=0;
	int i=0;
	bool Success=false;
	AnsiString S,L;

	while (!fs.eof()){
		L=GetLine(fs);

		if (NumWords(L)==2){       //Check if only two numbers in the line
			try {                  //Check if the data is really a number
				S=ReadWord(L,1);
				z=S.ToDouble();
				S=ReadWord(L,2);
				Bz=S.ToDouble();
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
void TBeamSolver::GetEllipticParameters(int Nknot, double& x0,double& y0, double& a,double& b,double& phi,double &Rx,double& Ry)
{
    Beam[Nknot]->GetEllipticParameters(x0,y0,a,b,phi,Rx,Ry);
}
//---------------------------------------------------------------------------
void TBeamSolver::GetCourantSneider(int Nknot, double& alpha,double& betta, double& epsilon)
{
    Beam[Nknot]->GetCourantSneider(alpha,betta,epsilon);
}
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
//---------------------------------------------------------------------------
TSpectrumBar *TBeamSolver::GetEnergySpectrum(int Nknot,bool Env,double& Wav,double& dW)
{
    TSpectrumBar *Spectrum;
    Spectrum=Beam[Nknot]->GetEnergySpectrum(Env,Wav,dW);
    return Spectrum;
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeamSolver::GetPhaseSpectrum(int Nknot,bool Env,double& Fav,double& dF)
{
    TSpectrumBar *Spectrum;
    Spectrum=Beam[Nknot]->GetPhaseSpectrum(Env,Fav,dF);
    return Spectrum;
}
//---------------------------------------------------------------------------
void TBeamSolver::GetBeamParameters(int Nknot,double *X,TBeamParameter Par)
{
    Beam[Nknot]->GetParameters(X,Par);
}
//---------------------------------------------------------------------------
double TBeamSolver::GetKernel()
{
    return Beam[0]->h;
}
//---------------------------------------------------------------------------
void TBeamSolver::GetStructureParameters(double *X,TStructureParameter Par)
{
    switch (Par) {
        case (KSI_PAR):{
            for (int i=0;i<Npoints;i++)
                X[i]=Structure[i].ksi;
            break;
        }
        case (Z_PAR):{
            for (int i=0;i<Npoints;i++)
                X[i]=Structure[i].ksi*Structure[i].lmb;
            break;
        }
        case (A_PAR):{
            for (int i=0;i<Npoints;i++)
                X[i]=Structure[i].A;
            break;
        }
        case (RP_PAR):{
            for (int i=0;i<Npoints;i++)
                X[i]=Structure[i].Rp;
            break;
        }
        case (B_PAR):{
            for (int i=0;i<Npoints;i++)
                X[i]=Structure[i].A*We0/Structure[i].lmb;
            break;
        }
        case (ALPHA_PAR):{
            for (int i=0;i<Npoints;i++)
                X[i]=Structure[i].alpha;
            break;
        }
        case (BETTA_F_PAR):{
            for (int i=0;i<Npoints;i++)
                X[i]=Structure[i].betta;
            break;
        }
        case (RA_PAR):{
            for (int i=0;i<Npoints;i++)
                X[i]=Structure[i].Ra*Structure[i].lmb;
            break;
        }
        case (B_EXT_PAR):{
            for (int i=0;i<Npoints;i++)
                X[i]=Structure[i].B_ext;
            break;
        }
        case (NUM_PAR):{
            for (int i=0;i<Npoints;i++)
                X[i]=Structure[i].CellNumber;
            break;
        }
    }
}
//---------------------------------------------------------------------------
void TBeamSolver::Abort()
{
    Stop=true;
}

//---------------------------------------------------------------------------
void TBeamSolver::Integrate(int Si, int Sj)
{

    double Rb=0,Lb=0,Fb=0,gamma=1,Mr=0,phic=0,Icur=0;
	int component=0;
    TParticle *Particle=Beam[Si]->Particle;

    phic=Beam[Si]->iGetAveragePhase(Par[Sj],K[Sj]);
    Par[Sj].SumSin=0;
    Par[Sj].SumCos=0;
    Par[Sj].SumSin=Beam[Si]->SinSum(Par[Sj],K[Sj]);
    Par[Sj].SumCos=Beam[Si]->CosSum(Par[Sj],K[Sj]);

    gamma=Beam[Si]->iGetAverageEnergy(Par[Sj],K[Sj]);
    Par[Sj].gamma=gamma;

    Lb=Beam[Si]->iGetBeamLength(Par[Sj],K[Sj],Nslices,false)/2;
    Fb=Lb*2*pi/lmb;

    Rb=Beam[Si]->iGetBeamRadius(Par[Sj],K[Sj],false);
    
	Icur=I;//*Lb/lmb;

    if (Rb==0)
        Mr=0;
    else
        Mr=FormFactor(gamma*Lb/Rb);

    Par[Sj].Bz_ext*=lmb*c/We0;
   //   Par[Sj].Bz_ext*=lmb/(myu0*We0);
    Par[Sj].Br_ext=0;
    if (Magnetized)
        Par[Sj].Cmag=Structure[0].B_ext*lmb*c/We0;
    else
        Par[Sj].Cmag=0;

    double phi=0,r=0;
    double Aqz=0,Aqr=0;

	Par[Sj].Aqz=new double[BeamPar.NParticles];
	Par[Sj].Aqr=new double[BeamPar.NParticles];

	for (int i=0;i<BeamPar.NParticles;i++){
        Par[Sj].Aqz[i]=0;
        Par[Sj].Aqr[i]=0;
        if (Particle[i].lost==LIVE){
            phi=Particle[i].phi+K[Sj][i].phi*Par[Sj].h;
            r=(Particle[i].x+K[Sj][i].r*Par[Sj].h)*lmb;
            double V=mod(sqr(Rb)*Lb);
            double z=(phi-phic)*lmb/(2*pi);            
            if (Rb!=0 && Lb!=0 && SpCharge){
			    if (Coulomb) {
//                    Par[Sj].Aqz=3*lmb*Mr*(phi-phic)*(Icur/Ia)/(sqr(gamma)*sqr(Rb)*Fb);     // old expression
//                    Par[Sj].Aqz[i]=kFc*(3*Icur*lmb)*(Mr*z/V);                              // Es; original Hellweg expression
                    Par[Sj].Aqz[i]=(-2)*2*kFc*(3*Icur*lmb)*(Mr*z/V);                         // Es; corrected (YuE) expression
                    Par[Sj].Aqz[i]*=(lmb/We0);  //A
                        
//                    Par[Sj].Aqr=sqr(lmb)*(1-Mr)*r*lmb*(Icur/Ia)/(sqr(gamma)*sqr(Rb)*Lb);   // old expression
//                    Par[Sj].Aqr[i]=kFc*(3*Icur*lmb/sqr(gamma))*(0.5*(1-Mr)*r/V);           // E; original Hellweg expression
                    Par[Sj].Aqr[i]=2*kFc*(3*Icur*lmb)/gamma*(0.5*(1+Mr)*r/V);                // E; corrected (YuE) expression
                    Par[Sj].Aqr[i]*=(lmb/We0);  //A
/*                    FILE *Fout;
					if ((i == 0) || (i == 999)) {
					    Fout=fopen("yeDebug_SK.log","a");
					    fprintf(Fout,"TBeamSolver::Integrate (Coulomb): i=%d, Si=%d, Sj=%d, Rb=%g, Lb=%g, r=%g, z=%g, Aqz[i]=%g, Aqr[i]=%g\n",
					            i,Si,Sj,Rb,Lb,r,z,Par[Sj].Aqz[i],Par[Sj].Aqr[i]);
					    fclose(Fout);  
				    } */
//                    k1F[i]:=dF_dx(bv[j-1],Beam[i,5,j-1],A[j-1],Btmp,SinSum);
				} 
				if (GWmethod == true) {
                    Par[Sj].Aqz[i]=kFc*Icur*lmb*GaussIntegration(r,z,Rb,Lb,3);  // E;  (YuE) expression
                    Par[Sj].Aqz[i]*=(lmb/We0);                     // A
                    Par[Sj].Aqr[i]=kFc*Icur*lmb*GaussIntegration(r,z,Rb,Lb,1);  // E;  (YuE) expression
                    Par[Sj].Aqr[i]*=(lmb/We0);                     // A
/*                    FILE *Fout;
					if ((i == 0) || (i == 999)) {
					    Fout=fopen("yeDebug_GW.log","a");
					    fprintf(Fout,"TBeamSolver::Integrate (GWmethod): i=%d, Si=%d, Sj=%d, Rb=%g, Lb=%g, r=%g, z=%g, Aqz[i]=%g, Aqr[i]=%g\n",
					            i,Si,Sj,Rb,Lb,r,z,Par[Sj].Aqz[i],Par[Sj].Aqr[i]);
					    fclose(Fout);  
				    }  */
                }
			}
        }
    }

    Beam[Si]->Integrate(Par[Sj],K,Sj);
    delete[] Par[Sj].Aqz;
    delete[] Par[Sj].Aqr;
}
//---------------------------------------------------------------------------
 double TBeamSolver::GaussIntegration(double r,double z,double Rb,double Lb,int component)
{
/*    double psi5[5]  ={-0.9061798,-0.5384093,0.0,0.5384093,0.9061798};
    double w5[5]    ={0.2369269,0.4786287,0.5688889,0.4786287,0.2369269};
    double psi6[6]  ={-0.9324695,-0.6612094,-0.2386192,0.2386192,0.6612094,0.9324695};
    double w6[6]    ={0.1713245,0.3607616,0.4679139,0.4679139,0.3607616,0.1713245};
    double psi7[7]  ={-0.9491079,-0.7415312,-0.4058452,0.0,      0.4058452,0.7415312,0.9491079};
    double w7[7]    ={ 0.1294850, 0.2797054, 0.3818301,0.4179592,0.3818301,0.2797054,0.1294850};
    double psi8[8]  ={-0.9602899,-0.7966665,-0.5255324,-0.1834346,0.1834346,0.5255324,0.7966665,0.9602899};
    double w8[8]    ={ 0.1012285, 0.2223810, 0.3137066, 0.3626838,0.3626838,0.3137066,0.2223810,0.1012285};
    double psi9[9]  ={-0.9681602,-0.8360311,-0.6133714,-0.3242534,0.0,      0.3242534,0.6133714,0.8360311,0.9681602};
    double w9[9]    ={ 0.0812744, 0.1806482, 0.2606107, 0.3123471,0.3302394,0.3123471,0.2606107,0.1806482,0.0812744};
    double psi10[10]={-0.9739065,-0.8650634,-0.6794096,-0.4333954,-0.1488743,0.1488743,0.4333954,0.6794096,0.8650634,0.9739065};
    double w10[10]  ={ 0.0666713, 0.1494513, 0.2190864, 0.2692602, 0.2955242,0.2955242,0.2692602,0.2190864,0.1494513,0.0666713};
    double psi11[11]={-0.9782287,-0.8870626,-0.7301520,-0.5190961,-0.2695432,0.0,
                       0.2695432, 0.5190961, 0.7301520, 0.8870626, 0.9782287};
    double w11[11]  ={ 0.0556686, 0.1255804, 0.1862902, 0.2551938, 0.2628045,0.2729251,
                       0.2628045, 0.2331938, 0.1862902, 0.1255804, 0.0556686};
*/
	double psi12[12]={-0.9815606,-0.9041173,-0.7699027,-0.5873180,-0.3678315,-0.1253334,
                       0.1253334, 0.3678315, 0.5873180, 0.7699027, 0.9041173, 0.9815606};
    double w12[12]  ={ 0.0471753, 0.1069393, 0.1600783, 0.2031674, 0.2334925, 0.2491470,
                       0.2491470, 0.2334925, 0.2031674, 0.1600783, 0.1069393, 0.0471753};
/*					   
    double psi16[16]={-0.9894009,-0.9445750,-0.8656312,-0.7554044,-0.6178762,-0.4580168,-0.2816036,-0.0950125,
                       0.0950125, 0.2816036, 0.4580168, 0.6178762, 0.7554044, 0.8656312, 0.9445750, 0.9894009};
    double w16[16]  ={ 0.0271525, 0.0622535, 0.0951585, 0.1246290, 0.1495960, 0.1691565, 0.1826034, 0.1894506,
                       0.1894506, 0.1826034, 0.1691565, 0.1495960, 0.1246290, 0.0951585, 0.0622535, 0.0271525};
*/
	
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
		#ifndef RSLINAC
		AnsiString S="Beam Lost!";
		ShowError(S);
        #endif
        Stop=true;
        return;
    }
}
//---------------------------------------------------------------------------
void TBeamSolver::Step(int Si)
{
    bool drift=false;
    lmb=Structure[Si].lmb;
    Beam[Si]->lmb=lmb;
	CountLiving(Si);
	I=BeamPar.Current*Nliv/BeamPar.NParticles;
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

    double db=Structure[Si+1].betta-Structure[Si].betta;
    Par[0].bw=Structure[Si].betta;
    Par[1].bw=Structure[Si].betta+db/2;
    Par[2].bw=Par[1].bw;
    Par[3].bw=Structure[Si+1].betta;

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

    double dBx=Structure[Si+1].B_ext-Structure[Si].B_ext;
    Par[0].Bz_ext=Structure[Si].B_ext;
    Par[1].Bz_ext=Structure[Si].B_ext+dBx/2;
    Par[2].Bz_ext=Par[1].Bz_ext;
    Par[3].Bz_ext=Structure[Si+1].B_ext;

    /*fprintf(logFile,"Phase Radius Betta\n");
    for (int i=0;i<Np;i++)
        fprintf(logFile,"%f %f %f\n",Beam[Si]->Particle[i].phi,Beam[Si]->Particle[i].x,Beam[Si]->Particle[i].betta);

    fclose(logFile);  */

    for (int j=0;j<Ncoef;j++)
        Integrate(Si,j);
}
//---------------------------------------------------------------------------
void TBeamSolver::Solve()
{
	#ifndef RSLINAC
	if (SmartProgress==NULL){
		ShowMessage("System Message: ProgressBar not assigned! Code needs to be corrected");
        return;
    }
    SmartProgress->Reset(Npoints-1/*Np*/);
    #endif

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

		if (Structure[i].Dump) {
			int jmin=0;
			int jmax=BeamPar.NParticles;
			//char *Fname=(char *)Structure[i].DumpParameters.File;
			std::ofstream fo(Structure[i].DumpParameters.File.c_str());
			AnsiString s;

			if (Structure[i].DumpParameters.N1>0 && Structure[i].DumpParameters.N2==0) {
				jmin=0;
				jmax=Structure[i].DumpParameters.N1;
			} else if (Structure[i].DumpParameters.N1>0 && Structure[i].DumpParameters.N2>0) {
				jmin=Structure[i].DumpParameters.N1-1;
				jmax=Structure[i].DumpParameters.N2;
			}

			if (jmin>BeamPar.NParticles || jmax>BeamPar.NParticles) {
				fo<<"WARNING: The defined range of particle numbers exceeds the number of available particles. The region was set to default.\n";
				jmin=0;
				jmax=BeamPar.NParticles;
			}

			fo<<"List of ";

			if (jmin==0 && jmax==BeamPar.NParticles)
				fo<<"ALL ";
			if (Structure[i].DumpParameters.LiveOnly)
					 fo<<"LIVE ";
			fo<<"particles ";

			if (!(jmin==0 && jmax==BeamPar.NParticles)){
				fo<<" from #";
				fo<<jmin+1;
				fo<<" to #";
				fo<<jmax;

			}
			fo<<" at z=";

			s=s.FormatFloat("#0.00",100*Structure[i].ksi*Structure[i].lmb);
			fo<<s.c_str();
			fo<<" cm\n";

			fo<<"Particle #\t";
			if (!Structure[i].DumpParameters.LiveOnly)
				fo<<"Lost\t";

			if (Structure[i].DumpParameters.Phase)
				fo<<"Phase [deg]\t";
			if (Structure[i].DumpParameters.Energy)
				fo<<"Energy [MeV]\t";
			if (Structure[i].DumpParameters.Radius)
				fo<<"Radius [mm]\t";
			if (Structure[i].DumpParameters.Azimuth)
				fo<<"Azimuth [deg]\t";
			if (Structure[i].DumpParameters.Vx)
				fo<<"Vx	[m/s]\t";
			//fo<<"Vth [m/s]\t";
			fo<<"\n";

			for (int j=jmin;j<jmax;j++){
				if(!Structure[i].DumpParameters.LiveOnly || (Structure[i].DumpParameters.LiveOnly && !Beam[i]->Particle[j].lost)){
					s=s.FormatFloat("#0000\t\t",j+1);
					fo<<s.c_str();
					if (!Structure[i].DumpParameters.LiveOnly) {
						if (Beam[i]->Particle[j].lost)
							fo<<"LOST\t";
						else
                            fo<<"LIVE\t";
					}
					if (Structure[i].DumpParameters.Phase){
						s=s.FormatFloat("#0.00\t\t",HellwegTypes::RadToDeg(Beam[i]->Particle[j].phi));
						fo<<s.c_str();
					}
					if (Structure[i].DumpParameters.Energy){
						s=s.FormatFloat("#0.00\t\t\t",VelocityToMeV(Beam[i]->Particle[j].betta));
						fo<<s.c_str();
					}
					if (Structure[i].DumpParameters.Radius){
						s=s.FormatFloat("#0.00\t\t\t",1000*Beam[i]->Particle[j].x*Structure[i].lmb);
						fo<<s.c_str();
					}
					if (Structure[i].DumpParameters.Azimuth){
						s=s.FormatFloat("#0.00\t\t",HellwegTypes::RadToDeg(Beam[i]->Particle[j].Th));
						fo<<s.c_str();
					}
					if (Structure[i].DumpParameters.Vx){
						s=s.FormatFloat("#0.00\t\t",Beam[i]->Particle[j].Bx*c);
						fo<<s.c_str();
					}
				 /*	s=s.FormatFloat("#0.00\t\t",Beam[i]->Particle[j].Bth*c);
					fo<<s.c_str();  */
					fo<<"\n";
				}
			}    
			fo.close();  
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

		for (int j=0;j<BeamPar.NParticles;j++){
			if (Beam[i+1]->Particle[j].lost==LIVE && mod(Beam[i+1]->Particle[j].x)>=Structure[i+1].Ra)
				Beam[i+1]->Particle[j].lost=RADIUS_LOST;
            Beam[i+1]->Particle[j].z=Structure[i+1].ksi*Structure[i+1].lmb;
            Beam[i+1]->Particle[j].phi-=Structure[i+1].dF;
		}
		
        #ifndef RSLINAC
        SmartProgress->operator ++();
        Application->ProcessMessages();
        #endif
        if (Stop){
            Stop=false;
			#ifndef RSLINAC
			AnsiString S="Solve Process Aborted!";
			ShowError(S);
			SmartProgress->Reset();
            #endif
            return;
        }
        for (int i=0;i<Ncoef;i++)
            memset(K[i], 0, sizeof(TIntegration));
        //}
    }

   //   

    #ifndef RSLINAC
    SmartProgress->SetPercent(100);
    SmartProgress->SetTime(0);
    #endif
}
//---------------------------------------------------------------------------
#ifndef RSLINAC
TResult TBeamSolver::Output(AnsiString& FileName,TMemo *Memo)
#else
TResult TBeamSolver::Output(AnsiString& FileName)
#endif
{
    AnsiString Line,s;
    TStringList *OutputStrings;
    OutputStrings= new TStringList;
    TResult Result;
  /*    int Nliv=0;
    Nliv=Beam[Npoints-1]->GetLivingNumber();
                     */
    OutputStrings->Clear();
	OutputStrings->Add("========================================");
    OutputStrings->Add("INPUT DATA from:");
	OutputStrings->Add(InputFile);
	OutputStrings->Add("========================================");

  /*    TStringList *InputStrings;
    InputStrings= new TStringList;
	InputStrings->LoadFromFile(InputFile);    */
    OutputStrings->AddStrings(InputStrings);

	OutputStrings->Add("========================================");
    OutputStrings->Add("RESULTS");
	OutputStrings->Add("========================================");
    OutputStrings->Add("");

    double Ws=0;
   //   AnsiString s;
    int j=Npoints-1;
    double z=100*Structure[j].ksi*Structure[j].lmb;

    double W=0,dW=0;

	TSpectrumBar *WSpectrum=GetEnergySpectrum(j,true,W,Ws);
    if (W!=0)
        dW=100*Ws/W;
    else
        dW=100;
	double Wm=Beam[j]->GetMaxEnergy();
	double I=Beam[j]->Ib;
	double I0=Beam[0]->Ib;
    double kc=100.0*Beam[j]->GetLivingNumber()/Beam[0]->GetLivingNumber();
    double r=1e3*Beam[j]->GetBeamRadius();

    double F=0,dF=0;

    TSpectrumBar *FSpectrum=GetPhaseSpectrum(j,true,F,dF);
    double f=1e-6*c/Structure[j].lmb;
	double Ra=1e3*Structure[j].Ra*Structure[j].lmb;
	double P=W*I;

	double W0=Beam[0]->GetAverageEnergy();
	//double Wout=Beam[j]->GetAverageEnergy();
	double Pin=W0*I0;


	double v=Structure[j].betta;
	double E=sqrt(2*Structure[j].Rp);
	double Pb=E!=0?1e-6*sqr(Structure[j].A*We0/E):0;

    /*double Pw=P0;
    for(int i=1;i<Npoints;i++)
        Pw=Pw*exp(-2*(Structure[i].ksi-Structure[i-1].ksi)*Structure[i].alpha*Structure[i].lmb);  */
	double Pw=1e-6*P0-(P-Pin+Pb);

    double alpha=0,betta=0,eps=0;
    GetCourantSneider(j,alpha,betta,eps);

  /*  double A=0;
  	int Na=j-Nmesh/2;
	if (Na>0)
		A=Structure[Na].A;     */

    Result.Length=z;
    Result.AverageEnergy=W;
    Result.MaximumEnergy=Wm;
	Result.EnergySpectrum=dW;
	Result.InputCurrent=1e3*I0;
    Result.BeamCurrent=1e3*I;
    Result.Captured=kc;
    Result.BeamRadius=r;
    Result.AveragePhase=F;
    Result.PhaseLength=dF;
    Result.BeamPower=P;
    Result.LoadPower=Pb;
    Result.Alpha=alpha;
    Result.Betta=100*betta;
    Result.Emittance=100*eps;
  //  Result.A=A;

	Line="Total Length = "+s.FormatFloat("#0.000",Result.Length)+" cm";
    OutputStrings->Add(Line);
    Line="Average Energy = "+s.FormatFloat("#0.000",Result.AverageEnergy)+" MeV";
    OutputStrings->Add(Line);
    Line="Maximum Energy = "+s.FormatFloat("#0.000",Result.MaximumEnergy)+" MeV";
    OutputStrings->Add(Line);
    Line="Energy Spectrum = "+s.FormatFloat("#0.00",Result.EnergySpectrum)+" %";
    OutputStrings->Add(Line);
    Line="Input Current = "+s.FormatFloat("#0.00",Result.InputCurrent)+" mA";
    OutputStrings->Add(Line);
    Line="Beam Current = "+s.FormatFloat("#0.00",Result.BeamCurrent)+" mA";
    OutputStrings->Add(Line);
    Line="Captured = "+s.FormatFloat("#0.00",Result.Captured)+" %";
    OutputStrings->Add(Line);
    Line="Beam Radius (RMS) = "+s.FormatFloat("#0.00",Result.BeamRadius)+" mm";
    OutputStrings->Add(Line);
    Line="Average Phase = "+s.FormatFloat("#0.00",Result.AveragePhase)+" deg";
	OutputStrings->Add(Line);
	Line="Phase Length = "+s.FormatFloat("#0.00",Result.PhaseLength)+" deg";
	OutputStrings->Add(Line);
	OutputStrings->Add("Twiss Parameters:");
	Line="alpha= "+s.FormatFloat("#0.00000",Result.Alpha);
	OutputStrings->Add(Line);
	Line="betta = "+s.FormatFloat("#0.00000",Result.Betta)+" cm/rad";
	OutputStrings->Add(Line);
	Line="epsilon = "+s.FormatFloat("#0.000000",Result.Emittance)+" cm*rad";
	OutputStrings->Add(Line);
	//OutputStrings->Add("========================================");

	int Nsec=0;
	double Pbeam0=0, Wb0=0;
	double Ib=0, Wb=0, P0=0;
	for (int i=0;i<=j;i++){
		if (Structure[i].jump && !Structure[i].drift || i==j) {
			Ib=Beam[i]->Ib;
			Wb=Beam[i]->GetAverageEnergy();

			if (Nsec>0) {
				double Pload=E!=0?1e-6*sqr(Structure[i-1].A*We0/E):0;
				double Pbeam=Ib*Wb;

				Line="===========Section #"+s.FormatFloat("#",Nsec)+" ======================";
				OutputStrings->Add(Line);
				Line="Input Power = "+s.FormatFloat("#0.0000",P0)+" MW";
				OutputStrings->Add(Line);
			   /*	Line="Beam Power = "+s.FormatFloat("#0.0000",Pbeam)+" MW";
				OutputStrings->Add(Line);
				Line="Beam Energy = "+s.FormatFloat("#0.0000",Wb)+" MeV";
				OutputStrings->Add(Line);   */
				Line="Beam Energy Gain = "+s.FormatFloat("#0.0000",Wb-Wb0)+" MeV";
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


    #ifndef RSLINAC
    if (Memo!=NULL){
        Memo->Lines->AddStrings(OutputStrings);
    }
    #endif
                       
    delete[] WSpectrum;
    delete[] FSpectrum;  
    OutputStrings->SaveToFile(FileName);
    delete OutputStrings;
   //   delete Strings;

    

    return Result;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
