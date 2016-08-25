//---------------------------------------------------------------------------

#ifndef SmartProgressBarH
#define SmartProgressBarH

#include <Vcl.ComCtrls.hpp>
#include "Types.h"

class TSmartProgress: public TProgressBar
{
private:
    TLabel *ProgressLabel;
    TLabel *TimeLabel;
    TPanel *TextPanel;

    clock_t start,finish,last;

    AnsiString s;

    void Initialize(TWinControl *ParentWindow);
    
    void Set();
public:
    __fastcall TSmartProgress(TWinControl *ParentWindow):TProgressBar(ParentWindow)
            {Initialize(ParentWindow);};

    void operator =(int x);
    void operator +=(int x);
    void operator ++();
    void Reset();
    void Reset(int M);
    void SetPercent(double p);
    void SetTime(int m);
};
//---------------------------------------------------------------------------
#endif
