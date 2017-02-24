//---------------------------------------------------------------------------
#pragma hdrstop

#include "Optimizer.h"

//---------------------------------------------------------------------------
__fastcall TOptimizer::TOptimizer()
{
    Nres=1;
    iRes=-1;
    Structure=new TOptResult[Nres];
}
//---------------------------------------------------------------------------
__fastcall TOptimizer::~TOptimizer()
{
    delete[] Structure;
}
//---------------------------------------------------------------------------
void TOptimizer::SetDimension(int N)
{
    delete[] Structure;
    Nres=N;
    Structure=new TOptResult[Nres];
}
//---------------------------------------------------------------------------
void TOptimizer::ResetDimension(int N)
{
    if (N==Nres)
        return;

    TOptResult *Structure1;
    Structure1=new TOptResult[Nres];
    for (int i=0;i<Nres;i++)
        Structure1[i]=Structure[i];


    int Nnew=N<Nres?N:Nres;

    SetDimension(N);

    for (int i=0;i<Nnew;i++)
        Structure[i]=Structure1[i];
    
    delete[] Structure1;
}
//---------------------------------------------------------------------------
void TOptimizer::PushResult(TOptResult& Result)
{
    iRes++;
    if (iRes>=Nres)
        ResetDimension(iRes+1);

    Structure[iRes]=Result;
}
//---------------------------------------------------------------------------
TOptResult TOptimizer::GetResult(int J)
{
    int I=J<=iRes?J:iRes;
    return Structure[I];
}
//---------------------------------------------------------------------------
int TOptimizer::ResultsPushed()
{
    return iRes+1;
}
//---------------------------------------------------------------------------
void TOptimizer::Output(AnsiString& FileName)
{
    FILE *F;
    F=fopen(FileName.c_str(),"w");

    fprintf(F,"#\t");
    fprintf(F,"Length, cm\t");
    fprintf(F,"Average Energy, MeV\t");
    fprintf(F,"Maximum Energy, MeV\t");
    fprintf(F,"Energy Spectrum, %\t");
    fprintf(F,"Input Current, mA\t");
    fprintf(F,"Beam Current, mA\t");
    fprintf(F,"Captured, %\t");
    fprintf(F,"Beam Radius, mm\t");
    fprintf(F,"Average Phase, deg\t");
    fprintf(F,"Phase Length, deg\t");
    fprintf(F,"Beam Power, MW\t");
    fprintf(F,"Load Power, MW\t");
    fprintf(F,"Alpha\t");
    fprintf(F,"Betta, cm/rad\t");
    fprintf(F,"Emittance, cm*rad\t");
    fprintf(F,"\n");

    for (int i=0;i<=iRes;i++){
        fprintf(F,"%i\t",i);
        fprintf(F,"%f\t",Structure[i].Result.Length);
		fprintf(F,"%f\t",Structure[i].Result.Energy.mean);
        fprintf(F,"%f\t",Structure[i].Result.MaximumEnergy);
		fprintf(F,"%f\t",Structure[i].Result.Energy.sigma);
        fprintf(F,"%f\t",Structure[i].Result.InputCurrent);
        fprintf(F,"%f\t",Structure[i].Result.BeamCurrent);
        fprintf(F,"%f\t",Structure[i].Result.Captured);
        fprintf(F,"%f\t",Structure[i].Result.BeamRadius);
		fprintf(F,"%f\t",Structure[i].Result.Phase.mean);
		fprintf(F,"%f\t",Structure[i].Result.Phase.FWHM);
        fprintf(F,"%f\t",Structure[i].Result.BeamPower);
        fprintf(F,"%f\t",Structure[i].Result.LoadPower);
		fprintf(F,"%f\t",Structure[i].Result.RTwiss.alpha);
		fprintf(F,"%f\t",Structure[i].Result.RTwiss.beta);
        fprintf(F,"%f\t",Structure[i].Result.RTwiss.epsilon);
        fprintf(F,"\n");
    }

    fclose(F);    

}
//---------------------------------------------------------------------------

#pragma package(smart_init)
