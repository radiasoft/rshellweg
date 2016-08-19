//---------------------------------------------------------------------------

#ifndef MatrixH
#define MatrixH

#include "Types.h"

namespace HellwegMatrix{

//---------------------------------------------------------------------------
class TMatrix
{
private:
	FILE *logfile;

	int N;
	int M;
	int K;
	int *Piv;
	double **A;
	double **X;//LU
	double **CreateArray(int N0=0,int M0=0);
	void ChangeDim(int N0,int M0);
	void DeleteArray(double **B,int N0=0,int M0=0);
	double **CloneArray(double **B);
	void CopyArray(double **B,double **C);
	void Decomposite_LU();
	void Inverse_LU();
	void InverseUpper();
	void InverseLower();
public:
	__fastcall TMatrix(int N0,int M0);
	__fastcall ~TMatrix();
	void Dim(int *N0,int *M0);
	void SetElement(int I,int J,double x);
	double GetElement(int I,int J);
	void SetPivot(int I,double x);
	double GetPivot(int I); 

	TMatrix operator+(TMatrix& B);
	TMatrix operator*(TMatrix& B);
	TMatrix &operator=(TMatrix& B);
	TMatrix &operator*=(TMatrix& B);

	void Transpose();
	void Inverse();

	bool IsSquare();
	bool IsDiagonal();

	void Ones();
	void Zeros();

};

};
//---------------------------------------------------------------------------
#endif
