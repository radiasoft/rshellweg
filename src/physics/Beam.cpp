//---------------------------------------------------------------------------


#pragma hdrstop

#include "Beam.h"
//---------------------------------------------------------------------------
__fastcall TBeam::TBeam(int N)
{
    Np=N;
    Nbars=200;
    Kernel=0.95;
    lmb=1;
    Cmag=0;
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
    h=sqrt(-2*ln(1-Kernel));
}
//---------------------------------------------------------------------------
void TBeam::TwoRandomGauss(double& x1,double& x2)
{
    double Ran1=0;
    double Ran2=0;

    Ran1=((double)rand() / ((double)RAND_MAX + 1));
    Ran2=((double)rand() / ((double)RAND_MAX + 1));

    if (Ran1<1e-5)
        Ran1=1e-5;
        
    x1=sqrt(-2*ln(Ran1))*cos(2*pi*Ran2);
    x2=sqrt(-2*ln(Ran1))*sin(2*pi*Ran2);
}
//---------------------------------------------------------------------------
int TBeam::CountCSTParticles(AnsiString &F)
{
	return NumPointsInFile(F,2);
}
//---------------------------------------------------------------------------
/*int TBeam::CountCSTParticles(TBeamType bType)
{
    //This function should not be used
	int Np1=0, Np2=0, N=0;
    AnsiString F;

    switch (bType) {
        case CST_X:{
            F=CST_FileX;
			Np1=CountCSTParticles(F);
            Np2=Np1;
            break;
        }
        case CST_Y: {
            F=CST_FileX;
			Np1=CountCSTParticles(F);
            F=CST_FileY;
			Np2=CountCSTParticles(F);
            break;
        }
        case CST_R: {
            F=CST_FileR;
            Np1=CountCSTParticles(F);
            Np2=Np1;
            break;
        }
        case RANDOM: {
            throw std::logic_error("Unhandled TBeamType RANDOM in TBeam::CountCSTParticles");   
        }
    }

    if (Np1 == Np2) {
        N=Np1;
    }

	return N;
}        */
//---------------------------------------------------------------------------
bool TBeam::ReadCSTEmittance(AnsiString &F, int Nmax)
{
	std::ifstream fs(F.c_str());
	float x=0,vx=0;
	int i=0;
	bool Success=false;
	AnsiString S,L;

	while (!fs.eof()){
		L=GetLine(fs);

		if (NumWords(L)==2){       //Check if only two numbers in the line
			try {                  //Check if the data is really a number
				S=ReadWord(L,1);
				x=S.ToDouble();
				S=ReadWord(L,2);
				vx=S.ToDouble();
			}
			catch (...){
				continue;          //Skip incorrect line
			}
			if (i==Nmax){     //if there is more data than expected
				i++;
				break;
			}
			Particle[i].x=x/lmb;
			Particle[i].Bx=vx/c;
			i++;
		}
	}

	fs.close();

	Success=(i==Nmax);
	return Success;
}
//---------------------------------------------------------------------------
void TBeam::MakeGaussEmittance(double alpha, double betta, double eps)
{
    double a=0,b=0,phi=0,C=0;
    double gamma=0,H=0;
    double sx=0,sy=0,r=0;

    eps/=100;  //to m*rad
    betta/=100; //to m/rad

    //phi=0.5*arctg(2*alpha*betta/(1+sqr(alpha)-sqr(betta)));
    gamma=(1+sqr(alpha))/betta;
    H=(betta+gamma)/2;

    phi=0.5*arctg(-2*alpha/(betta-gamma));
    b=sqrt(eps/2)*(sqrt(H+1)+sqrt(H-1));
    a=sqrt(eps/2)*(sqrt(H+1)-sqrt(H-1));

    //C=sqrt(eps*betta/pi);
   //   a=C/sqrt(1+sqr(alpha));
   //   b=C/betta;
  //    h=1;
    sx=a;///h; //RMS
    sy=b;///h;

    double xx=0,yy=0,Rc=0,Ran1=0,Ran2=0,Myx=0,Dyx=0;
    double Mx=0; //x0
    double My=0; //x'0

    FILE *F;
	F=fopen("particleInit.log","w");
    for (int i=0;i<Np;i++){
        TwoRandomGauss(Ran1,Ran2);
        xx=Mx+Ran1*sx;
        Myx=My+Rc*sy*(xx-Mx)/sx; //ellitic distribution
        Dyx=sqr(sy)*(1-sqr(Rc)); //dispersion
        yy=Myx+Ran2*sqrt(Dyx);
        double x=xx*cos(phi)-yy*sin(phi);
        Particle[i].x=x/lmb;
       //   Particle[i].x/=lmb;
        double px=xx*sin(phi)+yy*cos(phi);     //x'=bx/bz; Emittance is defined in space x-x'
        Particle[i].Bx=px*Particle[i].betta;  //bx=x'*bz
       //   Particle[i].Bx/=lmb;
	    if (i <= 100) {
 		   fprintf(F,"%d: lmb=%g, x(m)=%g, Bx=%g, betta=%g, z(m)=%g\n",
		           i,lmb,Particle[i].x,Particle[i].Bx,Particle[i].betta,Particle[i].z);
		}
   }
   fclose(F);

}
//---------------------------------------------------------------------------
void TBeam::SetParameters(double *X,TBeamParameter Par)
{
    switch (Par) {
        case (X_PAR):{
            for (int i=0;i<Np;i++)
                Particle[i].x=X[i];
            break;
        }
        case (BX_PAR):{
            for (int i=0;i<Np;i++)
                Particle[i].Bx=X[i];
            break;
        }
        case (TH_PAR):{
            for (int i=0;i<Np;i++)
                Particle[i].Th=X[i];
            break;
        }
        case (BTH_PAR):{
            for (int i=0;i<Np;i++)
                Particle[i].Bth=X[i];
            break;
        }
        case (PHI_PAR):{
            for (int i=0;i<Np;i++)
                Particle[i].phi=X[i];
            break;
        }
        case (BETTA_PAR):{
            for (int i=0;i<Np;i++)
                Particle[i].betta=X[i];
            break;
        }
    }
}
//---------------------------------------------------------------------------
void TBeam::GetParameters(double *X,TBeamParameter Par)
{
    switch (Par) {
        case (X_PAR):{
            for (int i=0;i<Np;i++)
                X[i]=Particle[i].x;//*lmb;
            break;
        }
        case (BX_PAR):{
            for (int i=0;i<Np;i++)
                X[i]=Particle[i].Bx;
            break;
        }
        case (TH_PAR):{
            for (int i=0;i<Np;i++)
                X[i]=Particle[i].Th;
            break;
        }
        case (BTH_PAR):{
            for (int i=0;i<Np;i++)
                X[i]=Particle[i].Bth;
            break;
        }
        case (PHI_PAR):{
            for (int i=0;i<Np;i++)
                X[i]=Particle[i].phi;
            break;
        }
        case (BETTA_PAR):{
            for (int i=0;i<Np;i++)
                X[i]=Particle[i].betta;
            break;
        }
    }
}
//---------------------------------------------------------------------------
void TBeam::MakeEquiprobableDistribution(double Xav, double dX,TBeamParameter Par)
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
    SetParameters(Xi,Par);
    delete[] Xi;
}
//---------------------------------------------------------------------------
void TBeam::MakeGaussDistribution(double Xav,double sX,TBeamParameter Par)
{
    double Ran1=0,Ran2=0,xx=0;
    double *Xi;
    Xi= new double[Np];

    for (int i=0;i<Np;i++){
        Ran1=((double)rand() / ((double)RAND_MAX + 1));
        Ran2=((double)rand() / ((double)RAND_MAX + 1));
        xx=sqrt(-2*ln(Ran1))*cos(2*pi*Ran2);
        Xi[i]=Xav+xx*sX/h;
    }
    SetParameters(Xi,Par);
    delete[] Xi;
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
double TBeam::FindEmittanceAngle(double& a,double& b)
{
    TBeam *Beam1;
    Beam1=new TBeam(Np);
    Beam1->SetBarsNumber(Nbars);
    Beam1->SetKernel(Kernel);

    TSpectrum *Spectrum1,*Spectrum2;
    Spectrum1=new TSpectrum;
    Spectrum2=new TSpectrum;

    double *X,*X0,*Bx,*Bx0;;

    double Ang0=-pi/2, Ang1=pi/2, dAngle,Angle;
    double Angle1,Angle2,Res;
    double Sx=0,Sbx=0;
    double Sy=0,Sby=0;
    double Err=1e-5;
    int Nmax=30;

    X=new double[Nliv];
    Bx=new double[Nliv];

    X0=new double[Np];
    Bx0=new double[Np];

    dAngle=(Ang1-Ang0)/2;
    Angle=Ang0+dAngle;

    int i=0;

    for (int k=0;k<Np;k++){
        if (Particle[k].lost==LIVE){
            X0[k]=Particle[k].x*lmb;
        /*  double bx=0;
            if (Particle[k].betta!=0)
                bx=;
            if (Particle[k].betta==0 || mod(bx)>pi/2){
                bx=Particle[k].Bx>0?pi/2:-pi/2;
            }        */
            Bx0[k]=PulseToAngle(Particle[k].Bx,Particle[k].betta);
        }
    }

    while (dAngle>Err && i<Nmax){
        dAngle/=2;
        Angle1=Angle+dAngle;
        Angle2=Angle-dAngle;
        for (int k=0;k<Np;k++){
         /* double x=Particle[k].x*lmb;
            double bx=0;
            if (Particle[k].betta!=0)
                bx=Particle[k].Bx/Particle[k].betta;
            if (Particle[k].betta==0 || mod(bx)>pi){
                bx=Particle[k].Bx>0?pi:-pi;
            }  */
            if (Particle[k].lost==LIVE){
                Beam1->Particle[k].x=X0[k]*cos(Angle1)+Bx0[k]*sin(Angle1);
                Beam1->Particle[k].Bx=Bx0[k]*cos(Angle1)-X0[k]*sin(Angle1);
            }
        }
        int j=0;
        for(int k=0;k<Np;k++){
            if(Particle[k].lost==LIVE){
                X[j]=Beam1->Particle[k].x;
                Bx[j]=Beam1->Particle[k].Bx;
                j++;
            }
        }
        Spectrum1->SetMesh(X,Nbars,Nliv);
        Spectrum2->SetMesh(Bx,Nbars,Nliv);
        Sx=Spectrum1->GetSquareDeviation();
        Sbx=Spectrum2->GetSquareDeviation();

        for (int k=0;k<Np;k++){
            if (Particle[k].lost==LIVE){
                Beam1->Particle[k].x=X0[k]*cos(Angle2)+Bx0[k]*sin(Angle2);
                Beam1->Particle[k].Bx=Bx0[k]*cos(Angle2)-X0[k]*sin(Angle2);
            }
        }
        j=0;
        for(int k=0;k<Np;k++){
            if(Particle[k].lost==LIVE){
                X[j]=Beam1->Particle[k].x;
            //  Bx[j]=Beam1->Particle[i].Bx;
                j++;
            }
        }
        Spectrum1->SetMesh(X,Nbars,Nliv);
        Sy=Spectrum1->GetSquareDeviation();

        if (Sx>Sy)
            Angle=Angle2;
        else if (Sy>Sx)
            Angle=Angle1;
        else if (Sx==Sy)
            break;
        i++;
    }

    delete Beam1;
    delete Spectrum1;
    delete Spectrum2;
    delete[] X; delete[] Bx;
    delete[] X0;delete[] Bx0;
    
    a=Sx;
    b=Sbx;
    return Angle;
}
//---------------------------------------------------------------------------
void TBeam::GetEllipticParameters(double& x0,double& y0, double& a,double& b,double& phi, double& Rx, double& Ry)
{
    double A,sx,sy;
    TSpectrum *Spectrum1,*Spectrum2;

    CountLiving();

    Spectrum1=new TSpectrum;
    Spectrum2=new TSpectrum;

    double *X,*Bx;
    

    X=new double[Nliv];
    Bx=new double[Nliv];

  /*    GetParameters(X,X_PAR);
    GetParameters(Bx,BX_PAR);    */
    int j=0;
    for(int i=0;i<Np;i++){
        if(Particle[i].lost==LIVE){
         /* double bx=0;

            if (Particle[i].betta!=0)
                bx=Particle[i].Bx/Particle[i].betta;
            if (Particle[i].betta==0 || mod(bx)>pi/2){
                bx=Particle[i].Bx>0?pi/2:-pi/2;
            }            */

            X[j]=Particle[i].x*lmb; //x=(r/lmb)*lmb
            Bx[j]=PulseToAngle(Particle[i].Bx,Particle[i].betta);  //x'=bx/bz
            j++;
        }
    }

  /*    FILE *F1;
    F1=fopen("spectrum.log","w");
    fprintf(F1,"%i %i\n\n",Nliv,j);

    for (int i=0;i<Nliv;i++){
        fprintf(F1,"%f %f\n",X[i],Bx[i]);
    }  

    fclose(F1);   */

    Spectrum1->SetMesh(X,Nbars,Nliv);
    Spectrum2->SetMesh(Bx,Nbars,Nliv);
    x0=Spectrum1->GetAverage(); //x
    y0=Spectrum2->GetAverage();// x'
    Rx=Spectrum1->GetSquareDeviation();
    Ry=Spectrum2->GetSquareDeviation();

    phi=FindEmittanceAngle(sx,sy);

 // A=sx/sy;
    a=sx;//;*h;
    b=sy;//*h;

    delete Spectrum1;
    delete Spectrum2;
    delete[] X; delete[] Bx;
}
//---------------------------------------------------------------------------
void TBeam::GetCourantSneider(double& alpha,double& betta, double& epsilon)
{
    double x0,y0,a,b,phi,Rx,Ry;
    double sx,sy,r,h;
    double A,B,C,S,gamma;

    GetEllipticParameters(x0,y0,sx,sy,phi,Rx,Ry);
    epsilon=sx*sy;
    if (epsilon==0)
        epsilon=1e-12;
    betta=sqr(Rx)/epsilon;
    gamma=sqr(Ry)/epsilon;
    if (gamma*betta>1)
        alpha=sqrt(gamma*betta-1);
    else
        alpha=0;


   /*   A=sx/sy;
    S=(1-sqr(A))*tg(2*phi)/(2*A);
    if (S<1)
        alpha=S/sqrt(1-sqr(S));
    else
        alpha=0;

    B=sqrt(1+sqr(alpha));
    betta=A*B;
    epsilon=(pi*sx*sy)*B; */



}
//---------------------------------------------------------------------------
TSpectrumBar *TBeam::GetSpectrum(bool Smooth,double *X,double& Xav,double& dX,bool width)
{
    TSpectrumBar *S,*SpectrumArray;
    TSpectrum *Spectrum;
    Spectrum=new TSpectrum;

	
    /*if (Nliv==997)
        Sleep(50);   */
    
	Spectrum->SetMesh(X,Nbars,Nliv);
 //   FILE *F;
 //   F=fopen("yeDebug.log","a");
 //   fprintf(F,"GetSpectrum: Nbars=%d, Nliv=%d, width=%d\n",Nbars,Nliv,width);
 //   fclose(F);  


    //if (Xav!=NULL)
        Xav=Spectrum->GetAverage();
    if (width)
        dX=Spectrum->GetWidth();
    else
        dX=Spectrum->GetSquareDeviation();

    S=Spectrum->GetSpectrum(Smooth && dX!=0);
    SpectrumArray=new TSpectrumBar[Nbars];
    for (int i=0;i<Nbars;i++) {
         SpectrumArray[i]=S[i];
//         F=fopen("yeDebug.log","a");
//         fprintf(F,"Spectrum (i,S[i]): %d, %g\n",i,S[i]);
//         fclose(F);  
	}

    delete[] X;
    delete Spectrum;

    return SpectrumArray;
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeam::GetPhaseSpectrum(bool Smooth,double *Radius,double *Phase,double& FavPhase,double& dPhase,bool width)
{
    TSpectrumBar *Sphase,*SpectrumPhaseArray;
    TSpectrumPhase *SpectrumPhase;
    SpectrumPhase=new TSpectrumPhase;

	
    /*if (Nliv==997)
        Sleep(50);   */
    
	SpectrumPhase->SetPhaseMesh(Radius,Phase,Nbars,Nliv);
 //   FILE *F;
 //   F=fopen("yeDebug.log","a");
 //   fprintf(F,"GetSpectrum: Nbars=%d, Nliv=%d, width=%d\n",Nbars,Nliv,width);
 //   fclose(F);  


    //if (Xav!=NULL)
        FavPhase=SpectrumPhase->GetPhaseAverage();
    if (width)
        dPhase=SpectrumPhase->GetPhaseWidth();
    else
        dPhase=SpectrumPhase->GetPhaseSquareDeviation();

    Sphase=SpectrumPhase->GetPhaseSpectrum(Smooth && dPhase!=0);
    SpectrumPhaseArray=new TSpectrumBar[Nbars];
    for (int i=0;i<Nbars;i++) {
         SpectrumPhaseArray[i]=Sphase[i];
//         F=fopen("yeDebug.log","a");
//         fprintf(F,"Spectrum (i,S[i]): %d, %g\n",i,S[i]);
//         fclose(F);  
	}

    delete[] Phase;
    delete SpectrumPhase;

    return SpectrumPhaseArray;
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeam::GetEnergySpectrum(bool Smooth,double& Wav,double& dW)
{
    TSpectrumBar *S;
    double *B;
    CountLiving();
    
    B=new double[Nliv];
   //   GetParameters(B,BETTA_PAR);
    int j=0;
    for (int i=0;i<Np;i++){
        if (Particle[i].lost==LIVE){
            B[j]=VelocityToMeV(Particle[i].betta);
            j++;
        }
    }
    S=GetSpectrum(Smooth,B,Wav,dW,true);
    //dW*=h;
   //   delete[] B;

    return S;
}
//---------------------------------------------------------------------------
TSpectrumBar *TBeam::GetPhaseSpectrum(bool Smooth,double& Fav,double& dF)
{
    TSpectrumBar *S;
    double *Phi;
    CountLiving();
    Phi=new double[Nliv];
   //   GetParameters(Phi,PHI_PAR);
    int j=0;
    for (int i=0;i<Np;i++){
        if (Particle[i].lost==LIVE){
            Phi[j]=HellwegTypes::RadToDeg(Particle[i].phi);
            j++;
        }
    }
    
    S=GetSpectrum(Smooth,Phi,Fav,dF,true);
    //dF*=h;
   //   delete[] Phi;
    return S;
}
//---------------------------------------------------------------------------
double TBeam::GetPhaseLength()
{
    double Fav=0,dF=0;
    TSpectrumBar *Spectrum;
    Spectrum=GetPhaseSpectrum(false,Fav,dF);
    delete[] Spectrum;

    return dF;
}
//---------------------------------------------------------------------------
double TBeam::GetMinPhase()
{
	double phi_min=1e32;
	for (int i=0;i<Np;i++){
		if (Particle[i].lost==LIVE && Particle[i].phi<phi_min)
			phi_min=Particle[i].phi;
	}
	return phi_min;
}
//---------------------------------------------------------------------------
double TBeam::GetMaxPhase()
{
	double phi_max=-1e32;
	for (int i=0;i<Np;i++){
		if (Particle[i].lost==LIVE && Particle[i].phi>phi_max)
			phi_max=Particle[i].phi;
	}
	return phi_max;
}
//---------------------------------------------------------------------------
double TBeam::GetAveragePhase()
{
    double Fav=0,dF=0;
    TSpectrumBar *Spectrum;
    Spectrum=GetPhaseSpectrum(false,Fav,dF);
    delete[] Spectrum;
    
    return Fav;
}
//---------------------------------------------------------------------------
double TBeam::GetAverageEnergy()
{
    double *B,Bav=0,dB=0;
    TSpectrumBar *Spectrum;
    Spectrum=GetEnergySpectrum(false,Bav,dB);
   /*   B=new double[Nliv];
   //   GetParameters(B,BETTA_PAR);

    int j=0;
	for (int i=0;i<Np;i++){
		if (Particle[i].lost==LIVE){
			B[i]=RadToDeg(Particle[i].betta);
			j++;
		}
	}
         */
   //   Spectrum=GetSpectrum(false,B,Bav,dB);
    delete[] Spectrum;
   //   delete[] B;
    return Bav;
}
//---------------------------------------------------------------------------
double TBeam::GetMaxEnergy()
{
    double Bmax=0;
    for (int i=0;i<Np;i++){
        if (Particle[i].lost==LIVE){
            if(Particle[i].betta>Bmax)
                Bmax=Particle[i].betta;
        }
    }
    return VelocityToMeV(Bmax);
}
//---------------------------------------------------------------------------
double TBeam::GetBeamRadius()
{
    double *R,Rav,dR;
    TSpectrumBar *Spectrum;
    CountLiving();

    R=new double[Nliv];
   //   GetParameters(R,X_PAR);

    int j=0;
    for (int i=0;i<Np;i++){
        if (Particle[i].lost==LIVE){
            R[j]=Particle[i].x;
            j++;
        }
    }

    Spectrum=GetSpectrum(false,R,Rav,dR,false);
    delete[] Spectrum;
 // delete[] R;
    return dR*lmb;
}
//---------------------------------------------------------------------------
int TBeam::GetLivingNumber()
{
    CountLiving();
    return Nliv;
}
//---------------------------------------------------------------------------
double TBeam::SinSum(TIntParameters& Par,TIntegration *I)
{
    return BesselSum(Par,I,SIN);
}
//---------------------------------------------------------------------------
double TBeam::CosSum(TIntParameters& Par,TIntegration *I)
{
    return BesselSum(Par,I,COS);
}
//---------------------------------------------------------------------------
double TBeam::BesselSum(TIntParameters& Par,TIntegration *I,TTrig Trig)
{
    double S=0,N=0,S1=0;
    double phi=0,r=0,bw=0,c=0;
    double Res=0;

    for (int i=0;i<Np;i++){
        if (Particle[i].lost==LIVE){
            phi=Particle[i].phi+I[i].phi*Par.h;
            r=Particle[i].x+I[i].r*Par.h;
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
double TBeam::iGetAverageEnergy(TIntParameters& Par,TIntegration *I)
{
    double gamma=1;
    bool err=false;
    int j=0;

    double G=0,Gav=0,dB=0;
   //   TSpectrumBar *Spectrum;
    

    //logFile=fopen("beam.log","w");

    for (int i=0;i<Np;i++){
        if (Particle[i].lost==LIVE){
            double betta=Particle[i].betta+I[i].betta*Par.h;
            double bx=Particle[i].Bx+I[i].bx*Par.h;   
            // b=sqrt(sqr(betta)+sqr(bx));
            double b=betta;
            if (mod(betta)>=1)
                Particle[i].lost=STEP_LOST;//BZ_LOST;
        /*  else if (mod(bx)>=1)
                Particle[i].lost=STEP_LOST;//BX_LOST;
            else if (mod(b)>=1)
                Particle[i].lost=STEP_LOST;    */
            else{
                G+=VelocityToEnergy(b);
                j++;
            }
          //    fprintf(logFile,"%f %f %f\n",Particle[i].betta,I[i].betta,Par.h);
        }
    }

   //   fclose(logFile);

   //   Spectrum=GetSpectrum(false,B,Bav,dB);
    //delete[] Spectrum;
    if (j>0)
        Gav=G/j;
    else
        Gav=0;

    gamma=Gav;

    return gamma;
}
//---------------------------------------------------------------------------
double TBeam::iGetBeamLength(TIntParameters& Par,TIntegration *I, bool SpectrumOutput)
{
    double L=1,Fmin=1e32,Fmax=-1e32;
    int j=0;

    double *F,Fav=0,dF=0;
	double phi=0;
	double *R;
	double r=0;
	double FavPhase=0,dPhase=0;
    TSpectrumBar *SpectrumPhase;
    for (int i=0;i<Np;i++){
        if (Particle[i].lost==LIVE){
            phi=Particle[i].phi+I[i].phi*Par.h;
           /*   if (phi<=HellwegTypes::DegToRad(MinPhase) || phi>=HellwegTypes::DegToRad(MaxPhase) )
                Particle[i].lost=PHASE_LOST; */
        }
    }

    CountLiving();

    F=new double[Nliv];
    R=new double[Nliv];
    for (int i=0;i<Np;i++){
        if (Particle[i].lost==LIVE){
            phi=Particle[i].phi+I[i].phi*Par.h;
            r=Particle[i].x+I[i].r*Par.h;
            F[j]=phi;
			R[j]=r;
            j++; 
//            double phi=Particle[i].phi+I[i].phi*Par.h;
            if (phi>Fmax)
                Fmax=phi;
            if (phi<Fmin)
                Fmin=phi;
        }
    }


   /*   Spectrum=GetSpectrum(false,F,Fav,dF,true);
    delete[] Spectrum;    */
    dF=mod(Fmax-Fmin);
    L=dF*lmb/(2*pi);
	
    SpectrumPhase=GetPhaseSpectrum(false,R,F,FavPhase,dPhase,false);
//	if (SpectrumOutput) {
//        FILE *Fout;
//        Fout=fopen("yeDebug.log","a");
//        fprintf(Fout,"After GetPhaseSpectrum: RavPhase=%g, dPhase=%g, lmb=%g\n             Phase Histogram:\n",
//		        100*FavPhase, 100*dPhase,100*lmb);
//        for (int i=0;i<100;i++){
//	         fprintf(Fout,"          %d    %g     %d     %g       %g\n",
//			         i,180/pi*SpectrumPhase[i].phase*lmb,SpectrumPhase[i].N,SpectrumPhase[i].xAv*lmb,SpectrumPhase[i].xRMS*lmb);
//	    }
 //       fclose(Fout);  
//	}
    delete[] SpectrumPhase;   

    return L;
}
//---------------------------------------------------------------------------
double TBeam::iGetAveragePhase(TIntParameters& Par,TIntegration *I)
{
    double F=0,Fav=0,dF=0;
   //   TSpectrumBar *Spectrum;
    /*CountLiving();
    F=new double[Nliv];*/
    int j=0;

    for (int i=0;i<Np;i++){
        if (Particle[i].lost==LIVE){
            double phi=Particle[i].phi+I[i].phi*Par.h;
           /*   if (phi<=HellwegTypes::DegToRad(MinPhase) || phi>=HellwegTypes::DegToRad(MaxPhase) )
                Particle[i].lost=PHASE_LOST;
            else{      */
                F+=phi;
                j++;
            //}
        }
    }

   /*   Spectrum=GetSpectrum(false,F,Fav,dF);
    delete[] Spectrum;*/
    if (j>0)
        Fav=F/j;
    else
        Fav=0;

    return Fav;
}
//---------------------------------------------------------------------------
double TBeam::iGetBeamRadius(TIntParameters& Par,TIntegration *I,bool SpectrumOutput)
{
    double X=1;
    int j=0;

    double *R,Rav=0,dR=0;
    TSpectrumBar *Spectrum;
    CountLiving();
    R=new double[Nliv];

    for (int i=0;i<Np;i++){
        if (Particle[i].lost==LIVE){
            double r=Particle[i].x+I[i].r*Par.h;
            R[j]=r;
            j++;
        }
    }

    Spectrum=GetSpectrum(false,R,Rav,dR,false);
//    if (SpectrumOutput) {
//        FILE *F;
//        F=fopen("yeDebug.log","a");
//        fprintf(F,"After GetSpectrum: Rav=%g, dR=%g, lmb=%g\n             Radius Histogram:\n",
//		        100*Rav, 100*dR,100*lmb);
//        for (int i=0;i<100;i++){
//	         fprintf(F,"          %d    %g     %d\n",i,Spectrum[i].x*lmb,Spectrum[i].N);
//	    }
//        fclose(F);  
//	}
    delete[] Spectrum;

    X=dR*lmb;

    return X;
}
//---------------------------------------------------------------------------
void TBeam::Integrate(TIntParameters& Par,TIntegration **I,int Si)
{
    double bz=1,Sr=0,gamma=1,C=0;
    double k_phi=0,k_Az=0,k_Ar=0,k_Hth=0,k_bz=0,k_bx=0,k_r=0,k_th=0,k_A=0,A=0,dA=0,th_dot=0;
    int Sj=0;
    double r=0,r0=0,phi=0,bx=0,bth=0;
    double s=-1;
    double rev=1;
    if (Reverse)
        rev=-1;

    Sj=(Si+1<Ncoef)?Si+1:0;

    CountLiving();

    Par.B*=Ib;

    if (Par.drift)
        I[Sj][0].A=0;//I[Si][0].A;
    else{
        A=Par.A+I[Si][0].A*Par.h;
        I[Sj][0].A=A*(Par.dL-rev*Par.w)-rev*2*Par.B*Par.SumCos; 
        dA=I[Sj][0].A;
    }

  /*    if (!Par.drift){
        A=Par.A+I[Si][0].A*Par.h;
        dA=I[Si][0].A;
    }      */
 
    //logFile=fopen("beam.log","w");

    for (int i=0;i<Np;i++){
        if (Particle[i].lost==LIVE){
            bz=Particle[i].betta+I[Si][i].betta*Par.h;
            bx=Particle[i].Bx+I[Si][i].bx*Par.h;
            //gamma=VelocityToEnergy(sqrt(sqr(bz)+sqr(bx)));
            gamma=VelocityToEnergy(bz);
            r=Particle[i].x+I[Si][i].r*Par.h;
            r0=Particle[i].x0;
           //   bth=I[Si][i].th*r*lmb;
            phi=Particle[i].phi+I[Si][i].phi*Par.h;
            Sr=2*pi*sqrt(1-sqr(Par.bw))/Par.bw;

			if (!Par.drift)
                k_phi=2*pi*(1/Par.bw-1/bz)+2*Par.B*Par.SumSin/A;
            else
                k_phi=2*pi*(1/Par.bw-1/bz);

		  /*  if (i==100){
                FILE *F;
                F=fopen("beam.log","a");
                fprintf(F,"%f %f\n",bz,k_phi);
                fclose(F);
            }      */

			k_Az=A*Ib0(r*Sr)*cos(phi)+Par.Aqz[i];   //k_Az = Az
		 //	if (Par.bw!=1) {
				k_Ar=-(1/Sr)*Ib1(r*Sr)*(dA*cos(phi)-(2*Par.B*Par.SumSin+2*pi*A/Par.bw)*sin(phi))+Par.Aqr[i]; //k_Ar = Ar
		 /*	} else {
				k_Ar=-(r/2)*(dA*cos(phi)-(2*Par.B*Par.SumSin+2*pi*A/Par.bw)*sin(phi))+Par.Aqr[i];
			}         */


			//k_Ar=-Par.bw*Ib1(Sr)*(dA*cos(phi)-(2*Par.B*Par.SumSin+2*pi*A/Par.bw)*sin(phi))/(2*pi*sqrt(1-sqr(Par.bw)))+Par.Aqr;
		//	if (Par.bw!=1) {
				k_Hth=(Par.bw*A*Ib1(r*Sr)*sin(phi))/sqrt(1-sqr(Par.bw));      //k_Hth = Hth
		 /*	} else {
				k_Hth=(pi*A*r*sin(phi));
			}        */
			k_bz=((1-sqr(bz))*k_Az+bx*(k_Hth-bz*k_Ar)-bth*Par.Br_ext)/(gamma*bz); //k_bz = dbz/dz
           //   k_th=-Par.Bz_ext/(2*gamma*bz);      //k_th = dbth/dz =
            if (r0==0)
                C=Par.Cmag;
            else
                C=Par.Cmag*sqr(r0/r);
            k_th=(C-Par.Bz_ext)/(2*gamma*bz);
            th_dot=k_th*bz;
            //bth=th_dot*r;
            //bth=k_th*r*lmb;
    //      k_bx=(k_Ar-bz*k_Hth-bx*(bz*k_Az+bx*k_Ar))/(gamma*bz)+s*((bth*Par.Bz_ext/(bz*gamma))+(r*sqr(Par.Bz_ext/(2*gamma*bz))/bz));       //previous
            k_bx=(k_Ar-bz*k_Hth-bx*(bz*k_Az+bx*k_Ar))/(gamma*bz)+th_dot*r*Par.Bz_ext/(gamma*bz)+r*sqr(th_dot)/bz;
            k_r=PulseToAngle(bx,bz);
           /*   if (mod(k_r)>100)
                k_r=1;   */

            I[Sj][i].phi=k_phi;
            I[Sj][i].Ar=k_Ar;
            I[Sj][i].Az=k_Az;
            I[Sj][i].Hth=k_Hth;
            I[Sj][i].betta=k_bz;
            I[Sj][i].bx=k_bx;
            I[Sj][i].th=k_th;
            I[Sj][i].r=k_r;
            I[Sj][i].bth=k_th;

        /*  if (i==200){
                fprintf(logFile,"bz=%f\n",bz);
                fprintf(logFile,"k_Az=%f\n",k_Az);
                fprintf(logFile,"bx=%f\n",bx);
                fprintf(logFile,"k_Hth=%f\n",k_Hth);
                fprintf(logFile,"k_Ar=%f\n",k_Ar);
                fprintf(logFile,"bth=%f\n",bth);
                fprintf(logFile,"Br_ext=%f\n",Par.Br_ext);
                fprintf(logFile,"gamma=%f\n",Par.gamma);
            }      */
        }
    }

   //fclose(logFile);
}
//---------------------------------------------------------------------------
void TBeam::Next(TBeam *nBeam,TIntParameters& Par,TIntegration **I)
{
    TParticle *nParticle;
    nParticle=nBeam->Particle;
    int Nbx=0,Nb=0;
    float b=0;
  //    logFile=fopen("beam.log","w");

    for (int i=0;i<Np;i++){
        nParticle[i].lost=Particle[i].lost;
        if (Particle[i].lost==LIVE){
            nParticle[i].x=Particle[i].x+(I[0][i].r+I[1][i].r+2*I[2][i].r+2*I[3][i].r)*Par.h/6;
            nParticle[i].Th=Particle[i].Th+(I[0][i].th+I[1][i].th+2*I[2][i].th+2*I[3][i].th)*Par.h/6;

            nParticle[i].Bx=0;
            nParticle[i].Bx=Particle[i].Bx+(I[0][i].bx+I[1][i].bx+2*I[2][i].bx+2*I[3][i].bx)*Par.h/6;
            nParticle[i].Bth=Particle[i].Bth+(I[0][i].bth+I[1][i].bth+2*I[2][i].bth+2*I[3][i].bth)*Par.h/6;

            if (nParticle[i].Bx>=1 || nParticle[i].Bx<=-1){
                nParticle[i].lost=BX_LOST;
      //            fprintf(logFile,"%f\n",nParticle[i].Bx);
               //   Nbx++;
            }
            nParticle[i].phi=0;
            nParticle[i].phi=Particle[i].phi+(I[0][i].phi+I[1][i].phi+2*I[2][i].phi+2*I[3][i].phi)*Par.h/6;
        /*  if (nParticle[i].phi>=HellwegTypes::DegToRad(MaxPhase) || nParticle[i].phi<=HellwegTypes::DegToRad(MinPhase)){
                nParticle[i].lost=PHASE_LOST;
            }    */
            nParticle[i].betta=0;
            nParticle[i].betta=Particle[i].betta+(I[0][i].betta+I[1][i].betta+2*I[2][i].betta+2*I[3][i].betta)*Par.h/6;
            if (nParticle[i].betta>=1 || nParticle[i].betta<=-1){
                nParticle[i].lost=BZ_LOST;
              //    Nb++;
            }
           //   fprintf(logFile,"%i %f %f %f %f %f\n",i,I[1][i].betta,I[2][i].betta,I[3][i].betta,I[0][i].betta,nParticle[i].betta);

            b=sqrt(sqr(nParticle[i].betta)+sqr(nParticle[i].Bx));

            for (int j=0;j<4;j++){
                I[j][i].r=0;
                I[j][i].th=0;
                I[j][i].bx=0;
                I[j][i].bth=0;
                I[j][i].phi=0;
                I[j][i].betta=0;
                I[j][i].Ar=0;
                I[j][i].A=0;
                I[j][i].Az=0;
                I[j][i].Hth=0;
            }
           /*   if (b>1)
                nParticle[i].lost=B_LOST;   */

           //   nParticle[i].Bth=(nParticle[i].Th-Particle[i].Th)*nParticle[i].x*lmb;
        }
    }
    CountLiving();
    nBeam->Ib=I0*Nliv/Np;
    //fclose(logFile);
}
//---------------------------------------------------------------------------
void TBeam::Next(TBeam *nBeam)
{
    TParticle *nParticle;
    nParticle=nBeam->Particle;
    int Nbx=0,Nb=0;
    float b=0;

    for (int i=0;i<Np;i++){
        nParticle[i].lost=Particle[i].lost;
        nParticle[i].x=Particle[i].x;
        nParticle[i].Th=Particle[i].Th;
        nParticle[i].Bx=Particle[i].Bx;
        nParticle[i].phi=Particle[i].phi;
        nParticle[i].betta=Particle[i].betta;
        nParticle[i].Bth=Particle[i].Bth;
    }
    nBeam->Ib=Ib;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
