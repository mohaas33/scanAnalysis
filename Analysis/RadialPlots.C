#include <iostream>
#include <fstream>
#include <string>
#include <TF1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TSystem.h>
#include <TLegend.h>

#include <sstream>

using namespace std;

TH2F *h_ibf_wind[31];
TH1F *h_ibf[31];
TH1F *h_ibf_corr[31];
TH1F *h_ibf_corr_minusconst[31];
TH1F *h_wind[31];
TH1F *h_wind_scaled[31];

TH2F *hGainMap;
TH2F *hIonMap;
TH2F *hIBFMap;
TH1F *PadCurrent[31];
TH1F *IonCurrent[31];
TCanvas *c_PadCurrent[31];
TCanvas *c_IonCurrentFlow[31];
TCanvas *c_IonCurrent[31];
TF1 *fit_Candle_f[31];
TF1 *fit_pol1_f[31];
TF1 *f_wind[31];
TH1F *GainDistribution;
TH1F *GainDistributionNoEdges;
TF1 *Candles;
//TNtuple *Currents;

TH2F *FineGain;
TH2F *FineIBF;
TH2F *FineIBF_corr;
TH2F *FineIBF_corr_minusconst;

TCanvas *c1;
TCanvas *c2;
TCanvas *c3;
TCanvas *c4;
TCanvas *c_capacitor;

TFile *output;

char label[500];

void l_Marker(double, double, double, double, TString, double, int);


