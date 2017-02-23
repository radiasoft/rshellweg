//---------------------------------------------------------------------------

#ifndef SplineH
#define SplineH

#include "Matrix.h"
#include "Types.h"
#include "Functions.h"

//using namespace HellwegTypes;
//---------------------------------------------------------------------------
class TSpline
{
private:
    int Nx;
    bool MeshSet;
    bool CoefReady;
    double p;

    FILE *logfile;

    TSplineCoef *Spline;
    int SplinePosition(double x);
public:
    __fastcall TSpline();
    __fastcall ~TSpline();
    void SetMesh(double *X, double *Y,int N,double *W=NULL);
    bool SoftBoundaries;

    void MakeLinearSpline(double *X,double *Y,int N);
    void MakeLinearSpline();
    void MakeCubicSpline(double *X,double *Y,int N);
    void MakeCubicSpline();
    void MakeSmoothSpline(double *X,double *Y,int N,double p,double *W=NULL);
    void MakeSmoothSpline();

    double Interpolate(double x);
    double *Interpolate(double *x,int Ny);
    double F(int i);
};
//---------------------------------------------------------------------------
#endif
