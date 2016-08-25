//---------------------------------------------------------------------------

#ifndef OptimizerResultsH
#define OptimizerResultsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>

#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>

#include "Optimizer.h"
//---------------------------------------------------------------------------
class TOptForm : public TForm
{
__published:    // IDE-managed Components
    TPanel *MainPanel;
    TPanel *BottomPanel;
    TChart *MainChart;
    TLineSeries *LineSeries;
    TButton *CloseButton;
    TSpeedButton *EnergyButton;
    TLineSeries *AddSeries;
    TSpeedButton *CaptureButton;
    TPanel *InfoPanel;
    TSpeedButton *SpectrumButton;
    TSpeedButton *PhaseButton;
    void __fastcall CloseButtonClick(TObject *Sender);
    void __fastcall EnergyButtonClick(TObject *Sender);
    void __fastcall CaptureButtonClick(TObject *Sender);
    void __fastcall SpectrumButtonClick(TObject *Sender);
    void __fastcall PhaseButtonClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:    // User declarations
    void ClearChart();
    void DrawEnergy();
    void DrawCapture();
    void DrawSpectrum();
    void DrawPhase();
public:     // User declarations
    __fastcall TOptForm(TComponent* Owner);
    TOptimizer Optimizer;
};
//---------------------------------------------------------------------------
extern PACKAGE TOptForm *OptForm;
//---------------------------------------------------------------------------
#endif
