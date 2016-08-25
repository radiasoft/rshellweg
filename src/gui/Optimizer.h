//---------------------------------------------------------------------------

#ifndef OptimizerH
#define OptimizerH

#include "Types.h"

using namespace HellwegTypes;

class TOptimizer
{
private:
    TOptResult *Structure;
    int Nres;
    int iRes;

    void ResetDimension(int N);
public:
    __fastcall TOptimizer();
    __fastcall ~TOptimizer();
    TOptType Type;

    void SetDimension(int N);
    void PushResult(TOptResult& Result);
    void Output(AnsiString& FileName);
    TOptResult GetResult(int J);
    int ResultsPushed();
};
//---------------------------------------------------------------------------
#endif
