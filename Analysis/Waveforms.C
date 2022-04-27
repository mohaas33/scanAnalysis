#include <iostream>
#include <fstream>
#include <string>

using namespace std;

TNtuple *Currents;

TH1D *Current0;
TH1D *Current1;
TH1D *Current2;
TH1D *Current3;
TH1D *Current4;
TH1D *Current5;
TH1D *Current6;
TH1D *Current7;
TH1D *Current8;
TH1D *Current9;
TH1D *Current10;
TH1D *Current11;

TH1D *Waveform0;
TH1D *Waveform1;
TH1D *Waveform2;
TH1D *Waveform3;
TH1D *Waveform4;
TH1D *Waveform5;
TH1D *Waveform6;
TH1D *Waveform7;
TH1D *Waveform8;
TH1D *Waveform9;
TH1D *Waveform10;
TH1D *Waveform11;

TH2D *Correlation;

TH1D *MeshCorr;
TH1D *IonCurrent;

TFile *output;

void Waveforms(string filename)
{
  char name[500];
  sprintf(name,"%s_ntupleoutput.root",filename.c_str());
  output=new TFile(name,"RECREATE");
  Currents = new TNtuple("Currents","Currents","pad:g4b:g4t:g3b:g3t:g2b:g2t:g1b:g1t:mask:mesh:wind");
  
  string line, word, temp;      
  vector<string> row;
  ifstream myfile (filename.c_str());

  int lines=0;
  if (myfile.is_open())
    {
      while ( getline (myfile,line) )
	{
	  row.clear();
	  stringstream s(line);
	  while (getline(s, word, ','))
	    {
	      row.push_back(word);
	    }
	  
	  float pad   = atof(row[0].c_str());
	  float g4b   = atof(row[1].c_str());
	  float g4t   = atof(row[2].c_str());
	  float g3b   = atof(row[3].c_str());
	  float g3t   = atof(row[4].c_str());
	  float g2b   = atof(row[5].c_str());
	  float g2t   = atof(row[6].c_str());
	  float g1b   = atof(row[7].c_str());
	  float g1t   = atof(row[8].c_str());
	  float mask  = atof(row[9].c_str());
	  float mesh  = atof(row[10].c_str());
	  float wind  = atof(row[11].c_str());

	  if (false)
	    {
	      cout << "l: " << row.size();
	      cout << " " << pad;
	      cout << " " << g4b;
	      cout << " " << g4t;
	      cout << " " << g3b;
	      cout << " " << g3t;
	      cout << " " << g2b;
	      cout << " " << g2t;
	      cout << " " << g1b;
	      cout << " " << g1t;
	      cout << " " << mask;
	      cout << " " << mesh;
	      cout << " " << wind;
	      cout << endl;
	    }

	  Currents->Fill(pad,g4b,g4t,g3b,g3t,g2b,g2t,g1b,g1t,mask,mesh,wind);
	  if ((++lines)%1000 == 0) Currents->Write();
	}
      //myfile.close();
    }  
  else
    {
      cout << "Unable to open file";
      exit;
    }

  // Now book some waveform histograms based upon the number of channels that were read1
  int nread = Currents->GetEntries();
  Waveform0  = new TH1D("Waveform0" ,"Waveform0" , nread, -0.5, (float)nread-0.5);
  Waveform1  = new TH1D("Waveform1" ,"Waveform1" , nread, -0.5, (float)nread-0.5);
  Waveform2  = new TH1D("Waveform2" ,"Waveform2" , nread, -0.5, (float)nread-0.5);
  Waveform3  = new TH1D("Waveform3" ,"Waveform3" , nread, -0.5, (float)nread-0.5);
  Waveform4  = new TH1D("Waveform4" ,"Waveform4" , nread, -0.5, (float)nread-0.5);
  Waveform5  = new TH1D("Waveform5" ,"Waveform5" , nread, -0.5, (float)nread-0.5);
  Waveform6  = new TH1D("Waveform6" ,"Waveform6" , nread, -0.5, (float)nread-0.5);
  Waveform7  = new TH1D("Waveform7" ,"Waveform7" , nread, -0.5, (float)nread-0.5);
  Waveform8  = new TH1D("Waveform8" ,"Waveform8" , nread, -0.5, (float)nread-0.5);
  Waveform9  = new TH1D("Waveform9" ,"Waveform9" , nread, -0.5, (float)nread-0.5);
  Waveform10 = new TH1D("Waveform10","Waveform10", nread, -0.5, (float)nread-0.5);
  Waveform11 = new TH1D("Waveform11","Waveform11", nread, -0.5, (float)nread-0.5);

  Current0  = new TH1D("Current0" ,"Current0" , 2000, -1.0, 1.0);
  Current1  = new TH1D("Current1" ,"Current1" , 2000, -1.0, 1.0);
  Current2  = new TH1D("Current2" ,"Current2" , 2000, -1.0, 1.0);
  Current3  = new TH1D("Current3" ,"Current3" , 2000, -1.0, 1.0);
  Current4  = new TH1D("Current4" ,"Current4" , 2000, -1.0, 1.0);
  Current5  = new TH1D("Current5" ,"Current5" , 2000, -1.0, 1.0);
  Current6  = new TH1D("Current6" ,"Current6" , 2000, -1.0, 1.0);
  Current7  = new TH1D("Current7" ,"Current7" , 2000, -1.0, 1.0);
  Current8  = new TH1D("Current8" ,"Current8" , 2000, -1.0, 1.0);
  Current9  = new TH1D("Current9" ,"Current9" , 2000, -1.0, 1.0);
  Current10  = new TH1D("Current10" ,"Current10" , 2000, -1.0, 1.0);
  Current11  = new TH1D("Current11" ,"Current11" , 2000, -1.0, 1.0);
  IonCurrent  = new TH1D("IonCurrent" ,"IonCurrent" , 2000, -1.0, 1.0);


  
  Float_t pad,g4b,g4t,g3b,g3t,g2b,g2t,g1b,g1t,mask,mesh,wind;
  Currents->SetBranchAddress("pad",&pad);
  Currents->SetBranchAddress("g4b",&g4b);
  Currents->SetBranchAddress("g4t",&g4t);
  Currents->SetBranchAddress("g3b",&g3b);
  Currents->SetBranchAddress("g3t",&g3t);
  Currents->SetBranchAddress("g2b",&g2b);
  Currents->SetBranchAddress("g2t",&g2t);
  Currents->SetBranchAddress("g1b",&g1b);
  Currents->SetBranchAddress("g1t",&g1t);
  Currents->SetBranchAddress("mask",&mask);
  Currents->SetBranchAddress("mesh",&mesh);
  Currents->SetBranchAddress("wind",&wind);
  for(int i=0; i<Currents->GetEntries(); i++)
    {
      Currents->GetEntry(i);
      Waveform0->Fill(i,pad);
      Waveform1->Fill(i,g4b);
      Waveform2->Fill(i,g4t);
      Waveform3->Fill(i,g3b);
      Waveform4->Fill(i,g3t);
      Waveform5->Fill(i,g2b);
      Waveform6->Fill(i,g2t);
      Waveform7->Fill(i,g1b);
      Waveform8->Fill(i,g1t);
      Waveform9->Fill(i,mask);
      Waveform10->Fill(i,mesh);
      Waveform11->Fill(i,wind);
      
      Current0->Fill(pad);
      Current1->Fill(g4b);
      Current2->Fill(g4t);
      Current3->Fill(g3b);
      Current4->Fill(g3t);
      Current5->Fill(g2b);
      Current6->Fill(g2t);
      Current7->Fill(g1b);
      Current8->Fill(g1t);
      Current9->Fill(mask);
      Current10->Fill(mesh);
      Current11->Fill(wind);
    }


  TH1D *Correction =  (TH1D*)Waveform11->Clone("Correction");
  Correction->Fit("pol1","q");
  Correction->Add(Correction->GetFunction("pol1"),-1.0);

  Correlation = new TH2D("Correlation","Correlation",50,Correction->GetMinimum(),Correction->GetMaximum(),50,Waveform10->GetMinimum(),Waveform10->GetMaximum());
  for (int i=1; i<=Waveform10->GetNbinsX(); i++)
    {
      double x = Correction->GetBinContent(i);
      double y = Waveform10->GetBinContent(i);
      Correlation->Fill(x,y);
    }

  // Clean out the grass around the true correlation
  for (int i=1; i<=Correlation->GetNbinsX(); i++)
    {
      for (int j=1; j<=Correlation->GetNbinsY(); j++)
	{
	  if (Correlation->GetBinContent(i,j) == 1)
	    {
	      Correlation->SetBinContent(i,j,0);
	    }
	}
    }

  Correlation->Fit("pol1");
  TH1D *ScaledCorrection =  (TH1D*)Correction->Clone("ScaledCorrection");
  ScaledCorrection->SetLineColor(kGreen);
  ScaledCorrection->Scale(Correlation->GetFunction("pol1")->GetParameter(1));

  TH1D *IonCorrected =  (TH1D*)Waveform10->Clone("IonCorrected");
  IonCorrected->SetLineColor(kRed);
  IonCorrected->Add(ScaledCorrection,-1.0);

  for (int i=1; i<=IonCorrected->GetNbinsX(); i++)
    {
      IonCurrent->Fill(IonCorrected->GetBinContent(i));
    }
  
  cout << "RMS0:  " << Current0->GetRMS() << endl;
  cout << "RMS1:  " << Current1->GetRMS() << endl;
  cout << "RMS2:  " << Current2->GetRMS() << endl;
  cout << "RMS3:  " << Current3->GetRMS() << endl;
  cout << "RMS4:  " << Current4->GetRMS() << endl;
  cout << "RMS5:  " << Current5->GetRMS() << endl;
  cout << "RMS6:  " << Current6->GetRMS() << endl;
  cout << "RMS7:  " << Current7->GetRMS() << endl;
  cout << "RMS8:  " << Current8->GetRMS() << endl;
  cout << "RMS9:  " << Current9->GetRMS() << endl;
  cout << "RMS10: " << Current10->GetRMS() << endl;
  cout << "RMS11: " << Current11->GetRMS() << endl;
  cout << "RMSIon: " << IonCurrent->GetRMS() << endl;


  Waveform10->Draw("hist");
  ScaledCorrection->Draw("histsame");
  IonCorrected->Draw("histsame");

  TCanvas *c_IonCorrected = new TCanvas("c_IonCorrected","c_IonCorrected",600,800);

  //IonCorrected->Rebin(20);
  //IonCorrected->Scale(1.0/20);
  
  IonCorrected->Draw("hist");
  //myfile.close();
  sprintf(name,"%s_hist.root",filename.c_str());
  TFile *f = new TFile(name ,"RECREATE");
  Waveform0 ->Write();
  Waveform1 ->Write();
  Waveform2 ->Write();
  Waveform3 ->Write();
  Waveform4 ->Write();
  Waveform5 ->Write();
  Waveform6 ->Write();
  Waveform7 ->Write();
  Waveform8 ->Write();
  Waveform9 ->Write();
  Waveform10->Write();
  Waveform11->Write();
  Current0  ->Write();
  Current1  ->Write();
  Current2  ->Write();
  Current3  ->Write();
  Current4  ->Write();
  Current5  ->Write();
  Current6  ->Write();
  Current7  ->Write();
  Current8  ->Write();
  Current9  ->Write();
  Current10 ->Write();
  Current11   ->Write();
  IonCorrected->Write();
  f->Write();
  f->Close();
  myfile.close();

}

