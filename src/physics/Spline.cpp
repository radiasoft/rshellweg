//---------------------------------------------------------------------------


#pragma hdrstop

#include "Spline.h"

//---------------------------------------------------------------------------
__fastcall TSpline::TSpline()
{
    Nx=1;
    MeshSet=false;
    CoefReady=false;
    SoftBoundaries=true;
    Spline=new TSplineCoef[Nx];

    //logfile=fopen("spline.log","w");
}
//---------------------------------------------------------------------------
__fastcall TSpline::~TSpline()
{
    delete[] Spline;
   //   fclose(logfile);
}
//---------------------------------------------------------------------------
void TSpline::SetMesh(double *X, double *Y,int N, double *W)
{
    delete[] Spline;
    Nx=N;
    double Wmin=1e32;

    Spline=new TSplineCoef[Nx];
    for (int i=0;i<Nx;i++){
        Spline[i].X=X[i];
        Spline[i].Y=Y[i];
    }
    if (W==NULL){
        //double *dY;
/*      dY=new double[Nx];
        for (int i=0;i<Nx-1;i++)
            dY[i]=abs(Y[i+1]-Y[i]);
        dY[Nx-1]=dY[Nx-2];
        for (int i=0;i<Nx;i++){
            if (abs(dY[i])<Wmin)
                Wmin=abs(dY[i]);
        }   */
        for (int i=0;i<Nx;i++)
          //    Spline[i].W=sqr(sqr(dY[i]/Wmin));    
            Spline[i].W=1;
        //delete[] dY;
    }else{
        for (int i=0;i<Nx;i++)
            Spline[i].W=W[i];
    }

    MeshSet=true;
}
//---------------------------------------------------------------------------
void TSpline::MakeSmoothSpline(double *X,double *Y,int N,double p0,double *W)
{
    p=p0;
    SetMesh(X,Y,N,W);
    MakeSmoothSpline();

}
//---------------------------------------------------------------------------
void TSpline::MakeSmoothSpline()
{
    if (!MeshSet){
        CoefReady=false;
        return;
    }

    double *dX,*dY,*X,*Y,*F,*dF,*d2F,*U;
    double *A,*B,*C,*D,*T,*TT;
    double r,q,s;
    HellwegMatrix::TMatrix R(Nx-2,Nx-2),Q(Nx-2,Nx),Qt(Nx-2,Nx),W(Nx,Nx),S(Nx-2,Nx-2);

    dX=new double[Nx-1];    dY=new double[Nx-1];
    X=new double[Nx]; Y=new double[Nx];
    A=new double[Nx]; B=new double[Nx];
    C=new double[Nx]; D=new double[Nx];
    dF=new double[Nx]; d2F=new double[Nx-2];
    U=new double[Nx-2]; F=new double[Nx];
    T=new double[Nx+1];TT=new double[Nx+1];

    for (int i=0;i<Nx;i++){
        X[i]=i;//Spline[i].X;
        Y[i]=Spline[i].Y;
    };

    for (int i=0;i<Nx-1;i++){
        dX[i]=X[i+1]-X[i];
        dY[i]=Y[i+1]-Y[i];
        dF[i]=dY[i]/dX[i];
    }

    for (int i=0;i<Nx-2;i++){
        if (i>0)
            R.SetElement(i,i-1,dX[i]);
        R.SetElement(i,i,2*(dX[i]+dX[i+1]));
        if (i<Nx-3)
            R.SetElement(i,i+1,dX[i+1]);
    }

    for (int i=0;i<Nx-2;i++){
        Q.SetElement(i,i,1/dX[i]);
        Q.SetElement(i,i+1,-(1/dX[i]+1/dX[i+1]));
        Q.SetElement(i,i+2,1/dX[i+1]);
    };

    for (int i=0;i<Nx;i++)
        W.SetElement(i,i,1/sqrt(Spline[i].W));

    Q*=W;
    Qt=Q;
    Qt.Transpose();
    Q*=Qt;

    for (int i=0;i<Nx-2;i++){
        for (int j=0;j<Nx-2;j++){
            r=R.GetElement(i,j);
            q=Q.GetElement(i,j);
            S.SetElement(i,j,6*(1-p)*q+p*r);
        }
    }

    S.Inverse();

   /*   for (int i=0;i<Nx-2;i++){
        for (int j=0;j<Nx-2;j++)
            fprintf(logfile,"%f ",1e7*S.GetElement(i,j));
        fprintf(logfile,"\n");
    }         */

    for (int i=0;i<Nx-2;i++)
        d2F[i]=dF[i+1]-dF[i];

    for (int i=0;i<Nx-2;i++){
        U[i]=0;
        for (int j=0;j<Nx-2;j++){
            s=S.GetElement(i,j);
            U[i]+=s*d2F[j];
        }
    }

    T[0]=0;
    T[Nx-1]=0;
    for (int i=1;i<Nx-1;i++)
        T[i]=U[i-1];
    for (int i=0;i<Nx-1;i++)
        TT[i]=T[i+1]-T[i];
    T[0]=0;
    T[Nx]=0;
    for (int i=1;i<Nx;i++)
        T[i]=TT[i-1];
    for (int i=0;i<Nx-1;i++)
        TT[i]=T[i+1]-T[i];

    for (int i=0;i<Nx;i++){
        F[i]=0;
        for (int j=0;j<Nx;j++){
            F[i]+=sqr(W.GetElement(i,j))*TT[j];
        }
        F[i]=Y[i]-6*(1-p)*F[i];
        Spline[i].F=F[i];
    }

   /*   for (int i=0;i<Nx-2;i++)
        fprintf(logfile,"%f\n",TT[i]);
      
    for (int i=0;i<Nx-2;i++){
        for (int j=0;j<Nx-2;j++)
            fprintf(logfile,"%f ",1e7*S.GetElement(i,j));
        fprintf(logfile,"\n");
    }       */

    T[0]=0;
    T[Nx-1]=0;
    for (int i=1;i<Nx-1;i++)
        T[i]=p*U[i-1];
    for (int i=0;i<Nx-1;i++)
        dY[i]=F[i+1]-F[i];
    for (int i=0;i<Nx-1;i++)
        dF[i]=dY[i]/dX[i];
    for (int i=0;i<Nx-1;i++)
        TT[i]=dF[i]-dX[i]*(T[i+1]+2*T[i]);
    for (int i=0;i<Nx;i++){
        Spline[i].A=F[i];
        Spline[i].C=3*T[i];
    }
    for (int i=0;i<Nx-1;i++){
        Spline[i].B=TT[i];
        Spline[i].D=(T[i+1]-T[i])/dX[i];
    }

    delete[] dX; delete[] X;
    delete[] dY; delete[] Y;
    delete[] A; delete[] B; delete[] C; delete[] D;
    delete[] dF; delete[]d2F;delete[] U; delete[] F;
    delete[] T;delete[]TT;

    CoefReady=true;

}
//---------------------------------------------------------------------------
void TSpline::MakeCubicSpline(double *X,double *Y,int N)
{
    SetMesh(X,Y,N);
    MakeCubicSpline();
}
//---------------------------------------------------------------------------
void TSpline::MakeCubicSpline()
{
    if (!MeshSet){
        CoefReady=false;
        return;
    }

    double *dX,*dY,*X,*Y;
    double *A,*B,*C,*D,*alpha,*betta;

    dX=new double[Nx-1];
    dY=new double[Nx-1];
    X=new double[Nx];
    Y=new double[Nx];
    A=new double[Nx];
    B=new double[Nx];
    C=new double[Nx];
    D=new double[Nx];
    alpha=new double[Nx];
    betta=new double[Nx];

    for (int i=0;i<Nx;i++){
        X[i]=Spline[i].X;
        Y[i]=Spline[i].Y;
    };

    for (int i=0;i<Nx-1;i++){
        dX[i]=X[i+1]-X[i];
        dY[i]=Y[i+1]-Y[i];
    }

    A[0]=0;
    B[0]=1;
    C[0]=0;
    D[0]=0;
    alpha[0]=0;
    betta[0]=0;
    A[Nx-1]=0;
    B[Nx-1]=1;
    C[Nx-1]=0;
    D[Nx-1]=0;

    for (int i=1;i<Nx-1;i++){
        A[i]=dX[i-1];
        B[i]=2*(dX[i]+dX[i-1]);
        C[i]=dX[i];
        D[i]=3*dY[i]/dX[i]-3*dY[i-1]/dX[i-1];
        alpha[i]=-C[i]/(A[i]*alpha[i-1]+B[i]);
        betta[i]=(D[i]-A[i]*betta[i-1])/(alpha[i-1]*A[i]+B[i]);
    }

    Spline[Nx-1].C=(D[Nx-1]-A[Nx-1]*betta[Nx-2])/(A[Nx-1]*alpha[Nx-2]+B[Nx-1]);
    for (int i=Nx-2;i>0;i--)
        Spline[i].C=-C[i]*Spline[i+1].C/(A[i]*alpha[i-1]+B[i])+(D[i]-A[i]*betta[i-1])/(A[i]*alpha[i-1]+B[i]);

    Spline[Nx-1].A=Y[Nx-1];
    Spline[0].C=0;
    Spline[Nx-1].C=0;

    for (int i=0;i<Nx-1;i++){
        Spline[i].A=Y[i];
        Spline[i].B=dY[i]/dX[i]-dX[i]*(Spline[i+1].C+2*Spline[i].C)/3;
        Spline[i].D=(Spline[i+1].C-Spline[i].C)/(3*dX[i]);
    };

    delete[] dX; delete[] X;
    delete[] dY; delete[] Y;
    delete[] A; delete[] B; delete[] C; delete[] D;
    delete[] alpha; delete[] betta;

    CoefReady=true;
}
//---------------------------------------------------------------------------
void TSpline::MakeLinearSpline(double *X,double *Y,int N)
{
    SetMesh(X,Y,N);
    MakeLinearSpline();
}
//---------------------------------------------------------------------------
void TSpline::MakeLinearSpline()
{
    if (!MeshSet){
        CoefReady=false;
        return;
    }

    double *X,*Y;

    X=new double[Nx];
    Y=new double[Nx];

    for (int i=0;i<Nx;i++){
        X[i]=Spline[i].X;
        Y[i]=Spline[i].Y;
    };

    for (int i=0;i<Nx-1;i++){
        Spline[i].B=(Y[i+1]-Y[i])/(X[i+1]-X[i]);
        Spline[i].A=Y[i];
        Spline[i].C=0;
        Spline[i].D=0;
    }

    delete[] X;
    delete[] Y;

    CoefReady=true;
}
//---------------------------------------------------------------------------
int TSpline::SplinePosition(double x)
{
    int Pos=0;

    if (x<Spline[0].X)
        Pos=0;
    else if(x>Spline[Nx-1].X)
        Pos=Nx-2;
    else{
        for (int i=0;i<Nx-1;i++){
            if (x>=Spline[i].X && x<=Spline[i+1].X){
                Pos=i;
                break;
            }
        }
    }

    return Pos;
}
//---------------------------------------------------------------------------
double TSpline::F(int i)
{
    return Spline[i].F;
}
//---------------------------------------------------------------------------
double *TSpline::Interpolate(double *x,int Ny)
{
    double *y;
    y=new double[Ny];

    for (int i=0;i<Ny;i++)
        y[i]=Interpolate(x[i]);

    return y;
}
//---------------------------------------------------------------------------
double TSpline::Interpolate(double x)
{
    if (!CoefReady)
        return 0;

    double y=0;
    double A,B,C,D,X;
    int k=0;
    k=SplinePosition(x);

    A=Spline[k].A;
    B=Spline[k].B;
    C=Spline[k].C;
    D=Spline[k].D;
    X=Spline[k].X;

    if (SoftBoundaries && x<Spline[0].X)
        y=Spline[0].Y;
    else if (SoftBoundaries && x>Spline[Nx-1].X)
        y=Spline[Nx-1].Y;
    else
        y=A+B*(x-X)+C*sqr(x-X)+D*cub(x-X);

    return y;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
