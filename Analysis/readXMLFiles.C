#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <fstream>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
//#include <rapidxml.hpp>
#include <sstream>

#include <array>

using namespace std;
//using namespace rapidxml;

char label[500];

vector<float> getData( const string &, string  );
vector<float> getData( const string &text, string tag ){
    std::vector<float> vect;
    string collection;
   //vector<string> collection;
   unsigned int pos = 0, start = -1;
   start = text.find( "<" + tag, pos );
    if (start>0){
        //cout << start << endl;
      start = text.find( ">" , start );
      start++;

      pos = text.find( "</" + tag, start ); 
      collection = text.substr( start, pos - start ) ;
      //cout<<collection<<endl;
      
      std::stringstream ss(collection);
      float i;
      while (ss >> i){
          vect.push_back(i);
          if (ss.peek() == ','){
            ss.ignore();
          }
        }
        //cout<<vect[0]<<endl;
    }
   return vect;
}


std::array<std::array<std::vector<float>,10>, 2> readFile(TString fileName);
std::array<std::array<std::vector<float>,10>, 2> readFile(TString fileName = "./waveform.xml"){
    string file, line, word, temp;      
    vector<string> row;
    std::array< std::array< vector<float>,10>, 2> vx;
    //std::array< std::array< vector<float>,10>, 2> vy;

    int nwave = 0;
    int nrows = 0;
    ifstream myfile (fileName);
    if (myfile.is_open()){
        while ( getline (myfile,line) ){
            if(line.find( "</CHN4>", 0 )!=string::npos) {
                nrows=0;
                nwave++;
            }
            if(nrows>0){
                vector<float> collection = getData(line, "Data");
                vx[0][nwave].push_back(collection[0]);
                vx[1][nwave].push_back(collection[1]);
                //h_waves.Fill(collection[0], collection[1]);
                nrows++;
            }
            if(line.find( "<CHN4>", 0 )!=string::npos) nrows++;
        }
    }
    return vx;
}

void readXMLFiles(){


    const int Nfiles = 20;
    TCanvas *c_files[Nfiles];
    TString fileNames[Nfiles] = {
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_2mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_4mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_6mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_8mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_10mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_12mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_14mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_16mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_18mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_20mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_22mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_24mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_26mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_28mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_30mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_32mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_34mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_36mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_38mV_cgx20_fg2_int100_diff500.xml",
                                "/Users/eshulga/Work/scanAnalysis/Waveforms_Calibration_2/Waveform_40mV_cgx20_fg2_int100_diff500.xml",
                                };
    //TString fileNames[Nfiles] = {"./waveform.xml", "./waveform.xml"};
    TH1F *h_maxv = new TH1F("h_maxv; V_{out} [mV]", "h_maxv; V_{out} [mV]", 1000, -1000, 0);
    TH2F *h_maxv_v = new TH2F("h_maxv_v", "h_maxv_v; V_{out} [mV]; V_{in} [mV];", 500, 0, 500 , 50, 0, 50);
    TH2F *h_q_v = new TH2F("h_q_v", "h_q_v; Q_{in} [e]; V_{in} [mV];", 5000, 0, 1e6 , 50, 0, 50);
    TH2F *h_q_v_out = new TH2F("h_q_v_out", "h_q_v_out; Q_{in} [e]; V_{out} [mV];", 5000, 0, 1e6 , 500, -500, 0);
    TH2F *h_v_out_gain = new TH2F("h_v_out_gain", "h_v_out_gain; V_{out} [mV]; Gain;", 500, -500, 0, 6000, 0, 6000  );
    for (int f=0; f<Nfiles; f++ ){
        std::array<std::array<std::vector<float>,10>, 2> vxy = readFile(fileNames[f]);
        TH1F *h_waves[10];
        TH1F *h_counts[10];
        for(int w=0; w<10; w++){
            sprintf(label,"h_waves_%d",w);
            h_waves[w] = new TH1F(label, label, 25, 0, 1500);
            sprintf(label,"h_counts_%d",w);
            h_counts[w] = new TH1F(label, label, 25, 0, 1500);
            //h_waves.push_back(h_waves_tmp);
        }
        for(int w=0; w<10; w++){
            for(int i=0; i<vxy[0][w].size(); i++){
                h_waves[w]->Fill( vxy[0][w][i] , vxy[1][w][i] );
                h_counts[w]->Fill( vxy[0][w][i]  );
            }
            h_waves[w]->Divide(h_counts[w]);
            float max_v = h_waves[w]->GetBinContent(h_waves[w]->GetMinimumBin());
            h_maxv->Fill(max_v);
            float mv_in = (f+1) * 2.0; 
            float q_in = mv_in /1e3 * 2.7 /1e12 /(1.6 / 1e19); 
            float gain = q_in /(260); 
            h_maxv_v->Fill(-1*max_v,mv_in);
            //cout<<" q_in="<<q_in<<endl;
            h_q_v->Fill(q_in, mv_in);
            h_q_v_out->Fill(q_in, max_v);
            h_v_out_gain->Fill(max_v,gain);
        }

        sprintf(label,"c_files_%d",f);
        c_files[f] = new TCanvas(label, label, 800, 600);


        for(int w=0; w<10; w++){
            if(w==0) h_waves[w]->Draw("HIST");
            else h_waves[w]->Draw("HISTsame");
        }
        sprintf(label,"./Plots/files_%d.png",f);
        c_files[f]->Print(label);
    }//Nfiles


    gStyle->SetPadTopMargin(0.05);
    gStyle->SetPadRightMargin(0.08);

    TCanvas *c_maxv = new TCanvas("c_maxv", "c_maxv", 800, 600);

    h_maxv->Draw("HIST");

    c_maxv->Print("./Plots/maxv.png");

    TCanvas *c_maxv_v = new TCanvas("c_maxv_v", "c_maxv_v", 800, 600);

    h_maxv_v->Draw("");
    c_maxv_v->Print("./Plots/maxv_v.png");


    TCanvas *c_q_v = new TCanvas("c_q_v", "c_q_v", 800, 600);

    h_q_v->Draw("");
    c_q_v->Print("./Plots/q_v.png");

    TCanvas *c_q_v_out = new TCanvas("c_q_v_out", "c_q_v_out", 800, 600);

    h_q_v_out->Draw("");
    c_q_v_out->Print("./Plots/q_v_out.png");

    TCanvas *c_v_out_gain = new TCanvas("c_v_out_gain", "c_v_out_gain", 800, 600);

    h_v_out_gain->Draw("");
    c_v_out_gain->Print("./Plots/v_out_gain.png");

}