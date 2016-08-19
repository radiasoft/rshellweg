//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "OptimizerResults.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptForm *OptForm;
//---------------------------------------------------------------------------
__fastcall TOptForm::TOptForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOptForm::CloseButtonClick(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------
void TOptForm::ClearChart()
{
	LineSeries->Clear();
	AddSeries->Clear();

	AnsiString S;
	if (Optimizer.Type==BUNCHER)
		MainChart->BottomAxis->Title->Caption="E,Ohm^-1/2";
	else if (Optimizer.Type==BUNCHER)
		MainChart->BottomAxis->Title->Caption="N";
}
//---------------------------------------------------------------------------
void TOptForm::DrawEnergy()
{
	ClearChart();
	int N=Optimizer.ResultsPushed();


	MainChart->Title->Caption="Energy";
	MainChart->LeftAxis->Title->Caption="W,MeV";

	for (int i=0;i<N;i++){
		TOptResult Structure;
		Structure=Optimizer.GetResult(i);
		LineSeries->AddXY(Structure.x,Structure.Result.AverageEnergy);
		AddSeries->AddXY(Structure.x,Structure.Result.MaximumEnergy);
	}
}
//---------------------------------------------------------------------------
void TOptForm::DrawCapture()
{
	ClearChart();
	int N=Optimizer.ResultsPushed();

	MainChart->Title->Caption="Capture";
	MainChart->LeftAxis->Title->Caption="k, %";

	for (int i=0;i<N;i++){
		TOptResult Structure;
		Structure=Optimizer.GetResult(i);
		LineSeries->AddXY(Structure.x,Structure.Result.Captured);
	}
}
//---------------------------------------------------------------------------
void TOptForm::DrawSpectrum()
{
	ClearChart();
	int N=Optimizer.ResultsPushed();

	MainChart->Title->Caption="Energy Spectrum";
	MainChart->LeftAxis->Title->Caption="dW,%";

	for (int i=0;i<N;i++){
		TOptResult Structure;
		Structure=Optimizer.GetResult(i);
		LineSeries->AddXY(Structure.x,Structure.Result.EnergySpectrum);
	}
}
//---------------------------------------------------------------------------
void TOptForm::DrawPhase()
{
	ClearChart();
	int N=Optimizer.ResultsPushed();

	MainChart->Title->Caption="Phase Length";
	MainChart->LeftAxis->Title->Caption="Phi,deg";

	for (int i=0;i<N;i++){
		TOptResult Structure;
		Structure=Optimizer.GetResult(i);
		LineSeries->AddXY(Structure.x,Structure.Result.PhaseLength);
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptForm::EnergyButtonClick(TObject *Sender)
{
	DrawEnergy();
}
//---------------------------------------------------------------------------
void __fastcall TOptForm::CaptureButtonClick(TObject *Sender)
{
	DrawCapture();
}
//---------------------------------------------------------------------------
void __fastcall TOptForm::SpectrumButtonClick(TObject *Sender)
{
	DrawSpectrum();	
}
//---------------------------------------------------------------------------
void __fastcall TOptForm::PhaseButtonClick(TObject *Sender)
{
	DrawPhase();
}
//---------------------------------------------------------------------------
void __fastcall TOptForm::FormShow(TObject *Sender)
{
	DrawCapture();	
}
//---------------------------------------------------------------------------
