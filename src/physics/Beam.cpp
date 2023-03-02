//---------------------------------------------------------------------------


#pragma hdrstop

#include "Beam.h"
// #include "Types.h"
//---------------------------------------------------------------------------
__fastcall TBeam::TBeam(int N)
{
    Np=N;
    Nbars=200;
    Kernel=0.95;
    lmb=1;
    Cmag=0;
    W0=We0;
    SetKernel(Kernel);

    Particle=new TParticle[Np];
    for (int i=0;i<Np;i++)
        Particle[i].lost=LIVE;

    Nliv=N;
    Reverse=false;
}
//---------------------------------------------------------------------------
__fastcall TBeam::~TBeam()
{
    delete[] Particle;
}
//---------------------------------------------------------------------------
void TBeam::SetBarsNumber(int N)
{
    Nbars=N;
}
//---------------------------------------------------------------------------
void TBeam::SetKernel(double Ker)
{
    Kernel=Ker;
    h=1;//sqrt(-2*ln(1-Kernel));   ALWAYS RMS!
}
//---------------------------------------------------------------------------
//MOVED TO TYPES.H
/*void TBeam::TwoRandomGauss(double& x1,double& x2)
{
	double Ran1=0;
    double Ran2=0;

    Ran1=((double)rand() / ((double)RAND_MAX + 1));
    Ran2=((double)rand() / ((double)RAND_MAX + 1));

    if (Ran1<1e-5)
        Ran1=1e-5;

    x1=sqrt(-2*ln(Ran1))*cos(2*pi*Ran2);
    x2=sqrt(-2*ln(Ran1))*sin(2*pi*Ran2);
}        */
//---------------------------------------------------------------------------
//REMOVED
/*int TBeam::CountCSTParticles(AnsiString &F)
{
	return NumPointsInFile(F,2);
}     */
//---------------------------------------------------------------------------
void TBeam::GenerateEnergy(TGauss G)
{
	std::cerr << "In GenerateEnergy(.) \n";
	//std::cerr << "Calling MakeGaussDistribution(., Par) with hardwired Par=BETA_PAR \n";
	//IVP  MakeGaussDistribution(G,BETA_PAR);
	std::cerr << "Calling MakeGaussDistribution(., Par), now with hardwired Par=GAMMA_PAR \n";
	MakeGaussDistribution(G, GAMMA_PAR);
}
//---------------------------------------------------------------------------
void TBeam::GeneratePhase(TGauss G)
{
	MakeGaussDistribution(G,PHI_PAR);
}
//---------------------------------------------------------------------------
void TBeam::GenerateAzimuth(TGauss G)
{
	MakeGaussDistribution(G,TH_PAR);
}
//---------------------------------------------------------------------------
void TBeam::ShiftPhase(double phi0)
{
	for (int i=0;i<Np;i++)
		Particle[i].phi+=phi0;
}
//---------------------------------------------------------------------------
double **TBeam::ImportFromFile(TBeamType BeamType, TBeamInput *BeamPar, bool T)
{
	AnsiString F, L, S;
	int LineLength=-1, i=0;
	bool Success=false;
	double **X;

	switch (BeamType){
		case CST_PIT:{
			F=BeamPar->RFile;
			LineLength=PIT_LENGTH;
			break;
		}
		case CST_PID:{
			F=BeamPar->RFile;
			LineLength=PID_LENGTH;
			break;
		}
		case PARMELA_T2:{
			F=BeamPar->RFile;
			LineLength=T2_LENGTH;
			break;
		}
		case FILE_1D:{
			F=BeamPar->ZFile;
			LineLength=1;
			break;
		}
		case FILE_2D:{
			F=T?BeamPar->RFile:BeamPar->ZFile;
			LineLength=2;
			break;
		}
		case TWO_FILES_2D:{
			F=BeamPar->YFile;
			LineLength=2;
			break;
		}
		case FILE_4D:{
			F=BeamPar->RFile;
			LineLength=4;
			break;
		}
		default: {break;}
	}

	std::ifstream fs(F.c_str());
	X=new double *[LineLength];
	for (int j = 0; j < LineLength; j++) {
		X[j]= new double [BeamPar->NParticles];
	}

	while (!fs.eof()){
		L=GetLine(fs);

		if (NumWords(L)==LineLength){       //Check the proper numbers of words in the line
			try {
				for (int j=1;j<=LineLength;j++){
					//Check if the data is really a number
					S=ReadWord(L,j);
					X[j-1][i]=S.ToDouble();
				}
			}
			catch (...){
				continue;          //Skip incorrect line
			}
			if (i==BeamPar->NParticles){     //if there is more data than expected
			  //	i++;
				break;
			}
			i++;
		}
	}
	fs.close();

	Success=(i==BeamPar->NParticles);

	if (!Success)
		X=DeleteDoubleArray(X,LineLength);

	return X;
}
//---------------------------------------------------------------------------
bool TBeam::ImportEnergy(TBeamInput *BeamPar)
{
	double **X=NULL;
	double phi=0,W=0;
	int i=0;
	bool Success=false;

	X = ImportFromFile(BeamPar->ZBeamType,BeamPar,false);

	if (X!=NULL) {
		for (int i = 0; i < BeamPar->NParticles; i++) {
			switch (BeamPar->ZBeamType){
				case FILE_1D:
				{
					//IVP  Particle[i].beta0=MeVToVelocity(X[0][i],W0);
					Particle[i].g = MeVToGamma(X[0][i], W0);
					break;
				}
				case FILE_2D:
				{
					Particle[i].phi=DegreeToRad(X[0][i]);
					//IVP  Particle[i].beta0=MeVToVelocity(X[1][i],W0);
					Particle[i].g = MeVToGamma(X[1][i], W0);
					break;
				}
				default: {};
			}
		}
		int N=0;
		switch (BeamPar->ZBeamType) {
			case FILE_1D:
                            N=1;
                            break;
			case FILE_2D:
                            N=2;
                            break;
                        default:
							throw std::runtime_error("ImportEnergy error: Unhandled ZBeamType");
		}
		X=DeleteDoubleArray(X,N);
		Success=true;
	} else
		Success=false;

	return Success;
}
//---------------------------------------------------------------------------
bool TBeam::BeamFromImport(TBeamInput *BeamPar)
{
	std::cerr << "In BeamFromImport(.) \n";
	std::cerr << "BeamPar->RBeamType = " << BeamPar->RBeamType << '\n';
	std::cerr << "BeamPar->ZBeamType = " << BeamPar->ZBeamType << '\n';
	std::cerr << "BeamPar->ZCompress = " << BeamPar->ZCompress << '\n';
	double **X=NULL;
	double x=0,y=0,z=0,px=0,py=0,pz=0,t=0;
	double phi=0,W=0;
	double r=0,th=0,p=0,pr=0,pth=0,beta=0;
	double gb, gbx, gby, gbz, gamma; 
	int i=0;
	bool Success=false;
	TPhaseSpace C;

   	//FILE *logFile;
	//logFile = fopen("BeamImport.log","w"); 
	//std::cerr << "Opened BeamImport.log \n";

	X = ImportFromFile(BeamPar->RBeamType, BeamPar, true);

	if (X!=NULL) {
		for (int i = 0; i < BeamPar->NParticles; i++) {
			switch (BeamPar->RBeamType){
				case CST_PIT:
				{
					t=X[9][i];
					Particle[i].phi = -2*pi*t*c/lmb;
					if (BeamPar->ZCompress){
					   /*	int phase_dig=-DigitConst*Particle[i].phi;
						int max_dig=DigitConst*2*pi;
						int phase_trunc=phase_dig%max_dig;
						Particle[i].phi=(1.0*phase_trunc)/DigitConst; */
						CompressPhase(Particle[i].phi);
					}
				}
				case CST_PID:
				{  /* IVP
					x=X[0][i];
					y=X[1][i];
					z=X[2][i];
					px=X[3][i];
					py=X[4][i];
					pz=X[5][i];

					C=CartesianToCylinrical(x,y,px,py);
					r=C.x;
					th=C.y;
					pr=C.px;
					pth=C.py;

					p=sqrt(px*px+py*py+pz*pz);
					beta=1/sqrt(1+1/sqr(p));

					Particle[i].beta0=beta;
					Particle[i].beta.z=(pz/p)*beta;
					//Particle[i].r=x/lmb;
					Particle[i].r=r/lmb;
					//Particle[i].Br=(px/p)*beta;
					Particle[i].beta.r=(pr/p)*beta;
					Particle[i].th=th;
					Particle[i].beta.th=(pth/p)*beta;

					//fprintf(logFile,"%f %f %f\n",px,pr*cos(th)-pth*sin(th),px-pr*cos(th)+pth*sin(th));
					IVP */
					x = X[0][i];
                                        y = X[1][i];
                                        z = X[2][i];
                                        gbx = X[3][i]; // gamma*beta_x 
                                        gby = X[4][i];
                                        gbz = X[5][i];

                                        C = CartesianToCylinrical(x, y, gbx, gby);
                                        r = C.x;
					Particle[i].r = r /lmb; 
                                        th = C.y;
					Particle[i].th = th; 
                                        Particle[i].gb.r = C.px;
                                        Particle[i].gb.th = C.py; 
					Particle[i].gb.z = gbz; 
					Particle[i].g = sqrt(1. +sqr(gbx) +sqr(gby) +sqr(gbz)); 

					/*
					fprintf(logFile, "%f %f %f\n", gbx, gby, gbz); 
					fprintf(logFile, "%f %f %f\n", C.px, C.py, gbz);
					fprintf(logFile, "%f %f %f %f\n", Particle[i].gb.r, Particle[i].gb.th, Particle[i].gb.z, Particle[i].g);
					fprintf(logFile, "%f %f \n", sqr(gbx) +sqr(gby), sqr(C.px) +sqr(C.py));  
					fprintf(logFile, "\n");
					*/
                    		break;
				}
				case PARMELA_T2:
				{
					x=X[0][i]/100;  //cm to m 
					px=X[1][i]/1000;  //mrad to rad 
					y=X[2][i]/100;
					py=X[3][i]/1000;
					phi=X[4][i];   //deg
					W=X[5][i];     //MeV (kinetic) 

					gamma = MeVToGamma(W, W0); 
					Particle[i].g = gamma; 
					gb = sqrt(sqr(gamma) -1.); 
					gbx = gb *px /sqrt(1. +sqr(px) +sqr(py)); 
					gby = gb *py /sqrt(1. +sqr(px) +sqr(py)); 
					gbz = gb /sqrt(1. +sqr(px) +sqr(py)); 

					C = CartesianToCylinrical(x, y, gbx, gby); 

					/* IVP 
                    			C=CartesianToCylinrical(x,y,px,py);
					r=C.x;
					th=C.y;
					pr=C.px;
					pth=C.py;

					beta=MeVToVelocity(W,W0);
					p=1/sqrt(1/sqr(beta)-1); 
					IVP */ 

					Particle[i].phi = DegreeToRad(phi);
					if (BeamPar->ZCompress)
						CompressPhase(Particle[i].phi);

					/* IVP 
					Particle[i].beta0=beta;
					Particle[i].beta.z=beta;
					Particle[i].r=r/lmb;
					Particle[i].beta.r=(pr/p)*beta;
					Particle[i].th=th;
					Particle[i].beta.th=(pth/p)*beta;
					IVP */ 

                                        Particle[i].r = C.x /lmb;
                                        Particle[i].th = C.y;
					Particle[i].gb.r = C.px; 
                                        Particle[i].gb.th = C.py;
					Particle[i].gb.z = gbz; 

                    		break;
				}
				default: throw std::runtime_error("BeamFromCST error: Unhandled RBeamType");
			}
		}
		int N=0;
		switch (BeamPar->RBeamType){
			case CST_PIT: {N=PIT_LENGTH;break;}
			case CST_PID: {N=PID_LENGTH;break;}
			case PARMELA_T2: {N=T2_LENGTH;break;}
			default: throw std::runtime_error("BeamFromCST error: Unhandled RBeamType value");
		}
		X=DeleteDoubleArray(X,N);
		Success=true;
	} else
		Success=false;

	//fclose(logFile);
	//std::cerr << "Closed BeamImport.log \n\n";

	return Success;
}
//---------------------------------------------------------------------------
bool TBeam::BeamFromTwiss(TBeamInput *BeamPar)
{
	TPhaseSpace *X, *Y, C;
	double x=0,y=0, r=0,th=0, px=0,py=0, pr=0,pth=0, pz=0;//,p=0;
	double beta0 =0.0;

	X = MakeTwissDistribution(BeamPar->XTwiss);
	Y = MakeTwissDistribution(BeamPar->YTwiss);

	//FILE *logFile;
	//logFile=fopen("TwissGen.log","w");
      
	for (int i=0; i < Np; i++) {
		x = X[i].x;
		y = Y[i].x;
		px = tg(X[i].px);
		py = tg(Y[i].px);
		//py=Y[i].px;
		//if (x!=0) {
		//	py=(y/x)*px;
		//} else
		//	px=0;  

		C = CartesianToCylinrical(x,y,px,py);
		r = C.x;
		th = C.y;
		pr = C.px;
		pth = C.py;

		//fprintf(logFile,"%f %f %f %f %f %f %f %f\n",x,y,px,py,C.x,C.y,C.px,C.py);
	     	//fprintf(logFile,"%f %f\n",x,px);
		Particle[i].r = r/lmb;
		Particle[i].th = th; //rad

		if (pr!=0 || pth!=0){
			//IVP  Particle[i].beta.z = Particle[i].beta0 /sqrt(1 +sqr(pr) +sqr(pth));
			beta0 = sqrt(sqr(Particle[i].g) -1.) /Particle[i].g;
			Particle[i].gb.z = Particle[i].g *beta0 /sqrt(1 +sqr(pr) +sqr(pth));
		}
		else
			//IVP  Particle[i].beta.z = Particle[i].beta0;
			Particle[i].gb.z = sqrt(sqr(Particle[i].g) -1.);
		
		//IVP  Particle[i].beta.r = pr*Particle[i].beta.z;
		//IVP  Particle[i].beta.th = pth*Particle[i].beta.z;
		Particle[i].gb.r = pr *Particle[i].gb.z;
                Particle[i].gb.th = pth *Particle[i].gb.z;

		//Particle[i].Bth=0;

	    	//Particle[i].Br=pr*Particle[i].beta;
		//Particle[i].Bth=0;//
		//Particle[i].Bth=pth*Particle[i].beta;//0;
		//pz=sqrt(1-sqr(pr)-sqr(pth));
		//Particle[i].Bz=pz*Particle[i].beta;   

		//fprintf(logFile,"%f %f\n",Particle[i].r,Particle[i].Br);
	}
      

/*	TTwiss Tgr=GetTwiss(R_PAR);
	TTwiss Tgt=GetTwiss(TH_PAR);
	double eps=sqrt(sqr(Tgr.epsilon)+sqr(Tgt.epsilon));
	for (int i = 0; i < Np; i++) {
		Particle[i].Bth=0;
	}

	TTwiss T=GetTwiss(R_PAR);
	for (int i = 0; i < Np; i++) {
		Particle[i].Br*=eps/T.epsilon;
	}         */

   //	fclose(logFile);

	delete[] X;
	delete[] Y;

	return true;
}
//---------------------------------------------------------------------------
bool TBeam::BeamFromFile(TBeamInput *BeamPar)
{
	//std::cerr << "BeamFromFile functionality is disabled in this branch\n";
	double **X=NULL, **Y=NULL;
	double x=0,y=0,px=0,py=0;
	double r=0,th=0,p=0,pr=0,pth=0,beta=0;
	int i=0;
	bool Success=false,Full4D=true;
	TPhaseSpace C;

	X=ImportFromFile(BeamPar->RBeamType,BeamPar,true);
	if (BeamPar->RBeamType==TWO_FILES_2D)
		Y=ImportFromFile(FILE_2D,BeamPar,true);
	else
		Y=X;

	if (X!=NULL && Y!=NULL) {
		for (int i = 0; i < BeamPar->NParticles; i++) {
			switch (BeamPar->RBeamType){
				case FILE_2D:
				{
					x=mod(X[0][i]);
                    //x=X[0][i];
					px=X[1][i];
					Particle[i].r=0.01*x/lmb;
					//IVP  Particle[i].beta.r=px*Particle[i].beta0;
					Particle[i].gb.r = px *sqrt(sqr(Particle[i].g) -1.0);
					//Particle[i].Th=0;  // IVP wonders: Why is theta not assigned a value?
					//IVP  Particle[i].beta.th=0;
					Particle[i].gb.th = 0.0;
					Full4D=false;
					break;
				}
				case TWO_FILES_2D:
				{
					x=Y[0][i];
					px=Y[1][i];
					y=X[0][i];
					py=X[1][i];
					break;

				}
				case FILE_4D:
				{
					x=X[0][i];
					px=X[1][i];
					y=X[2][i];
					py=X[3][i];
					break;
				}
				default: {};
			}
			if (Full4D) {
				C=CartesianToCylinrical(x,y,px,py);
				r=C.x;
				th=C.y;
				pr=C.px;
				pth=C.py;

				Particle[i].r=0.01*r/lmb;
				//IVP  Particle[i].beta.r=pr*Particle[i].beta0;
				Particle[i].gb.r = pr *sqrt(sqr(Particle[i].g) -1.0);
				Particle[i].th=th;
				//IVP  Particle[i].beta.th=pth*Particle[i].beta0; //check! debugging may be required
				Particle[i].gb.th = pth *sqrt(sqr(Particle[i].g) -1.0); 
			}
			//CHECK sqrt(negative)!
			//IVP  Particle[i].beta.z = BzFromOther(Particle[i].beta0, Particle[i].beta.r, Particle[i].beta.th);//double-check!!!
			Particle[i].gb.z = sqrt(sqr(Particle[i].g) -1. -sqr(Particle[i].gb.r) -sqr(Particle[i].gb.th));
		}
		int N=0;
		switch (BeamPar->RBeamType){
			case FILE_2D: {N=2; break;}
			case TWO_FILES_2D: {N=2; Y=DeleteDoubleArray(Y,N); break;}
			case FILE_4D: {N=4; break;}
			default: throw std::runtime_error("BeamFromFile error: Unhandled RBeamType value");
		}
		X=DeleteDoubleArray(X,N);
		Success=true;
	} else
		Success=false;

	return Success;
}
//---------------------------------------------------------------------------
bool TBeam::BeamFromSphere(TBeamInput *BeamPar)
{
	double *X, *Y, *V;
	double r=0,th=0,pr=0,pz=0,v=0,psi=0,m=0,q=qe;
	TGauss Gx, Gy, Gv;
	TPhaseSpace C;
	double beta0;
 
	Gx.mean=0;
	Gx.limit=BeamPar->Sph.Rcath;
	Gx.sigma=BeamPar->Sph.Rcath;
	Gy.mean=0;
	Gy.limit=pi;
	Gy.sigma=100*pi;
	Gv.mean=0;
	Gv.limit=0;
	if (BeamPar->Sph.kT>0){
		switch (BeamPar->Species.Type) {
			case PROTON:{m=mp;break;}
			case ION:{
				m=mu*BeamPar->Species.A;
				q*=BeamPar->Species.Q;
				break;
			}
			case ELECTRON:{}
			default: {m=me;}
        }

		Gv.sigma=sqrt(2*q*BeamPar->Sph.kT/m);
	}else
		Gv.sigma=0;

	X=MakeRayleighDistribution(Gx);
	Y=MakeGaussDistribution(Gy);
	V=MakeGaussDistribution(Gv);

	for (int i=0; i < Np; i++) {
		r=X[i];
		th=Y[i];

		beta0 = sqrt(sqr(Particle[i].g) -1.0) /Particle[i].g; 

		pr=BeamPar->Sph.Rsph==0?0:-sin(r/BeamPar->Sph.Rsph);
		//pz=BeamPar->Sph.Rsph==1?0:-cos(r/BeamPar->Sph.Rsph);
		//psi=RandomCos();
			//IVP  pr+=V[i]/(Particle[i].beta0*c);
			pr+=V[i]/(beta0*c);

		Particle[i].r=r/lmb;
		//IVP  Particle[i].beta.r=pr*Particle[i].beta0;
		Particle[i].gb.r = pr *sqrt(sqr(Particle[i].g) -1.0); 
		Particle[i].th=th;
		//IVP  Particle[i].beta.th=0;
		Particle[i].gb.th = 0.0; 
		//IVP  Particle[i].beta.z=BzFromOther(Particle[i].beta0,Particle[i].beta.r,0);//double-check!!!
		Particle[i].gb.z = sqrt(sqr(Particle[i].g) -1. -sqr(Particle[i].gb.r));
    }

	delete[] X;
	delete[] Y;

	return true;
}
//---------------------------------------------------------------------------
bool TBeam::BeamFromEllipse(TBeamInput *BeamPar)
{
	double sx=0,sy=0,r=0,th=0,x=0,y=0;
	double a=0,b=0,phi=0;
	TPhaseSpace C;

	std::cerr << "Generation of beam from ellipse is disabled in this branch \n";
/* IVP 
	phi=BeamPar->Ell.phi;
	b=BeamPar->Ell.by;
	a=BeamPar->Ell.ax;

	sx=a/BeamPar->Ell.h;
	sy=b/BeamPar->Ell.h;

	double xx=0,yy=0,Rc=0,Ran1=0,Ran2=0,Myx=0,Dyx=0;
	double Mx=0; //x0
	double My=0; //y0

	for (int i=0;i<Np;i++){
		do{
			TwoRandomGauss(Ran1,Ran2);
			xx=Mx+Ran1*sx;
			Myx=My+Rc*sy*(xx-Mx)/sx; //ellitic distribution
			Dyx=sqr(sy)*(1-sqr(Rc)); //dispersion
			yy=Myx+Ran2*sqrt(Dyx);

		} while (sqr(xx/a)+sqr(yy/b)>1);

		x=xx*cos(phi)-yy*sin(phi);
		y=xx*sin(phi)+yy*cos(phi);

		C=CartesianToCylinrical(x,y,0,0);

		r=C.x;
		th=C.y;

		Particle[i].r=r/lmb;
		Particle[i].beta.r=0;
		Particle[i].th=th;
		Particle[i].beta.th=0;
		Particle[i].beta.z=BzFromOther(Particle[i].beta0,Particle[i].beta.r,0);//double-check!!!
	}
IVP */ 
	return true;
}
//---------------------------------------------------------------------------
TPhaseSpace *TBeam::MakeTwissDistribution(TTwiss T)
{
	TPhaseSpace *X=NULL;
	TEllipse E;

	X = new TPhaseSpace[Np];

	T.gamma = (1+sqr(T.alpha))/T.beta;
	E = GetEllipse(T);

	double xx=0, yy=0, Rc=0, Ran1=0, Ran2=0, Myx=0, Dyx=0;
	double Mx=0; //x0
	double My=0; //x'0

	for (int i=0; i<Np; i++){
		TwoRandomGauss(Ran1, Ran2);
		xx = Mx +Ran1*E.Rx;
		Myx = My +Rc*E.Ry*(xx-Mx)/E.Rx; //elliptic distribution
		Dyx = sqr(E.Ry)*(1-sqr(Rc)); //dispersion
		yy = Myx +Ran2*sqrt(Dyx);
		X[i].x = xx*cos(E.phi) -yy*sin(E.phi);
	  //	Particle[i].x=x/lmb;
		X[i].px = xx*sin(E.phi) +yy*cos(E.phi);     //x'=bx/bz; Emittance is defined in space x-x'
	   //	Particle[i].Bx=px*Particle[i].betta;  //bx=x'*bz
	}

   	return X;
}
//---------------------------------------------------------------------------
void TBeam::SetParameters(double *X, TBeamParameter Par)
{
	std::cerr << "Entering TBeam::SetParameters(., Par) \n"; 
	std::cerr << "Par = " << Par << '\n';
	switch (Par) {
		case (R_PAR):{
			for (int i=0;i<Np;i++)
				Particle[i].r=X[i];
			break;
		}
		//IVP case (BR_PAR):{
		//IVP 	for (int i=0;i<Np;i++)
		//IVP 		Particle[i].beta.r=X[i];
		//IVP 	break;
		//IVP }
		case (TH_PAR):{
			for (int i=0;i<Np;i++)
				Particle[i].th=X[i];
			break;
		}
		//IVP case (BTH_PAR):{
		//IVP 	for (int i=0;i<Np;i++)
		//IVP 		Particle[i].beta.th=X[i];
		//IVP 	break;
		//IVP }
		case (PHI_PAR):{
			for (int i=0;i<Np;i++)
				Particle[i].phi=X[i];
			break;
		}
		//IVP case (BETA_PAR):{
		//IVP 	for (int i=0;i<Np;i++)
		//IVP 		Particle[i].beta0=MeVToVelocity(X[i],W0);
		//IVP 	break;
		//IVP }
		//IVP case (BZ_PAR):{
		//IVP 	for (int i=0;i<Np;i++)
		//IVP 		Particle[i].beta.z=X[i];
		//IVP 	break;
		//IVP }
		case (GBR_PAR):{
                        for (int i=0; i<Np; i++)
                                Particle[i].gb.r = X[i];
                        break;
                }
		case (GBTH_PAR):{
                        for (int i=0; i<Np; i++)
                                Particle[i].gb.th = X[i];
                        break;
                }
		case (GBZ_PAR):{
                        for (int i=0; i<Np; i++)
                                Particle[i].gb.z = X[i];
                        break;
                }
		case (GAMMA_PAR):{
                        for (int i=0; i<Np; i++)
                                Particle[i].g = MeVToGamma(X[i], W0);
                        break;
                }
	//std::cerr << "In TBeam::SetParameters(...) switch statement, after known Par-s \n";
        default:
			throw std::runtime_error("SetParameters error: Unhandled TBeamParameter value");
	}
}
//---------------------------------------------------------------------------
void TBeam::SetCurrent(double I)
{
	Ib=I;
}
//---------------------------------------------------------------------------
void TBeam::SetInputCurrent(double I)
{
	I0=I;
}
//---------------------------------------------------------------------------
void TBeam::SetRestEnergy(double W)
{
	W0=W;
}
//---------------------------------------------------------------------------
double *TBeam::MakeEquiprobableDistribution(double Xav, double dX)
{
	double Ran=0;
	double *Xi;
	Xi= new double[Np];
	double a=Xav-dX;
	double b=Xav+dX;

	for (int i=0;i<Np;i++){
		Ran=((double)rand() / ((double)RAND_MAX + 1));
		Xi[i]=a+(b-a)*Ran;
	}
	return Xi;
}
//---------------------------------------------------------------------------
void TBeam::MakeEquiprobableDistribution(double Xav, double dX, TBeamParameter Par)
{
	std::cerr << "In MakeEquiprobableDistribution(., ., Par) \n";
	std::cerr << "Par = " << Par << '\n'; 
	double *Xi;
	Xi=MakeEquiprobableDistribution(Xav,dX); 
	std::cerr << "Calling SetParameters(., Par) \n";
    	SetParameters(Xi,Par);
	delete[] Xi;
}
//---------------------------------------------------------------------------
double *TBeam::MakeGaussDistribution(TGauss G)
{
	return MakeGaussDistribution(G.mean,G.sigma,G.limit);
}
//---------------------------------------------------------------------------
double *TBeam::MakeRayleighDistribution(TGauss G)
{
	return MakeRayleighDistribution(G.mean,G.sigma,G.limit);
}
//---------------------------------------------------------------------------
double *TBeam::MakeGaussDistribution(double Xav,double sX,double Xlim)
{
	double *Xi;
	double xx=Xlim+1;

	if (Xlim<=0)
		Xi=MakeGaussDistribution(Xav,sX);
	else if (sqr(Xlim)<12*sqr(sX))
		Xi=MakeEquiprobableDistribution(Xav,Xlim);
	else {
		Xi=new double[Np];
		for (int i=0;i<Np;i++){
			do {
				xx=RandomGauss()*sX/h;
			} while (mod(xx)>Xlim);
			Xi[i]=Xav+xx;
		}
	}

	return Xi;
}  //---------------------------------------------------------------------------
double *TBeam::MakeRayleighDistribution(double Xav,double sX,double Xlim)
{
	double *Xi;
	double xx=Xlim+1;

	if (Xlim<=0)
		Xi=MakeRayleighDistribution(Xav,sX);
  /*	else if (sqr(Xlim)<12*sqr(sX))
		Xi=MakeEquiprobableDistribution(Xav,Xlim);   */
	else {
		Xi=new double[Np];
		for (int i=0;i<Np;i++){
			do {
				xx=RandomRayleigh()*sX;
			} while (mod(xx)>Xlim);
			Xi[i]=Xav+xx;
		}
	}

	return Xi;
}
//---------------------------------------------------------------------------
double *TBeam::MakeGaussDistribution(double Xav,double sX)
{
	double xx=0;
	double *Xi;
	Xi= new double[Np];

	for (int i=0;i<Np;i++){
		xx=RandomGauss();
		Xi[i]=Xav+xx*sX/h;
	}
	return Xi;
}
//---------------------------------------------------------------------------
double *TBeam::MakeRayleighDistribution(double Xav,double sX)
{
	double Ran1=0,Ran2=0,xx=0;
	double *Xi;
	Xi= new double[Np];

	for (int i=0;i<Np;i++){
		xx=RandomRayleigh();
		Xi[i]=Xav+xx*sX;
	}
	return Xi;
}
//---------------------------------------------------------------------------
void TBeam::MakeGaussDistribution(double Xav, double sX, TBeamParameter Par, double Xlim)
{
	std::cerr << "In MakeGaussDistribution(., ., Par, .) \n";
        std::cerr << "Par = " << Par << '\n';
	double *Xi;
	Xi=MakeGaussDistribution(Xav,sX,Xlim);
	std::cerr << "Calling SetParameters(., Par) " << '\n';
	SetParameters(Xi,Par);
	delete[] Xi;
}
//---------------------------------------------------------------------------
void TBeam::MakeGaussDistribution(TGauss G, TBeamParameter Par)
{
	std::cerr << "In MakeGaussDistribution(., Par) \n"; 
	std::cerr << "Par = " << Par << '\n';
	std::cerr << "Calling MakeGaussDistribution(., ., Par, .) \n"; 
	return MakeGaussDistribution(G.mean, G.sigma, Par, G.limit);
}
//---------------------------------------------------------------------------
void TBeam::CountLiving()
{
	Nliv=0;
	for (int i=0;i<Np;i++){
		if (Particle[i].lost==LIVE)
			Nliv++;
	}
}
//---------------------------------------------------------------------------
TEllipse TBeam::FindEmittanceAngle(TBeamParameter P)
{
	TBeam *Beam1;
	Beam1 = new TBeam(Np);
	Beam1->SetBarsNumber(Nbars);
	Beam1->SetKernel(Kernel);

	double Ang0=-pi/2, Ang1=pi/2, dAngle, Angle;
	double Angle1, Angle2, Res;
	double Sx=0, Sbx=0;
	double Sy=0, Sby=0;
	double Err=1e-5;
	int Nmax=30;

   //	TGauss Gx0,Gy0;
	double *X0, *Bx0;
	double *Gm0;
	TBeamParameter P1, P2;

   //	FILE *logFile;
   //	logFile=fopen("Ang_new.log","w");

	//P1=ComplementaryParameter(P);
	switch (P) {
		//IVP  case R_PAR:{P1 = BR_PAR; break;}
		//IVP  case X_PAR:{P1 = BX_PAR; break;}
		//IVP  case Y_PAR:{P1 = BY_PAR; break;}
		case R_PAR:{P1 = GBR_PAR; break;}
                case X_PAR:{P1 = GBX_PAR; break;}
                case Y_PAR:{P1 = GBY_PAR; break;}
		default: {P1 = NO_PAR;};
	}

	X0 = GetLivingParameter(P); //r/lmb
	Bx0 = GetLivingParameter(P1); //rad
	//IVP: Bx0 now contains gamma*betas, not betas
	Gm0 = GetLivingParameter(GAMMA_PAR);

	/*for (int k=0;k<Nliv;k++){
		X0[k]*=lmb;
	   //	fprintf(logFile,"%f %f \n",X0[k],Bx0[k]);
	}   */

	dAngle = (Ang1 -Ang0)/2;  //IVP   = pi/2 
	Angle = Ang0 +dAngle;    //IVP   = 0

	int i=0;
	while (dAngle>Err && i<Nmax){    //IVP    Nmax = 30
        	dAngle/=2;               //IVP    = pi/4, pi/8, ...
        	Angle1 = Angle +dAngle;
		Angle2 = Angle -dAngle;

		int j=0;
		for (int k=0; k<Np; k++){
			//IVP  Beam1->Particle[k].lost = Particle[k].lost;
			//IVP  Beam1->Particle[k].beta = Particle[k].beta;
			Beam1->Particle[k].th = Particle[k].th;
			Beam1->Particle[k].lost = Particle[k].lost;
			if (Particle[k].lost==LIVE){
				//IVP  Beam1->Particle[k].r = X0[j]*cos(Angle1) +Bx0[j]*sin(Angle1);
				//IVP  Beam1->Particle[k].beta.r = Bx0[j]*cos(Angle1) -X0[j]*sin(Angle1);
				Beam1->Particle[k].r = X0[j]*cos(Angle1) +Bx0[j]*sin(Angle1) /Gm0[j];
                                Beam1->Particle[k].gb.r = Bx0[j]*cos(Angle1)/Gm0[j] -X0[j]*sin(Angle1);
				//IVP: Beam1's gb now actually contains beta, not gamma*beta 
				j++;
			  //	fprintf(logFile,"%f %f %f %f \n",X0[k],Bx0[k],Beam1->Particle[k].r,Beam1->Particle[k].Br);
			}
		}
		Sx = Beam1->GetDeviation(R_PAR);
		//IVP  Sbx = Beam1->GetDeviation(BR_PAR);
		Sbx = Beam1->GetDeviation(GBR_PAR);  //IVP: b/c gb's contain beta, this is rms beta, not rms gamma*beta

		j=0;
		for (int k=0;k<Np;k++){
			if (Particle[k].lost==LIVE){
				//IVP  Beam1->Particle[k].r = X0[j]*cos(Angle2) +Bx0[j]*sin(Angle2);
				//IVP  Beam1->Particle[k].beta.r = Bx0[j]*cos(Angle2) -X0[j]*sin(Angle2);
				Beam1->Particle[k].r = X0[j]*cos(Angle2) +Bx0[j]*sin(Angle2)/Gm0[j];
                                Beam1->Particle[k].gb.r = Bx0[j]*cos(Angle2)/Gm0[j] -X0[j]*sin(Angle2);
				j++;
			}
		}

		Sy = Beam1->GetDeviation(R_PAR);

		if (mod(Sx-Sy) < Err)
			break;
		else if (Sx > Sy)
			Angle = Angle2;
		else if (Sy > Sx)
			Angle = Angle1;
	     /* else if (Sx==Sy)
			break;  */
		i++;
	}

	delete Beam1;
	delete[] X0;
	delete[] Bx0;
	delete[] Gm0;

   //	fclose(logFile);

	TEllipse E;
	E.ax = Sx;
	E.by = Sbx;
	E.phi = Angle;

	return E;
}
//---------------------------------------------------------------------------
TEllipse TBeam::GetEllipseDirect(TBeamParameter P)
{
	TEllipse E,E1;

	TGauss Gx,Gy;

	if (GetLivingNumber()>1) {
		TBeamParameter P1=ComplementaryParameter(P);
		Gx = GetStatistics(P); //r/lmb
		Gy = GetStatistics(P1); //rad
	}

	E.x0 = Gx.mean; //x
	E.y0 = Gy.mean;// x'
	E.Rx = Gx.sigma;
	E.Ry = Gy.sigma;

//	E.phi=FindEmittanceAngle(E.ax,E.by);

	E1 = FindEmittanceAngle(P);
	E.ax = E1.ax;
	E.by = atan2(E1.by, MeVToVelocity(GetAverageEnergy(),W0));

	E.phi = E1.phi;

	return E;
}
//---------------------------------------------------------------------------
TTwiss TBeam::GetTwissDirect(TBeamParameter P)
{
	TEllipse E;
	TTwiss T;

	E=GetEllipseDirect(P);
	T.epsilon=E.ax*E.by;
	T.beta=sqr(E.Rx)/T.epsilon;
	T.gamma=sqr(E.Ry)/T.epsilon;
	T.alpha=(T.gamma-T.beta)*tg(E.phi)/2;
	/*if (T.gamma*T.beta>1)
		T.alpha=sqrt(T.gamma*T.beta-1);
	else
		T.alpha=0; */

	return T;
}
//---------------------------------------------------------------------------
TEllipse TBeam::GetEllipse(TTwiss T)
{
	TEllipse E;
	double H=0;

	T.gamma=(1+sqr(T.alpha))/T.beta;
	H=(T.beta+T.gamma)/2;

	//phi=0.5*arctg(2*alpha*betta/(1+sqr(alpha)-sqr(betta))); - old
	//E.phi=0.5*arctg(-2*T.alpha/(T.beta-T.gamma));
	E.phi = 0.5*atan2(-2*T.alpha,T.beta-T.gamma);
	E.ax = sqrt(T.epsilon/2)*(sqrt(H+1)+sqrt(H-1));
	E.by = sqrt(T.epsilon/2)*(sqrt(H+1)-sqrt(H-1));

	E.Rx = E.ax;///h; //sigma RMS
	E.Ry = E.by;///h; //sigma RMS

	return E;
}
//---------------------------------------------------------------------------
TEllipse TBeam::GetEllipse(TBeamParameter P)
{
	TEllipse E;
	TTwiss T;
	TGauss Gx,Gy;

	Gx.mean=0;
	Gy.mean=0;

	T=GetTwiss(P);
	E=GetEllipse(T);

	if (GetLivingNumber()>1) {
		TBeamParameter P1=ComplementaryParameter(P);
		Gx=GetStatistics(P); //r/lmb
		Gy=GetStatistics(P1); //rad
	}

	E.x0=Gx.mean;
	E.y0=Gy.mean;

	return E;
}
//---------------------------------------------------------------------------
double TBeam::Get4DEmittance(bool Norm)
{
	TTwiss T=GetTwiss(RTH_PAR, Norm);
	return T.epsilon;
}
//---------------------------------------------------------------------------
TTwiss TBeam::GetTwiss(TBeamParameter P, bool Norm)
{
	TTwiss T;
	double *X=NULL, *Y=NULL, *Z=NULL;
	TGauss Gx,Gy,Gz;
	bool Square=true;
	bool E4D = P==RTH_PAR;

	T.epsilon=0;
	T.alpha=0;
	T.beta=0;

	if (E4D)
		P = R_PAR;

	//FILE *logFile;
	//logFile=fopen("TwissRead.log","a");
	TBeamParameter P1 = ComplementaryParameter(P);

	if (GetLivingNumber()>1) {
		X = GetLivingParameter(P); //r/lmb
		Y = GetLivingParameter(P1); //rad
		if (E4D)
			Z = GetLivingParameter(ATH_PAR); //rad
		if (P==TH_PAR) {
			Z = GetLivingParameter(R_PAR);
		}
	}

	double Sx=0, Spx=0, Sxpx=0;
	double Sr=0, Sprpf=0, Srpr=0, Srpf=0;
	for (int i = 0; i < Nliv; i++) {
		//if (i==0)
		   //	fprintf(logFile,"%f %f\n",X[i],Y[i]);
		if (E4D) {
			Sr += sqr(X[i]);
			//Sprpf+=sqr(Y[i])+sqr(X[i]*Z[i]);
			Sprpf += sqr(Y[i]) +sqr(Z[i]);
			Srpr += X[i]*Y[i];
		   //	Srpf+=sqr(X[i])*Z[i];
		   	Srpf += X[i]*Z[i];
		 /*	Spy+=sqr(Z[i]);
			Sxpy+=X[i]*Z[i];   */

		/*	} else if (P==TH_PAR) {
		  	Sx+=sqr(Z[i]);
			Spx+=sqr(Z[i]*Y[i]);
			Sxpx+=sqr(Z[i])*Y[i];
		 */
		} else {
			if (P==TH_PAR)
				X[i] = Z[i];
			Sx += sqr(X[i]);
			Spx += sqr(Y[i]);
			Sxpx += X[i]*Y[i];
			}
	}

   /*	if (P==TH_PAR)
		Sx=1;
	else */if (GetLivingNumber()>1) {
			Gx = GetStatistics(P); //r/lmb
			Gy = GetStatistics(P1); // rad
	  /*	if (P==R_PAR)
			Gz=GetStatistics(ATH_PAR);  */
		}

	if (E4D) {
	   //	T.epsilon=sqrt(Sx*(Spx+Spy)-sqr(Sxpx)-sqr(Sxpy))/(2*Nliv);
			T.epsilon = sqrt(Sr*Sprpf -sqr(Srpr) -sqr(Srpf))/(2*Nliv);
	  /*	if (T.epsilon==0) {
        	T.epsilon=1e-12;
		}

		T.beta=sqr(Gx.sigma)/T.epsilon;
		T.gamma=(sqr(Gy.sigma)+sqr(Gz.sigma))/T.epsilon;

		if (T.gamma*T.beta-1>0)
			T.alpha=sqrt(T.gamma*T.beta-1);
		else
			T.alpha=-((Sxpx+Sxpy)/Nliv)/T.epsilon;  */
	}else{
		T.epsilon = sqrt(Sx*Spx -sqr(Sxpx))/Nliv;
		if (T.epsilon==0) {
			T.epsilon=1e-12;
		}
		T.beta = sqr(Gx.sigma)/T.epsilon;
		//T.gamma=sqr(Gy.sigma)/T.epsilon;
	   /*	if (T.gamma*T.beta-1>0) {
			T.alpha=sqrt(T.gamma*T.beta-1);
		} else   */
		T.alpha = -(Sxpx/Nliv)/T.epsilon;
		T.gamma = (sqr(T.alpha)+1)/T.beta;
		if (P==R_PAR || P==TH_PAR){
			T.epsilon/=2;
			T.alpha/=4;
			T.beta*=2;
		}
	}

	if (Norm) {
		double W = GetAverageEnergy();
		double beta_gamma = MeVToVelocity(W,W0)*MeVToGamma(W,W0);
		T.epsilon = beta_gamma*T.epsilon;
		T.beta = T.beta/beta_gamma;
	}
	DeleteArray(X);
	DeleteArray(Y);
	DeleteArray(Z);

   //	fclose(logFile);

	return T;
}
//---------------------------------------------------------------------------
double TBeam::GetParameter(int i, TBeamParameter P)
{
	//std::cerr << "In TBeam::GetParameter(i, P) \n";
	//std::cerr << "i = " << i << ", P = " << P << '\n';

	double x=0;
	double gb0; 
	TPhaseSpace C,R;

	if (IsRectangular(P)) {
		//std::cerr << "IsRectangular = True \n";
		C.x = Particle[i].r;
		C.y = Particle[i].th;
		//IVP  C.px=Particle[i].beta.r;//Particle[i].beta;
		//IVP  C.py=Particle[i].beta.th;//Particle[i].beta;
		C.px = Particle[i].gb.r; 
		C.py = Particle[i].gb.th; 
		R = CylinricalToCartesian(C);
	   //	C=CartesianToCylinrical(R);
	}

	switch (P) {
		case (R_PAR):{
			x=mod(Particle[i].r*lmb);
			//x=Particle[i].r*lmb;
			//x=C.x*lmb;
			break;
		}
		case (TH_PAR):{
			//x=arctg(tg(Particle[i].Th));
			x=Particle[i].th;
			//x=C.y;
			break;
		}
		case (X_PAR):{
			x=R.x*lmb;
			break;
		}
		case (Y_PAR):{
			x=R.y*lmb;
			break;
		}
		/* IVP
		case (BR_PAR):{
			//x=Particle[i].beta.r;//*sign(Particle[i].r);
			x=Particle[i].beta.r*sign(Particle[i].r);
			//x=C.px;
			break;
		}
		case (BTH_PAR):{  //bth=r*th_dot
			x=Particle[i].beta.th;//*Particle[i].r;;
			//x=C.py;
			break;
		}
		case (BX_PAR):{
			x=R.px;//*Particle[i].beta;
			break;
		}
		case (BY_PAR):{
			x=R.py;//*Particle[i].beta;
			break;
		}
		case (BZ_PAR):{
			x=Particle[i].beta.z;
			break;
		}
		IVP */

		case (GBR_PAR):{
                        x = Particle[i].gb.r;//*sign(Particle[i].r); Can Particle[i].r be < 0?? 
                        //x=Particle[i].beta.r*sign(Particle[i].r);
                        break;
                }
                case (GBTH_PAR):{  //bth=r*th_dot
                        x = Particle[i].gb.th;//*Particle[i].r;;
                        break;
                }
                case (GBX_PAR):{
                        x = R.px; 
                        break;
                }
                case (GBY_PAR):{
                        x = R.py; 
                        break;
                }
                case (GBZ_PAR):{
                        x = Particle[i].gb.z;
                        break;
                }		

		case (AR_PAR):{
			//x=arctg(Particle[i].Br/Particle[i].beta);
			//IVP  x=atan2(Particle[i].beta.r,Particle[i].beta0);
			//x=atan2(C.px,Particle[i].Bz);
			gb0 = sqrt(sqr(Particle[i].g) -1.);
			x = atan2(Particle[i].gb.r, gb0);
			break;
		}
		case (ATH_PAR):{   //th'=bth/(r*bz)
			//x=arctg(C.py/Particle[i].beta)/(Particle[i].r*lmb);
			//IVP  x=atan2(Particle[i].beta.th,Particle[i].beta0);
			//x=atan2(C.py,Particle[i].Bz);
			gb0 = sqrt(sqr(Particle[i].g) -1.);
                        x = atan2(Particle[i].gb.th, gb0); 
			break;
		}
		case (AX_PAR):{
			//IVP x=atan2(R.px,Particle[i].beta.z);
			x = atan2(R.px, Particle[i].gb.z);
			break;
		}
		case (AY_PAR):{
			//IVP x=atan2(R.py,Particle[i].beta.z); 
			x = atan2(R.py, Particle[i].gb.z); 
			break;
		}
		case (AZ_PAR):{
			x=0;//atan2(Particle[i].Bz,Particle[i].beta);
			break;
		}

		case (PHI_PAR):{
			x=Particle[i].phi;
			break;
		}
		case (ZREL_PAR):{
			//x=lmb*(Particle[i].phi-Particle[i].phi0)/(2*pi);
			x=lmb*Particle[i].phi/(2*pi);
			break;
		}
		case (Z0_PAR):{
			x=Particle[i].z;
			break;
		}
		//IVP case (BETA_PAR):{
		//IVP 	x=Particle[i].beta0;
		//IVP 	break;
		//IVP }
		case (GAMMA_PAR):{
                        x = Particle[i].g;
                        break;
                }
		case (W_PAR):{  // kinetic 
			//IVP  x=VelocityToMeV(Particle[i].beta0,W0);
			x = GammaToMeV(Particle[i].g, W0);
			break;
			}
		case (LIVE_PAR):{
			x=Particle[i].lost==LIVE?1:0;
			break;
			}
		default:{
			x=-1;
			break;
		}
	}

	return x;
}
//---------------------------------------------------------------------------
TBeamParameter TBeam::ComplementaryParameter(TBeamParameter P)
{
	TBeamParameter P1;
	switch (P) {
		case R_PAR:{P1=AR_PAR;break;}
		case TH_PAR:{P1=ATH_PAR;break;}
		case X_PAR:{P1=AX_PAR;break;}
		case Y_PAR:{P1=AY_PAR;break;}
		default: {P1=P;};
	}

	return P1;
}
//---------------------------------------------------------------------------
bool TBeam::IsRectangular(TBeamParameter P)
{
	bool Rec=false;

	switch (P) {
		case X_PAR:{}
		case Y_PAR:{}
		case AX_PAR:{}
		case AY_PAR:{}
		case GBX_PAR:{}  // IVP: I think this was missing 
		case GBY_PAR:{}  // 
		case BX_PAR:{}
		case BY_PAR:{
			Rec=true;
			break;
		}
		default: Rec=false;
	}

	return Rec;
}
//---------------------------------------------------------------------------
double *TBeam::GetLivingParameter(TBeamParameter P)
{
	double *X=NULL;
	CountLiving();
	X=new double[Nliv];
	int j=0;

	for (int i=0;i<Np;i++){
		if (Particle[i].lost==LIVE) {
			X[j]=GetParameter(i,P);
			j++;
		}
	}

	return X;
}
//---------------------------------------------------------------------------
TGauss TBeam::GetStatistics(TBeamParameter P,bool FWHM)
{
	TGauss G;
	double *X=GetLivingParameter(P);

	G=GetStatistics(X,FWHM);

	return G;
}
//---------------------------------------------------------------------------
TGauss TBeam::GetStatistics(double *X,bool FWHM,bool Core)
{
	TGauss G;

	TSpectrum *Spectrum;
	Spectrum=GetSpectrum(X);

	G.mean=Spectrum->GetAverage();
	G.FWHM=FWHM?Spectrum->GetWidth():0;
	G.sigma=Spectrum->GetSquareDeviation();

	if (Core)
		G.core=Spectrum->GetCore();

	delete Spectrum;

	return G;
}
//---------------------------------------------------------------------------
TSpectrum *TBeam::GetSpectrum(TBeamParameter P)
{
	double *X=GetLivingParameter(P);

	TSpectrum *Spectrum=GetSpectrum(X);

	return Spectrum;
}
//---------------------------------------------------------------------------
TSpectrum *TBeam::GetSpectrum(double *X)
{
	TSpectrum *Spectrum;
	Spectrum=new TSpectrum;

	Spectrum->SetMesh(X,Nbars,Nliv);

	return Spectrum;
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeam::GetSpectrumBar(TBeamParameter P,bool Smooth)
{
	double *X=GetLivingParameter(P);

	TSpectrumBar *SpectrumBar=GetSpectrumBar(X,Smooth);

	return SpectrumBar;
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeam::GetSpectrumBar(double *X,bool Smooth)
{
	TSpectrum *Spectrum=GetSpectrum(X);
	TSpectrumBar *S=Spectrum->GetSpectrum(Smooth);

	TSpectrumBar *SpectrumArray;
	SpectrumArray=new TSpectrumBar[Nbars];

	for (int i=0;i<Nbars;i++) {
		 SpectrumArray[i]=S[i];
	}

	delete Spectrum;
	//delete[] X;

	return SpectrumArray;
}
//---------------------------------------------------------------------------
TGauss TBeam::GetEnergyDistribution(TDeviation D)
{
	return GetStatistics(W_PAR,D==D_FWHM);
}
//---------------------------------------------------------------------------
TGauss TBeam::GetPhaseDistribution(TDeviation D)
{
	return GetStatistics(PHI_PAR,D==D_FWHM);
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeam::GetEnergySpectrum(bool Smooth)
{
	return GetSpectrumBar(W_PAR,Smooth);
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeam::GetPhaseSpectrum(bool Smooth)
{
	return GetSpectrumBar(PHI_PAR,Smooth);
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeam::GetRSpectrum(bool Smooth)
{
	return GetSpectrumBar(R_PAR,Smooth);
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeam::GetXSpectrum(bool Smooth)
{
	return GetSpectrumBar(X_PAR,Smooth);
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeam::GetYSpectrum(bool Smooth)
{
	return GetSpectrumBar(Y_PAR,Smooth);
}
//---------------------------------------------------------------------------
double TBeam::GetPhaseLength(TDeviation D)
{
    return GetDeviation(PHI_PAR,D);
}
//---------------------------------------------------------------------------
double TBeam::GetMinPhase()
{
	return GetMinValue(PHI_PAR);
}
//---------------------------------------------------------------------------
double TBeam::GetMaxPhase()
{
	return GetMaxValue(PHI_PAR);
}
//---------------------------------------------------------------------------
double TBeam::GetMaxDivergence()
{
	double Xmin=mod(GetMinValue(AR_PAR));
	double Xmax=mod(GetMaxValue(AR_PAR));

	return Xmin>Xmax?Xmin:Xmax;
}
//---------------------------------------------------------------------------
double TBeam::GetAveragePhase()
{
	return GetMean(PHI_PAR);
}
//---------------------------------------------------------------------------
double TBeam::GetAverageEnergy()
{
	return GetMean(W_PAR);
}
//---------------------------------------------------------------------------
double TBeam::GetMaxEnergy()
{
	return GetMaxValue(W_PAR);
}
//---------------------------------------------------------------------------
double TBeam::GetCurrent()
{
	return Ib;
}
//---------------------------------------------------------------------------
double TBeam::GetInputCurrent()
{
	return I0;
}
//---------------------------------------------------------------------------
double TBeam::GetMean(TBeamParameter P)
{
	TGauss G;
	G=GetStatistics(P);

	return G.mean;
}
//---------------------------------------------------------------------------
//double GetDeviation(TBeamParameter P, TDeviation D = D_RMS);
double TBeam::GetDeviation(TBeamParameter P, TDeviation D)
{
	TGauss G;
	G=GetStatistics(P,D==D_FWHM);
	double sx=D==D_FWHM?G.FWHM:G.sigma;

	return sx;
}
//---------------------------------------------------------------------------
double TBeam::GetMinValue(TBeamParameter P)
{
	double x_min=1e32;
	double *X=GetLivingParameter(P);

	for (int i=0;i<Nliv;i++){
		if (X[i]<x_min)
			x_min=X[i];
	}
	delete[] X;

	return x_min;
}
//---------------------------------------------------------------------------
double TBeam::GetMaxValue(TBeamParameter P)
{
	double x_max=-1e32;
	double *X=GetLivingParameter(P);

	for (int i=0;i<Nliv;i++){
		if (X[i]>x_max)
			x_max=X[i];
	}
	delete[] X;

	return x_max;
}
//---------------------------------------------------------------------------
double TBeam::GetRadius(TBeamParameter P,TDeviation D)
{
	double Rb=GetDeviation(P,D);

	return Rb;
}
//---------------------------------------------------------------------------
double TBeam::GetDivergence(TBeamParameter P,TDeviation D)
{
	return GetDeviation(P,D);
}
//---------------------------------------------------------------------------
int TBeam::GetLivingNumber()
{
    CountLiving();
    return Nliv;
}
//---------------------------------------------------------------------------
double TBeam::SinSum(TIntParameters& Par, TIntegration *I)
{
    return BesselSum(Par,I,SIN);
}
//---------------------------------------------------------------------------
double TBeam::CosSum(TIntParameters& Par, TIntegration *I)
{
    return BesselSum(Par,I,COS);
}
//---------------------------------------------------------------------------
double TBeam::BesselSum(TIntParameters& Par, TIntegration *I, TTrig Trig)
{
    double S=0,N=0,S1=0;
    double phi=0,r=0,bw=0,c=0,bz=0;
    double Res=0; 
    double gamma, gammai; 

	for (int i=0;i<Np;i++){
		if (Particle[i].lost==LIVE){ 
			//IVP  bz=Particle[i].beta.z +I[i].beta.z*Par.h;
			gamma = sqrt(1. +sqr(Particle[i].gb.r) +sqr(Particle[i].gb.th) +sqr(Particle[i].gb.z)); 
			//gammai = sqrt(1. +sqr(I[i].gb.r) +sqr(I[i].gb.th) +sqr(I[i].gb.z));
			//bz = Particle[i].gb.z /gamma +I[i].gb.z *Par.h /gammai;
		        bz = Particle[i].gb.z /gamma +I[i].gb.z *Par.h /gamma;	

			if (bz<0 || bz>1) {
			  //	Particle[i].lost=BZ_LOST;
				continue;
			}
            phi=Particle[i].phi+I[i].phi*Par.h;
	    r=Particle[i].r+I[i].r*Par.h;
            bw=Par.bw;
            if (Trig==SIN)
                c=sin(phi);
            else if (Trig==COS)
                c=cos(phi);

            double arg=2*pi*sqrt(1-sqr(bw))*r/bw;
            double Bes=Ib0(arg)*c;
           /*   if (mod(Bes)>100)
                ShowMessage("this!");      */
            S+=Bes;
            N++;
        }
    }

    Res=N>0?S/N:0;

    return Res;
}
//---------------------------------------------------------------------------
double TBeam::iGetAverageEnergy(TIntParameters& Par, TIntegration *I)
{
    bool err=false;
    int j=0;

	double G=0,Gav=0,dB=0;
	double bz=0,br=0,bth=0,beta=0;
	double gamma=1, gb = 0;
	double gbr = 0, gbth = 0, gbz = 0;

	//std::cerr << "In TBeam::iGetAverageEnergy(TIntParameters& Par, TIntegration *I): \n";
	for (int i=0; i<Np; i++){
		if (Particle[i].lost==LIVE){
			/* IVP 
			bz=Particle[i].beta.z +I[i].beta.z*Par.h;
			br=Particle[i].beta.r +I[i].beta.r*Par.h;
			bth=Particle[i].beta.th +I[i].beta.th*Par.h;
			beta=sqrt(sqr(bz)+sqr(br)+sqr(bth));
			IVP  */
			gbr = Particle[i].gb.r +I[i].gb.r *Par.h;
			gbth = Particle[i].gb.th +I[i].gb.th *Par.h;
			gbz = Particle[i].gb.z +I[i].gb.z *Par.h;
			gb = sqrt(sqr(gbr) +sqr(gbth) +sqr(gbz));
			//gb = sqrt(sqr(Particle[i].gb.r) +sqr(Particle[i].gb.th) +sqr(Particle[i].gb.z));
			gamma = sqrt(1. +sqr(gb));
			beta = gb /gamma; 
			//if(i < 2){
			//std::cerr << "Particle " << i << ": beta = " << beta << ", gamma = " << gamma << '\n';
			//}
		   /*	double betta=Particle[i].beta+I[i].betta*Par.h;
			double bx=Particle[i].Br+I[i].bx*Par.h;
			double b=betta;  */
			if (mod(beta)>=1){
			  //	Particle[i].lost=LIVE;//STEP_LOST;//BZ_LOST;
				continue;
			}else{
			//if (mod(beta)<1) {
                		//IVP  G+=VelocityToEnergy(beta);
				G += gamma;
				j++;
			}
		}
	}
		//std::cerr << "G = " << G << ", j = " << j << '\n';
		//std::cerr << "Exiting TBeam::iGetAverageEnergy(., .) \n";
    if (j>0)
        Gav=G/j;
    else
        Gav=0;

    gamma=Gav;

    return gamma;
}
//---------------------------------------------------------------------------
TGauss TBeam::iGetBeamLength(TIntParameters& Par,TIntegration *I, int Nslices)
{
	TGauss G;
	int j=0;
	double *L;
	L=new double [Nliv];

	//	double L[Nliv];
	double phi=0,Iphi=0,x=0,beta=1,Ib=0,b=1;
	double gamma = 1, gammai = 1;

	CountLiving();

    for (int i=0;i<Np;i++){
		if (Particle[i].lost==LIVE){
			phi = Particle[i].phi;
			Iphi = I[i].phi;
			x = phi +Iphi*Par.h;

			/* IVP 
			beta=Particle[i].beta.z;
			Ib=I[i].beta.z;
			b=beta+Ib*Par.h;
			IVP */

			gamma = sqrt(1. +sqr(Particle[i].gb.r) +sqr(Particle[i].gb.th) +sqr(Particle[i].gb.z));
                        gammai = sqrt(1. +sqr(I[i].gb.r) +sqr(I[i].gb.th) +sqr(I[i].gb.z));
                        b = Particle[i].gb.z /gamma +I[i].gb.z *Par.h /gammai;

			L[j] = x*b*lmb/(2*pi);
			j++;
		}
	}

	bool FWHM=false,Core=true;
	G=GetStatistics(L,FWHM,Core);

	delete[] L;

	return G;
}
//---------------------------------------------------------------------------
TGauss TBeam::iGetBeamRadius(TIntParameters& Par,TIntegration *I,TBeamParameter P)
{
	TGauss G;
	int j=0;
	double *R;

	R=new double [Nliv];
	//double R[Nliv];
	double r=0, th=0,Ir=0,Ith=0,x=0;
	//TSpectrumBar *Spectrum;
	CountLiving();

    for (int i=0;i<Np;i++){
		if (Particle[i].lost==LIVE){
			r=Particle[i].r+I[i].r*Par.h;
			th=Particle[i].th+I[i].th*Par.h;
		   /*	Ir=I[i].r;
			Ith=I[i].th;   */

			switch (P) {
				case X_PAR:{
					x=r*cos(th);//+Ir*cos(Ith)*Par.h;
					break;
				}
				case Y_PAR:{
					x=r*sin(th);//+Ir*sin(Ith)*Par.h;
					break;
				}
				case R_PAR:{}
				default: {
					x=r;//+Ir*Par.h;
					break;
				}
			}
			R[j]=x*lmb;
			j++;
		}
	}
	bool FWHM=false,Core=true;
	G=GetStatistics(R,FWHM,Core);

	delete[] R;

	return G;
}
//---------------------------------------------------------------------------
/*double TBeam::iGetBeamRadius(TIntParameters& Par,TIntegration *I,bool SpectrumOutput)
{
	double X=1;
	int j=0;

	double *R,Rav=0,dR=0;
    TSpectrumBar *Spectrum;
	CountLiving();
    R=new double[Nliv];

    for (int i=0;i<Np;i++){
		if (Particle[i].lost==LIVE){
			double r=Particle[i].r+I[i].r*Par.h;
			R[j]=r;
            j++;
		}
	}

	TGauss Gr=GetStatistics(R);
	dR=Gr.sigma;


	X=dR*lmb;

	return X;
}      */
//---------------------------------------------------------------------------

/* IVP
void TBeam::Integrate(TIntParameters& Par,TIntegration **I,int Si)
{
	double Sr=0,beta0=1,gamma=1,C=0;
	double k_phi=0,k_r=0,k_th=0,k_A=0,A=0,dA=0,th_dot=0;//k_Az=0,k_Ar=0,k_Hth=0,k_bz=0,k_br=0,k_bth=0;
	double k_rr=0, k_rth=0;
	int Sj=0;
	double r=0,r0=0,phi=0,th=0;//,bz=0,br=0,bth=0;
	double s=-1;
	double rev=1;
	TField E;
	TField H;
	TField Hx,Hm;
	TField beta;
	TField k_beta;
	//logFile=fopen("beam.log","a");

	if (Reverse)
		rev=-1;

	Sj=(Si+1<Ncoef)?Si+1:0;

	CountLiving();

	Par.B*=Ib;

	//Hx=Par.Hext;

    if (Par.drift)
        I[Sj][0].A=0;//I[Si][0].A;
    else{
		A=Par.A+I[Si][0].A*Par.h;
		I[Sj][0].A=A*(Par.dL-rev*Par.w)-rev*2*Par.B*Par.SumCos;
		dA=I[Sj][0].A;
    }

	for (int i=0;i<Np;i++){
		if (Particle[i].lost==LIVE){
			beta.z=Particle[i].beta.z+I[Si][i].beta.z*Par.h;
			beta.r=Particle[i].beta.r+I[Si][i].beta.r*Par.h;
			beta.th=Particle[i].beta.th+I[Si][i].beta.th*Par.h;
			beta0=sqrt(sqr(beta.z)+sqr(beta.r)+sqr(beta.th));
			//bz=beta;  //I don't understand why, but without it, the emittance doesn't preserve
			if (beta0>1) {
            	Particle[i].lost=STEP_LOST;
				continue;
			}
			gamma=VelocityToEnergy(beta0);
			//gamma=VelocityToEnergy(beta.z);
			//C=Particle[i].Cmag;

			r=Particle[i].r+I[Si][i].r*Par.h;
			th=Particle[i].th+I[Si][i].th*Par.h;
			phi=Particle[i].phi+I[Si][i].phi*Par.h;
			Sr=2*pi*sqrt(1-sqr(Par.bw))/Par.bw;

			if (!Par.drift)
				k_phi=2*pi*(1/Par.bw-1/beta.z)+2*Par.B*Par.SumSin/A;
			else
				k_phi=2*pi*(1/Par.bw-1/beta.z);

			//RF FIELDS
			E.z=A*Ib0(r*Sr)*cos(phi)+Par.Eq[i].z;   //k_Az = Az
			E.r=-(1/Sr)*Ib1(r*Sr)*(dA*cos(phi)-(2*Par.B*Par.SumSin+2*pi*A/Par.bw)*sin(phi))+Par.Eq[i].r; //k_Ar = Ar
			E.th=Par.Eq[i].th;
			H.z=0;
			H.r=0;
			H.th=(Par.bw*A*Ib1(r*Sr)*sin(phi))/sqrt(1-sqr(Par.bw));      //k_Hth = Hth

			Hx.r=0;
			Hx.th=0;
			Hx.z=0;

			//GLOBAL EXTERNAL FIELDS (SOLENOID)
			if (Par.Hext.Field!=NULL) {
				if (Par.Hext.Dim.Ny==1)
					Hx=LinInterpolation(r*lmb,Par.Hext.Piv.X,Par.Hext.Dim.Nx,Par.Hext.Field[0]);
				else
					Hx=BiLinearInterpolation(th,r*lmb,Par.Hext.Piv.Y,Par.Hext.Piv.X,Par.Hext.Dim.Ny,Par.Hext.Dim.Nx,Par.Hext.Field);
			}

			//Hx.r=0;
		   //	Hx.th=0;

            //LOCAL EXTERNAL FIELDS (QUADS)
			if (Par.Hmap.Field!=NULL) {
				TPhaseSpace C,R;
				R.x=r*cos(th);
				R.y=r*sin(th);

				Hm=BiLinearInterpolation(R.x,R.y,Par.Hmap.Piv.X,Par.Hmap.Piv.Y,Par.Hmap.Dim.Nx,Par.Hmap.Dim.Ny,Par.Hmap.Field);
				R.px=Hm.r;
				R.py=Hm.th;
				C=CartesianToCylinrical(R);
				Hx.r+=C.px;
				Hx.th+=C.py;
			}
		   //beta.th=0;
			k_beta.z=((1-sqr(beta.z))*E.z+beta.r*(H.th+Hx.th-beta.z*E.r)-beta.th*(H.r+Hx.r+beta.z*E.th))/(gamma*beta.z);
			k_rr=r==0?0:sqr(beta.th)/(r*beta.z);
			k_beta.r=((1-sqr(beta.r))*E.r+beta.th*(H.z+Hx.z-beta.r*E.th)-beta.z*(H.th+Hx.th+beta.r*E.z))/(gamma*beta.z)+k_rr;//sqr(beta.th)/(r*beta.z);
			k_rth=r==0?0:beta.th*beta.r/(r*beta.z);
			k_beta.th=((1-sqr(beta.th))*E.th+beta.z*(H.r+Hx.r-beta.th*E.z)-beta.r*(H.z+Hx.z+beta.th*E.r))/(gamma*beta.z)-k_rth;//beta.th*beta.r/(r*beta.z);

                        //k_gb.z = (sqr(1. +gb.r*gb.r +gb.th*gb.th +gb.z*gb.z)*E.z +gb.r*(H.th +Hx.th) -gb.th*(H.r +Hx.r))/gb.z; 
                        //k_rr_gb =  r==0?0:sqr(gb.th)/(r*gb.z); 
                        //k_gb.r = (sqr(1. +gb.r*gb.r +gb.th*gb.th +gb.z*gb.z)*E.r +gb.th*(H.z +Hx.z) -gb.z*(H.th +Hx.th))/gb.z +k_rr_gb; 
                        //k_rth_gb =  r==0?0:gb.th*gb.r/(r*gb.z); 
                        //k_gb.th = (sqr(1. +gb.r*gb.r +gb.th*gb.th +gb.z*gb.z)*E.th +gb.z*(H.r +Hx.r) -gb.r*(H.z +Hx.z))/gb.z -k_rth_gb;

			//k_beta.th=0;
		     
////			k_beta.z=((1-sqr(beta.z))*E.z+beta.r*(H.th-beta.z*E.r)-beta.th*r*Par.Hext.r)/(gamma*beta.z); //k_bz = dbz/dz  ; Br=-Bz'/2!;
////			k_beta.r=((E.r-beta.z*H.th-beta.r*(beta.z*E.z+beta.r*E.r))+beta.th*Par.Hext.z)/(gamma*beta.z)+r*sqr(th_dot)/(beta.z);
////			k_beta.th=(-beta.r*H.z+beta.z*H.r-beta.th*(beta.z*E.z+beta.r*E.r))/(gamma*beta.z)-beta.r*beta.th/(r*beta.z);
			//k_bth=-(br*(C/sqr(r)+Par.Bz_ext)+r*bz*Par.dH)/(2*gamma*bz);
			//k_bth=-br*bth/(r*bz);      


			//k_r=PulseToAngle(br,bz);  //I don't understand why, but without it, the emittance doesn't preserve
			k_r=beta.r/beta.z;
			k_th=r==0?0:beta.th/(r*beta.z);
			//k_th=PulseToAngle(bth/r,bz);

		   //	if (i==0)
			   //	fprintf(logFile,"%i %f %f %f\n",i,1e3*k_r,1e3*br,1e3*bz);

			I[Sj][i].phi=k_phi;
			I[Sj][i].E.r=E.r;
			I[Sj][i].E.z=E.z;
			I[Sj][i].H.th=H.th;
			I[Sj][i].beta.z=k_beta.z;
			I[Sj][i].beta.r=k_beta.r;
			I[Sj][i].beta.th=k_beta.th;
			I[Sj][i].th=k_th;
			I[Sj][i].r=k_r;
		}
    }

   //fclose(logFile);
}
//--------------------------------------------------------------------------- 
IVP */

void TBeam::Integrate(TIntParameters& Par, TIntegration **I, int Si)    // Si from 0 to 3 
{
        double Sr=0, beta0=1, gamma=1, C=0;
        double k_phi=0,/*k_Az=0,k_Ar=0,k_Hth=0,k_bz=0,k_br=0,k_bth=0,*/k_r=0,k_th=0, k_A=0, A=0,dA=0, th_dot=0;
        double k_rr=0,    k_rth=0;
	double k_rr_gb=0, k_rth_gb=0;
        int Sj=0;
        double r=0, r0=0, phi=0, th=0;//,bz=0,br=0,bth=0;
        double s=-1;
        double rev=1;
        TField E;
        TField H;
        TField Hx,Hm;
        TField gb;
        TField k_gb;
        //logFile=fopen("beam.log","a");
	//std::cerr << "In TBeam::Integrate(TIntPar-s& Par, TInt-n **I, int Si): Si = " << Si; // << '\n';

        if (Reverse)
                rev=-1;

        Sj = (Si+1<Ncoef)?Si+1:0;
	//std::cerr << ";    set Sj = " << Sj << '\n';

        CountLiving();

        Par.B *= Ib;

        //Hx=Par.Hext;

    if (Par.drift)
        I[Sj][0].A = 0;//I[Si][0].A;
    else{
                A = Par.A +I[Si][0].A*Par.h;
                I[Sj][0].A = A*(Par.dL -rev*Par.w) -rev*2*Par.B*Par.SumCos;
                dA = I[Sj][0].A;
    }

        for (int i=0; i<Np; i++){
                if (Particle[i].lost==LIVE){
                        //IVP beta.z = Particle[i].beta.z +I[Si][i].beta.z*Par.h;
			gb.z = Particle[i].gb.z +I[Si][i].gb.z *Par.h;
                 /*     if (beta.z < 0) {
                                Particle[i].lost = BZ_LOST;
                                continue;
                        }  */
                        //IVP beta.r=Particle[i].beta.r+I[Si][i].beta.r*Par.h;
                        //IVP beta.th=Particle[i].beta.th+I[Si][i].beta.th*Par.h;
                        //IVP beta0=sqrt(sqr(beta.z)+sqr(beta.r)+sqr(beta.th));
			gb.r = Particle[i].gb.r +I[Si][i].gb.r *Par.h;
                        gb.th = Particle[i].gb.th +I[Si][i].gb.th *Par.h;
                        beta0 = sqrt(sqr(gb.z) +sqr(gb.r) +sqr(gb.th)) /sqrt(1. +sqr(gb.z) +sqr(gb.r) +sqr(gb.th)); 
                        //bz=beta;  //I don't understand why, but without it, the emittance doesn't preserve
                        if (beta0 > 1) {
                                Particle[i].lost = STEP_LOST;
                                continue;
                        }
                        //IVP gamma=VelocityToEnergy(beta0);
			gamma = sqrt(1. +sqr(gb.z) +sqr(gb.r) +sqr(gb.th)); 
                        //gamma=VelocityToEnergy(beta.z);
                        //C=Particle[i].Cmag;

                        r = Particle[i].r +I[Si][i].r *Par.h;
                        th = Particle[i].th +I[Si][i].th *Par.h;
                        phi = Particle[i].phi +I[Si][i].phi *Par.h;
                        Sr = 2*pi *sqrt(1-sqr(Par.bw)) /Par.bw;

                        if (!Par.drift)
                                //IVP k_phi=2*pi*(1/Par.bw-1/beta.z)+2*Par.B*Par.SumSin/A;
			        k_phi = 2*pi*(1/Par.bw -gamma/gb.z) +2*Par.B *Par.SumSin /A;
                        else
                                //IVP k_phi=2*pi*(1/Par.bw-1/beta.z);
			        k_phi = 2*pi*(1/Par.bw -gamma/gb.z);

                        //RF FIELDS
                        E.z = A *Ib0(r*Sr) *cos(phi) +Par.Eq[i].z;   //k_Az = Az
                        E.r = -(1/Sr) *Ib1(r*Sr) *(dA*cos(phi) -(2*Par.B *Par.SumSin +2*pi*A /Par.bw) *sin(phi)) +Par.Eq[i].r; //k_Ar = Ar
                        E.th = Par.Eq[i].th;
                        H.z=0;
                        H.r=0;
                        H.th = (Par.bw *A *Ib1(r*Sr) *sin(phi)) /sqrt(1-sqr(Par.bw));      //k_Hth = Hth

                        Hx.r=0;
                        Hx.th=0;
                        Hx.z=0;

                        //GLOBAL EXTERNAL FIELDS (SOLENOID)
                        if (Par.Hext.Field!=NULL) {
                                if (Par.Hext.Dim.Ny==1)
                                        Hx=LinInterpolation(r*lmb,Par.Hext.Piv.X,Par.Hext.Dim.Nx,Par.Hext.Field[0]);
                                else
                                        Hx=BiLinearInterpolation(th,r*lmb,Par.Hext.Piv.Y,Par.Hext.Piv.X,Par.Hext.Dim.Ny,Par.Hext.Dim.Nx,Par.Hext.Field);
                        }

                        //Hx.r=0;
                        //Hx.th=0;

            		//LOCAL EXTERNAL FIELDS (QUADS)
                        if (Par.Hmap.Field!=NULL) {
                                TPhaseSpace C,R;
                                R.x=r*cos(th);
                                R.y=r*sin(th);

                                Hm=BiLinearInterpolation(R.x,R.y,Par.Hmap.Piv.X,Par.Hmap.Piv.Y,Par.Hmap.Dim.Nx,Par.Hmap.Dim.Ny,Par.Hmap.Field);
                                R.px=Hm.r;
                                R.py=Hm.th;
                                C=CartesianToCylinrical(R);
                                Hx.r+=C.px;
                                Hx.th+=C.py;
                        }
                   //beta.th=0;
                        //IVP  k_beta.z=((1-sqr(beta.z))*E.z+beta.r*(H.th+Hx.th-beta.z*E.r)-beta.th*(H.r+Hx.r+beta.z*E.th))/(gamma*beta.z);
                        //IVP  k_rr=r==0?0:sqr(beta.th)/(r*beta.z);
                        //IVP  k_beta.r=((1-sqr(beta.r))*E.r+beta.th*(H.z+Hx.z-beta.r*E.th)-beta.z*(H.th+Hx.th+beta.r*E.z))/(gamma*beta.z)+k_rr;//sqr(beta.th)/(r*beta.z);
                        //IVP  k_rth=r==0?0:beta.th*beta.r/(r*beta.z);
                        //IVP  k_beta.th=((1-sqr(beta.th))*E.th+beta.z*(H.r+Hx.r-beta.th*E.z)-beta.r*(H.z+Hx.z+beta.th*E.r))/(gamma*beta.z)-k_rth;//beta.th*beta.r/(r*beta.z);

                        k_rr_gb =  r==0?0:sqr(gb.th)/(r*gb.z); 
                        k_gb.r  = (sqrt(1. +gb.r*gb.r +gb.th*gb.th +gb.z*gb.z)*E.r  +gb.th*(H.z +Hx.z)  -gb.z*(H.th +Hx.th))/gb.z +k_rr_gb; 
                        k_rth_gb =  r==0?0:gb.th*gb.r/(r*gb.z); 
                        k_gb.th = (sqrt(1. +gb.r*gb.r +gb.th*gb.th +gb.z*gb.z)*E.th +gb.z*(H.r +Hx.r)   -gb.r *(H.z +Hx.z))/gb.z -k_rth_gb;
			k_gb.z  = (sqrt(1. +gb.r*gb.r +gb.th*gb.th +gb.z*gb.z)*E.z  +gb.r*(H.th +Hx.th) -gb.th*(H.r +Hx.r))/gb.z;

                        //k_beta.th=0;
                   /*
                        k_beta.z=((1-sqr(beta.z))*E.z+beta.r*(H.th-beta.z*E.r)-beta.th*r*Par.Hext.r)/(gamma*beta.z); //k_bz = dbz/dz  ; Br=-Bz'/2!;
                        k_beta.r=((E.r-beta.z*H.th-beta.r*(beta.z*E.z+beta.r*E.r))+beta.th*Par.Hext.z)/(gamma*beta.z)+r*sqr(th_dot)/(beta.z);
                        k_beta.th=(-beta.r*H.z+beta.z*H.r-beta.th*(beta.z*E.z+beta.r*E.r))/(gamma*beta.z)-beta.r*beta.th/(r*beta.z);
                        //k_bth=-(br*(C/sqr(r)+Par.Bz_ext)+r*bz*Par.dH)/(2*gamma*bz);
                        //k_bth=-br*bth/(r*bz);    */


                        //k_r=PulseToAngle(br,bz);  //I don't understand why, but without it, the emittance doesn't preserve
                        //IVP  k_r=beta.r/beta.z;
                        //IVP  k_th=r==0?0:beta.th/(r*beta.z);
			k_r = gb.r /gb.z;
                        k_th = r==0?0:gb.th/(r*gb.z);
                        //k_th=PulseToAngle(bth/r,bz);

                   //   if (i==0)
                           //   fprintf(logFile,"%i %f %f %f\n",i,1e3*k_r,1e3*br,1e3*bz);

                        I[Sj][i].phi = k_phi;
                        I[Sj][i].E.r = E.r;
                        I[Sj][i].E.z = E.z;
                        I[Sj][i].H.th = H.th;
                        //IVP  I[Sj][i].beta.z=k_beta.z;
                        //IVP  I[Sj][i].beta.r=k_beta.r;
                        //IVP  I[Sj][i].beta.th=k_beta.th;
			I[Sj][i].gb.z = k_gb.z;
                        I[Sj][i].gb.r = k_gb.r;
                        I[Sj][i].gb.th = k_gb.th;
                        I[Sj][i].th = k_th;
                        I[Sj][i].r = k_r; 

                }
    }

   //fclose(logFile);
}
//---------------------------------------------------------------------------


void TBeam::Next(TBeam *nBeam, TIntParameters& Par, TIntegration **I)
{
    TParticle *nParticle;
    nParticle = nBeam->Particle;
    int Nbx=0, Nb=0;
    double dr=0, dth=0, dbz=0, dbr=0, dbth=0; 
    double dphi = 0;
    double dgbr = 0., dgbth = 0., dgbz = 0.; 
    double gamma, gb;
    //logFile=fopen("next.log","a");
    //std::cerr << '\n' << "In TBeam::Next(TBeam *nBeam, TIntParameters& Par, TIntegration **I) \n";

	for (int i=0; i<Np; i++){
		nParticle[i].lost = Particle[i].lost;
		if (Particle[i].lost == LIVE){
			nParticle[i].phi = 0;
			dphi = (I[0][i].phi +I[1][i].phi +2*I[2][i].phi +2*I[3][i].phi) *Par.h /6;
			nParticle[i].phi = Particle[i].phi +(I[0][i].phi +I[1][i].phi +2*I[2][i].phi +2*I[3][i].phi) *Par.h /6;

			dr = (I[0][i].r +I[1][i].r +2*I[2][i].r +2*I[3][i].r) *Par.h /6;
			nParticle[i].r = Particle[i].r +dr;

			dth = (I[0][i].th +I[1][i].th +2*I[2][i].th +2*I[3][i].th) *Par.h /6;
			nParticle[i].th = Particle[i].th +dth;

			/*if(i==0){
				std::cerr << "Particle 0: \n";
				std::cerr << "Par.h = " << Par.h << '\n';
				std::cerr << "I[0][0].r, .., I[3][0].r: \n";
                                std::cerr << I[0][0].r << " " << I[1][0].r << " " << I[2][0].r << " " << I[3][0].r << '\n';
				std::cerr << "Particle[0].r, dr, nParticle[0].r: \n";
                                std::cerr << Particle[i].r << " " << dr << " " << nParticle[i].r << '\n';
				std::cerr << "Particle[0].th, dth, nParticle[0].th: \n";
                                std::cerr << Particle[i].th << " " << dth << " " << nParticle[i].th << '\n';
				std::cerr << "Particle[0].phi, dphi, nParticle[0].phi: \n";
				std::cerr << Particle[i].phi << " " << dphi << " " << nParticle[i].phi << '\n';

			}*/
/* IVP 
			nParticle[i].beta.z=0;
			dbz=(I[0][i].beta.z+I[1][i].beta.z+2*I[2][i].beta.z+2*I[3][i].beta.z)*Par.h/6;
			nParticle[i].beta.z=Particle[i].beta.z+dbz; 

			//nParticle[i].beta=0;
			//nParticle[i].beta=Particle[i].beta+dbz;//(I[0][i].bz+I[1][i].bz+2*I[2][i].bz+2*I[3][i].bz)*Par.h/6;

			nParticle[i].beta.r=0;
			dbr=(I[0][i].beta.r+I[1][i].beta.r+2*I[2][i].beta.r+2*I[3][i].beta.r)*Par.h/6;
			nParticle[i].beta.r=Particle[i].beta.r+dbr;

			nParticle[i].beta.th=0;
			dbth=(I[0][i].beta.th+I[1][i].beta.th+2*I[2][i].beta.th+2*I[3][i].beta.th)*Par.h/6;
			nParticle[i].beta.th=Particle[i].beta.th+dbth; 
IVP */ 
			nParticle[i].gb.z = 0;
                        dgbz = (I[0][i].gb.z +I[1][i].gb.z +2*I[2][i].gb.z +2*I[3][i].gb.z) *Par.h /6;
                        nParticle[i].gb.z = Particle[i].gb.z +dgbz;

			nParticle[i].gb.r = 0;
                        dgbr = (I[0][i].gb.r +I[1][i].gb.r +2*I[2][i].gb.r +2*I[3][i].gb.r) *Par.h /6;
                        nParticle[i].gb.r = Particle[i].gb.r +dgbr;

                        nParticle[i].gb.th = 0;
                        dgbth = (I[0][i].gb.th +I[1][i].gb.th +2*I[2][i].gb.th +2*I[3][i].gb.th) *Par.h /6;
                        nParticle[i].gb.th = Particle[i].gb.th +dgbth;
/* IVP 
			//nParticle[i].Bz=0;
			//nParticle[i].Bz=sqrt(sqr(nParticle[i].beta)-sqr(nParticle[i].Br)-sqr(nParticle[i].Bth));
			nParticle[i].beta0=0;
			nParticle[i].beta0=sqrt(sqr(nParticle[i].beta.z)+sqr(nParticle[i].beta.r)+sqr(nParticle[i].beta.th));

			if (mod(nParticle[i].beta0)>1){
				nParticle[i].lost=BETA_LOST;
			}
			if (mod(nParticle[i].beta.r)>1){
				nParticle[i].lost=BR_LOST;
			}
			if (mod(nParticle[i].beta.th)>1){
				nParticle[i].lost=BTH_LOST;
			}
			if (nParticle[i].beta.z>1){ //|| nParticle[i].beta.z<0){
				nParticle[i].lost=BZ_LOST;
			}

			if (nParticle[i].beta.z<0){
				nParticle[i].lost=PHASE_LOST;
			}
			//if ((nParticle[i].phi+Particle[i+1].phi0)<-2*pi){
			//	nParticle[i].lost=PHASE_LOST;
			//} 
IVP */ 
			gamma = sqrt(1. +sqr(nParticle[i].gb.z) +sqr(nParticle[i].gb.r) +sqr(nParticle[i].gb.th)); 
			nParticle[i].g = gamma; 
			gb = sqrt(sqr(nParticle[i].gb.z) +sqr(nParticle[i].gb.r) +sqr(nParticle[i].gb.th)); 

			if(gb /gamma > 1.0) 
				nParticle[i].lost = BETA_LOST;

			if(mod(nParticle[i].gb.r) /gamma > 1.0) 
                                nParticle[i].lost = BETA_LOST; 

			if(mod(nParticle[i].gb.th) /gamma > 1.0)
                                nParticle[i].lost = BETA_LOST; 

			if(mod(nParticle[i].gb.z) /gamma > 1.0)
                                nParticle[i].lost = BETA_LOST; 

			if(nParticle[i].gb.z < 0.0)
                                nParticle[i].lost = BETA_LOST; 

			for (int j=0; j<4; j++){
				I[j][i].r = 0;
				I[j][i].th = 0;
				/* IVP 
				I[j][i].beta.z=0;
				I[j][i].beta.r=0;
				I[j][i].beta.th=0;
				IVP */ 
				I[j][i].gb.z = 0;
                                I[j][i].gb.r = 0;
                                I[j][i].gb.th = 0;
				I[j][i].phi = 0;
				I[j][i].E.z = 0;
				I[j][i].E.r = 0;
				I[j][i].E.th = 0;
				I[j][i].H.z = 0;
				I[j][i].H.r = 0;
				I[j][i].H.th = 0;
				I[j][i].A = 0;

            }
           /*   if (b>1)
				nParticle[i].lost=B_LOST;   */

           //   nParticle[i].Bth=(nParticle[i].Th-Particle[i].Th)*nParticle[i].x*lmb;
        }
    }
    CountLiving();
    nBeam->Ib = I0 *Nliv /Np;
	//fclose(logFile);
    //std::cerr << "Exiting TBeam::Next(., ., .) \n";

}
//---------------------------------------------------------------------------
void TBeam::Next(TBeam *nBeam)
{
    TParticle *nParticle;
    nParticle = nBeam->Particle;
    int Nbx=0, Nb=0;
    float b=0;

    for (int i=0; i<Np; i++){
        nParticle[i].lost = Particle[i].lost;
	nParticle[i].r = Particle[i].r;
	nParticle[i].th = Particle[i].th;
	nParticle[i].phi = Particle[i].phi;
	/* IVP
	nParticle[i].beta0=Particle[i].beta0;
	nParticle[i].beta.z=Particle[i].beta.z;
	nParticle[i].beta.th=Particle[i].beta.th;
	nParticle[i].beta.r=Particle[i].beta.r;
	IVP */
	nParticle[i].g = Particle[i].g; 
	nParticle[i].gb.z = Particle[i].gb.z;
        nParticle[i].gb.th = Particle[i].gb.th;
        nParticle[i].gb.r = Particle[i].gb.r;
	}
    nBeam->Ib = Ib;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
