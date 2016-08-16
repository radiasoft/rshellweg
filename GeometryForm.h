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
#define emittance_chart 0
#define portrait_chart 1
#define section_chart 2
#define energy_chart 3
#define phase_chart 4
//Envelope Chart
#define ellipse_env 0

//---------------------------------------------------------------------------
class TGeomForm : public TForm
{
__published:	// IDE-managed Components
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
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall OpenButton1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Compare1Click(TObject *Sender);
	void __fastcall ChartGroupClick(TObject *Sender);
	void __fastcall BeamGroupClick(TObject *Sender);
private:	// User declarations
	AnsiString Path;
	TBeamSolver *CompData1;
	bool C1;
	int Np_max;
	double h;
	void DrawChart(TLineSeries *Series0,TBeamSolver *Solver,TColor Col1, TColor Col2);
	void DrawBeam(TPointSeries *Series0,TBeamSolver *Solver,TColor Col1);
	void DrawBeamEnvelope(TLineSeries *Series0,TBeamSolver *Solver,int env_type,TColor Col1);
	void DrawBarChart(TBarSeries *Series0,TBeamSolver *Solver,TColor Col1);
	void SignChart(TChartType ChartType);
	void SetParameters();
	void ClearParameters();
public:		// User declarations
	__fastcall TGeomForm(TComponent* Owner);
	bool Beam;
	TBeamSolver *MainSolver;
};
//---------------------------------------------------------------------------
extern PACKAGE TGeomForm *GeomForm;
//---------------------------------------------------------------------------
#endif
