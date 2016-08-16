//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("MainUnit.cpp", MainForm);
USEFORM("GeometryForm.cpp", GeomForm);
USEFORM("ResultsForm.cpp", ResForm);
USEFORM("OptimizerUnit.cpp", OptimizerForm);
USEFORM("OptimizerResults.cpp", OptForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TGeomForm), &GeomForm);
		Application->CreateForm(__classid(TResForm), &ResForm);
		Application->CreateForm(__classid(TOptimizerForm), &OptimizerForm);
		Application->CreateForm(__classid(TOptForm), &OptForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
