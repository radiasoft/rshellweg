//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "OptimizerUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptimizerForm *OptimizerForm;
//---------------------------------------------------------------------------
__fastcall TOptimizerForm::TOptimizerForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOptimizerForm::FormShow(TObject *Sender)
{
  //    BuncherPanel->Width=MainPanel->Width/2;
	FrequencyEdit->Text=Solver->GetFrequency(0);
    PowerEdit->Text=Solver->GetPower(0);
    ChangeSweep();
    Abort=false;
    ResultsButton->Enabled=false;
    RefreshStart();
    ChangeStart();
  //    Optimizer=new TOptimizer;
}
//---------------------------------------------------------------------------
void __fastcall TOptimizerForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  //    delete Optimizer;
}
//---------------------------------------------------------------------------
void __fastcall TOptimizerForm::CloseButtonClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void TOptimizerForm::ChangeSweep()
{
    ParameterCombo->Enabled=(OptimumRadio->Checked);
    SamplesSpin->Enabled=(SweepRadio->Checked);
}
//---------------------------------------------------------------------------
void TOptimizerForm::ChangeStart()
{
    StartCellSpin->Enabled=StartCellCheck->Checked;
    if (StartCellCheck->Checked)
        ShowMessage("Please be sure to select enough cells. The energy oscillations may lead to erroreuos result!");
}
//---------------------------------------------------------------------------
void TOptimizerForm::RefreshStart()
{
    StartCellSpin->MinValue=1;
    StartCellSpin->MaxValue=Solver->GetNumberOfCells();
    StartCellSpin->Value=Solver->GetNumberOfCells();
}
//---------------------------------------------------------------------------
void __fastcall TOptimizerForm::FormResize(TObject *Sender)
{
//  BuncherPanel->Width=MainPanel->Width/2;
}
//---------------------------------------------------------------------------
void __fastcall TOptimizerForm::SweepRadioClick(TObject *Sender)
{
    ChangeSweep();  
}
//---------------------------------------------------------------------------
void __fastcall TOptimizerForm::OptimumRadioClick(TObject *Sender)
{
    ChangeSweep();
}
//---------------------------------------------------------------------------
void __fastcall TOptimizerForm::ParameterComboChange(TObject *Sender)
{
    if (ParameterCombo->ItemIndex!=pCapture)
        ShowMessage("The programme's algrithm uses the phase velocity and field dependenced suited for the best capture. For other parameters these dependances may lead to the results that are not optimum!");
}
//---------------------------------------------------------------------------
void __fastcall TOptimizerForm::FormCanResize(TObject *Sender, int &NewWidth,
      int &NewHeight, bool &Resize)
{
    Resize=false;   
}
//---------------------------------------------------------------------------
bool TOptimizerForm::PrepareCell(TCell& Cell)
{
    bool Res=true;
    try{
        Cell.F0=1e6*FrequencyEdit->Text.ToDouble();
    } catch(...){
        ShowMessage("Frequency value is not correct!");
        Res=false;
    }
    try{
        Cell.P0=1e6*PowerEdit->Text.ToDouble();
    } catch(...){
        ShowMessage("Power value is not correct!");
        Res=false;
    }
    if (ModeCombo->ItemIndex==m90)
        Cell.Mode=90;
    else if (ModeCombo->ItemIndex==m60)
        Cell.Mode=120;

    return Res;
}
//---------------------------------------------------------------------------
bool TOptimizerForm::BuncherInput(TCell& Cell)
{
    bool R=false;

    if (!PrepareCell(Cell))
        return false;

    try{
        E1=LimitEdit1->Text.ToDouble();
        E2=LimitEdit2->Text.ToDouble();
        if (E2<E1){
            double E=E2;
            E2=E1;
            E1=E;
        }
        if (FieldCombo->ItemIndex==fA){
            E1=E1*We0/sqrt(Cell.P0);
            E2=E2*We0/sqrt(Cell.P0);
        }else if (FieldCombo->ItemIndex==fE){
            double lmb=c/Cell.F0;
            E1=1e6*E1*lmb/sqrt(Cell.P0);
            E2=1e6*E2*lmb/sqrt(Cell.P0);
        }
        R=true;
    } catch(...){
        ShowMessage("Field limits are incorrect!");
    }

    return R;
}
//---------------------------------------------------------------------------
double TOptimizerForm::GetValue(TOptResult& Structure)
{
    double y=0;

    switch (ParameterCombo->ItemIndex){
        case pCapture: y=100-Structure.Result.Captured; break;
		case pESpec: y=Structure.Result.Energy.sigma; break;
        case pFSpec: y=Structure.Result.Phase.FWHM; break;
    }

    return y;
}
//---------------------------------------------------------------------------
double TOptimizerForm::GetEnergy(TOptResult& Structure)
{
	double y=Structure.Result.Energy.mean;
    return y;
}
//---------------------------------------------------------------------------
TOptResult TOptimizerForm::Solve(bool Save)
{
    TOptResult Structure;
    AnsiString FileName,SaveName;

    try{
        Solver->CreateGeometry();
        Solver->CreateBeam();
        Solver->Solve();
        
        FileName="Results\\OUTPUT"+IntToStr(counter)+".txt";
        Structure.Result=Solver->Output(FileName);
            //Optimizer.PushResult(Structure);
        if (SaveGraphCheck->Checked){
            SaveName="Results\\OUTPUT"+IntToStr(counter)+".res";
            Solver->SaveToFile(SaveName);
        }

        if(Save)
            counter++;

    }  catch (...){
        ShowMessage("Error occured while solving the task. Check the values in input file!");
    }

    return Structure;
}
//---------------------------------------------------------------------------
void TOptimizerForm::Sweep()
{
    TCell Cell;
    int Nmax=SamplesSpin->Value;
    AnsiString FileName;
    TOptimizer Optimizer;
    TOptResult Structure;
    Optimizer.Type=BUNCHER;
    int i=0;

    OptimizerProgress->Min=0;
    OptimizerProgress->Max=Nmax;
    OptimizerProgress->Position=0;
    Optimizer.SetDimension(Nmax);

    if (!BuncherInput(Cell))
        return;

    for (int i=0;i<Nmax;i++){
        Cell.ELP=E1+i*(E2-E1)/(Nmax-1);
        Solver->MakeBuncher(Cell);
        Structure=Solve();
        Structure.x=Cell.ELP;
        Optimizer.PushResult(Structure);
        OptimizerProgress->Position++;

        if (Abort){
            Abort=false;
            break;
        }
    }

    FileName="OPTIMIZER.TXT";
    Optimizer.Output(FileName);
    ResultsButton->Enabled=true;
    OptForm->Optimizer=Optimizer;
}
//---------------------------------------------------------------------------
void TOptimizerForm::OptimizeBuncher()
{
    TCell Cell;
    AnsiString FileName;
    TOptimizer Optimizer;
    TOptResult Structure;
    Optimizer.Type=BUNCHER;
    int i=0;
    double eps=Nerr;
    
    OptimizerProgress->Min=0;
    OptimizerProgress->Max=100;
    OptimizerProgress->Position=0;
    Optimizer.SetDimension(3);

    double x0=0,x1=0,x2=0,y1=0,y2=0,y0=0;
    double Nmax=0;

    if (!BuncherInput(Cell))
        return;

    x1=E1;
    x2=E2;

    Nmax=CountSteps(E2-E1,eps)+2;
    OptimizerProgress->Min=0;
    OptimizerProgress->Max=Nmax;
    OptimizerProgress->Position=0;

    Cell.ELP=x1;
    Solver->MakeBuncher(Cell);
    Structure=Solve();
    Structure.x=Cell.ELP;
    y1=GetValue(Structure);
    Optimizer.PushResult(Structure);
    OptimizerProgress->Position++;

    Cell.ELP=x2;
    Solver->MakeBuncher(Cell);
    Structure=Solve();
    Structure.x=Cell.ELP;
    y2=GetValue(Structure);
    Optimizer.PushResult(Structure);
    OptimizerProgress->Position++;

    while (x2-x1>eps){
        x0=(x1+x2)/2;

        Cell.ELP=x0;
        Solver->MakeBuncher(Cell);
        Structure=Solve();
        Structure.x=Cell.ELP;
        y0=GetValue(Structure);
        Optimizer.PushResult(Structure);
        OptimizerProgress->Position++;

        if (y1<=y0 && y0<=y2){
            x2=x0;
            y2=y0;
        }
        else if ((y0<=y1 && y0<=y2) || (y0>=y1 && y0>=y2)){
            if (y1<=y2){
                x2=x0;
                y2=y0;
            }else{
                x1=x0;
                y1=y0;
            }
        }
        else if (y2<=y0 && y0<=y1){
            x1=x0;
            y1=y0;
        }
    }
    FileName="OPTIMIZER.TXT";
    Optimizer.Output(FileName);
    ResultsButton->Enabled=true;
    OptForm->Optimizer=Optimizer;

}
//---------------------------------------------------------------------------
void TOptimizerForm::OptimizeCurrent(TOptResult& Structure)
{
    double I0=Structure.Result.InputCurrent;
	double W0=Structure.Result.Energy.mean;
    double dI=CurrentStep, eps=CurrentErr,t=1; //mA

    double I2=0,I1=0,W1=0;
    AnsiString M,s;

    int i=1;

    double x1=0,x2=0,y1=0,y2=0,x0=0,y0=0;
    TOptResult S1,S2,S0;

    if (W0==W)
        return;

    I2=I0;
    S2=Structure;

    if (W0<W){  //decrease current
        do {
            if (t==1){
                W1=W0;
                I1=I2;
                S1=S2;

            }
            I2=I0-i*t*dI;
            if (I2<=0){
                if (t*dI<eps){
                    M="Energy Limit "+s.FormatFloat("#.00",W)+" MeV at zero current";
                    ShowMessage(M);
                    Structure=S2;
                    return;
                }
                t*=0.5;
                continue;
            }
            Solver->ChangeInputCurrent(1e-3*I2);
            S2=Solve();
            W0=GetEnergy(S2);
            OptimizerProgress->Max++;
            OptimizerProgress->Position++;
            
            i++;
            t=1;
        } while (W0<W);
        y0=I1;
        I1=I2;
        I2=y0;
        y0=W0;
        W0=W1;
        W1=y0;
        S0=S1;
        S1=S2;
        S2=S0;
    } else if (W0>W){
        do {
            if (t==1){
                W1=W0;
                I1=I2;
                S1=S2;
            }
            I2=I0+i*t*dI;
            Solver->ChangeInputCurrent(1e-3*I2);
            S2=Solve();
            W0=GetEnergy(S2);
            OptimizerProgress->Max++;
            OptimizerProgress->Position++;
            if (W0>W1){
                if (t*dI<eps){
                    M="Current Limit "+s.FormatFloat("#.00",I2)+" mA acheived";
                    ShowMessage(M);
                    Structure=S2;
                } else {
                    t*=0.5;
                    continue;
                }
            }
            i++;
            t=1;
        } while (W0>W);
    }

    x1=I1;
    x2=I2;
    y1=W1;
    y2=W0;

    while (x2-x1>eps){
        x0=(x1+x2)/2;
        Solver->ChangeInputCurrent(1e-3*x0);
        S0=Solve();
        y0=GetEnergy(S0);
        OptimizerProgress->Max++;
        OptimizerProgress->Position++;

        if (y0>W){
            x1=x0;
            y1=y0;
            S1=S0;
        } else if (W>y0){
            x2=x0;
            y2=y0;
            S2=S0;
        }else{
            Structure=S0;
            return;
        }
    }

    Structure=(mod(y2-W)<mod(y1-W))?S2:S1;


}
//---------------------------------------------------------------------------
void TOptimizerForm::OptimizeImpedance()
{
    AnsiString FileName;
    AnsiString M,s;

    TOptimizer Optimizer;
    TOptResult Structure;
    Optimizer.Type=ACCELERATOR;
    int i=0,dN=Nstep,N=0;
    double eps=Nerr;

    Optimizer.SetDimension(3);

    double W0=0,W1=0,W2=0;
    double Wo=0;
    double Nmax=0;

    try{
        W=EnergyEdit->Text.ToDouble();
    } catch (...){
        ShowMessage("Energy value is incorrect");
        return;
    }

    Solver->LoadData();
    Structure=Solve();
    Structure.x=0;
    W0=GetEnergy(Structure);
    Optimizer.PushResult(Structure);
    OptimizerProgress->Max+=2;
    OptimizerProgress->Position++;

    i++;

    if (W0>W){
        M="Target Energy "+s.FormatFloat("#.00",W)+" MeV is lesser than Buncher Energy "+s.FormatFloat("#.00",W0)+" MeV";
        ShowMessage(M);
        return;
    }

    do {
        W1=W0;
        Solver->LoadData();
        Solver->AddCells(dN*i);
        Structure=Solve();
        Structure.x=dN*i;
        W0=GetEnergy(Structure);
        Optimizer.PushResult(Structure);
        OptimizerProgress->Max++;
        OptimizerProgress->Position++;
        i++;
    } while (!((W0>W) || (W0<W1)));

    int j=i,x0=0,x1=0,x2=0;
    double y1=0,y2=0,y0=0;

    x1=dN*(i-2);
    x2=dN*(i-1);
    y1=W1;
    y2=W0;

    if (W0>W){
        while (x2-x1>eps){
            x0=(x1+x2)/2;
            Solver->LoadData();
            Solver->AddCells(x0);
            Structure=Solve();
            Structure.x=x0;
            y0=GetEnergy(Structure);
            Optimizer.PushResult(Structure);
            OptimizerProgress->Max++;
            OptimizerProgress->Position++;

            if (y0>W){
                x2=x0;
                y2=y0;
            } else if (W>y0){
                x1=x0;
                y1=y0;
            }else{
                break;
            }
        }
        if(mod(y2-W)<mod(y1-W)){
            N=x2;
            Wo=y2;
        } else {
            N=x1;
            Wo=y1;
        }
        
        if (ExactCheck->Checked){
            OptimizeCurrent(Structure);
			M="Output Energy "+s.FormatFloat("#.00",Structure.Result.Energy.mean)+" MeV is acheved at "+s.FormatFloat("0",N)+" additional cells and "+s.FormatFloat("#.00",Structure.Result.InputCurrent)+" mA input current";
        } else{
            M="Output Energy "+s.FormatFloat("#.00",Wo)+" MeV is acheved at "+s.FormatFloat("0",N)+" additional cells";
        }
        ShowMessage(M);

    } else if (W0<W1){
        y2=W1;
        for (int k=x1+1;k<x2;k++){
            y1=y2;
            Solver->LoadData();
            Solver->AddCells(k);
            Structure=Solve();
            Structure.x=k;
            y2=GetEnergy(Structure);
            Optimizer.PushResult(Structure);
            OptimizerProgress->Max++;
            OptimizerProgress->Position++;

            if (y2>W){
                if(mod(y2-W)<mod(y1-W)){
                    N=x2;
                    Wo=y2;
                } else {
                    N=x1;
                    Wo=y1;
                }
                if (ExactCheck->Checked){
                    OptimizeCurrent(Structure);
					M="Output Energy "+s.FormatFloat("#.00",Structure.Result.Energy.mean)+" MeV is acheved at "+s.FormatFloat("0",N)+" additional cells and "+s.FormatFloat("#.00",Structure.Result.InputCurrent)+" mA input current";
                } else{
                M="Output Energy "+s.FormatFloat("#.00",Wo)+" MeV is acheved at "+s.FormatFloat("0",N)+" additional cells";
                }
                ShowMessage(M);
                break;
            } else if (y2<y1){
                Wo=y2>y1?y2:y1;

                M="Output Energy is unacheivable. Estimated Energy Limit "+s.FormatFloat("#.00",Wo)+". Try to lower the target energy";
                ShowMessage(M);
                break;
            }
        }

    }
    
    OptimizerProgress->Position++;
    FileName="OPTIMIZER.TXT";
    Optimizer.Output(FileName);
    ResultsButton->Enabled=true;
    OptForm->Optimizer=Optimizer;
                
}
//---------------------------------------------------------------------------
void TOptimizerForm::OptimizeGradient()
{
    AnsiString FileName;
    AnsiString M,s;

    TOptimizer Optimizer;
    TOptResult Structure;
    Optimizer.Type=ACCELERATOR;
    int i=0,j=0,N=0;
    int Ngap=0,Nc=0,Nreg=0;
    double dE=Estep,P=0,eps=Eerr,betta=0;
    TCell *Cell;
    Optimizer.SetDimension(3);

    double Wa=0,W0=0,W1=0,W2=0;
    double A0=0,A1=0,A2=0;
    double x0=0,x1=0,x2=0;
    double y0=0,y1=0,y2=0;

    try{
        W=EnergyEdit->Text.ToDouble();
    } catch (...){
        ShowMessage("Energy value is incorrect");
        return;
    }

    Nc=Solver->GetNumberOfCells();
    if (StartCellCheck->Checked)
        Ngap=Nc-StartCellSpin->Value;
    Nreg=Nc-Ngap;

    Solver->LoadData();

    j=1;
    i=1;
    Cell=new TCell[Ngap+j];

    for (int k=0;k<Ngap;k++)
        Cell[k]=Solver->GetCell(Nreg+k);

    Solver->LoadData(Nreg);
    Structure=Solve(true);
    Structure.x=0;
    W0=GetEnergy(Structure);
    A0=Structure.Result.A;
   //   ShowMessage(A0);

    Optimizer.PushResult(Structure);
    OptimizerProgress->Max+=2;
    OptimizerProgress->Position++;

    

    FILE *F;
    F=fopen("optim.log","w");

    do{
        TCell *C1;
        C1=new TCell[Ngap+j];
        for (int k=0;k<Ngap+j;k++)
            C1[k]=Cell[k];
        delete[] Cell;
        Cell=new TCell[Ngap+i];
        for (int k=0;k<Ngap+i;k++)
            Cell[k]=C1[k];
        delete[] C1;
        j=i;

      /*    Solver->LoadData();
        for (int k=0;k<i;k++){
            Cell[k]=Solver->LastCell();
            Solver->AppendCells(Cell[k]);
        }        */

        Wa=W0;
        //OPTIMIZE A
        bool newCell=true;
        int step=0;
        do{
            step++;
            Solver->LoadData(Nreg);
            y1=y2;
            W1=W2;

            if (i<=Ngap)
				betta=Cell[i-1].beta;

            for (int k=0;k<i-1;k++)
                Solver->AppendCells(Cell[k]);
            if (newCell){
                Cell[i-1]=Solver->LastCell();
                x1=Cell[i-1].ELP-dE;
                x2=Cell[i-1].ELP;
                newCell=false;
            }else{
                x1=x2;
                x2=x1+dE;
                Cell[i-1].ELP=x2;
            }

            if (i<=Ngap)
				Cell[i-1].beta=betta;

            Solver->AppendCells(Cell[i-1]);
            Structure=Solve();
            A2=Structure.Result.A;
            y2=A2;
			W2=Structure.Result.Energy.mean;
            P=Structure.Result.LoadPower;
            if (P<=0){
                ShowMessage("Maximum Energy Acheived");
                break;
            }  
        } while (A2<A0);

        if (step==1){
            Solver->LoadData(Nreg);
            for (int k=0;k<i-1;k++)
                Solver->AppendCells(Cell[k]);
            Cell[i-1]=Solver->LastCell();
            Cell[i-1].ELP=x1;
            if (i<=Ngap)
                Cell[i-1].beta=betta;

            Solver->AppendCells(Cell[i-1]);
            Structure=Solve(false);
            y1=Structure.Result.A;
			W1=Structure.Result.Energy.mean;
        }
        while (x2-x1>eps){
            x0=(x1+x2)/2;
            Solver->LoadData(Nreg);
            for (int k=0;k<i-1;k++)
                Solver->AppendCells(Cell[k]);
            Cell[i-1].ELP=x0;
            Solver->AppendCells(Cell[i-1]);
            Structure=Solve(false);
            y0=Structure.Result.A;
			W0=Structure.Result.Energy.mean;
            if (y0>A0){
                x2=x0;
                y2=y0;
                W2=W0;
            } else if (A0>y0){
                x1=x0;
                y1=y0;
                W1=W0;
            }else{
                break;
            }
        }
        Cell[i-1].ELP=(mod(y2-A0)<mod(y1-A0))?x2:x1;
        W0=(mod(y2-A0)<mod(y1-A0))?W2:W1;
        fprintf(F,"%f\n",(mod(y2-A0)<mod(y1-A0))?y2:y1);
        //=FloatToStr(i)+" : "+FloatToStr(Cell[i-1].ELP)+" : "+FloatToStr((mod(y2-A0)<mod(y1-A0))?y2:y1);
        //AnsiString F=FloatToStr(A2)+" / "+FloatToStr(A0);
       //   ShowMessage(F);
        i++;
    //}while ();
    } while (!((W0>W) /*|| (W0<Wa)*/));

    if (W0>W){
        Wa=(mod(W0-W)<mod(Wa-W))?W0:Wa;
        if (ExactCheck->Checked){
            OptimizeCurrent(Structure);
			M="Output Energy "+s.FormatFloat("#.00",Structure.Result.Energy.mean)+" MeV is acheved at "+s.FormatFloat("0",N)+" additional cells and "+s.FormatFloat("#.00",Structure.Result.InputCurrent)+" mA input current";
		} else{
            M="Output Energy "+s.FormatFloat("#.00",Wa)+" MeV is acheved at "+s.FormatFloat("0",i-1)+" additional cells";
        }
        ShowMessage(M);
    }else if (W0<Wa || P<=0){
        M="Output Energy is unacheivable. Estimated Energy Limit "+s.FormatFloat("#.00",Wa)+". Try to lower the target energy";
        ShowMessage(M);
    }

        
    fclose(F);


    delete[] Cell;

    OptimizerProgress->Position++;
    FileName="OPTIMIZER.TXT";
    Optimizer.Output(FileName);
    ResultsButton->Enabled=true;
    OptForm->Optimizer=Optimizer;
}
//---------------------------------------------------------------------------
void __fastcall TOptimizerForm::StartButtonClick(TObject *Sender)
{
    ResultsButton->Enabled=false;
    mkdir("Results");
    counter=1;
    OptimizerProgress->Position=0;

    if (BuncherCheck->Checked){
        if (SweepRadio->Checked)
            Sweep();
        else
            OptimizeBuncher();
    } else if (AccelCheck->Checked){
        if (ImpedanceRadio->Checked)
            OptimizeImpedance();
        else
            OptimizeGradient();
    }
}
//---------------------------------------------------------------------------
void __fastcall TOptimizerForm::AbortButtonClick(TObject *Sender)
{
    Solver->Abort();
    Abort=true;
}
//---------------------------------------------------------------------------

void __fastcall TOptimizerForm::ResultsButtonClick(TObject *Sender)
{
    OptForm->Show();    
}
//---------------------------------------------------------------------------
void __fastcall TOptimizerForm::StartCellCheckClick(TObject *Sender)
{
    ChangeStart();
}
//---------------------------------------------------------------------------

