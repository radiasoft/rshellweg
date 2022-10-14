//---------------------------------------------------------------------------


#pragma hdrstop

#include "SmartProgressBar.h"

//---------------------------------------------------------------------------
void TSmartProgress::Initialize(TWinControl *ParentWindow)
{
	AnsiString S;

	TextPanel=new TPanel(ParentWindow);
	TextPanel->Parent=ParentWindow;
    TextPanel->Height=25;
    TextPanel->Align=alBottom;

    Parent=ParentWindow;
    Align=alClient;

    ProgressLabel=new TLabel(TextPanel);
    ProgressLabel->Parent=static_cast <TWinControl *> (TextPanel);
    ProgressLabel->Align=alLeft;
    SetPercent(0);

	TimeLabel=new TLabel(TextPanel);
	TimeLabel->Parent=static_cast <TWinControl *> (TextPanel);
	TimeLabel->Align=alRight;
	SetTime(0);

	MessageLabel=new TLabel(TextPanel);
	MessageLabel->Parent=static_cast <TWinControl *> (TextPanel);
	MessageLabel->Left=ParentWindow->Width*0.4;
	ShowMessage("Ready");

    Max=100;
    Min=0;
    Position=0;
}
//---------------------------------------------------------------------------
void TSmartProgress::SetPercent(double p)
{
    ProgressLabel->Caption="   Done: "+s.FormatFloat("#0",p)+"%";
}
//---------------------------------------------------------------------------
void TSmartProgress::ShowMessage(char *S)
{
    MessageLabel->Caption=S;
}
//---------------------------------------------------------------------------
void TSmartProgress::TerminateTime()
{
	TimeLabel->Caption="Time Left: Simulation is over...";
}
//---------------------------------------------------------------------------
void TSmartProgress::SetTime(int m)
{
    int h=0,M=0,sec=0;
    h=m/(60*60*1000);
    M=m/(60*1000)-60*h;
    sec=ceil(m/1000.0)-60*M-3600*h;

	TimeLabel->Caption="Time Left: "+s.FormatFloat("00 h",h)+" : "+s.FormatFloat("00 m",M)+" : "+s.FormatFloat("00 s    ",sec);
}
//---------------------------------------------------------------------------
void TSmartProgress::Set()
{
    float percent=0;

    finish = clock();
	if (Position>0 && finish-last>250){
		SetTime((Max-Position)*(finish-start)/Position);
        last=finish;

        percent=100.0*Position/Max;
		SetPercent(percent);
	}
}
//---------------------------------------------------------------------------
void TSmartProgress::Reset()
{
    Position=0;
    start = clock();
    last=start;
	Set();
	SetTime(0);
}
//---------------------------------------------------------------------------
void TSmartProgress::Reset(int M)
{
    Max=M;
    Reset();
}
//---------------------------------------------------------------------------
void TSmartProgress::operator =(int x)
{
    Position=x;
    Set();
}
//---------------------------------------------------------------------------
void TSmartProgress::operator +=(int x)
{
    Position+=x;
    Set();
}
//---------------------------------------------------------------------------
void TSmartProgress::operator ++()
{
    Position++;
    Set();
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