void RadialPlots(int sectorN, int moduleN)
{

  gStyle->SetPalette(1);
  //gStyle->SetPaperSize(20,26);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.05);

  c_capacitor = new TCanvas("c_capacitor","c_capacitor",800,600);
  c_capacitor->Divide(5,6,0,0);

  c1 = new TCanvas("c1","c1",800,600);
  c3 = new TCanvas("c3","c3",800,600);
  c4 = new TCanvas("c4","c4",800,600);
  
  c1->Divide(5,6);
  int CanvasPointer=1;


  // Definig histograms
  
  double MeasuredGain[16][31];
  GainDistribution = new TH1F("GainDistribution","GainDistribution",1000,3,110);
  GainDistributionNoEdges = new TH1F("GainDistributionNoEdges","GainDistributionNoEdges",1000,3,110);
  
  TString s_xyI = ";X;Y;I, [nA]";
  TString s_dyI = ";#phi, [#circ];R, [arb.];I, [nA]";

  hGainMap = new TH2F("hGainMap","hGainMap"+s_dyI, 31, -15.5, 15.5, 16, -0.5, 15.5);
  hIonMap = new TH2F("hIonMap","hIonMap"+s_dyI,   31, -15.5, 15.5, 16, -0.5, 15.5);
  hIBFMap = new TH2F("hIBFMap","hIBFMap"+s_dyI,   31, -15.5, 15.5, 16, -0.5, 15.5);
  //Currents = new TNtuple("Currents","Currents","pad:g4b:g4t:g3b:g3t:g2b:g2t:g1b:g1t:mask:mesh:wind");


  TString s_dtI = ";#phi, [#circ];t, [msec.];I, [nA]";

  FineGain = new TH2F("FineGain","FineGain"+s_dtI, 31, -15.5, 15.5, 45000, -0.5, 44999.5);
  FineIBF = new TH2F("FineIBF","FineIBF"+s_dtI, 31, -15.5, 15.5, 45000, -0.5, 44999.5);
  FineIBF_corr = new TH2F("FineIBF_corr","FineIBF_corr"+s_dtI, 31, -15.5, 15.5, 45000, -0.5, 44999.5);
  FineIBF_corr_minusconst = new TH2F("FineIBF_corr_minusconst","FineIBF_corr_minusconst"+s_dtI, 31, -15.5, 15.5, 300, -0.5, 44999.5);

 TString s_tI = ";t, [msec.];I, [nA]";
 TString s_I = ";I, [nA];N";
  int NFiles = 31;
  double maximum_gain =0 ;
  // Loop over the txt files in the directory
  const int nt_bins = 30000;
  const int nt_max = 30000;
  for (int i=0; i<NFiles; i++){
    // Creating canvases to put distributions inside later on
    sprintf(label,"PadCurrent%d",i);
    PadCurrent[i] = new TH1F(label,label+s_tI,nt_bins,-0.5,nt_max-0.5);
    sprintf(label,"c_PadCurrent%d",i);
    c_PadCurrent[i] = new TCanvas(label,label,800, 600);
    sprintf(label,"IonCurrent%d",i);
    IonCurrent[i] = new TH1F(label,label+s_tI,nt_bins,-0.5,nt_max-0.5);
    sprintf(label,"c_IonCurrentFlow%d",i);
    c_IonCurrentFlow[i] = new TCanvas(label,label,800, 600);
    sprintf(label,"c_IonCurrent%d",i);
    c_IonCurrent[i] = new TCanvas(label,label,800, 600);
    
    //sprintf(label,"h_ibf_wind_%d",i);
    //h_ibf_wind[i] = new TH2F(label,label, 18,-0.3,0.3, 90, -1,1);
    sprintf(label,"h_ibf_wind_%d",i);
    h_ibf_wind[i] = new TH2F(label,label, 30, -0.9, 0.9 , 30, -0.9, 0.9);

    sprintf(label,"h_ibf_%d",i);
    h_ibf[i] = new TH1F(label,label+s_I, nt_bins,-0.5,nt_max-0.5);
    sprintf(label,"h_ibf_corr_%d",i);
    h_ibf_corr[i] = new TH1F(label,label+s_I, nt_bins,-0.5,nt_max-0.5);
    sprintf(label,"h_wind_%d",i);
    h_wind[i] = new TH1F(label,label+s_I, nt_bins,-0.5,nt_max-0.5);
    sprintf(label,"h_wind_scaled_%d",i);
    h_wind_scaled[i] = new TH1F(label,label+s_I, nt_bins,-0.5,nt_max-0.5);

    
    double begin=-9999;
    double end=-9999;
    bool found1=false;
    bool found2=false;
      
    // Ugggh...the date in the filename makes life tough.
    // Since was originally written on a windows bex, I need to use the windows dir command instead of ls.
    // On a linux box this needs to be ls rather than dir
    // Since gSystem does NOT return a list of files, 
    string file, line, word, temp;      
    vector<string> row;

    // Loading the list of files
  
    char DuhCommand[500];
    //sprintf(DuhCommand,"dir /b R2_22_%d_* > NextFile.txt",i-15);
    sprintf(DuhCommand,"ls ../Scans/%d/R%d_%d_%d_* > NextFile.txt", moduleN, sectorN, moduleN, i-15);
    cout << DuhCommand << endl;
    gSystem->Exec(DuhCommand);

    ifstream DuhFile ("./NextFile.txt");
    if (DuhFile.is_open()){
	    getline(DuhFile,file);
	    cout << "The next file shall be: " << file << endl;
	  }
    else{
	    cout << "Cannot open the file with the dated name" << endl;
	    break;
	  }
  
    // Open file
    ifstream myfile (file.c_str());
    if (myfile.is_open())
	  {
	    int nread=0;
	    double sum=0;
	    while ( getline (myfile,line) ){
	      row.clear();
	      // used for breaking words
	      stringstream s(line);
        
	      // read every column data of a row and
	      // store it in a string variable, 'word'
	      while (getline(s, word, ',')) {
        
	  	    // add all the column data
	  	    // of a row to a vector
	  	    row.push_back(word);
	      }
	    
	      //cout << row[0] << " " << row[1] << " " << row[6] << " " << endl;

	      if (row.size() == 12){
		      double gain = atof(row[0].c_str());
		      double g4b  = atof(row[1].c_str());
		      double g4t  = atof(row[2].c_str());
		      double g3b  = atof(row[3].c_str());
		      double g3t  = atof(row[4].c_str());
		      double g2b  = atof(row[5].c_str());
		      double g2t  = atof(row[6].c_str());
		      double g1b  = atof(row[7].c_str());
		      double g1t  = atof(row[8].c_str());
		      double mask  = atof(row[9].c_str());
		      double mesh  = atof(row[10].c_str());
		      double wind  = atof(row[11].c_str());
		
		      double ion = mesh;
		
		      //int blankreads=4000;
          //
		      //if (nread<blankreads){
		      //  sum += gain;
		      //}
		      //else{
		      //  if (!found1){
			    //    if (gain > 12 + sum/((float)blankreads)){
			    //      begin = nread;
			    //      found1 = true;
			    //    }
			    //  }
		    //
		      //  if (found1 && !found2 && nread > (begin+3*blankreads) ){
        	//	  if (gain < 12 + sum/((float)blankreads)){
			    //      end = nread;
			    //      found2 = true;
			    //    }
			    //  }
		      //}

		
          // Fill histograms with the values of the currents
		
		      PadCurrent[i]->Fill(nread,gain);
		      IonCurrent[i]->Fill(nread,ion);
          h_ibf[i]->Fill(nread,ion);
          h_ibf_corr[i]->Fill(nread,ion);
          h_wind[i]->Fill(nread,wind);
          h_wind_scaled[i]->Fill(nread,wind);

		      FineGain->Fill(i-15,nread,gain);
 		      FineIBF->Fill(i-15,nread,ion);

		      nread++;
		    }
	    }
	    myfile.close();
	  }
      
    float max_gain = PadCurrent[i]->GetBinContent(PadCurrent[i]->GetMaximumBin());
    if(max_gain> maximum_gain) maximum_gain = max_gain;
    float min_fit = max_gain / 4;
    float par_0_15 = max_gain * 0.8;
    float par_0_15_min = max_gain * 0.5;
    float par_0_15_max = max_gain * 1.5;
    cout<< "Min Fit = "<<min_fit<<endl;
	  int nread=0;
	  double sum=0;
    cout<<"N bins = "<<PadCurrent[i]->GetNbinsX()<<endl;
    for (int nread = 0; nread<PadCurrent[i]->GetNbinsX(); nread++){

          double gain = PadCurrent[i]->GetBinContent(nread+1);

		      int blankreads=4000;
          
		      if (nread<blankreads){
		        sum += gain;
		      }
		      else{
		        if (!found1){
			        if (gain > min_fit + sum/((float)blankreads)){
			          begin = nread;
			          found1 = true;
			        }
			      }
		    
		        if (found1 && !found2 && nread > (begin+3*blankreads) ){
        		  if (gain < min_fit + sum/((float)blankreads)){
			          end = nread;
			          found2 = true;
			        }
			      }
		      }
    }



    cout<<"\n Fitting in the range:"<<endl;
    cout<<"begin: "<< begin << "end: " << end << endl;

    if (begin > 0){

      //else cout << "Unable to open file"; 
      cout<<"Candles"<<endl;
      Candles = new TF1("Candles","[0]*exp(-(x-([16]+[19]))^2/(2*[18]^2))+[1]*exp(-(x-([16]+1*[17]))^2/(2*[18]^2))+[2]*exp(-(x-([16]+2*[17]))^2/(2*[18]^2))+[3]*exp(-(x-([16]+3*[17]))^2/(2*[18]^2))+[4]*exp(-(x-([16]+4*[17]))^2/(2*[18]^2))+[5]*exp(-(x-([16]+5*[17]))^2/(2*[18]^2))+[6]*exp(-(x-([16]+6*[17]))^2/(2*[18]^2))+[7]*exp(-(x-([16]+7*[17]))^2/(2*[18]^2))+[8]*exp(-(x-([16]+8*[17]))^2/(2*[18]^2))+[9]*exp(-(x-([16]+9*[17]))^2/(2*[18]^2))+[10]*exp(-(x-([16]+10*[17]))^2/(2*[18]^2))+[11]*exp(-(x-([16]+11*[17]))^2/(2*[18]^2))+[12]*exp(-(x-([16]+12*[17]))^2/(2*[18]^2))+[13]*exp(-(x-([16]+13*[17]))^2/(2*[18]^2))+[14]*exp(-(x-([16]+14*[17]))^2/(2*[18]^2))+[15]*exp(-(x-([16]+15*[17]-[20]))^2/(2*[18]^2))",0,25000);
      // Fit function:
      // [0]*exp(-(x-([16]+[19]))^2/(2*[18]^2))+
      // [1]*exp(-(x-([16]+1*[17]))^2/(2*[18]^2))+
      // [2]*exp(-(x-([16]+2*[17]))^2/(2*[18]^2))+
      // [3]*exp(-(x-([16]+3*[17]))^2/(2*[18]^2))+
      // [4]*exp(-(x-([16]+4*[17]))^2/(2*[18]^2))+
      // [5]*exp(-(x-([16]+5*[17]))^2/(2*[18]^2))+
      // [6]*exp(-(x-([16]+6*[17]))^2/(2*[18]^2))+
      // [7]*exp(-(x-([16]+7*[17]))^2/(2*[18]^2))+
      // [8]*exp(-(x-([16]+8*[17]))^2/(2*[18]^2))+
      // [9]*exp(-(x-([16]+9*[17]))^2/(2*[18]^2))+
      // [10]*exp(-(x-([16]+10*[17]))^2/(2*[18]^2))+
      // [11]*exp(-(x-([16]+11*[17]))^2/(2*[18]^2))+
      // [12]*exp(-(x-([16]+12*[17]))^2/(2*[18]^2))+
      // [13]*exp(-(x-([16]+13*[17]))^2/(2*[18]^2))+
      // [14]*exp(-(x-([16]+14*[17]))^2/(2*[18]^2))+
      // [15]*exp(-(x-([16]+15*[17]-[20]))^2/(2*[18]^2))"      

      // Robustly fit the histogram using the present values of begin and end:
      double range = end - begin;
      double spacing = range/16.0;

      // These parameters have to be taken from the maximum current in the file ! 
      Candles->SetParameter(0,par_0_15);
      Candles->SetParameter(1,par_0_15);
      Candles->SetParameter(2,par_0_15);
      Candles->SetParameter(3,par_0_15);
      Candles->SetParameter(4,par_0_15);
      Candles->SetParameter(5,par_0_15);
      Candles->SetParameter(6,par_0_15);
      Candles->SetParameter(7,par_0_15);
      Candles->SetParameter(8,par_0_15);
      Candles->SetParameter(9,par_0_15);
      Candles->SetParameter(10,par_0_15);
      Candles->SetParameter(11,par_0_15);
      Candles->SetParameter(12,par_0_15);
      Candles->SetParameter(13,par_0_15);
      Candles->SetParameter(14,par_0_15);
      Candles->SetParameter(15,par_0_15);
      Candles->SetParameter(16,begin+0.1*spacing);
      Candles->SetParameter(17,spacing);
      Candles->SetParameter(18,spacing/5.0);
      Candles->SetParameter(19,0);
      Candles->SetParameter(20,0);

      Candles->SetParLimits(0,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(1,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(2,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(3,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(4,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(5,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(6,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(7,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(8,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(9,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(10,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(11,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(12,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(13,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(14,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(15,par_0_15_min,par_0_15_max);
      Candles->SetParLimits(16,begin,begin+3*spacing);
      Candles->SetParLimits(17,range/(16.0*1.25),range/15.0);
      Candles->SetParLimits(18,spacing/10.0,spacing/2.0);
      Candles->SetParLimits(19,0,spacing/2.0);
      Candles->SetParLimits(20,0,spacing/2.0);

      PadCurrent[i]->Sumw2();
      PadCurrent[i]->Rebin(20);
      PadCurrent[i]->Scale(1./20);      
      PadCurrent[i]->Fit(Candles,"0","",begin,end);

      if (i>2 && i<29){


        //float wind_mean = h_wind_scaled[i]->GetMean();
        //for (int b=1;b<=h_wind_scaled[i]->GetNbinsX(); b++){
        //  h_wind_scaled[i]->SetBinContent(b,h_wind_scaled[i]->GetBinContent(b)-wind_mean);
        //}


        // wind_scaled is the current on the capacitor
        // it has a slope to remove the slope it is fitted and the finction is subtrackted from the distribution
        TH1F *h_wind_tmp = (TH1F*)h_wind[i]->Clone();
        h_wind_tmp->Rebin(500);
        h_wind_tmp->Scale(1./500);
        h_wind_tmp->Fit("pol1","QE0");//, "", 1000, 24000);
        sprintf(label,"f_wind_pol1_%d",i);
        f_wind[i] = (TF1*)h_wind_tmp->GetFunction("pol1")->Clone(label);

        //h_wind[i]->GetFunction("pol1")->Draw()
        h_wind_scaled[i]->Add(f_wind[i],-1.0);

        // is correlation plot  to find the scale factor to correct current from capacitor 
        for (int n=1; n<=h_wind_scaled[i]->GetNbinsX(); n++)
        {
          //double x = h_ibf[i]->GetBinContent(n);
          //double y = h_wind_scaled[i]->GetBinContent(n);
          double x = h_wind_scaled[i]->GetBinContent(n);
          double y = h_ibf[i]->GetBinContent(n);
          if ((h_ibf[i]->GetBinCenter(n)>1000 && h_ibf[i]->GetBinCenter(n)<4000) || h_ibf[i]->GetBinCenter(n)>22000){
            h_ibf_wind[i]->Fill(x,y);
          }
        }
        // Fit of the correlation plot
        h_ibf_wind[i]->Fit("pol1");
        sprintf(label,"f_pol1_%d",i);
        fit_pol1_f[i] = (TF1*) h_ibf_wind[i]->GetFunction("pol1")->Clone(label);
        // Scale the currents on the capacitor
        h_wind_scaled[i]->Scale(fit_pol1_f[i]->GetParameter(1));
        h_ibf_corr[i]->Add(h_wind_scaled[i],-1);

        
        for (int n=1; n<=h_ibf_corr[i]->GetNbinsX(); n++)
        {
          FineIBF_corr->Fill(i-15,n,h_ibf_corr[i]->GetBinContent(n));
        }

        // Draw Candle plots and fit
        c1->cd(CanvasPointer);
	      PadCurrent[i]->Draw("hist");
        sprintf(label,"f_Candle_%d",i);
	      fit_Candle_f[i] = (TF1*) PadCurrent[i]->GetFunction("Candles")->Clone(label);
        fit_Candle_f[i]->SetLineColor(2);
        fit_Candle_f[i]->Draw("same");

        c_capacitor->cd(CanvasPointer);
        h_wind[i]->SetLineColor(2);
        h_wind[i]->Draw("hist");
        f_wind[i]->Draw("same");

	      CanvasPointer++;
	    

        //cout<< i <<" binstart"<<endl;
        float binstart  = fit_Candle_f[i]->GetParameter(16);
        float bingap    = fit_Candle_f[i]->GetParameter(17);
        float delta1    = fit_Candle_f[i]->GetParameter(19);
        float delta16   = fit_Candle_f[i]->GetParameter(20);

        cout<<"Don't plot the end thetas as the fits are bad..."<<endl;
        //  Don't plot the end thetas as the fits are bad...
        //if (i>2 && i<29){
        for (int j=0; j<16; j++){
  	      float bincenter = binstart + j*bingap;
	        if (j==0)  bincenter += delta1;
	        if (j==15) bincenter -= delta16;
	        MeasuredGain[j][i] = PadCurrent[i]->GetBinContent(PadCurrent[i]->FindBin(bincenter+0.5)) ;
	        cout << j << i << " " << bincenter << " MeasuredGain[j][i] " << MeasuredGain[j][i] << "int(bincenter+0.5) " << int(bincenter+0.5) << endl;
	        GainDistribution->Fill(MeasuredGain[j][i]);
	        if (j!=0 && j!=15) GainDistributionNoEdges->Fill(MeasuredGain[j][i]);
          cout << "Fill GainMap" << endl;
	        hGainMap->Fill( i-15, j,MeasuredGain[j][i]);
          cout << "Filled GainMap" << endl;
	      }
	    }
    
    }
    else{ 
      cout<<"Fit"<<endl;
      cout<<"begin "<< begin << "end" << end << "  !!!!!!!"<<endl;
      continue;
    }    
  }//end of loop over phi positions

  cout<<"c1->Print();"<<endl;
  sprintf(label,"./Plots/AllFits_R%d_M%d.png",sectorN, moduleN);
  c1->Print(label);
  
  cout<<"c_capacitor->Print();"<<endl;
  sprintf(label,"./Plots/capacitor_R%d_M%d.png",sectorN, moduleN);
  c_capacitor->Print(label);

  cout<<"c3->cd();"<<endl;
  c3->cd();
  GainDistribution->Draw();
  sprintf(label,"R %d Module %d RMS/Mean=%.4f", sectorN, moduleN, GainDistribution->GetRMS()/GainDistribution->GetMean());
  l_Marker(.75 ,.45 ,.9 ,.5,label,0.04,1);

  
  cout<<"c4->cd();"<<endl;
  c4->cd();
  GainDistributionNoEdges->Draw();
  sprintf(label,"R %d Module %d RMS/Mean=%.4f", sectorN, moduleN, GainDistributionNoEdges->GetRMS()/GainDistributionNoEdges->GetMean());
  l_Marker(.75 ,.45 ,.9 ,.5,label,0.04,1);

  sprintf(label,"./Plots/GainDistribution_R%d_M%d.png",sectorN, moduleN);
  c3->Print(label);
  sprintf(label,"./Plots/GainDistributionNoEdges_R%d_M%d.png",sectorN, moduleN);
  c4->Print(label);
  for (int i=3; i<NFiles-2; i++){

    c_IonCurrentFlow[i]->cd();
    h_wind[i]->SetLineColor(2);
    h_wind_scaled[i]->SetLineColor(4);

    h_ibf_corr[i]->SetLineColor(413);
    h_ibf_corr[i]->Rebin(150);
    h_ibf_corr[i]->Scale(1./150);
    
    IonCurrent[i]->GetXaxis()->SetRangeUser(0., 25000.);
    IonCurrent[i]->GetYaxis()->SetRangeUser(-5., 1.);

	  IonCurrent[i]->Draw("hist");
    h_wind[i]->Draw("histsame");
    h_wind_scaled[i]->Draw("histsame");
    h_ibf_corr[i]->Draw("histsame");


    sprintf(label,"R %d Module %d #phi=%d#circ", sectorN, moduleN, 15-i);
    l_Marker(.75 ,.45 ,.9 ,.5,label,0.04,1);

    TLegend *leg_fit = new TLegend(0.6, 0.25, 0.85,0.43, NULL,"brNDC");
    TLegendEntry  * entry_fit;

    entry_fit = leg_fit->AddEntry(IonCurrent[i],  "Ion Current", "l");
    entry_fit = leg_fit->AddEntry(h_wind[i],  "wind", "l");
    entry_fit = leg_fit->AddEntry(h_wind_scaled[i],  "wind scaled", "l");
    entry_fit = leg_fit->AddEntry(h_ibf_corr[i],  "Ion Current Corrected", "l");

    //leg_fit-> SetNColumns(2);
    leg_fit->SetTextSize(0.04);
    leg_fit-> SetTextFont(42);
    leg_fit->SetFillColor(0);
    leg_fit->SetFillStyle(0);
    leg_fit->SetBorderSize(0);
    leg_fit->Draw();

    c_IonCurrent[i]->cd();
    
    h_ibf_corr[i]->GetXaxis()->SetRangeUser(0., 25000.);
    h_ibf_corr[i]->GetYaxis()->SetRangeUser(-0.299, 0.01);


    sprintf(label,"h_ibf_corr_minusconst_R%d_M%d_%d", sectorN, moduleN, i);
    h_ibf_corr_minusconst[i] = (TH1F*)h_ibf_corr[i]->Clone(label);
    h_ibf_corr_minusconst[i]->Fit("pol0", "", "", 0, 4000);
    TF1 *fit = h_ibf_corr_minusconst[i]->GetFunction("pol0");
    Double_t par_0 = fit->GetParameter(0);

    TF1 *f_pol0 = new TF1("f_pol0","pol0",0,30000);
    f_pol0->SetParameter(0,par_0);
    f_pol0->SetLineColor(7);
    h_ibf_corr_minusconst[i]->Add(f_pol0,-1);

    h_ibf_corr_minusconst[i]->SetLineColor(6);
    h_ibf_corr[i]->Draw("hist");
    h_ibf_corr_minusconst[i]->GetFunction("pol0")->Draw("same");
    h_ibf_corr_minusconst[i]->Draw("histsame");
    f_pol0->Draw("same");

    sprintf(label,"R %d Module %d #phi=%d#circ", sectorN, moduleN, 15-i);
    l_Marker(.75 ,.45 ,.9 ,.5,label,0.04,1);



    for (int n=1; n<=h_ibf_corr_minusconst[i]->GetNbinsX(); n++)
    {
      FineIBF_corr_minusconst->Fill(i-15,n*150,h_ibf_corr_minusconst[i]->GetBinContent(n));
    }

    // Draw and save plots
    
    c_PadCurrent[i]->cd();
    PadCurrent[i]->GetXaxis()->SetRangeUser(0., 25000.);
    PadCurrent[i]->GetYaxis()->SetRangeUser(-0.6, 1.5*maximum_gain);
	  PadCurrent[i]->Draw("hist");
	  fit_Candle_f[i]->Draw("same");
    h_ibf_corr[i]->Draw("histsame");

    sprintf(label,"R %d Module %d #phi=%d#circ", sectorN, moduleN, 15-i);
    l_Marker(.75 ,.85 ,.9 ,.9,label,0.04,1);

    TLegend *leg_pad = new TLegend(0.6, 0.65, 0.9,0.8, NULL,"brNDC");
    TLegendEntry  * entry_pad;

    entry_pad = leg_pad->AddEntry(PadCurrent[i],  "Pad Current", "l");
    entry_pad = leg_pad->AddEntry(fit_Candle_f[i],  "Fit", "l");
    entry_pad = leg_pad->AddEntry(h_ibf_corr[i],  "Ion Current Corrected", "l");

    //leg_pad-> SetNColumns(2);
    leg_pad->SetTextSize(0.04);
    leg_pad-> SetTextFont(42);
    leg_pad->SetFillColor(0);
    leg_pad->SetFillStyle(0);
    leg_pad->SetBorderSize(0);
    leg_pad->Draw();


    sprintf(label,"./Plots/IonCurrentFlow_R%d_M%d_%d.png", sectorN, moduleN, i);
    c_IonCurrentFlow[i]->Print(label);
    sprintf(label,"./Plots/IonCurrent_R%d_M%d_%d.png", sectorN, moduleN, i);
    c_IonCurrent[i]->Print(label);
    sprintf(label,"./Plots/PadCurrent_R%d_M%d_%d.png", sectorN, moduleN, i);
    c_PadCurrent[i]->Print(label);
  }

  gStyle->SetPalette(1);
  //gStyle->SetPaperSize(20,26);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.16);
  
  TCanvas *c_correlation = new TCanvas("c_correlation","c_correlation",1600,1200);
  c_correlation->Divide(5,6,0,0);

  for (int i=3; i<NFiles-2; i++){
    c_correlation->cd(i);
    h_ibf_wind[i]->GetXaxis()->SetRangeUser(-0.5, 0.5);
    h_ibf_wind[i]->GetYaxis()->SetRangeUser(-0.5, 0.5);
    h_ibf_wind[i]->Draw("colz");
  }
  sprintf(label,"./Plots/correlations_R%d_M%d.png",sectorN, moduleN);
  c_correlation->Print(label);


  c2 = new TCanvas("GainMap","GainMap",800,600);
  hGainMap->GetZaxis()->SetRangeUser(0., maximum_gain*1.1);
  hGainMap->Draw("colz");
  sprintf(label,"./Plots/GainMap_R%d_M%d.png",sectorN, moduleN);
  c2->Print(label);

  TCanvas *c_FineGain     = new TCanvas("c_FineGain",    "c_FineGain",     800, 600);
  FineGain->GetYaxis()->SetRangeUser(0., 25000.);
  FineGain->GetZaxis()->SetRangeUser(0., maximum_gain*1.1);
  FineGain->Draw("colz");
  sprintf(label,"./Plots/FineGain_R%d_M%d.png", sectorN, moduleN);
  c_FineGain->Print(label);

  TCanvas *c_FineIBF      = new TCanvas("c_FineIBF",     "c_FineIBF",      800, 600);
  FineIBF->GetYaxis()->SetRangeUser(0., 25000.);
  FineIBF->GetZaxis()->SetRangeUser(-0.299, 0.001);
  FineIBF->Draw("colz");
  sprintf(label,"./Plots/FineIBF_R%d_M%d.png", sectorN, moduleN);
  c_FineIBF->Print(label);

  TCanvas *c_FineIBF_corr = new TCanvas("c_FineIBF_corr","c_FineIBF_corr", 800, 600);
  FineIBF_corr->GetYaxis()->SetRangeUser(0., 25000.);
  FineIBF_corr->GetZaxis()->SetRangeUser(-0.299, 0.001);
  FineIBF_corr->Draw("colz");
  sprintf(label,"./Plots/FineIBF_corr_R%d_M%d.png", sectorN, moduleN);
  c_FineIBF_corr->Print(label);



  TCanvas *c_FineIBF_corr_minusconst = new TCanvas("c_FineIBF_corr_minusconst","c_FineIBF_corr_minusconst", 800, 600);
  FineIBF_corr_minusconst->GetYaxis()->SetRangeUser(0., 25000.);
  FineIBF_corr_minusconst->GetZaxis()->SetRangeUser(-0.299, 0.001);
  FineIBF_corr_minusconst->Draw("colz");
  sprintf(label,"./Plots/FineIBF_corr_minusconst_R%d_M%d.png", sectorN, moduleN);
  c_FineIBF_corr_minusconst->Print(label);

  TCanvas *c_FineIBF_corr_minusconst_zoom = new TCanvas("c_FineIBF_corr_minusconst_zoom","c_FineIBF_corr_minusconst_zoom", 800, 600);
  FineIBF_corr_minusconst->GetYaxis()->SetRangeUser(4500., 22000.);
  FineIBF_corr_minusconst->GetZaxis()->SetRangeUser(-0.199, -0.0001);
  FineIBF_corr_minusconst->Draw("colz");
  sprintf(label,"./Plots/FineIBF_corr_minusconst_zoom_R%d_M%d.png", sectorN, moduleN);
  c_FineIBF_corr_minusconst_zoom->Print(label);

  output = new TFile("output.root","RECREATE");

  FineGain     ->Write();
  FineIBF      ->Write();
  FineIBF_corr ->Write();
  FineIBF_corr_minusconst ->Write();

  for (int i=3; i<NFiles-2; i++){
    PadCurrent[i]->Write();
    IonCurrent[i]->Write();

    h_ibf_wind[i]->Write();
    h_ibf[i]->Write();
    h_wind[i]->Write();
    h_wind_scaled[i]->Write();

    fit_pol1_f[i]->Write();
    fit_Candle_f[i]->Write();


  }
  output->Write();
  output->Close();

  //hGainMap->Fill( 1,1,0.1);

}

void l_Marker(double x1, double y1, double x2, double y2, TString Mark, double size, int aligne){

  TLegend *atl_std = new TLegend(x1,y1,x2,y2,NULL,"brNDC");
  atl_std -> SetFillStyle(0); 
  atl_std -> SetTextSize(size); 
  if(aligne==2)atl_std -> SetTextAlign(12); 
  if(aligne==1)atl_std -> SetTextAlign(32); 
  atl_std -> SetBorderSize(0);
  atl_std -> SetTextFont(42);
  atl_std -> AddEntry((TObject*)0,Mark,"");	 
  atl_std ->Draw();

}
