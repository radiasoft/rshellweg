//---------------------------------------------------------------------------

#ifndef GeometryFormH
#define GeometryFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Menus.hpp>

#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>

#include "Types.h"
#include "BeamSolver.h"


//Geometry Chart
#define betta_chart 0
#define A_chart 1
#define ELP_chart 2
#define B_chart 3
#define alpha_chart 4
#define Ra_chart 5
#define Bz_chart 6
//Beam Chart
#define rpr_chart 0
#define xpx_chart 1
#define ypy_chart 2
#define xy_chart 3
#define rth_chart 4
#define thpth_chart 5
#define wphi_chart 6
#define w_chart 7
#define phi_chart 8
#define r_chart 9
#define x_chart 10
#define y_chart 11

//---------------------------------------------------------------------------
class TGeomForm : public TForm
{
__published:    // IDE-managed Components
    TPanel *BottomPanel;
    TPanel *WorkPanel;
    TPanel *RightPanel;
    TPanel *GraphPanel;
    TChart *GChart;
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Exit1;
    TGroupBox *CompBox1;
    TCheckBox *Compare1;
    TLabeledEdit *FileEdit1;
    TButton *OpenButton1;
    TOpenDialog *OpenDialog1;
    TShape *LineColor1;
    TLineSeries *Series2;
    TRadioGroup *ChartGroup;
    TLineSeries *Series1;
    TRadioGroup *BeamGroup;
	TPointSeries *BeamSeries1;
	TPointSeries *BeamSeries2;
	TCheckBox *EnvelopeCB;
	TCheckBox *BeamBox;
	TLineSeries *EnvelopeSeries1;
	TGroupBox *ParametersGroup;
	TBarSeries *BarSeries1;
	TLabel *aParLabel;
	TLabel *bParLabel;
	TLabel *eParLabel;
	TLabel *WavParLabel;
	TLabel *dWParLabel;
	TLabel *FavParLabel;
	TLabel *dFParLabel;
	TLabel *rParLabel;
	TLabel *enParLabel;
	TLineSeries *EnvelopeSeries2;
	void __fastcall FormShow(TObject *Sender);
    void __fastcall Exit1Click(TObject *Sender);
    void __fastcall OpenButton1Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall Compare1Click(TObject *Sender);
    void __fastcall ChartGroupClick(TObject *Sender);
    void __fastcall BeamGroupClick(TObject *Sender);
	void __fastcall BeamBoxClick(TObject *Sender);
	void __fastcall EnvelopeCBClick(TObject *Sender);
private:    // User declarations
	AnsiString Path;
    TBeamSolver *CompData1;
    bool C1;
    int Np_max;
	double h;
	void Draw();
	void Rescale();
	bool IsSpace(int index);
    bool IsSpectrum(int index);
    void DrawChart(TLineSeries *Series0,TBeamSolver *Solver,TColor Col1, TColor Col2);
    void DrawBeam(TPointSeries *Series0,TBeamSolver *Solver,TColor Col1);
	void DrawBeamEnvelope(TLineSeries *Series0,TBeamSolver *Solver,TColor Col1);
    void DrawBarChart(TBarSeries *Series0,TBeamSolver *Solver,TColor Col1);
    void SignChart(TChartType ChartType);
    void SetParameters();
    void ClearParameters();
public:     // User declarations
    __fastcall TGeomForm(TComponent* Owner);
    bool Beam;
    TBeamSolver *MainSolver;
};
//---------------------------------------------------------------------------
extern PACKAGE TGeomForm *GeomForm;
//---------------------------------------------------------------------------
#endif
